#include "RamDiskCleaner.h"
#include <iostream>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;

RamDiskCleaner::RamDiskCleaner(const std::vector<fs::path>& directories, std::chrono::seconds ageLimit)
    : directories(directories), ageLimit(ageLimit), running(true) {}

void RamDiskCleaner::startCleaning() {
    while (running) {
        cleanOldFiles();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void RamDiskCleaner::cleanOldFiles() {
    auto now = std::chrono::system_clock::now();

    for (const auto& directory : directories) {
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                auto lastWriteTime = fs::last_write_time(entry);
                auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                    lastWriteTime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
                );

                if (now - sctp > ageLimit) {
                    fs::remove(entry);
                }
            }
        }
    }
}
