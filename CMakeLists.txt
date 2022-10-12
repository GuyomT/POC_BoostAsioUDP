cmake_minimum_required(VERSION 3.16.0)

project(rtype LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CLIENT_SRC
    ${PROJECT_SOURCE_DIR}/udpClient.cpp
)

set(SERVER_SRC
    ${PROJECT_SOURCE_DIR}/Server.cpp
    ${PROJECT_SOURCE_DIR}/Server.hpp
    ${PROJECT_SOURCE_DIR}/IServer.hpp
    ${PROJECT_SOURCE_DIR}/LockedQueue.hpp
)

set (SERVER_TARGET udpServer)
set (CLIENT_TARGET udpClient)

add_executable(${CLIENT_TARGET} ${CLIENT_SRC})
set_target_properties(udpClient PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(${SERVER_TARGET} ${SERVER_SRC})
set_target_properties(udpServer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})