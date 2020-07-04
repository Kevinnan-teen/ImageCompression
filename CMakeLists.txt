cmake_minimum_required(VERSION 3.5)

project(ImageCompression VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

#file(GLOB SOURCES "src/*.cpp")

set(SOURCES src/Image.cpp src/ImageIO.cpp src/main.cpp src/tools.cpp)


add_executable(main ${SOURCES})

