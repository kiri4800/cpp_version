cmake_minimum_required(VERSION 3.27)
project(cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Принудительное указание путей CUDA перед поиском Torch
set(CUDAToolkit_ROOT "/usr/local/cuda-12.8")
set(CUDAToolkit_INCLUDE_DIR "${CUDAToolkit_ROOT}/include")
set(CUDAToolkit_LIBRARY_DIR "${CUDAToolkit_ROOT}/lib64")

# Убедитесь, что пути существуют
if(NOT EXISTS "${CUDAToolkit_INCLUDE_DIR}/cuda_runtime.h")
    message(FATAL_ERROR "cuda_runtime.h not found in ${CUDAToolkit_INCLUDE_DIR}")
endif()

# Добавление путей
list(APPEND CMAKE_PREFIX_PATH "${CUDAToolkit_ROOT}")
list(APPEND CMAKE_LIBRARY_PATH "${CUDAToolkit_LIBRARY_DIR}")
include_directories(${CUDAToolkit_INCLUDE_DIR})

# Путь к LibTorch
set(Torch_DIR "/usr/local/libtorch/share/cmake/Torch")
list(APPEND CMAKE_PREFIX_PATH "/usr/local/libtorch")

# Сначала найдем CUDA вручную
find_path(CUDA_INCLUDE_DIR cuda_runtime.h
        PATHS ${CUDAToolkit_INCLUDE_DIR}
        NO_DEFAULT_PATH
        REQUIRED
)

find_library(CUDA_CUDART_LIBRARY cudart
        PATHS ${CUDAToolkit_LIBRARY_DIR}
        NO_DEFAULT_PATH
        REQUIRED
)

message(STATUS "CUDA include dir: ${CUDA_INCLUDE_DIR}")
message(STATUS "CUDA library dir: ${CUDAToolkit_LIBRARY_DIR}")

# Поиск остальных пакетов
find_package(OpenCV REQUIRED)
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Переменные для обхода проверки Torch
set(CAFFE2_STATIC_LINK_CUDA OFF)
set(CUDAToolkit_FOUND TRUE)
set(CUDAToolkit_VERSION 12.8.61)

# Поиск Torch
#find_package(Torch REQUIRED)

add_executable(cpp main.cpp
        FullScreen/FullScreen.h
        FullScreen/FullScreen.cpp
        Model/Model.cpp
        Model/Model.h)

target_link_libraries(cpp
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        ${OpenCV_LIBS}
        ${TORCH_LIBRARIES}
        ${CUDA_CUDART_LIBRARY}
)

target_include_directories(cpp PRIVATE
        ${OpenCV_INCLUDE_DIRS}
        ${TORCH_INCLUDE_DIRS}
        ${CUDA_INCLUDE_DIR}
)

target_compile_options(cpp PRIVATE ${TORCH_CXX_FLAGS})