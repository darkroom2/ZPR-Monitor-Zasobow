#include "ConditionManager.h"

ConditionManager::ConditionManager(std::unique_ptr<ConfigLoader> configLoader,
                                   std::unique_ptr<PollManager> pollManager,
                                   std::unique_ptr<ServerManager> serverManager)
        :
        _configLoader(std::move(configLoader)),
        _pollManager(std::move(pollManager)),
        _serverManager(std::move(serverManager)),
        _pollThread(),
        _serverThread() {

}

ConditionManager::~ConditionManager() {
    this->_stopPollThread = true;
    if (this->_pollThread.joinable()) {
        this->_pollThread.join();
        this->condVar.notify_all();
        this->ready = true;
    }

    this->_stopServerThread = true;
    if (this->_serverThread.joinable())
        this->_serverThread.join();
}

void ConditionManager::startPolling() {
    while (!this->_stopPollThread) {
//        std::cout << "_pollThread:: dodaje zuzycie zasobow" << std::endl;
        this->_usagesVec.push_back(this->_pollManager->getUsages());
        this->startConditionCheck();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void ConditionManager::start() {
    this->_pollThread = std::move(std::thread(&ConditionManager::startPolling, this));
    this->_serverThread = std::move(std::thread(&ConditionManager::startListening, this));
}

void ConditionManager::startConditionCheck() {
//    std::cout << "_sprawdzam warunki" << std::endl;
    if (!this->_usagesVec.empty()) {
        auto last_usage = this->_usagesVec.back();
        if (this->conditionMet(last_usage)) {

            // dodaj do kolejki i powiadom serwer ze dodales
            std::lock_guard<std::mutex> guard(myMutex);
            if (_badUsages.size() > MAX_SIZE)
                _badUsages.pop();
            _badUsages.push(last_usage);
//            std::cout << "Warunek powiadomienia spelniony" << std::endl;
            condVar.notify_all();
            ready = true;
        }
        _usagesVec.pop_back();
    }
}

bool ConditionManager::conditionMet(const std::tuple<int, int, int> &usages) {
    // sprawdz cpu
    int cpu, ram, hdd;
    std::tie(cpu, ram, hdd) = usages;

    for (const auto &i : this->_configLoader->cfgCases) {
        if (cpu >= i.cpu && ram >= i.ram && hdd >= i.hdd) {
            return true;
        }
    }
    return false;
}

void ConditionManager::startListening() {
    while (!this->_stopServerThread) {
        if (_badUsages.empty()) {
//            std::cout << "ide spac" << std::endl;
            std::unique_lock<std::mutex> myLock(myMutex);
            condVar.wait(myLock, [&] { return ready; });
            myLock.unlock();
            ready = false;
//            std::cout << "wstalem" << std::endl;
        } else {
//            std::cout << "biore wartosc" << std::endl;
            std::tuple<int, int, int> usages;
            int cpu, ram, hdd;
            {
                std::lock_guard<std::mutex> guard(myMutex);
                usages = _badUsages.front();
                std::tie(cpu, ram, hdd) = usages;

                _badUsages.pop();
            }
            this->_serverManager->send_usages(cpu, ram, hdd);
        }
    }
}
