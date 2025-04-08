#include "AcquisitionUnit.h" 

//////////////////////////
//DefaultAcquisitionUnit//
//////////////////////////


DefaultAcquisitionUnit::DefaultAcquisitionUnit(const std::filesystem::path& cameraFolderPath, int cameraIndex)
: cameraFolderPath(cameraFolderPath),cameraIndex(cameraIndex)    {
    initializeCamera();
}

bool DefaultAcquisitionUnit::initializeCamera() {
    capture.open(cameraIndex);
    if (!capture.isOpened()) {
        return false;
    }
    // Capturar el primer frame
    cv::Mat firstFrame;
    capture >> firstFrame;
    cameraOpen = true;
    return true;
}


cv::Mat DefaultAcquisitionUnit::Capturar(){
    cv::Mat frame;
    capture >> frame; // Capturar un frame desde la cámara
    return frame;
}



//////////////////////////
/////IPAcquisitionUnit////
//////////////////////////


IPAcquisitionUnit::IPAcquisitionUnit(const std::filesystem::path& cameraFolderPath, int cameraIndex, std::string ipAddress)
: cameraFolderPath(cameraFolderPath),ipAddress(ipAddress) {
    initializeCamera();
}

bool IPAcquisitionUnit::initializeCamera() {
    // Formar la URL de la cámara IP (asegúrate de que `ipAddress` esté definida)
    std::string url = "rtsp://" + ipAddress + "/stream";
    std::cerr << "Intentando abrir: " << url << std::endl;

    // Abre el stream de video si aún no está abierto
    if (!capture.isOpened()) {
        capture.open(url);

        // Reducir el tamaño del buffer
        capture.set(cv::CAP_PROP_BUFFERSIZE, 1);

        if (!capture.isOpened()) {
            std::cerr << "Error al abrir el flujo de la cámara en " << url << std::endl;
            return false;
        }
    }

    // Captura un frame para asegurarte de que la cámara funcione correctamente
    cv::Mat frame;
    capture >> frame;
    if (frame.empty()) {
        std::cerr << "Error al capturar frame de la cámara en " << url << std::endl;
        return false;
    }

    std::cerr << "Cámara abierta correctamente en " << url << std::endl;

    // Establecer el estado de la cámara como abierta y notificar al gestor de imágenes
    cameraOpen = true;

    return true;
}

cv::Mat IPAcquisitionUnit::Capturar() {
    cv::Mat frame;

    // Capturar el último frame disponible
    if (capture.grab()) {
        capture.retrieve(frame);
    }

    return frame;
}


///////////////////
//AcquisitionUnit//
///////////////////


AcquisitionUnit *AcquisitionUnit::createInstance(int cameraIndex, const std::string &type, const std::filesystem::path &cameraFolderPath, std::string ipAddress)
{
    if (type == "Default") {
        return new DefaultAcquisitionUnit(cameraFolderPath, cameraIndex);
    }
    else if (type == "IP") {
        return new IPAcquisitionUnit(cameraFolderPath, cameraIndex, ipAddress);
    }
    else{
        return nullptr;
    }
}

cv::Mat AcquisitionUnit::Capturar() {
    return Capturar();
}

