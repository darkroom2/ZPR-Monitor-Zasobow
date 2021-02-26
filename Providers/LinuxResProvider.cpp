#ifdef __linux__

#include "LinuxResProvider.h"

int LinuxResProvider::getCpuUsage() {
    long total_jiffies1 = 0;
    long work_jiffies1 = 0;
    long total_jiffies2 = 0;
    long work_jiffies2 = 0;
    auto jiffies = this->get_jiffies();
    for (int i = 0; i < jiffies.size(); ++i) {
        if (i < 3)
            work_jiffies1 += jiffies[i];
        total_jiffies1 += jiffies[i];
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    jiffies = this->get_jiffies();
    for (int i = 0; i < jiffies.size(); ++i) {
        if (i < 3)
            work_jiffies2 += jiffies[i];
        total_jiffies2 += jiffies[i];
    }
    float cpu_usage = (work_jiffies2 - work_jiffies1) / (float) (total_jiffies2 - total_jiffies1) * 100;
    return cpu_usage;
}

int LinuxResProvider::getRamUsage() {
    std::fstream file;
    file.open("/proc/meminfo", std::ios::in);
    long totalMem = 0;
    long freeMem = 0;
    if (file.good()) {
        std::string temp;
        std::string line;

        getline(file, line);
        std::size_t startpos = line.find_first_of("0123456789");
        std::size_t endpos = line.find(' ', startpos);
        totalMem = atoi(line.substr(startpos, endpos).c_str());
        getline(file, line);
        startpos = line.find_first_of("0123456789");
        endpos = line.find(' ', startpos);
        freeMem = atoi(line.substr(startpos, endpos).c_str());
    }
    file.close();
    float ramUsage = (float) (totalMem - freeMem) / totalMem * 100;
    return (int) ramUsage;
}

int LinuxResProvider::getHddUsage() {
    int hddTimeUsage1 = getHddTimeUsage();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    int hddTimeUsage2 = getHddTimeUsage();
    int hddUsage = ((float) (hddTimeUsage2 - hddTimeUsage1) / 1000) * 100;
    return hddUsage;
}

std::vector<int> LinuxResProvider::get_jiffies() {
    std::fstream file;
    std::vector<int> jiffies;
    file.open("/proc/stat", std::ios::in);
    if (file.good()) {
        std::string line;
        getline(file, line);
        std::size_t current, previous = 0;
        current = line.find(' ');
        previous = current + 2;
        current = line.find(' ', previous);
        while (current != std::string::npos) {
            jiffies.push_back(atoi(line.substr(previous, current - previous).c_str()));
            previous = current + 1;
            current = line.find(' ', previous);
        }
        jiffies.push_back(atoi(line.substr(previous, current - previous).c_str()));

    }
    file.close();
    return jiffies;
}

int LinuxResProvider::getHddTimeUsage() {
    std::fstream file;
    file.open("/proc/diskstats", std::ios::in);
    int hddTimeUsage = 0;
    if (file.good()) {
        std::string line;
        getline(file, line);
        while (line.npos == line.find("sda")) {
            getline(file, line);
        }
        std::size_t pos = line.find("sda");
        for (int i = 0; i < 10; i++) {
            pos = line.find(' ', pos) + 1;
        }
        std::size_t pos2 = line.find(' ', pos);
        hddTimeUsage = atoi(line.substr(pos, pos2).c_str());
    }
    file.close();
    return hddTimeUsage;
}


#endif // __linux__
