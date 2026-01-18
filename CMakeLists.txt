cmake_minimum_required(VERSION 3.0)
project(GeneralGraphicsFile)

if(MSVC)
message("MSVC")
add_definitions(-D_USE_MATH_DEFINES)
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

find_package(Eigen REQUIRED)


file(GLOB INC_LIST   "./include/*")
file(GLOB SRC_LIST   "./src/*.cpp")

include_directories(./include/)

add_library(general_graphics_lib
${INC_LIST}
${SRC_LIST}
)

target_link_libraries(general_graphics_lib ${BOOST_LIBRARIES})

install(TARGETS general_graphics_lib DESTINATION lib/${CMAKE_BUILD_TYPE})

link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(graphics_test graphics_test.cpp)
target_link_libraries(graphics_test general_graphics_lib)
install(TARGETS graphics_test DESTINATION bin)
