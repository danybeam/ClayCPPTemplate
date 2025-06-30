#include <FWCore.h>
#include <flecs.h>

#include "Font.h"

int main()
{
    // Double scoped for profiling potential memory leak
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

        auto world = std::make_unique<flecs::world>();

        framework.Run(std::move(world));
    }

    return 0;
}
