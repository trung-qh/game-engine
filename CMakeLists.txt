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

# Some vendored static libraries contain platform-specific files that compile
# to empty objects on macOS. This keeps Apple's ranlib from warning about them.
set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> qcS <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> qcS <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_OBJC_ARCHIVE_CREATE "<CMAKE_AR> qcS <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_OBJCXX_ARCHIVE_CREATE "<CMAKE_AR> qcS <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_C_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols <TARGET>")
set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols <TARGET>")
set(CMAKE_OBJC_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols <TARGET>")
set(CMAKE_OBJCXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols <TARGET>")

set(VENDOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/engine/vendor")

# A fresh clone may not have submodules yet. If a vendored dependency is
# missing, ask Git to fetch it before CMake gives up.
function(require_vendored_file path)
  if(EXISTS "${path}")
    return()
  endif()

  find_package(Git QUIET)
  if(Git_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
    message(STATUS "Fetching vendored dependencies with git submodule update...")
    execute_process(
      COMMAND "${GIT_EXECUTABLE}" submodule update --init --recursive
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      RESULT_VARIABLE submodule_result
    )
  endif()

  if(NOT EXISTS "${path}")
    message(FATAL_ERROR
      "Missing vendored dependency: ${path}\n"
      "Run: git submodule update --init --recursive")
  endif()
endfunction()

require_vendored_file("${VENDOR_DIR}/SDL/CMakeLists.txt")
require_vendored_file("${VENDOR_DIR}/SDL_ttf/CMakeLists.txt")
require_vendored_file("${VENDOR_DIR}/SDL_ttf/external/freetype/CMakeLists.txt")
require_vendored_file("${VENDOR_DIR}/SDL_ttf/external/harfbuzz/CMakeLists.txt")
require_vendored_file("${VENDOR_DIR}/SDL_ttf/external/plutosvg/CMakeLists.txt")
require_vendored_file("${VENDOR_DIR}/SDL_ttf/external/plutovg/CMakeLists.txt")

# The engine is a .dylib, but SDL and SDL_ttf are built as static libraries
# and linked into it. PIC is needed when static code becomes part of a dylib.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_DISABLE_FIND_PACKAGE_PkgConfig ON CACHE BOOL "" FORCE)

# Keep SDL focused on the library we need.
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "" FORCE)

# Keep SDL_ttf self-contained: use its vendored font/shaping/svg libraries.
set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)
set(SDLTTF_STRICT ON CACHE BOOL "" FORCE)
set(SDLTTF_HARFBUZZ ON CACHE BOOL "" FORCE)
set(SDLTTF_PLUTOSVG ON CACHE BOOL "" FORCE)
set(SDLTTF_SAMPLES OFF CACHE BOOL "" FORCE)
set(SDLTTF_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory("${VENDOR_DIR}/SDL" "${CMAKE_CURRENT_BINARY_DIR}/vendor/SDL" EXCLUDE_FROM_ALL)
add_subdirectory("${VENDOR_DIR}/SDL_ttf" "${CMAKE_CURRENT_BINARY_DIR}/vendor/SDL_ttf" EXCLUDE_FROM_ALL)

# Vendored libraries can be chatty on newer compilers. Silence warnings for
# third-party targets only, so warnings in engine code still remain visible.
function(silence_vendor_target target_name)
  if(TARGET "${target_name}")
    target_compile_options("${target_name}" PRIVATE
      $<$<COMPILE_LANGUAGE:C>:-w>
      $<$<COMPILE_LANGUAGE:CXX>:-w>
      $<$<COMPILE_LANGUAGE:OBJC>:-w>
      $<$<COMPILE_LANGUAGE:OBJCXX>:-w>
    )
  endif()
endfunction()

foreach(vendor_target
    SDL3-static
    SDL3_ttf-static
    freetype
    harfbuzz
    plutosvg
    plutovg)
  silence_vendor_target("${vendor_target}")
endforeach()

add_library(engine SHARED
  engine/src/Application.cpp
  engine/src/platform/renderer/Renderer.cpp
  engine/src/platform/window/Window.cpp
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
  PRIVATE
    SDL3::SDL3-static
    SDL3_ttf::SDL3_ttf-static
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
  COMMAND "${CMAKE_COMMAND}" -E copy
          "$<TARGET_FILE:engine>"
          "${ENGINE_DIST_DIR}/lib/$<TARGET_FILE_NAME:engine>"
  DEPENDS engine
  COMMENT "Bundling engine headers and dylib in ${ENGINE_DIST_DIR}"
)