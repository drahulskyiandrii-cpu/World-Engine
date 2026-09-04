cmake_minimum_required(VERSION 3.25)

project(
    WorldEngine
    VERSION 0.2.0
    DESCRIPTION "World Engine modular runtime"
    LANGUAGES CXX
)

option(WORLD_ENGINE_BUILD_TESTS "Build World Engine unit tests" ON)
option(WORLD_ENGINE_BUILD_SKSE_PLUGIN "Build the Skyrim SE/AE SKSE plugin" OFF)

if(WORLD_ENGINE_BUILD_TESTS)
    include(CTest)
endif()

add_subdirectory(modules/MOD-002_Game_Adapter)
