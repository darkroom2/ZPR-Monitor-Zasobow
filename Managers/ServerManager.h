#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

class ServerManager {

//    std::string url = "http://localhost:1234/usages";
    std::string url = "usage_data.json";

public:
    ServerManager() = default;

    void send_usages(int cpu, int ram, int hdd);
};
