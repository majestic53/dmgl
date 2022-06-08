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

#ifndef DMGL_SERVICE_H_
#define DMGL_SERVICE_H_

#include <common.h>

typedef enum {
    DMGL_BUTTON_A = 0,
    DMGL_BUTTON_B,
    DMGL_BUTTON_START,
    DMGL_BUTTON_SELECT,
    DMGL_BUTTON_RIGHT,
    DMGL_BUTTON_LEFT,
    DMGL_BUTTON_UP,
    DMGL_BUTTON_DOWN,
    DMGL_BUTTON_MAX,
} dmgl_button_e;

typedef enum {
    DMGL_COLOR_BLACK = 0,
    DMGL_COLOR_GREY_DARK,
    DMGL_COLOR_GREY_LIGHT,
    DMGL_COLOR_WHITE,
    DMGL_COLOR_MAX,
} dmgl_color_e;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

bool dmgl_service_button(dmgl_button_e button);

dmgl_error_e dmgl_service_initialize(const dmgl_t *context, const char *title);

void dmgl_service_pixel(dmgl_color_e color, uint8_t x, uint8_t y);

bool dmgl_service_poll(void);

dmgl_error_e dmgl_service_sync(void);

void dmgl_service_uninitialize(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMGL_SERVICE_H_ */
