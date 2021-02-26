#include <iostream>
#include <utility>
#include <cstring>

#include "Config/ConfigLoader.h"
#include "Managers/PollManager.h"
#include "Managers/ConditionManager.h"

int main(int argc, char *argv[]) {

    std::cout << "Resource Monitor v1.0" << std::endl;

    std::string path;

    if (argc == 2)
        path = std::string(argv[1]);
    else {
        std::cout << "ERROR: Podaj jeden parametr." << std::endl;
        return -1;
    }

    try {
        auto cfgLoader = std::make_unique<ConfigLoader>(path);

        auto serverManager = std::make_unique<ServerManager>();

        ResProvider *factoredResProvider = ResProvider::makeResProvider();

        std::unique_ptr<ResProvider> resProvider(factoredResProvider);

        auto pollManager = std::make_unique<PollManager>(std::move(resProvider));

        auto cndManager = std::make_unique<ConditionManager>(std::move(cfgLoader), std::move(pollManager),
                                                             std::move(serverManager));

        cndManager->start();

        std::cout << "Wpisz 'q' aby zakonczyc program" << std::endl;

        std::string choice;

        while (std::cin >> choice && strcmp(choice.c_str(), "q") != 0) {
            std::cout << "Wpisz 'q' aby zakonczyc program" << std::endl;
        }
    } catch (std::invalid_argument &ex) {
        std::cout << "Error: " << ex.what() << std::endl;
    }

    return 0;
}
