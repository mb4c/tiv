cmake_minimum_required(VERSION 3.26)
project(tiv)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/glad)

find_package(OpenImageIO REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

file(GLOB_RECURSE tiv_src src/*.cpp src/*.hpp)

add_executable(tiv ${tiv_src})
target_link_libraries(tiv PRIVATE glad glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} OpenImageIO::OpenImageIO glm::glm)

install(TARGETS tiv)
install(FILES tiv.desktop DESTINATION share/applications )