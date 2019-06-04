cmake_minimum_required(VERSION 3.5)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(Listpad)

# headers
file (GLOB HEADERS
    "*.h"
    "core/*.h"
    "engines/*.h"
    "handlers/*.h"
    "tools/*.h"
    "widgets/elements/*.h")

# sources
file (GLOB SRCS
    "*.cpp"
    "core/*.cpp"
    "engines/*.cpp"
    "handlers/*.cpp"
    "tools/*.cpp"
    "widgets/elements/*.cpp")

# libs
set (LIBS)

# qt .ui
file (GLOB QTUI
    "*.ui"
    "widgets/elements/*.ui")

# qt .qrc
file (GLOB QRC
    "*.qrc")

find_package(Qt5Widgets REQUIRED)
qt5_add_resources(QT_QRC ${QRC})
qt5_wrap_ui(QTUI_HEADERS ${QTUI})

# HEADERS is added to be showed in IDE
add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS} ${QTUI_HEADERS} ${QT_QRC} windows.rc)
set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE TRUE)
target_link_libraries(${PROJECT_NAME} ${LIBS})

qt5_use_modules(${PROJECT_NAME} Widgets)
