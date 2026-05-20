cmake_minimum_required(VERSION 3.28)
project(engine LANGUAGES C CXX)

# This project intentionally supports Apple platforms only.
if(NOT APPLE)
  message(FATAL_ERROR "game_engine currently supports Apple platforms only.")
endif()

# Project-wide language settings. Targets below can ask for newer features,
# but these defaults keep every target in the same C++ dialect.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# The engine is a .dylib. PIC is needed when static module code becomes part of it.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_subdirectory(modules/core)
add_subdirectory(modules/entity)
add_subdirectory(modules/platform)

add_library(engine SHARED
  engine/src/ecs/World.cpp
  engine/src/runtime/Application.cpp
)

add_library(engine::engine ALIAS engine)

# PUBLIC includes are visible to users of engine; PRIVATE includes are only
# for engine's own implementation files.
target_include_directories(engine
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/engine/include"
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/engine/src"
)

target_link_libraries(engine
  PUBLIC
    engine::core
    engine::entity
  PRIVATE
    engine::platform
)

# A bundle target is a convenience target for shipping the engine. It creates
# build/engine so another project can copy that folder into its vendor folder.
set(ENGINE_DIST_DIR "${CMAKE_CURRENT_BINARY_DIR}/engine")

add_custom_target(engine_dist ALL
  COMMAND "${CMAKE_COMMAND}" -E rm -rf "${ENGINE_DIST_DIR}"
  COMMAND "${CMAKE_COMMAND}" -E make_directory "${ENGINE_DIST_DIR}/include"
  COMMAND "${CMAKE_COMMAND}" -E make_directory "${ENGINE_DIST_DIR}/lib"
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
          "${CMAKE_CURRENT_SOURCE_DIR}/engine/include/engine"
          "${ENGINE_DIST_DIR}/include/engine"
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
          "${CMAKE_CURRENT_SOURCE_DIR}/modules/core/include/engine/core"
          "${ENGINE_DIST_DIR}/include/engine/core"
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
          "${CMAKE_CURRENT_SOURCE_DIR}/modules/entity/include/engine/entity"
          "${ENGINE_DIST_DIR}/include/engine/entity"
  COMMAND "${CMAKE_COMMAND}" -E copy
          "$<TARGET_FILE:engine>"
          "${ENGINE_DIST_DIR}/lib/$<TARGET_FILE_NAME:engine>"
  DEPENDS engine
  COMMENT "Bundling engine headers and dylib in ${ENGINE_DIST_DIR}"
)
