set(PLATFORM_VENDOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor")

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

# A fresh clone may not have submodules yet. If a vendored dependency is
# missing, ask Git to fetch it before CMake gives up.
function(require_platform_vendored_file path)
  if(EXISTS "${path}")
    return()
  endif()

  find_package(Git QUIET)
  if(Git_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    message(STATUS "Fetching platform vendored dependencies with git submodule update...")
    execute_process(
      COMMAND "${GIT_EXECUTABLE}" submodule update --init --recursive
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
      RESULT_VARIABLE submodule_result
    )
  endif()

  if(NOT EXISTS "${path}")
    message(FATAL_ERROR
      "Missing platform vendored dependency: ${path}\n"
      "Run: git submodule update --init --recursive")
  endif()
endfunction()

require_platform_vendored_file("${PLATFORM_VENDOR_DIR}/SDL/CMakeLists.txt")
require_platform_vendored_file("${PLATFORM_VENDOR_DIR}/SDL_ttf/CMakeLists.txt")
require_platform_vendored_file("${PLATFORM_VENDOR_DIR}/SDL_ttf/external/freetype/CMakeLists.txt")
require_platform_vendored_file("${PLATFORM_VENDOR_DIR}/SDL_ttf/external/harfbuzz/CMakeLists.txt")
require_platform_vendored_file("${PLATFORM_VENDOR_DIR}/SDL_ttf/external/plutosvg/CMakeLists.txt")
require_platform_vendored_file("${PLATFORM_VENDOR_DIR}/SDL_ttf/external/plutovg/CMakeLists.txt")

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

add_subdirectory("${PLATFORM_VENDOR_DIR}/SDL" "${CMAKE_CURRENT_BINARY_DIR}/vendor/SDL" EXCLUDE_FROM_ALL)
add_subdirectory("${PLATFORM_VENDOR_DIR}/SDL_ttf" "${CMAKE_CURRENT_BINARY_DIR}/vendor/SDL_ttf"
                 EXCLUDE_FROM_ALL)

# Vendored libraries can be chatty on newer compilers. Silence warnings for
# third-party targets only, so warnings in engine and platform code still remain visible.
function(silence_platform_vendor_target target_name)
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
  silence_platform_vendor_target("${vendor_target}")
endforeach()

add_library(engine_platform STATIC
  src/Context.cpp
  src/errors/SdlError.cpp
  src/renderer/Font.cpp
  src/renderer/Renderer.cpp
  src/renderer/Text.cpp
  src/time/Clock.cpp
  src/window/Window.cpp
)

add_library(engine::platform ALIAS engine_platform)

target_include_directories(engine_platform
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(engine_platform
  PUBLIC
    engine::core
    SDL3::SDL3-static
    SDL3_ttf::SDL3_ttf-static
)
