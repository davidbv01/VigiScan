# AI-Powered Image Processing & RamDisk Cleaner

Welcome to this repository, where AI meets optimization! 🚀 This project integrates YOLO_V8 image processing with a highly scalable architecture, offering a powerful solution for object detection and system storage management.

## 🌟 Scalable Architecture
One of the standout features of this project is its scalable and modular architecture. The system is designed to handle increasing complexity, making it suitable for large-scale deployments.

CentralUnit: Governs the entire process flow, ensuring that each part of the system operates efficiently and communicates effectively. This central management allows the system to scale and adapt to various use cases without major redesigns.

AcquisitionUnits: These units are responsible for acquiring images from different sources (e.g., cameras) and passing them to the ImageBuffer.

ImageBuffer: The buffer manages a queue of images, acting as a mediator between the acquisition process and the processing module. It ensures that images are processed in the correct order, optimizing performance.

ProcessingModule (YOLO Detection): This module takes the images from the buffer and performs the object detection process using YOLO. It processes images one at a time, detecting objects and drawing bounding boxes, ensuring that even with high volumes of images, the system can scale efficiently.

Why is this architecture scalable?
Centralized control (via CentralUnit) means that additional AcquisitionUnits, ImageBuffers, or even ProcessingModules can be added easily without disrupting the entire system.

You can add more image sources or detection tasks and distribute the workload across multiple modules. As the number of cameras or detection units grows, the system continues to perform without significant performance degradation.

The image processing pipeline is highly decoupled, which means that parts of the system can be upgraded or replaced independently, keeping the architecture flexible and adaptable to future requirements.

## 🛠 Installation

To get started with this project, follow the steps below.

### Prerequisites:

1. **Install ImDisk**:  
   To use the RamDisk functionality, you need to install [ImDisk](https://www.ltr-data.se/opencode.html/#ImDisk). Once installed, create a disk (e.g., `F:`) which will be used to store the processed images between processes.

2. **OpenCV with CUDA**:  
   For optimal inference time, **OpenCV** should be installed with **CUDA support**. You can follow this [tutorial](https://www.youtube.com/watch?v=d8Jx6zO1yw0) to set up OpenCV with CUDA.

3. **CMake**:  
   You'll also need **CMake** installed for building the project.

4. **C++ Compiler**:  
   Ensure that you have a compatible C++ compiler installed (e.g., GCC, Clang).

Here’s the corrected version of your steps:

### Steps:

1. **Clone the repository:**
   ```bash
   git clone https://github.com/davidbv01/VigiScan.git
   cd VigiScan
   ```

2. **Build the project:**

   1. **Create a build directory:**
      ```bash
      mkdir build
      cd build
      ```

   2. **Run CMake to generate the Makefile:**
      ```bash
      cmake ..
      ```

   3. **Build the project:**
      ```bash
      make
      ```

3. **Set up the environment:**

   Make sure the YOLO model file (`best.onnx`) and the `databest.yaml` are correctly set up and located in the project directory.

4. **Run the application:**

   Use the compiled executable to start processing images or cleaning old files:
   ```bash
   ./imageProcessor
   ``` 

## 🎯 Key Features

- **Real-time Object Detection:** The YOLO model detects objects in real-time and provides bounding boxes for identified objects in images.
  
- **File Cleaning:** The RamDisk Cleaner module runs in the background, automatically deleting files that have surpassed the defined age limit, ensuring efficient storage usage.
  
- **Customizable:** Both modules are highly configurable, allowing you to change parameters like object detection thresholds and the age limit for file cleaning.

## ⚙️ Configuration

### YOLO Object Detection
- **Confidence Threshold:** Controls the minimum confidence level for detected objects. Objects below this threshold will be ignored.
  
- **IOU Threshold:** Used for non-maximum suppression to avoid multiple bounding boxes for the same object.
  
- **Image Size:** Defines the input image resolution for the YOLO model.

### RamDisk Cleaner
- **Age Limit:** Sets the maximum age of files (in seconds) before they are deleted.

## 📝 License
This project is licensed under the MIT License - see the LICENSE file for details.

## 🤝 Contributing
Feel free to contribute by submitting pull requests, reporting issues, or suggesting new features. This project thrives with your contributions!

If you have any questions or need assistance, don't hesitate to reach out. Happy coding! 👨‍💻🚀
