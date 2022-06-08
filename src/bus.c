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

#include <bus.h>
#include <memory.h>

typedef struct {
    dmgl_memory_t memory;

    /* TODO */

} dmgl_bus_t;

static dmgl_bus_t g_bus = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

dmgl_error_e dmgl_bus_clock(void)
{
    dmgl_error_e result;

    /* TODO */
    result = DMGL_COMPLETE;
    /* ---- */

    return result;
}

dmgl_error_e dmgl_bus_initialize(const dmgl_t *context)
{
    dmgl_error_e result;

    if((result = dmgl_memory_initialize(&g_bus.memory, context)) != DMGL_SUCCESS) {
        goto exit;
    }

    /* TODO */

exit:
    return result;
}

void dmgl_bus_interrupt(dmgl_interrupt_e interrupt)
{
    dmgl_bus_write(0xFF0F, dmgl_bus_read(0xFF0F) | (1 << interrupt));
}

uint8_t dmgl_bus_read(uint16_t address)
{
    uint8_t result = 0;

    switch(address) {

        /* TODO */

        default:
            result = dmgl_memory_read(&g_bus.memory, address);
            break;
    }

    return result;
}

const char *dmgl_bus_title(void)
{
    return dmgl_memory_title(&g_bus.memory);
}

void dmgl_bus_uninitialize(void)
{
    /* TODO */

    dmgl_memory_uninitialize(&g_bus.memory);
    memset(&g_bus, 0, sizeof(g_bus));
}

void dmgl_bus_write(uint16_t address, uint8_t value)
{

    switch(address) {

        /* TODO */

        default:
            dmgl_memory_write(&g_bus.memory, address, value);
            break;
    }
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
