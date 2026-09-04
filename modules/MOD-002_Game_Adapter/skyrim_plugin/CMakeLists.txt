find_package(CommonLibSSE CONFIG REQUIRED)

add_commonlibsse_plugin(WorldEngineGameAdapterSkyrim
    SOURCES
        src/plugin.cpp
)

target_link_libraries(WorldEngineGameAdapterSkyrim
    PRIVATE
        WorldEngine::GameAdapter
)

target_compile_features(WorldEngineGameAdapterSkyrim PRIVATE cxx_std_23)
target_compile_definitions(WorldEngineGameAdapterSkyrim
    PRIVATE
        WORLD_ENGINE_TARGET_SKYRIM_SE_AE=1
        WORLD_ENGINE_TARGET_SKYRIM_VR=0
)
