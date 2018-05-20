cmake_minimum_required(VERSION 3.10)
project(180424_203124_esp12e)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)
include_directories(src/OpenSenseMap)

add_executable(180424_203124_esp12e
        src/OpenSenseMap/Sender.cpp
        src/OpenSenseMap/Sender.h
        src/Sensor.cpp
        src/Sensor.h
        src/Settings.h
        platformio.ini)
