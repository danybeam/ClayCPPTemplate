#include <Clay/clay.h>
#include <Clay/clay_renderer_raylib.cpp>  // NOLINT(bugprone-suspicious-include)

#include <flecs.h>

#include <iostream>

#include <SDL.h>

uint64_t clay_RequiredMemory;
Clay_Arena clay_MemoryArena;
Clay_Dimensions clay_Resolution;
Clay_ErrorHandler clay_ErrorHandler;

enum FONT_WEIGHT : short
{
    FONT_EXTRALIGHT = 0,
    FONT_ITALIC = 1,
    FONT_LIGHT = 2,
    FONT_SEMILIGHT = 4,
    FONT_REGULAR = 6,
    FONT_SEMIBOLD = 8,
    FONT_BOLD = 10,

    FONT_WEIGHT_END
};

Font clay_DummyFont[12] = {};
Clay_RenderCommandArray clay_RenderCommandArray;

void clay_ErrorHanderFunction(Clay_ErrorData ERR)
{
    std::cout << "CLAY ERROR: " << ERR.errorText.chars << std::endl;
}

Color raylib_ClearColour = {255, 255, 255, 255};

int main()
{
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

    clay_DummyFont[0] = LoadFontEx("resources/CaskaydiaCove/CaskaydiaCoveExtraLight.otf", 32, 0, 400);
    clay_DummyFont[1] = LoadFontEx("resources/CaskaydiaCove/CaskaydiaCoveExtraLightItalic.otf", 32, 0, 400);
    clay_DummyFont[2] = LoadFontEx("resources/CaskaydiaCove/CaskaydiaCoveLight.otf", 32, 0, 400);
    clay_DummyFont[3] = LoadFontEx("resources/CaskaydiaCove/CaskaydiaCoveLightItalic.otf", 32, 0, 400);
    clay_DummyFont[4] = LoadFontEx("resources/CaskaydiaCove/CaskaydiaCoveSemiLight.otf", 32, 0, 400);
    clay_DummyFont[5] = LoadFontEx("resources/CaskaydiaCove/CaskaydiaCoveSemiLightItalic.otf", 32, 0, 400);
    clay_DummyFont[6] = LoadFontEx("resources/CaskaydiaCove/CaskaydiaCoveRegular.otf", 32, 0, 400);
    clay_DummyFont[7] = LoadFontEx("resources/CaskaydiaCove/CaskaydiaCoveRegularItalic.otf", 32, 0, 400);
    clay_DummyFont[8] = LoadFontEx("resources/CaskaydiaCove/CaskaydiaCoveSemiBold.otf", 32, 0, 400);
    clay_DummyFont[9] = LoadFontEx("resources/CaskaydiaCove/CaskaydiaCoveSemiBoldItalic.otf", 32, 0, 400);
    clay_DummyFont[10] = LoadFontEx("resources/CaskaydiaCove/CaskaydiaCoveBold.otf", 32, 0, 400);
    clay_DummyFont[11] = LoadFontEx("resources/CaskaydiaCove/CaskaydiaCoveBoldItalic.otf", 32, 0, 400);

    GetWindowHandle();

    Clay_Initialize(clay_MemoryArena, clay_Resolution, clay_ErrorHandler);
    Clay_SetMeasureTextFunction(Raylib_MeasureText, clay_DummyFont);

    Clay_ElementDeclaration t_baseframe = {};
    t_baseframe.id = CLAY_ID("BaseFrame");
    t_baseframe.backgroundColor = {0, 0, 0, 255};
    t_baseframe.layout.sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()};
    t_baseframe.layout.layoutDirection = CLAY_TOP_TO_BOTTOM;

    Clay_ElementDeclaration t_BlueBox = {};
    t_BlueBox.id = CLAY_ID("BlueBox");
    t_BlueBox.backgroundColor = {0, 0, 255, 255};
    t_BlueBox.layout.sizing = {CLAY_SIZING_PERCENT(0.25),CLAY_SIZING_GROW()};

    Clay_ElementDeclaration t_RedBox = {};
    t_RedBox.id = CLAY_ID("RedBox");
    t_RedBox.backgroundColor = {255, 255, 0, 255};
    t_RedBox.layout.sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()};

    Clay_ElementDeclaration t_YellowBox = {};
    t_YellowBox.id = CLAY_ID("YellowBox");
    t_YellowBox.backgroundColor = {255, 255, 0, 255};
    t_YellowBox.layout.sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()};

    Clay_ElementDeclaration t_PurpleBox = {};
    t_PurpleBox.id = CLAY_ID("PurpleBox");
    t_PurpleBox.backgroundColor = {255, 0, 255, 255};
    t_PurpleBox.layout.sizing = {CLAY_SIZING_PERCENT(0.10),CLAY_SIZING_GROW()};

    SDL_Event event;
   
    
    while (!WindowShouldClose())
    {
/*
        Clay_BeginLayout();

        CLAY(t_baseframe)
        {
            CLAY_TEXT(
                CLAY_STRING("Hello world in extra light"),
                CLAY_TEXT_CONFIG(
                    {
                    .textColor = {255,255,0,255},
                    .fontId = FONT_WEIGHT::FONT_EXTRALIGHT ,
                    .fontSize = 32,
                    }
                )
            );
            CLAY_TEXT(
                CLAY_STRING("Hello world in extra light in italics"),
                CLAY_TEXT_CONFIG(
                    {
                    .textColor = {255,255,0,255},
                    .fontId = FONT_WEIGHT::FONT_EXTRALIGHT + FONT_WEIGHT::FONT_ITALIC,
                    .fontSize = 32,
                    }
                )
            );

            CLAY_TEXT(
                CLAY_STRING(" "),
                CLAY_TEXT_CONFIG({ .fontSize = 32, })
            );

            CLAY_TEXT(
                CLAY_STRING("Hello world in light"),
                CLAY_TEXT_CONFIG(
                    {
                    .textColor = {255,255,0,255},
                    .fontId = FONT_WEIGHT::FONT_LIGHT ,
                    .fontSize = 32,
                    }
                )
            );
            CLAY_TEXT(
                CLAY_STRING("Hello world in light in italics"),
                CLAY_TEXT_CONFIG(
                    {
                    .textColor = {255,255,0,255},
                    .fontId = FONT_WEIGHT::FONT_LIGHT + FONT_WEIGHT::FONT_ITALIC,
                    .fontSize = 32,
                    }
                )
            );

            CLAY_TEXT(
                CLAY_STRING(" "),
                CLAY_TEXT_CONFIG({ .fontSize = 32, })
            );

            CLAY_TEXT(
                CLAY_STRING("Hello world in semi light"),
                CLAY_TEXT_CONFIG(
                    {
                    .textColor = {255,255,0,255},
                    .fontId = FONT_WEIGHT::FONT_SEMILIGHT ,
                    .fontSize = 32,
                    }
                )
            );
            CLAY_TEXT(
                CLAY_STRING("Hello world in semi light in italics"),
                CLAY_TEXT_CONFIG(
                    {
                    .textColor = {255,255,0,255},
                    .fontId = FONT_WEIGHT::FONT_SEMILIGHT + FONT_WEIGHT::FONT_ITALIC,
                    .fontSize = 32,
                    }
                )
            );

            CLAY_TEXT(
                CLAY_STRING(" "),
                CLAY_TEXT_CONFIG({ .fontSize = 32, })
            );

            CLAY_TEXT(
                CLAY_STRING("Hello world in regular"),
                CLAY_TEXT_CONFIG(
                    {
                    .textColor = {255,255,0,255},
                    .fontId = FONT_WEIGHT::FONT_REGULAR ,
                    .fontSize = 32,
                    }
                )
            );
            CLAY_TEXT(
                CLAY_STRING("Hello world in regular in italics"),
                CLAY_TEXT_CONFIG(
                    {
                    .textColor = {255,255,0,255},
                    .fontId = FONT_WEIGHT::FONT_REGULAR + FONT_WEIGHT::FONT_ITALIC,
                    .fontSize = 32,
                    }
                )
            );

            CLAY_TEXT(
                CLAY_STRING(" "),
                CLAY_TEXT_CONFIG({ .fontSize = 32, })
            );

            CLAY_TEXT(
                CLAY_STRING("Hello world in semibold"),
                CLAY_TEXT_CONFIG(
                    {
                    .textColor = {255,255,0,255},
                    .fontId = FONT_WEIGHT::FONT_SEMIBOLD ,
                    .fontSize = 32,
                    }
                )
            );
            CLAY_TEXT(
                CLAY_STRING("Hello world in semibold in italics"),
                CLAY_TEXT_CONFIG(
                    {
                    .textColor = {255,255,0,255},
                    .fontId = FONT_WEIGHT::FONT_SEMIBOLD + FONT_WEIGHT::FONT_ITALIC,
                    .fontSize = 32,
                    }
                )
            );
            CLAY_TEXT(
                CLAY_STRING(" "),
                CLAY_TEXT_CONFIG({ .fontSize = 32, })
            );
            CLAY_TEXT(
                CLAY_STRING("Hello world in bold"),
                CLAY_TEXT_CONFIG(
                    {
                    .textColor = {255,255,0,255},
                    .fontId = FONT_WEIGHT::FONT_BOLD ,
                    .fontSize = 32,
                    }
                )
            );
            CLAY_TEXT(
                CLAY_STRING("Hello world in bold and italics"),
                CLAY_TEXT_CONFIG(
                    {
                    .textColor = {255,255,0,255},
                    .fontId = FONT_WEIGHT::FONT_BOLD + FONT_WEIGHT::FONT_ITALIC,
                    .fontSize = 32,
                    }
                )
            );
        };

        clay_RenderCommandArray = Clay_EndLayout();

        BeginDrawing();
        ClearBackground(raylib_ClearColour);
        Clay_Raylib_Render(clay_RenderCommandArray, clay_DummyFont);
        EndDrawing();*/

        // TODO(danybeam) it doesn't work if polled after render... why?
        while( SDL_PollEvent( &event ) ){
            /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
            switch( event.type ){
            case SDL_EVENT_KEY_DOWN:
                throw "Key down";
                break;

            case SDL_EVENT_KEY_UP:
                throw "Key up";
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                throw "Mouse up";
                break;

            default:
                break;
            }
        }
    }

    CloseWindow();
    return 0;
}
