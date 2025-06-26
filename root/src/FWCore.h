#pragma once

namespace flecs
{
    class world;
}

namespace fw
{
    class FWCore
    {
    public:
        FWCore(uint32_t width, uint32_t height);
        virtual ~FWCore();

        bool init();
        void shutdown();

        uint32_t run(flecs::world& world);

    private:
        flecs::world* m_world_;

        uint32_t m_window_client_width_ = 0;
        uint32_t m_window_client_height_ = 0;

        bool m_key_states_[256] = {};
        bool m_mouse_button_states_[3] = {};
        float m_mouse_wheel_ = 0;
        bool m_old_key_states_[256] = {};
        bool m_old_mouse_button_states_[3] = {};
        bool m_window_is_active_ = false;
        bool m_fullscreen_mode_ = false;
    };
}
