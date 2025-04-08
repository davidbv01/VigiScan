#ifndef ACQUISITION_UNIT_H
#define ACQUISITION_UNIT_H

#include <opencv2/opencv.hpp> 
#include <functional> // Para std::function 
#include <filesystem> 
#include "../Libs/RapidXML/rapidxml.hpp"
#include "../Libs/RapidXML/rapidxml_utils.hpp"
#include <ctime>
#include <chrono>
#include <random>
#include <stdio.h>
#include <iostream> 
#include <iostream>

// Define el tipo para funciones callback
using FrameCallback = std::function<void(const cv::Mat& colorFrame, const cv::Mat& depthFrame)>;

class AcquisitionUnit {
public:
    virtual cv::Mat Capturar() = 0;
    virtual bool initializeCamera() = 0;
    virtual ~AcquisitionUnit() {}
    static AcquisitionUnit* createInstance(int cameraIndex, const std::string& type, const std::filesystem::path& cameraFolderPath, std::string ipAddress);

private:
    int cameraIndex;
    int cameraId;
    bool cameraOpen=false;
    std::string name;
    std::string type;
    cv::VideoCapture capture;
    FrameCallback frameCallback;
    bool capturing;
};

class DefaultAcquisitionUnit : public AcquisitionUnit {
public:
    DefaultAcquisitionUnit(const std::filesystem::path& cameraFolderPath, int cameraId);
    bool initializeCamera() override;
    cv::Mat Capturar();

private:
    int cameraId;
    int cameraIndex;
    bool cameraOpen;
    cv::VideoCapture capture;
    bool capturing = true; 
    cv::Mat currentImage; 
    std::filesystem::path cameraFolderPath;
};

class IPAcquisitionUnit : public AcquisitionUnit {
public:
    IPAcquisitionUnit(const std::filesystem::path& cameraFolderPath, int cameraId, std::string ipAddress);
    bool initializeCamera() override;
    cv::Mat Capturar();

private:
    std::string ipAddress;
    int cameraId;
    int cameraIndex;
    bool cameraOpen;
    cv::VideoCapture capture;
    bool capturing = true; 
    cv::Mat currentImage; 
    std::filesystem::path cameraFolderPath;
};


#endif // ACQUISITION_UNIT_H