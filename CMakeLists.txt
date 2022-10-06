cmake_minimum_required(VERSION 3.0)
project(GeneralGraphicsFile)

if(MSVC)
message("MSVC")
add_definitions(-D_USE_MATH_DEFINES)
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

#find_package(PCL 1.8 REQUIRED)

set(CMAKE_MODULE_PATH  "D:/3rdParties/cmake/Modules")

find_package(Boost REQUIRED)

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

add_executable(test_bin test.cpp)
target_link_libraries(test_bin general_graphics_lib)
install(TARGETS test_bin DESTINATION bin)
