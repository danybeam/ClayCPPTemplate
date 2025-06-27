#include "FWCore.h"

fw::FWCore::FWCore(uint32_t width, uint32_t height) :
m_window_client_width_(width),
m_window_client_height_(height)
{
    init();
}

fw::FWCore::~FWCore()
{
    shutdown();
}

bool fw::FWCore::init()
{
    // TODO(danybeam): Initialize Clay
    return false;
}

void fw::FWCore::shutdown()
{
    // TODO(danybeam) clear Clay memory
}

uint32_t fw::FWCore::run(flecs::world& world)
{
    // TODO(danybeam): Save world reference
    // TODO(danybeam): loop updates
    return 0;
}
