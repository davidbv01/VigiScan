#include "ImageBufferManager.h"

void ImageBufferManager::getImage(int i) {
    std::unique_lock<std::mutex> lock(bufferMutex);
    bufferCondition.wait(lock, [this] { return imageBuffer.size() < 2; }); // Esperar hasta que haya menos de 2 imágenes en el búfer
    
    // Capturar una nueva imagen
    cv::Mat capturedFrame = unitAcquisition->Capturar();

    // Añadir la imagen al búfer
    imageBuffer.push_back({capturedFrame, cameraIndex});

    // Notificar a los posibles hilos en espera que hay una nueva imagen disponible
    bufferCondition.notify_all();
}

void ImageBufferManager::processImage() {
    std::unique_lock<std::mutex> lock(bufferMutex);
    bufferCondition.wait(lock, [this] { return !imageBuffer.empty(); }); // Esperar hasta que haya al menos una imagen en el búfer

    // Obtener la imagen más antigua del búfer
    ImageWithCameraID imageWithID = imageBuffer.front();
    
    lock.unlock(); // Desbloquear el mutex antes de procesar la imagen para permitir la adquisición de imágenes simultánea

    // Procesar la imagen solo si no está vacía
    cv::Mat& image = imageWithID.image;
    if (!image.empty()) {
        unitProcessing->Process(image, 1); // Procesar la imagen con el ProcessingModule
    } else {
        std::cerr << "Advertencia: Intento de procesar una imagen vacía." << std::endl;
    }

    // Eliminar la imagen del búfer después de procesarla
    lock.lock(); // Bloquear de nuevo para manipular el buffer de manera segura
    imageBuffer.pop_front();
}

void ImageBufferManager::start(int i){
    while (true) {
            getImage(i);      // Obtener una imagen del búfer
            processImage(); // Procesar la imagen obtenida
        }
}

ImageBufferManager* ImageBufferManager::createInstance(int cameraIndex, const std::filesystem::path& cameraFolderPath, AcquisitionUnit* acqUnit, ProcessingModule* procModule) {
    return new ImageBufferManager(cameraIndex, cameraFolderPath, acqUnit, procModule);
}

ImageBufferManager::ImageBufferManager(int cameraIndex, const std::filesystem::path& cameraFolderPath, AcquisitionUnit* unitAcquisition, ProcessingModule* unitProcessing)
    : cameraIndex(cameraIndex), cameraFolderPath(cameraFolderPath), unitAcquisition(unitAcquisition), unitProcessing(unitProcessing) {
    // Puedes realizar otras inicializaciones aquí si es necesario
}