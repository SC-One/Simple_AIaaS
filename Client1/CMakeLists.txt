cmake_minimum_required(VERSION 3.5)

project(Client1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick LinguistTools)


# If you installed grpc with vcpkg
# please use below command and involve output explaination
# vcpkg integrate install
find_package(gRPC CONFIG REQUIRED)
set(TS_FILES Client1_fa_IR.ts)

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE proto_out_srcs
    ${INC_DIR}/proto_out/*.cpp
    ${INC_DIR}/proto_out/*.cc
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

set(PROJECT_SOURCES
  main.cpp
  qml.qrc
  ${TS_FILES}

  ${HEADER_FILES}
  ${SOURCE_FILES}
)

include_directories(
    ${INC_DIR}
)

add_executable(Client1
  ${PROJECT_SOURCES}
)

target_link_libraries(Client1
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Quick
  gRPC::grpc gRPC::grpc++
)


#if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#  qt_add_executable(Client1
#    MANUAL_FINALIZATION
#    ${PROJECT_SOURCES}
#  )
## Define target properties for Android with Qt 6 as:
##    set_property(TARGET Client1 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
##                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
## For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

#qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
#else()
#  if(ANDROID)
#    add_library(Client1 SHARED
#      ${PROJECT_SOURCES}
#    )
#  # Define properties for Android with Qt 5 after find_package() calls as:
#  #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#else()
##  add_executable(Client1
##    ${PROJECT_SOURCES}
##  )
#endif()

#qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
#endif()


#set_target_properties(Client1 PROPERTIES
#  MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
#  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#  MACOSX_BUNDLE TRUE
#  WIN32_EXECUTABLE TRUE
#)

#install(TARGETS Client1
#  BUNDLE DESTINATION .
#  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

#if(QT_VERSION_MAJOR EQUAL 6)
#  qt_import_qml_plugins(Client1)
#  qt_finalize_executable(Client1)
#endif()


