#include "PollManager.h"

PollManager::PollManager(std::unique_ptr<ResProvider> resProvider) : _resProvider(std::move(resProvider)) {

}

std::tuple<int, int, int> PollManager::getUsages() {
    return std::make_tuple(this->_resProvider->getCpuUsage(), this->_resProvider->getRamUsage(),
                           this->_resProvider->getHddUsage());
}
