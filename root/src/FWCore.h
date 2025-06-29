#pragma once

#include <Clay/clay.h>
#include <raylib.h>
#include <SDL_events.h>

namespace flecs
{
    // ReSharper disable once CppInconsistentNaming
    struct world;
}

namespace fw
{
    class FWCore
    {
    public:
        enum ERRORCODES : uint8_t
        {
            NONE = 0,
            CLAY_INIT_ERROR = 1 << 0,
            SDL_INIT_ERROR = 1 << 1,
        };

        FWCore(uint32_t width, uint32_t height);
        virtual ~FWCore();

        uint32_t Run(flecs::world& world);

    private:
        uint8_t Init();
        bool InitClay();
        bool InitSDL();

        void Shutdown();
        void ShutdownClay();
        void ShutdownSDL();
        static void Clay_errorHandlerFunction(Clay_ErrorData err);
        static bool ProcessSDLEvent_wrapper(void* userData, SDL_Event* event);
        bool ProcessSDLEvent(SDL_Event* event);

    private:
        // Clay members
        Clay_ErrorHandler m_clay_ErrorHandler;
        Clay_Dimensions m_clay_Resolution;
        Clay_RenderCommandArray m_clay_RenderCommandArray;
        uint64_t m_clay_RequiredMemory;
        Clay_Arena m_clay_MemoryArena;

        // flecs members
        flecs::world* m_world_;

        // framework members
        uint8_t m_errorCodes;

    public:
        [[nodiscard]] uint8_t Get_errorCodes() const
        {
            return m_errorCodes;
        }

    private:
        // IO state members
        bool m_fullscreen_mode_ = false;
        bool m_key_states_[256] = {};
        bool m_mouse_button_states_[3] = {};
        float m_mouse_wheel_ = 0;
        bool m_old_key_states_[256] = {};
        bool m_old_mouse_button_states_[3] = {};
        bool m_window_is_active_ = false;

        // Raylib members
        Color raylib_ClearColour = {255, 255, 255, 255};
        Font m_clay_Font[12];

        // Window members
        uint32_t m_window_client_height_ = 0;
        uint32_t m_window_client_width_ = 0;
    };
}
