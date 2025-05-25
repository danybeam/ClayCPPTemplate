#include "Clay/clay.h"
#include "Clay/clay_renderer_raylib.cpp"

#include <iostream>

uint64_t clay_RequiredMemory;
Clay_Arena clay_MemoryArena;
Clay_Dimensions clay_Resolution;
Clay_ErrorHandler clay_ErrorHandler;

Font clay_DummyFont;
Clay_RenderCommandArray clay_RenderCommandArray;

void clay_ErrorHanderFunction(Clay_ErrorData ERR) {
    std::cout << "CLAY ERROR: " << ERR.errorText.chars << std::endl;
}
Color raylib_ClearColour = { 255, 255, 255, 255 };

int main() {
    Clay_Raylib_Initialize(1280, 720, "raylib clay base", 0);

    clay_RequiredMemory = 8 * Clay_MinMemorySize();
    clay_MemoryArena = {};
    clay_MemoryArena.capacity = clay_RequiredMemory;
    clay_MemoryArena.memory = (char*)malloc(clay_RequiredMemory);
    clay_Resolution = {};
    clay_Resolution.width = GetScreenWidth();
    clay_Resolution.height = GetScreenHeight();
    clay_ErrorHandler = {};
    clay_ErrorHandler.errorHandlerFunction = clay_ErrorHanderFunction;
    clay_ErrorHandler.userData = nullptr;
    clay_DummyFont = LoadFont("Resources/Roboto-Regular.ttf");

    Clay_Initialize(clay_MemoryArena, clay_Resolution, clay_ErrorHandler);

    Clay_SetMeasureTextFunction(Raylib_MeasureText, &clay_DummyFont);

    Clay_ElementDeclaration t_baseframe = {};
    t_baseframe.id = CLAY_ID("BaseFrame");
    t_baseframe.backgroundColor = { 0,0,0,255 };
    t_baseframe.layout.sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() };
    t_baseframe.layout.layoutDirection = CLAY_TOP_TO_BOTTOM;

    Clay_ElementDeclaration t_BlueBox = {};
    t_BlueBox.id = CLAY_ID("BlueBox");
    t_BlueBox.backgroundColor = { 0, 0, 255, 255 };
    t_BlueBox.layout.sizing = { CLAY_SIZING_PERCENT(0.25),CLAY_SIZING_GROW() };

    Clay_ElementDeclaration t_RedBox = {};
    t_RedBox.id = CLAY_ID("RedBox");
    t_RedBox.backgroundColor = { 255,255,0,255 };
    t_RedBox.layout.sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() };

    Clay_ElementDeclaration t_YellowBox = {};
    t_YellowBox.id = CLAY_ID("YellowBox");
    t_YellowBox.backgroundColor = { 255, 255, 0, 255 };
    t_YellowBox.layout.sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() };

    Clay_ElementDeclaration t_PurpleBox = {};
    t_PurpleBox.id = CLAY_ID("PurpleBox");
    t_PurpleBox.backgroundColor = { 255, 0, 255, 255 };
    t_PurpleBox.layout.sizing = { CLAY_SIZING_PERCENT(0.10),CLAY_SIZING_GROW() };

    while (!WindowShouldClose()) {

        Clay_BeginLayout();

        CLAY(t_baseframe) {
            CLAY_TEXT(
                CLAY_STRING("Hello world"),
                CLAY_TEXT_CONFIG(
                    {
                        .textColor = {255,255,0,255},
                        .fontSize = 32,
                    }
                    )
            );
        };

        clay_RenderCommandArray = Clay_EndLayout();

        BeginDrawing();
        ClearBackground(raylib_ClearColour);
        Clay_Raylib_Render(clay_RenderCommandArray, &clay_DummyFont);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}