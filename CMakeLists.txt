cmake_minimum_required(VERSION 3.5)

project(ImageCompression VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

#file(GLOB SOURCES "src/*.cpp")

set(SOURCES src/main.cpp src/Image.cpp src/ImageIO.cpp src/tools.cpp)

set(SOURCES_2 src/main2.cpp src/Image.cpp src/ImageIO.cpp src/tools.cpp src/LZWcompress.cpp
	src/HuffmanCompression.cpp)

set(SOURCES_3 src/main3.cpp src/Image.cpp src/ImageIO.cpp src/tools.cpp src/LZWcompress.cpp
	src/HuffmanCompression.cpp)


add_executable(main ${SOURCES})
add_executable(main2 ${SOURCES_2})
add_executable(main3 ${SOURCES_3})

