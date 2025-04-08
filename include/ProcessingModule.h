#ifndef PROCESSING_MODULE_H
#define PROCESSING_MODULE_H


#include <opencv2/opencv.hpp> 
#include <iostream>
#include <vector>
#include <chrono>
#include <filesystem>
#include <ctime>
#include <chrono>
#include <random>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <iomanip>
#include <fstream>

#include "tracker.h"
#include "inference.h"

struct ProcessedImage {
    cv::Mat image;
    int cameraID;
};

class ProcessingModule {
public:
    static ProcessingModule& getInstance(const std::filesystem::path& saveDirectory, int cameraID) {
        static ProcessingModule instance(saveDirectory, cameraID);;
        return instance;
    }

    ProcessingModule(const std::filesystem::path& saveDirectory, int cameraID);
    ~ProcessingModule();
    void Process(cv::Mat& frame, int cameraID);
    void processImage(cv::Mat& frame, int cameraID, YOLO_V8*& p, float delT);
    void saveImage(const cv::Mat& image);
    int ReadCocoYaml(YOLO_V8*& p);

    static ProcessingModule* createInstance(int cameraIndex, const std::filesystem::path& cameraFolderPath);

private:
    Tracker tracker;
    YOLO_V8* yoloDetector; 
    bool firstImage=true;
    std::filesystem::path saveDirectory_;
    int cameraID_;
    bool isFirstFrame=true;
    std::chrono::steady_clock::time_point lastTime; //Variable para calular el incremento de tiempoo
};

#endif // PROCESSING_MODULE_H
