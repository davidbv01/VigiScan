#include "ProcessingModule.h"

ProcessingModule::ProcessingModule(const std::filesystem::path& saveDirectory, int cameraID)
    : saveDirectory_(saveDirectory), cameraID_(cameraID), yoloDetector(nullptr) {}

ProcessingModule::~ProcessingModule() {}

void ProcessingModule::Process(cv::Mat& frame, int cameraID){
    if(firstImage){
        yoloDetector = new YOLO_V8;
        ReadCocoYaml(yoloDetector);
        DL_INIT_PARAM params;
        params.rectConfidenceThreshold = 0.1;
        params.iouThreshold = 0.4;
        params.modelPath = "best.onnx";
        params.imgSize = { 640, 640 };

        params.cudaEnable = true;
        params.modelType = YOLO_DETECT_V8;
        yoloDetector->CreateSession(params);
        firstImage=false;
        //Calcular incremento de tiempo
        lastTime = std::chrono::steady_clock::now(); // Captura el tiempo actual
        float delT = 0;
        processImage(frame, cameraID, yoloDetector, delT);
    }
    else{
        //Calcular incremento de tiempo
        auto currentTime = std::chrono::steady_clock::now(); // Captura el tiempo actual
        // Calcula el tiempo transcurrido desde la última llamada a Process
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime);
        float delT = elapsedTime.count() / 1000.0f; // Convertir a segundos (si es necesario)
        // Actualiza lastTime con el tiempo actual para la próxima llamada
        lastTime = currentTime;

        processImage(frame, cameraID, yoloDetector, delT);
    }
}

void ProcessingModule::processImage(cv::Mat& frame, int cameraID, YOLO_V8*& p, float delT) {
    std::vector<DL_RESULT> res;
    std::vector<DetectedObject> detectedObjects; // Vector de objetos detectados

    p->RunSession(frame, res); // Suponiendo que 'p' es un puntero a un objeto YOLO_V8 válido

    for (auto& re : res) {
        if (floor(100 * re.confidence)>40){
            DetectedObject obj;
            obj.boundingBox = re.box; // Establecer el bounding box del objeto
            obj.classId = re.classId; // Establecer la clase del objeto
            obj.confidence = floor(100 * re.confidence); 

            detectedObjects.push_back(obj);
        }
    }

    // Actualizar el tracker con los objetos detectados
    std::vector<Tracker::TrackedObject> trackedObjects = tracker.update(detectedObjects);

    // Iterar sobre los objetos en seguimiento y dibujarlos en el frame
    for (const auto& obj : trackedObjects) {
        if(!obj.disappeared){
        // Dibujar la bounding box
        cv::Scalar color;
        if (obj.scanned) {
            color = cv::Scalar(0, 255, 0); // Verde si está escaneado
        } else {
            color = cv::Scalar(0, 0, 255); // Rojo si no está escaneado
        }
        cv::rectangle(frame, obj.boundingBox, color, 2);

        // Mostrar el object ID y la confidence
        std::string text = "ID: " + std::to_string(obj.id) + ", Confidence: " + std::to_string(obj.confidence) + "|" + std::to_string(obj.classId);
        cv::putText(frame, text, cv::Point(obj.boundingBox.x, obj.boundingBox.y - 10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 2);
        }
    }

    saveImage(frame);
}


void ProcessingModule::saveImage(const cv::Mat& image) {
    std::filesystem::path cameraFolderPath = saveDirectory_;
    // Obtener la fecha y hora actual
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&now_time_t);

    // Formatear la fecha y hora en el formato deseado
    std::stringstream ss;
    ss << std::put_time(&local_tm, "%Y%m%d_%H%M%S");

    // Generar un número aleatorio de 4 dígitos
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 9999);
    int random_number = dis(gen);

    // Crear el nombre del archivo
    std::string file_name = ss.str() + "_" + std::to_string(random_number) + ".jpg";

    // Construir la ruta completa del archivo
    std::string full_file_path = (cameraFolderPath / file_name).string();
    
    // Guardar la imagen
    cv::imwrite(full_file_path, image);
}

ProcessingModule *ProcessingModule::createInstance(int cameraIndex, const std::filesystem::path& cameraFolderPath)
{
    return new ProcessingModule(cameraFolderPath, cameraIndex);
}


int ProcessingModule::ReadCocoYaml(YOLO_V8*& p) {
    // Open the YAML file
    std::ifstream file("databest.yaml");
    if (!file.is_open())
    {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }

    // Read the file line by line
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }

    // Find the start and end of the names section
    std::size_t start = 0;
    std::size_t end = 0;
    for (std::size_t i = 0; i < lines.size(); i++)
    {
        if (lines[i].find("names:") != std::string::npos)
        {
            start = i + 1;
        }
        else if (start > 0 && lines[i].find(':') == std::string::npos)
        {
            end = i;
            break;
        }
    }

    // Extract the names
    std::vector<std::string> names = {"Product"};
    //std::vector<std::string> names;
    for (std::size_t i = start; i < end; i++)
    {
        std::stringstream ss(lines[i]);
        std::string name;
        std::getline(ss, name, ':'); // Extract the number before the delimiter
        std::getline(ss, name); // Extract the string after the delimiter
        names.push_back(name);
    }

    p->classes = names;
    return 0;
}