cmake_minimum_required(VERSION 3.10)

project(gtk3-throwback C)

# Check for GTK 3
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0>=3.24)

# Set directories
set(CMAKE_INSTALL_PREFIX "/usr")
include(GNUInstallDirs)
include_directories(${GTK3_INCLUDE_DIRS})

# Find source files
file(GLOB SOURCE src/*.c)

add_library(${PROJECT_NAME} SHARED ${SOURCE})

target_include_directories(${PROJECT_NAME} PRIVATE ${GTK3_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${GTK3_CFLAGS_OTHER})
target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES})

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-deprecated-declarations)
endif()

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/gtk-3.0/modules)

