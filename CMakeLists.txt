cmake_minimum_required(VERSION 3.17)
project(CPP_VIGISCAN VERSION 0.1.0 LANGUAGES C CXX) 

set(CMAKE_INCLUDE_CURRENT_DIR ON) # Habilitar la inclusión del directorio actual en la búsqueda de archivos de encabezado


include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17) # Enforce C++17 
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Make C++17 a hard requirement

# Suppress warning related to the removal of the FindCUDA module
cmake_policy(SET CMP0146 OLD)

# -------------- OPENCV  ------------------#
set(OpenCV_DIR "C:/LoboScan/OpenCV4.7/build/install/x64/vc17") 
set(OpenCV_INCLUDE_DIRS "C:/LoboScan/OpenCV4.7/build/install/include")
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui tracking)

# -------------- CUDA  ------------------#
set(CMAKE_CUDA_STANDARD 11)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

# -------------- ONNXRUNTIME  ------------------#
set(ONNXRUNTIME_VERSION 1.16.2)
set(ONNXRUNTIME_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/Libs/onnxruntime-win-x64-gpu-${ONNXRUNTIME_VERSION}")

# -------------- RAPIDXML  ------------------#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# -------------- EIGEN  ------------------#
set(Eigen3_INCLUDE_DIR "C:/Users/david/OneDrive/Desktop/CPP_VIGISCAN/Libs")

# -------------- QT (GUI)  ------------------#
#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
#find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
#include_directories("C:/Qt/6.8.0/msvc2019_64/include")

# Save the command line compile commands in the build output
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Make project require C++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

# Definición del ejecutable
add_executable(CPP_VIGISCAN
  src/CentralUnit.cpp
  src/AcquisitionUnit.cpp
  src/ImageBufferManager.cpp
  src/inference.cpp
  src/tracker.cpp
  src/ProcessingModule.cpp
  src/RamDiskCleaner.cpp
  src/resource.rc
)

include_directories(${PROJECT_NAME} 
${ONNXRUNTIME_ROOT}/include 
${ONNXRUNTIME_ROOT}/include/core
${ONNXRUNTIME_ROOT}/include/core/sessions
${ONNXRUNTIME_ROOT}/include/core/providers
${PROJECT_SOURCE_DIR}/include
${PROJECT_SOURCE_DIR}/src
${Eigen3_INCLUDE_DIR}
${OpenCV_INCLUDE_DIRS}
)

# Vincular las dependencias
target_link_libraries(CPP_VIGISCAN
  ${OpenCV_LIBS}
  ${Python_LIBRARIES}
)
set_target_properties(CPP_VIGISCAN PROPERTIES FOLDER Examples)


install(
    TARGETS
    CPP_VIGISCAN
    RUNTIME DESTINATION
    ${CMAKE_INSTALL_PREFIX}/bin
)

# CPack (Empaquetado)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${ONNXRUNTIME_ROOT}/lib/onnxruntime.lib )

# Copy onnxruntime.dll to the same folder of the executable file
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
"${ONNXRUNTIME_ROOT}/lib/onnxruntime.dll"
$<TARGET_FILE_DIR:${PROJECT_NAME}>)
