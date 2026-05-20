add_library(engine_entity STATIC
  src/entity/Registry.cpp
)

add_library(engine::entity ALIAS engine_entity)

target_include_directories(engine_entity
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
