cmake_minimum_required(VERSION 3.27)
project(cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Явное указание пути к CUDA Toolkit
set(CUDAToolkit_ROOT "/usr/local/cuda-12.6")
set(CMAKE_CUDA_COMPILER "${CUDAToolkit_ROOT}/bin/nvcc")

# Установка архитектуры CUDA
set(CMAKE_CUDA_ARCHITECTURES "89")

# Добавление путей к библиотекам CUDA
list(APPEND CMAKE_LIBRARY_PATH "${CUDAToolkit_ROOT}/lib64")
list(APPEND CMAKE_PREFIX_PATH "${CUDAToolkit_ROOT}")

# Правильное указание пути к LibTorch
set(LIBTORCH_PATH "/home/kiri4800/libtorch")
list(APPEND CMAKE_PREFIX_PATH "${LIBTORCH_PATH}")

# Поиск необходимых пакетов (CUDA должна быть перед Torch)
find_package(CUDAToolkit REQUIRED)
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
find_package(OpenCV REQUIRED)

# Отключение проверки хэша для libnvrtc (если проблема не решается)
set(CAFFE2_STATIC_LINK_CUDA ON)

find_package(Torch REQUIRED)

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
        CUDA::nvrtc  # Явное указание библиотеки nvrtc
)

# Добавление include директорий
target_include_directories(cpp PRIVATE
        ${OpenCV_INCLUDE_DIRS}
        ${TORCH_INCLUDE_DIRS}
)

# Добавление флагов компиляции для Torch
target_compile_options(cpp PRIVATE ${TORCH_CXX_FLAGS})