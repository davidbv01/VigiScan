# AI-Powered Image Processing & RamDisk Cleaner

Welcome to this repository, where AI meets optimization! 🚀 This project integrates two powerful modules:

1. **YOLO Object Detection**: An AI-powered image processing module that detects objects using YOLO_V8, draws bounding boxes around detected items, and saves the images for further analysis.
   
2. **RamDisk Cleaner**: A utility that keeps your system running smoothly by automatically cleaning old files from your RamDisk (or any specified directories) based on an age threshold.

## 🚀 Project Overview

This repository consists of two key components:

### 1. **Image Processing with YOLO (You Only Look Once)**

- This module utilizes a pre-trained YOLO_V8 model to perform real-time object detection in images. It automatically draws bounding boxes around detected objects and saves the processed images with unique filenames based on the timestamp.
  
  #### Key Functions:
  - **`Process(cv::Mat& frame, int cameraID)`**: This function processes an image, performs object detection using the YOLO_V8 model, and draws bounding boxes on detected objects.
  - **`saveImage(const cv::Mat& image)`**: Saves the processed image to a specified directory with a unique filename that includes the current timestamp.
  - **`ReadCocoYaml(YOLO_V8*& p)`**: Reads a YAML file containing the class names that the YOLO model should recognize.

### 2. **RamDisk Cleaner**

- This module helps you optimize your storage by cleaning old files from a RamDisk or specified directories based on a configurable age limit. It runs in the background and ensures your system doesn’t accumulate outdated files.

  #### Key Features:
  - **`startCleaning()`**: Starts the cleaning process by running in a loop, checking and removing files that are older than the defined age limit.
  - **`cleanOldFiles()`**: Scans specified directories and removes files that exceed the specified age threshold.

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
