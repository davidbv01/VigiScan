#ifndef IMAGE_BUFFER_MANAGER_H
#define IMAGE_BUFFER_MANAGER_H

#include "ProcessingModule.h"
#include "AcquisitionUnit.h"

#include <queue>
#include <mutex>
#include <condition_variable>
#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <memory>
#include <iostream> 
#include <filesystem> 
#include <thread>



struct ImageWithCameraID {
    cv::Mat image;
    int cameraID;
};

class ImageBufferManager {
public:
    static ImageBufferManager* createInstance(int cameraIndex, const std::filesystem::path& cameraFolderPath, AcquisitionUnit* acqUnit, ProcessingModule* procModule);
    void getImage(int i);
    void processImage();
    void start(int i);
private:
    YOLO_V8* yoloDetector;
    std::mutex bufferManagerMutex;
    std::unordered_map<int, std::filesystem::path> saveDirectories;
    std::deque<ImageWithCameraID> imageBuffer;
    std::mutex bufferMutex;
    std::condition_variable bufferCondition;    
    std::unordered_map<int, std::shared_ptr<ProcessingModule>> processingModules;
    int cameraIndex;
    std::filesystem::path cameraFolderPath;
    AcquisitionUnit* unitAcquisition;
    ProcessingModule* unitProcessing;
    std::thread acquisitionThread;
    std::thread processingThread;
    ImageBufferManager(int cameraIndex, const std::filesystem::path& cameraFolderPath, AcquisitionUnit* unitAcquisition, ProcessingModule* unitProcessing);

};

#endif // IMAGE_BUFFER_MANAGER_H
