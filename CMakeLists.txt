cmake_minimum_required(VERSION 3.10)

#set(CMAKE_CXX_STANDARD 20) incase we want c++ instead of c

project(FTMGui VERSION 1.0)

add_executable(
    FTMGui 
    ${CMAKE_SOURCE_DIR}/FTMGui/src/EntryPoint.cpp
    ${CMAKE_SOURCE_DIR}/FTMGui/src/FTMGui.cpp
    ${CMAKE_SOURCE_DIR}/FTMGui/src/Window/Window.cpp

)


target_include_directories(FTMGui PRIVATE
    ${CMAKE_SOURCE_DIR}/FTMGui/src
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin-int/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib-int/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib-int/debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin-int/release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib-int/release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib-int/release)

set(CMAKE_SUPPRESS_REGENERATION true)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/ExternalLibs/glfw)

target_link_libraries(FTMGui glfw)

#TODO: Check which os the library is being ran from
find_package(OpenGL REQUIRED)

target_link_libraries(FTMGui OpenGL::GL)

target_include_directories(glfw PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)