#include <SKSE/SKSE.h>

SKSEPluginLoad(const SKSE::LoadInterface* load_interface) {
    SKSE::Init(load_interface);
    return true;
}
