cmake_minimum_required(VERSION 3.16)

project(VTKAndroid VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(VTK_DIR $ENV{VTK_QTQUICK})
    message("system:qt quick")
elseif(CMAKE_SYSTEM_NAME MATCHES "Android")
    set(VTK_DIR $ENV{VTK_ANDROID})
    message("system:Android arm64-v8a")
else()
    return()
endif()

message("VTK_DIR:${VTK_DIR}")

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Widgets OpenGL Gui)

find_package(VTK REQUIRED)

qt_standard_project_setup()

qt_add_executable(libVTK
    main.cpp
)

set(LIB_SOURCE
    gui/QQuickVTKItem.cxx
    gui/QQuickVTKItem.h
    gui/QVTKInteractor.cxx
    gui/QVTKInteractor.h
    gui/QVTKInteractorAdapter.cxx
    gui/QVTKInteractorAdapter.h
    gui/QVTKInteractorInternal.h
    gui/QVTKRenderWindowAdapter.cxx
    gui/QVTKRenderWindowAdapter.h
    gui/QVTKWin32Header.h
    gui/vtkandroidinteractorstyle.h
    gui/vtkandroidinteractorstyle.cpp
    gui/vtkandroidinteractoradapter.h
    gui/vtkandroidinteractoradapter.cpp
)

qt_add_qml_module(libVTK
    URI VTKAndroid
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES ${LIB_SOURCE}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(libVTK PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.libVTK
    QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(libVTK
    PRIVATE Qt6::Quick
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::Gui
    ${VTK_LIBRARIES}
)

vtk_module_autoinit(
  TARGETS libVTK
  MODULES ${VTK_LIBRARIES}
)

include(GNUInstallDirs)
install(TARGETS libVTK
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
