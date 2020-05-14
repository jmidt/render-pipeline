cmake_minimum_required(VERSION 3.10)

# set the project name
project(Rendering VERSION 1.0)

# configure header file
configure_file(RenderingConfig.h.in RenderingConfig.h)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(Rendering main.cpp)
# include directories
target_include_directories(Rendering PUBLIC "${PROJECT_BINARY_DIR}") 

# specify libraries
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries")

set(GLFW_DIR "${LIB_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(Rendering "glfw" "${GLFW_LIBRARIES}")
target_include_directories(Rendering PRIVATE ${GLFW_DIR}/include)
target_compile_definitions(Rendering PRIVATE "GLFW_INCLUDE_NONE")

set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE ${GLAD_DIR}/include)
target_include_directories(Rendering PRIVATE ${GLAD_DIR}/include)
target_link_libraries(Rendering "glad" "${GLAD_LIBRARIES}")