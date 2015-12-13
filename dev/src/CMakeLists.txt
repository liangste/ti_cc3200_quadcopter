include_directories(inc)
project(QuadUI_main)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

add_subdirectory(joystick)

add_executable(QuadUI main.cc)

qt5_use_modules(QuadUI Widgets Core Gui)
target_link_libraries(QuadUI joystick)
