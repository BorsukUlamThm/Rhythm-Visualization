cmake_minimum_required(VERSION 3.23)
project(Rythme-Visualization)
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
        src/model/Rythme.cpp
        src/model/Rythme.h)
set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin})
