cmake_minimum_required(VERSION 2.8) 
project(blit-test CXX C)

find_package(OpenGL REQUIRED)
MESSAGE(STATUS "found OPENGL, lib = " ${OPENGL_LIBRARIES} )
add_definitions(-DGL_GLEXT_PROTOTYPES )

find_package(Qt4 COMPONENTS QtCore QtGui QtMain QtOpenGL REQUIRED)
INCLUDE(${QT_USE_FILE})

file(GLOB SOURCE *.cpp)

# Now add the binary
add_executable(blit-test ${SOURCE})

target_link_libraries(blit-test ${QT_LIBRARIES} ${OPENGL_LIBRARIES})
