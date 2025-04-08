#include "CentralUnit.h"

using namespace std;
using namespace cv;
using namespace cuda;


CentralUnit::CentralUnit() {
    // Constructor de CentralUnit
}

CentralUnit::~CentralUnit() {
    // Destructor de CentralUnit
}

bool CentralUnit::initializeSystem() {
    ReadConfig(); // Lee la configuración al inicializar el sistema
    // Lógica adicional para inicializar el sistema según la configuración leída
    return true; // Reemplazar con la lógica apropiada de éxito o fracaso
}

void CentralUnit::ReadConfig() {
    std::string configFilePath = "../../Config/AppConfig.xml";
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo AppConfig.xml" << std::endl;
        return;
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(configFile)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');

    rapidxml::xml_document<> doc;
    doc.parse<0>(&buffer[0]);

    rapidxml::xml_node<>* rootNode = doc.first_node("AppConfig");
    if (!rootNode) {
        std::cerr << "Error: Nodo AppConfig no encontrado" << std::endl;
        return;
    }

    rapidxml::xml_node<>* vsNode = rootNode->first_node("VisionSystems");
    if (!vsNode) {
        std::cerr << "Error: Nodo VisionSystems no encontrado" << std::endl;
        return;
    }

    int cameraIndex = 0; // Iniciar con el índice de cámara 0

    for (rapidxml::xml_node<>* visionSystem = vsNode->first_node("VisionSystem"); visionSystem; visionSystem = visionSystem->next_sibling()) {
    rapidxml::xml_node<>* nameNode = visionSystem->first_node("Name");
    rapidxml::xml_node<>* typeNode = visionSystem->first_node("Type");
    rapidxml::xml_node<>* IPNode = visionSystem->first_node("IP");
    int x = atoi(visionSystem->first_node("X")->value());
    int y = atoi(visionSystem->first_node("Y")->value());
    int w = atoi(visionSystem->first_node("W")->value());
    int l = atoi(visionSystem->first_node("L")->value());

    if (typeNode) {
        std::string type = typeNode->value();
        std::string IP = "";

        //Grabar la IP en caso que sea necesario
        if (type=="IP"){IP = IPNode->value();}

        // Extract Camera Name
        rapidxml::xml_node<>* nameNode = visionSystem->first_node("Name");
        if (nameNode) {
            std::string cameraName = nameNode->value();

            // Create Folder
            std::filesystem::path rootFolderPath = "F:/";
            std::filesystem::path cameraFolderPath = rootFolderPath / cameraName;
            try {
                std::filesystem::create_directory(cameraFolderPath);
                cameraFolders.push_back(cameraFolderPath); // Guardar la ruta de la carpeta en el vector
            } catch (const std::filesystem::filesystem_error& err) {
                std::cerr << "Error al crear carpeta para la cámara "  << cameraName << ": " << err.what() << std::endl;
            }

            // Crear una instancia de AcquisitionUnit para este VisionSystem
            AcquisitionUnit* unitAcquisition = AcquisitionUnit::createInstance(cameraIndex, type, cameraFolderPath, IP);
            if (unitAcquisition) {
                AcquisitionUnitVector.push_back(unitAcquisition); // Almacenar la instancia creada en el vector
                std::cout << "Instancia de cámara creada para: " << cameraName << std::endl; // Agregar esta línea para verificar si se crea la instancia
                }

            // Crear una instancia de ProcessingModule para este VisionSystem
            ProcessingModule* unitProcessing = ProcessingModule::createInstance(cameraIndex, cameraFolderPath);
            if (unitProcessing) {
                ProcessingModuleVector.push_back(unitProcessing); // Almacenar la instancia creada en el vector
                std::cout << "Instancia de procesado creada para: " << cameraName << std::endl; // Agregar esta línea para verificar si se crea la instancia
                }

            // Crear una instancia de ImageBufferManager para este VisionSystem
            ImageBufferManager* unitBuffer = ImageBufferManager::createInstance(cameraIndex, cameraFolderPath, unitAcquisition, unitProcessing);
            if (unitBuffer) {
                ImageBufferManagerVector.push_back(unitBuffer); // Almacenar la instancia creada en el vector
                std::cout << "Instancia de buffer de imágenes creada para: " << cameraName << std::endl; // Agregar esta línea para verificar si se crea la instancia
            }
            
            cameraIndex++; // Incrementar el índice de la cámara aquí
            } 
        }
    }

}


int main(int argc, char *argv[]) {
    CentralUnit centralUnit;
    centralUnit.initializeSystem(); // Inicializa el sistema al inicio de la aplicación

    // Crear un vector para almacenar los hilos
    std::vector<std::thread> startThreads;

    // Crear un hilo para cada instancia de ImageBufferManager
    for (size_t i = 0; i < centralUnit.ImageBufferManagerVector.size(); ++i) {
        ImageBufferManager* bufferManager = centralUnit.ImageBufferManagerVector[i];
        startThreads.emplace_back([bufferManager, i]() {
            bufferManager->start(i);
        });
    }
    
     // Crear una instancia de RamDiskCleaner
    RamDiskCleaner cleaner(centralUnit.cameraFolders, std::chrono::seconds(2));

    // Crear un hilo para RamDiskCleaner
    startThreads.emplace_back(&RamDiskCleaner::startCleaning, &cleaner);

    // Esperar a que todos los hilos terminen después de cerrar la aplicación Qt
    for (std::thread& t : startThreads) {
        if (t.joinable()) {
            t.join();
        }
    }

    
    return 1;
}




  


 