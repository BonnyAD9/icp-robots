cmake_minimum_required(VERSION 3.16)

project(icp-robots VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(icp-robots
    main.cpp
    window.cpp
    window.hpp
    menu.cpp
    menu.hpp
    obstacle.cpp
    obstacle.hpp
    room.cpp
    room.hpp
    robot.cpp
    robot.hpp
    auto_robot.cpp
    auto_robot.hpp
    sim_controls.cpp
    sim_controls.hpp
    redit_menu.cpp
    redit_menu.hpp
    control_robot.cpp
    control_robot.hpp
    loader.cpp
    loader.hpp
    scene_obj.cpp
    scene_obj.hpp
)

# TODO: REMOVE SANITIZE BEFORE SUBMIT
# add_compile_options(-fsanitize=address)

target_link_libraries(icp-robots PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
#     -fsanitize=address
)
