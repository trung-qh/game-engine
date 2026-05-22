cmake_minimum_required(VERSION 3.28)

project(snake LANGUAGES CXX)

set(ENGINE_DIST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../build/engine")
set(SHARED_RES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../res")

if(NOT EXISTS "${ENGINE_DIST_DIR}/include/engine/Engine.h"
   OR NOT EXISTS "${ENGINE_DIST_DIR}/lib/libengine.dylib")
  message(FATAL_ERROR
    "Engine bundle not found. Build it from the repo root first: "
    "cmake --build build --target engine_dist"
  )
endif()

add_executable(snake src/Main.cpp)

target_compile_features(snake PRIVATE cxx_std_20)
target_include_directories(snake PRIVATE "${ENGINE_DIST_DIR}/include")
target_compile_definitions(snake PRIVATE SNAKE_RES_DIR="${SHARED_RES_DIR}")
target_link_libraries(snake PRIVATE "${ENGINE_DIST_DIR}/lib/libengine.dylib")

set_target_properties(snake PROPERTIES
  BUILD_RPATH "${ENGINE_DIST_DIR}/lib"
)
