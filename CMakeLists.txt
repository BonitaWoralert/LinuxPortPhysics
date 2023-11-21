cmake_minimum_required(VERSION 3.26)
project(LinuxPort)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL)
find_package(GLUT)

include_directories(${GLUT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${OPEN_EGL_INCLUDE_DIRS})

add_executable(LinuxPort main.cpp
        Vec3.h
        BoxManager.cpp
        BoxManager.h)
target_link_libraries(LinuxPort ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
