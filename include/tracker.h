#ifndef TRACKER_H
#define TRACKER_H

#include <vector>
#include <opencv2/core.hpp>

// Estructura para representar un objeto detectado
struct DetectedObject {
    cv::Rect boundingBox; // Bounding box del objeto
    int classId; // Clase del objeto
    float confidence; // Confianza de detección
};

// Clase Tracker para realizar el seguimiento de objetos
class Tracker {
public:
    Tracker();
    ~Tracker();

    // Función para iniciar el seguimiento
    void startTracking(const std::vector<DetectedObject>& objects);

    // Estructura para representar un objeto en seguimiento
    struct TrackedObject {
        bool disappeared; // Flag que indica si el objeto ha desaparecido en el cuadro actual
        cv::Rect boundingBox; // Bounding box del objeto
        int classId; // Clase del objeto
        float confidence; // Confianza de detección
        int id; // Identificador único del objeto
        bool scanned; // Estado de escaneo
        int framesDisappeared;
    };

    // Función para actualizar el seguimiento en cada cuadro de video
    std::vector<TrackedObject> update(const std::vector<DetectedObject>& objects);

    // Función para obtener el estado de escaneo de un objeto dado su ID único
    bool getObjectScannedState(int objectId);

    float euclideanDistance(const cv::Point& p1, const cv::Point& p2);

private:
    std::vector<TrackedObject> trackedObjects; // Vector de objetos en seguimiento
    std::vector<int> disappearedIds; // IDs de objetos que han desaparecido temporalmente
    int nextObjectId; // ID único para el próximo objeto en seguimiento
};

#endif // TRACKER_H
