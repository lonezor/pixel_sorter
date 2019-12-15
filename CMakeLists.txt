cmake_minimum_required(VERSION 3.0)

project(pixel_sorter)

set(CMAKE_BUILD_TYPE Debug) # Release / Debug

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK REQUIRED gtk+-3.0)

file(GLOB SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
  pthread
  ${GTK_LIBRARIES}
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${GTK_INCLUDE_DIRS}
  include
)

target_compile_options(${PROJECT_NAME}
  PUBLIC
  ${CAIRO_CFLAGS_OTHER})

