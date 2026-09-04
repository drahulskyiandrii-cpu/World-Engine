add_library(world_engine_game_adapter STATIC
    src/game_adapter.cpp
)

add_library(WorldEngine::GameAdapter ALIAS world_engine_game_adapter)

target_include_directories(world_engine_game_adapter
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(world_engine_game_adapter PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(world_engine_game_adapter PRIVATE /W4 /WX /permissive- /EHsc)
else()
    target_compile_options(world_engine_game_adapter PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

if(WORLD_ENGINE_BUILD_TESTS)
    add_executable(world_engine_game_adapter_tests
        tests/game_adapter_tests.cpp
    )
    target_link_libraries(world_engine_game_adapter_tests PRIVATE WorldEngine::GameAdapter)
    target_compile_features(world_engine_game_adapter_tests PRIVATE cxx_std_20)

    if(MSVC)
        target_compile_options(world_engine_game_adapter_tests PRIVATE /W4 /WX /permissive- /EHsc)
    else()
        target_compile_options(world_engine_game_adapter_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()

    add_test(NAME MOD002.GameAdapter COMMAND world_engine_game_adapter_tests)
endif()

if(WORLD_ENGINE_BUILD_SKSE_PLUGIN)
    add_subdirectory(skyrim_plugin)
endif()
