cmake_minimum_required(VERSION 3.21)
project(Rhythm-Visualization)
set(CMAKE_CXX_STANDARD 14)


# Some directories
set(bin ${CMAKE_SOURCE_DIR}/bin)
set(data ${CMAKE_SOURCE_DIR}/data)
file(MAKE_DIRECTORY ${bin})
file(MAKE_DIRECTORY ${data})


# Executable
set(exe_name rythme-visualization)
add_executable(${exe_name}
        src/main.cpp
        src/model/Rhythm.cpp
        src/model/Rhythm.h
        src/view/Canvas.cpp
        src/view/Canvas.h)

target_link_libraries(${exe_name} -lsfml-graphics -lsfml-window -lsfml-system)
set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin})
