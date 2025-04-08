#include "tracker.h"

Tracker::Tracker() {
    nextObjectId = 0;
}

Tracker::~Tracker() {}

void Tracker::startTracking(const std::vector<DetectedObject>& objects) {
    // Iniciar el seguimiento para los objetos detectados
    for (const auto& obj : objects) {
        TrackedObject trackedObj;
        trackedObj.id = nextObjectId++;
        trackedObj.boundingBox = obj.boundingBox;
        trackedObj.classId = obj.classId;
        trackedObj.confidence = obj.confidence;
        trackedObj.scanned = false; // Inicialmente no escaneado
        trackedObj.disappeared = false;
        // Puedes inicializar más información adicional si es necesario
        trackedObjects.push_back(trackedObj);
    }
}

std::vector<Tracker::TrackedObject> Tracker::update(const std::vector<DetectedObject>& objects) {
    // Actualizar el seguimiento en cada cuadro de video
    for (auto& obj : trackedObjects) {
        obj.scanned = false; // Marcar todos los objetos como no escaneados
    }

    // Actualizar el contador de cuadros desaparecidos y eliminar los objetos que han desaparecido durante demasiados cuadros
    for (auto& obj : trackedObjects) {
        bool objectFound = false;

        for (const auto& newObj : objects) {
            float distance = euclideanDistance(obj.boundingBox.tl(), newObj.boundingBox.tl());
            if (distance < 50) {
                // Se encontró el objeto en el cuadro actual
                objectFound = true;
                obj.framesDisappeared = 0; // Reiniciar el contador de cuadros desaparecidos
                break;
            }
        }

        if (!objectFound) {
            // Incrementar el contador de cuadros desaparecidos para objetos que no se encontraron en este cuadro
            obj.framesDisappeared++;
        }
    }

    // Eliminar objetos que han desaparecido durante demasiados cuadros
    trackedObjects.erase(
        std::remove_if(trackedObjects.begin(), trackedObjects.end(),
                       [&](const TrackedObject& obj) {
                           return obj.framesDisappeared >= 20; // Eliminar si ha desaparecido durante más de 5 cuadros
                       }),
        trackedObjects.end());

    // Asociar objetos en el cuadro actual con objetos en seguimiento en el cuadro anterior
    for (const auto& obj : objects) {
        float minDistance = std::numeric_limits<float>::max();
        TrackedObject* closestObj = nullptr;

        for (auto& trackedObj : trackedObjects) {
            float distance = euclideanDistance(obj.boundingBox.tl(), trackedObj.boundingBox.tl());
            if (distance < minDistance) {
                minDistance = distance;
                closestObj = &trackedObj;
            }
        }

        if (closestObj && minDistance < 50) { // Umbral de distancia para asociación
            // Asociar el objeto detectado con el objeto en seguimiento más cercano
            closestObj->boundingBox = obj.boundingBox;
            closestObj->classId = obj.classId;
            closestObj->confidence = obj.confidence;
            closestObj->scanned = false; // Se marcará como escaneado según la lógica de tu aplicación
        } else {
            // Si no se encuentra una asociación cercana, crear un nuevo objeto en seguimiento
            TrackedObject newTrackedObj;
            newTrackedObj.id = nextObjectId++;
            newTrackedObj.boundingBox = obj.boundingBox;
            newTrackedObj.classId = obj.classId;
            newTrackedObj.confidence = obj.confidence;
            newTrackedObj.scanned = false; // Inicialmente no escaneado
            newTrackedObj.framesDisappeared = 0; // Reiniciar el contador de cuadros desaparecidos
            // Puedes inicializar más información adicional si es necesario
            trackedObjects.push_back(newTrackedObj);
        }
    }

    return trackedObjects;
}


bool Tracker::getObjectScannedState(int objectId) {
    // Obtener el estado de escaneo de un objeto dado su ID único
    for (const auto& obj : trackedObjects) {
        if (obj.id == objectId) {
            return obj.scanned;
        }
    }
    // Devolver falso si el objeto no se encuentra
    return false;
}

float Tracker::euclideanDistance(const cv::Point& p1, const cv::Point& p2) {
    return std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
}
