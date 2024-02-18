cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)
project(StrataSolutionsWorkers)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Charts)


set(INCLUDES
    include/mainwindow.h
    include/generateWorker.h
)

set(SRC
    src/main.cpp
    src/mainwindow.cpp
    src/generateWorker.cpp
)

set(FORMS
    forms/mainwindow.ui
)

set(RESOURCES
    resourses/resourses.qrc
)

qt5_wrap_ui(UI_HEADERS ${FORMS})

add_executable(
    ${PROJECT_NAME}
    ${INCLUDES}
    ${SRC}
    ${FORMS}
    ${RESOURCES}
)


target_include_directories(${PROJECT_NAME}
    PUBLIC
    ./include
    PRIVATE
    ./src
    ./forms
    ./resourses
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Charts
        )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)


