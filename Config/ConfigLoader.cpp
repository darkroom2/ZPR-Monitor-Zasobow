#include "ConfigLoader.h"

ConfigLoader::ConfigLoader(const std::string &config) {

    std::fstream config_file;
    config_file.open(config, std::fstream::in);
    if (!config_file) {
        throw std::invalid_argument("File " + config + "doesnt exist");
    }
    std::string line;
    int lineCounter = 1;
    while (std::getline(config_file, line)) {
//        std::cout << line << std::endl;
        std::size_t position;
        std::string value;
        cfgCase cfg = {0, 0, 0};
        position = line.find("cpu");
        if (position != line.npos) {
            value = line.substr(position + 4, line.find(' ', position + 4) - (position + 4));
            if (isValidValue(value))
                cfg.cpu = atoi(value.c_str());
            else
                std::cerr << "Invalid Value for cpu in line:" << lineCounter << std::endl;
        }
        position = line.find("ram");
        if (position != line.npos) {
            value = line.substr(position + 4, line.find(' ', position + 4) - (position + 4));
            if (isValidValue(value))
                cfg.ram = atoi(value.c_str());
            else
                std::cerr << "Invalid Value for ram in line:" << lineCounter << std::endl;
        }
        position = line.find("hdd");
        if (position != line.npos) {
            value = line.substr(position + 4, line.find(' ', position + 4) - (position + 4));
            if (isValidValue(value))
                cfg.hdd = atoi(value.c_str());
            else
                std::cerr << "Invalid Value for hdd in line:" << lineCounter << std::endl;
        }
        this->cfgCases.push_back(cfg);
        lineCounter++;
    }
}

ConfigLoader::~ConfigLoader() = default;

bool ConfigLoader::isValidValue(std::string s) {
    if (!isNumber(s)) {
        return false;
    }
    int value = atoi(s.c_str());
    return !(value > 100 || value < 0);
}

bool ConfigLoader::isNumber(std::string s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
