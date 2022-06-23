/*
 * DMGL
 * Copyright (C) 2022 David Jolly
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <common.h>

#include <SDL2/SDL.h>
#include <bus.h>
#include <service.h>

typedef struct {
    uint32_t tick;
    uint32_t pixel[160 * 144];
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Cursor *cursor;
    SDL_GameController *controller;
    SDL_JoystickID joystick;
} dmgl_sdl_t;

static dmgl_sdl_t g_sdl = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static void dmgl_service_clear(void)
{

    for(uint8_t y = 0; y < 144; ++y) {

        for(uint8_t x = 0; x < 160; ++x) {
            dmgl_service_pixel(DMGL_COLOR_WHITE, x, y);
        }
    }
}

bool dmgl_service_button(dmgl_button_e button)
{
    bool result = false;
    const int keys[] = {
        SDL_SCANCODE_L, SDL_SCANCODE_K, SDL_SCANCODE_C, SDL_SCANCODE_SPACE,
        SDL_SCANCODE_D, SDL_SCANCODE_A, SDL_SCANCODE_W, SDL_SCANCODE_S,
        };

    if(g_sdl.controller) {
        const SDL_GameControllerButton KEY[] = {
            SDL_CONTROLLER_BUTTON_A, SDL_CONTROLLER_BUTTON_B, SDL_CONTROLLER_BUTTON_BACK, SDL_CONTROLLER_BUTTON_START,
            SDL_CONTROLLER_BUTTON_DPAD_RIGHT, SDL_CONTROLLER_BUTTON_DPAD_LEFT, SDL_CONTROLLER_BUTTON_DPAD_UP, SDL_CONTROLLER_BUTTON_DPAD_DOWN,
            };

        result = SDL_GameControllerGetButton(g_sdl.controller, KEY[button]) ? true : false;
    }

    if(!result) {
        result = SDL_GetKeyboardState(NULL)[keys[button]] ? true : false;
    }

    return result;
}

dmgl_error_e dmgl_service_initialize(const dmgl_t *context, const char *title)
{
    int scale = 2;
    dmgl_error_e result = DMGL_SUCCESS;
    const char *controller_map[] = {
        "03000000790000001100000010010000,Retro Controller,a:b1,b:b2,back:b8,dpdown:+a1,dpleft:-a0,dpright:+a0,dpup:-a1,leftshoulder:b6,lefttrigger:b7,rightshoulder:b4,righttrigger:b5,start:b9,x:b0,y:b3,platform:Linux",
        };

    if(context->window.scale) {
        scale = context->window.scale;

        if(scale < 1) {
            scale = 1;
        } else if(scale > 8) {
            scale = 8;
        }
    }

    if(SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_VIDEO)) {
        result = DMGL_ERROR("SDL_Init failed -- %s", SDL_GetError());
        goto exit;
    }

    if(!(g_sdl.window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160 * scale, 144 * scale, 0))) {
        result = DMGL_ERROR("SDL_CreateWindow failed -- %s", SDL_GetError());
        goto exit;
    }

    if(!(g_sdl.renderer = SDL_CreateRenderer(g_sdl.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))) {
        result = DMGL_ERROR("SDL_CreateRenderer failed -- %s", SDL_GetError());
        goto exit;
    }

    if(SDL_RenderSetLogicalSize(g_sdl.renderer, 160, 144)) {
        result = DMGL_ERROR("SDL_RenderSetLogicalSize failed -- %s", SDL_GetError());
        goto exit;
    }

    if(SDL_SetRenderDrawColor(g_sdl.renderer, 0, 0, 0, 0)) {
        result = DMGL_ERROR("SDL_SetRenderDrawColor failed -- %s", SDL_GetError());
        goto exit;
    }

    if(SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1") == SDL_FALSE) {
        result = DMGL_ERROR("SDL_SetHint failed -- %s", SDL_GetError());
        goto exit;
    }

    if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0") == SDL_FALSE) {
        result = DMGL_ERROR("SDL_SetHint failed -- %s", SDL_GetError());
        goto exit;
    }

    if(!(g_sdl.texture = SDL_CreateTexture(g_sdl.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 160, 144))) {
        result = DMGL_ERROR("SDL_CreateTexture failed -- %s", SDL_GetError());
        goto exit;
    }

    if(!(g_sdl.cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR))) {
        result = DMGL_ERROR("SDL_CreateSystemCursor failed -- %s", SDL_GetError());
        goto exit;
    }

    SDL_SetCursor(g_sdl.cursor);

    for(size_t index = 0; index < sizeof(controller_map) / sizeof(*controller_map); ++index) {

        if(SDL_GameControllerAddMapping(controller_map[index]) == -1) {
            result = DMGL_ERROR("SDL_GameControllerAddMapping failed -- %s", SDL_GetError());
            goto exit;
        }
    }

    dmgl_service_clear();

exit:
    return result;
}

void dmgl_service_pixel(dmgl_color_e color, uint8_t x, uint8_t y)
{
    const uint32_t colors[] = {
        0xFF081820, 0xFF346856, 0xFF88C070, 0xFFE0F8D0,
        };

    g_sdl.pixel[(y * 160) + x] = colors[color];
}

dmgl_error_e dmgl_service_poll(void)
{
    SDL_Event event;
    dmgl_error_e result = DMGL_SUCCESS;

    while(SDL_PollEvent(&event)) {

        switch(event.type) {
            case SDL_CONTROLLERDEVICEADDED:

                if(!g_sdl.controller && SDL_IsGameController(event.cdevice.which)) {
                    SDL_Joystick *joystick = NULL;

                    if(!(g_sdl.controller = SDL_GameControllerOpen(event.cdevice.which))) {
                        result = DMGL_ERROR("SDL_GameControllerOpen failed -- %s", SDL_GetError());
                        goto exit;
                    }

                    if(!(joystick = SDL_GameControllerGetJoystick(g_sdl.controller))) {
                        result = DMGL_ERROR("SDL_GameControllerGetJoystick failed -- %s", SDL_GetError());
                        goto exit;
                    }

                    if((g_sdl.joystick = SDL_JoystickInstanceID(joystick)) == -1) {
                        result = DMGL_ERROR("SDL_JoystickInstanceID failed -- %s", SDL_GetError());
                        goto exit;
                    }
                }
                break;
            case SDL_CONTROLLERDEVICEREMOVED:

                if(g_sdl.controller && (g_sdl.joystick == event.cdevice.which)) {
                    SDL_GameControllerClose(g_sdl.controller);
                    g_sdl.controller = NULL;
                }
                break;
            case SDL_KEYUP:

                if(!event.key.repeat) {

                    switch(event.key.keysym.scancode) {
                        case SDL_SCANCODE_R:
                            dmgl_service_clear();
                            dmgl_bus_reset();
                            break;
                        default:
                            break;
                    }
                }
                break;
            case SDL_QUIT:
                result = DMGL_COMPLETE;
                goto exit;
            default:
                break;
        }
    }

exit:
    return result;
}

dmgl_error_e dmgl_service_sync(void)
{
    uint32_t elapsed;
    dmgl_error_e result = DMGL_SUCCESS;

    if(SDL_UpdateTexture(g_sdl.texture, NULL, g_sdl.pixel, 160 * sizeof(uint32_t))) {
        result = DMGL_ERROR("SDL_UpdateTexture failed -- %s", SDL_GetError());
        goto exit;
    }

    if(SDL_RenderClear(g_sdl.renderer)) {
        result = DMGL_ERROR("SDL_RenderClear failed -- %s", SDL_GetError());
        goto exit;
    }

    if(SDL_RenderCopy(g_sdl.renderer, g_sdl.texture, NULL, NULL)) {
        result = DMGL_ERROR("SDL_RenderCopy failed -- %s", SDL_GetError());
        goto exit;
    }

    if((elapsed = (SDL_GetTicks() - g_sdl.tick)) < (1000 / (float)60)) {
        SDL_Delay((1000 / (float)60) - elapsed);
    }

    SDL_RenderPresent(g_sdl.renderer);
    g_sdl.tick = SDL_GetTicks();

exit:
    return result;
}

void dmgl_service_uninitialize(void)
{

    if(g_sdl.controller) {
        SDL_GameControllerClose(g_sdl.controller);
    }

    if(g_sdl.cursor) {
        SDL_FreeCursor(g_sdl.cursor);
    }

    if(g_sdl.texture) {
        SDL_DestroyTexture(g_sdl.texture);
    }

    if(g_sdl.renderer) {
        SDL_DestroyRenderer(g_sdl.renderer);
    }

    if(g_sdl.window) {
        SDL_DestroyWindow(g_sdl.window);
    }

    SDL_Quit();
    memset(&g_sdl, 0, sizeof(g_sdl));
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
