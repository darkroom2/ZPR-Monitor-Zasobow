#pragma once

#ifdef __linux__

#include "ResProvider.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>

class LinuxResProvider : public ResProvider {

    static std::vector<int> get_jiffies();

    static int getHddTimeUsage();

public:

// Inherited via ResProvider
    virtual int getCpuUsage() override;

    virtual int getRamUsage() override;

    virtual int getHddUsage() override;
};

#endif // __linux__
