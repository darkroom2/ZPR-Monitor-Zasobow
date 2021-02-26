#include "ServerManager.h"

void ServerManager::send_usages(int cpu, int ram, int hdd) {
    std::fstream file_to_append;
    file_to_append.open(this->url, std::fstream::in | std::fstream::out | std::fstream::app);

    if (!file_to_append) {
        file_to_append.open(this->url, std::fstream::in | std::fstream::out | std::fstream::trunc);
        file_to_append << "{\n\t\"cpu\": " << cpu << ",\n\t\"ram\": " << ram << ",\n\t\"hdd\": " << hdd << ",\n\t\"time\": " << std::time(nullptr) << "\n}";
        file_to_append.close();

    } else {
        file_to_append << "\n";
        file_to_append << "{\n\t\"cpu\": " << cpu << ",\n\t\"ram\": " << ram << ",\n\t\"hdd\": " << hdd << ",\n\t\"time\": " << std::time(nullptr) << "\n}";
        file_to_append.close();
    }
}
