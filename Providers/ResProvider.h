#pragma once

#include <string>


class ResProvider {
public:
    ResProvider();

    static ResProvider *makeResProvider();

    virtual int getCpuUsage() = 0;

    virtual int getRamUsage() = 0;

    virtual int getHddUsage() = 0;

    virtual ~ResProvider();
};
