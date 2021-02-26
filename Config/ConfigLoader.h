#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

struct cfgCase {
    int cpu;
    int ram;
    int hdd;
};


class ConfigLoader {


public:
    std::vector<cfgCase> cfgCases;

    ConfigLoader(const std::string &config);

    virtual ~ConfigLoader();

    bool isNumber(std::string s);

    bool isValidValue(std::string s);
};
