cmake_minimum_required(VERSION 3.16)

set(ROOT_PROJECT_NAME Server)
project(${ROOT_PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Threads REQUIRED)
include(common.cmake) # copied from grpc examples :D , I tested generating from cmake ,gprc has problem.

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PROTOS
    ${CMAKE_CURRENT_SOURCE_DIR}/../protos/image.proto
)

file(GLOB_RECURSE proto_out_files
    ${INC_DIR}/proto_out/*.hpp
    ${INC_DIR}/proto_out/*.h
    ${INC_DIR}/proto_out/*.cpp
    ${INC_DIR}/proto_out/*.cc
)

include_directories(${INC_DIR})

add_executable(${ROOT_PROJECT_NAME}
  main.cpp
  ${proto_out_files}
)

target_link_libraries(${ROOT_PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Threads::Threads
    gRPC::grpc++
)

#install(TARGETS ${ROOT_PROJECT_NAME}
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
