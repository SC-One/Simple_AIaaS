cmake_minimum_required(VERSION 3.16)

set(ROOT_PROJECT_NAME Server)
project(${ROOT_PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui)
find_package(Threads REQUIRED)
include(common.cmake) # copied from grpc examples :D , I tested generating from cmake ,gprc has problem.

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(THIRD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty)
set(DARKNET_DIR ${THIRD_DIR}/darknet)
#add_compile_definitions(DARKNET_ROOT_DIRECTORY=${DARKNET_DIR})

set(PROTOS
    ${CMAKE_CURRENT_SOURCE_DIR}/../protos/image.proto
)

file(GLOB_RECURSE proto_out_srcs
    ${INC_DIR}/proto_out/*.cpp
    ${INC_DIR}/proto_out/*.cc
)

file(GLOB_RECURSE HEADER_FILES
    ${INC_DIR}/*.hpp
    ${INC_DIR}/*.hxx
    ${INC_DIR}/*.h
)

file(GLOB_RECURSE SOURCE_FILES
    ${SRC_DIR}/*.cpp
    ${SRC_DIR}/*.cxx
    ${SRC_DIR}/*.cc
    ${SRC_DIR}/*.c
    ${proto_out_srcs}
)

include_directories(
    ${INC_DIR}
    ${DARKNET_DIR}/include/
)

add_executable(${ROOT_PROJECT_NAME}
  main.cpp
  ${HEADER_FILES}
  ${SOURCE_FILES}
)

target_link_libraries(${ROOT_PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Threads::Threads
    gRPC::grpc++
)

#install(TARGETS ${ROOT_PROJECT_NAME}
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
