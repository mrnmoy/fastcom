cmake_minimum_required(VERSION 3.16)

project(fastcom VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick SerialPort)

qt_standard_project_setup(REQUIRES 6.11)

qt_add_executable(appfastcom
    main.cpp
    serialManager.cpp
    serialManager.h
)

# set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
# set(QML_IMPORT_PATH ${CMAKE_SOURCE_DIR}/qml ${CMAKE_BINARY_DIR}/imports CACHE STRING "" FORCE)

qt_add_qml_module(appfastcom
    URI fastcom
    QML_FILES
        Main.qml
    RESOURCES
        res/images/ic_bin.png
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appfastcom PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appfastcom
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appfastcom
    PRIVATE 
        Qt6::Quick 
        Qt6::SerialPort
)

install(TARGETS appfastcom
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
