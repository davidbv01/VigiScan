#ifndef CENTRAL_UNIT_H
#define CENTRAL_UNIT_H

#include "AcquisitionUnit.h"
#include "ProcessingModule.h"
#include "ImageBufferManager.h"
#include "RamDiskCleaner.h"

#include <vector>
#include <string>
#include <filesystem> 
#include <iostream>
#include <chrono>
#include <fstream>
#include <thread>
#include "../Libs/RapidXML/rapidxml.hpp"
#include "../Libs/RapidXML/rapidxml_utils.hpp"
#include <opencv2/core/cuda.hpp>


struct CameraConfig {
    std::string name;
    std::string type;
    int number;
};

class CentralUnit {
public:
    CentralUnit();
    ~CentralUnit();

    bool initializeSystem();
    void startProcessing();
    void stopProcessing();
    void ReadConfig();
    void refreshImages();
    std::vector<AcquisitionUnit*> AcquisitionUnitVector;
    std::vector<ProcessingModule*> ProcessingModuleVector;
    std::vector<ImageBufferManager*> ImageBufferManagerVector;
    bool shouldContinueCleaning=false;
    std::chrono::steady_clock::time_point lastScanTime; // Time of the last successful scan
    const int minScanInterval = 1000; // Minimum interval between scans (milliseconds)
    std::vector<std::filesystem::path> cameraFolders;

private:
    std::vector<ProcessingModule> processingModules;
    std::vector<std::string> cameraIdentifiers;
    std::vector<CameraConfig> cameraConfigs; // Added vector to store camera configurations
    std::thread ramCleanerThread;
};

#endif // CENTRAL_UNIT_H

