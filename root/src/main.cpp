#include <FWCore.h>
#include <flecs.h>

int main()
{
    auto framework = fw::FWCore(1280, 720);

    switch (framework.Get_errorCodes())
    {
    case fw::FWCore::ERRORCODES::CLAY_INIT_ERROR:
        throw std::runtime_error(
            "Something went wrong initializing Clay. I currently don't have detailed errors so please take note of what happened and share it with me.");
    case fw::FWCore::ERRORCODES::SDL_INIT_ERROR:
        throw std::runtime_error(
            "Something went wrong initializing SDL. I currently don't have detailed errors so please take note of what happened and share it with me.");
    default:
    case fw::FWCore::ERRORCODES::NONE:
        break;
    }

    auto world = flecs::world();

    framework.Run(world);

    return 0;
}
