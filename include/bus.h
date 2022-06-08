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

#ifndef DMGL_BUS_H_
#define DMGL_BUS_H_

#include <common.h>

typedef enum {
    DMGL_INTERRUPT_VBLANK = 0,
    DMGL_INTERRUPT_SCREEN,
    DMGL_INTERRUPT_TIMER,
    DMGL_INTERRUPT_SERIAL,
    DMGL_INTERRUPT_BUTTON,
    DMGL_INTERRUPT_MAX,
} dmgl_interrupt_e;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

dmgl_error_e dmgl_bus_clock(void);

dmgl_error_e dmgl_bus_initialize(const dmgl_t *context);

void dmgl_bus_interrupt(dmgl_interrupt_e interrupt);

uint8_t dmgl_bus_read(uint16_t address);

const char *dmgl_bus_title(void);

void dmgl_bus_uninitialize(void);

void dmgl_bus_write(uint16_t address, uint8_t value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMGL_BUS_H_ */
