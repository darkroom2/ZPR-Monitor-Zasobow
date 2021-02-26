#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <atomic>
#include <queue>

#include "../Config/ConfigLoader.h"
#include "PollManager.h"
#include "ServerManager.h"

#define MAX_SIZE 20

class ConditionManager {

    std::unique_ptr<ConfigLoader> _configLoader;
    std::unique_ptr<PollManager> _pollManager;
    std::unique_ptr<ServerManager> _serverManager;

    std::vector<std::tuple<int, int, int>> _usagesVec;
    std::queue<std::tuple<int, int, int>> _badUsages;

    std::thread _pollThread;
    std::thread _serverThread;

    std::mutex myMutex;
    std::condition_variable condVar;
    bool ready = false;

    bool _stopPollThread = false;
    bool _stopServerThread = false;

    void startPolling();

    void startConditionCheck();

    void startListening();


public:
    ConditionManager(std::unique_ptr<ConfigLoader> configLoader, std::unique_ptr<PollManager> pollManager,
                     std::unique_ptr<ServerManager> serverManager);

    bool conditionMet(const std::tuple<int, int, int> &usages);

    virtual ~ConditionManager();

    void start();

};
