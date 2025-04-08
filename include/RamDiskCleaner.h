#ifndef RAMDISKCLEANER_H
#define RAMDISKCLEANER_H

#include <string>
#include <chrono>
#include <thread>
#include <filesystem>
#include <vector>

class RamDiskCleaner {
public:
    RamDiskCleaner(const std::vector<std::filesystem::path>& directories, std::chrono::seconds ageLimit);
    void startCleaning();

private:
    void cleanOldFiles();

    std::vector<std::filesystem::path> directories;
    std::chrono::seconds ageLimit;
    bool running;
};

#endif // RAMDISKCLEANER_H
