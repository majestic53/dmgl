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
#include <processor.h>

typedef struct {
    dmgl_memory_t memory;
    dmgl_processor_t processor;

    /* TODO: ADD SUBSYSTEMS */
    uint32_t cycle;
    /* ---- */

} dmgl_bus_t;

static dmgl_bus_t g_bus = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

dmgl_error_e dmgl_bus_clock(void)
{
    dmgl_error_e result;

    if((result = dmgl_processor_clock(&g_bus.processor)) != DMGL_SUCCESS) {
        goto exit;
    }

    /* TODO: CLOCK SUBSYSTEMS */
    if(++g_bus.cycle > 4194304 / 60) {
        result = DMGL_COMPLETE;
        g_bus.cycle = 0;
    }
    /* ---- */

exit:
    return result;
}

dmgl_error_e dmgl_bus_initialize(const dmgl_t *context)
{
    dmgl_error_e result;

    if((result = dmgl_memory_initialize(&g_bus.memory, context)) != DMGL_SUCCESS) {
        goto exit;
    }

    dmgl_processor_initialize(&g_bus.processor, dmgl_memory_has_bootloader(&g_bus.memory), dmgl_memory_checksum(&g_bus.memory));

    /* TODO: INITIALIZE SUBSYSTEMS */

exit:
    return result;
}

void dmgl_bus_interrupt(dmgl_interrupt_e interrupt)
{
    dmgl_bus_write(0xFF0F, dmgl_bus_read(0xFF0F) | (1 << interrupt));
}

uint8_t dmgl_bus_read(uint16_t address)
{
    uint8_t result = 0xFF;

    switch(address) {

        /* TODO: READ BYTE FROM SUBSYSTEMS */

        case 0xFF0F:
        case 0xFFFF:
            result = dmgl_processor_read(&g_bus.processor, address);
            break;
        default:
            result = dmgl_memory_read(&g_bus.memory, address);
            break;
    }

    return result;
}

void dmgl_bus_reset(void)
{
    dmgl_memory_reset(&g_bus.memory);
    dmgl_processor_reset(&g_bus.processor);

    /* TODO: RESET SUBSYSTEMS */
}

const char *dmgl_bus_title(void)
{
    return dmgl_memory_title(&g_bus.memory);
}

void dmgl_bus_uninitialize(void)
{
    /* TODO: UNINITIALIZE SUBSYSTEMS */

    dmgl_processor_uninitialize(&g_bus.processor);
    dmgl_memory_uninitialize(&g_bus.memory);
    memset(&g_bus, 0, sizeof(g_bus));
}

void dmgl_bus_write(uint16_t address, uint8_t value)
{

    switch(address) {

        /* TODO: WRITE BYTE TO SUBSYSTEMS */

        case 0xFF0F:
        case 0xFFFF:
            dmgl_processor_write(&g_bus.processor, address, value);
            break;
        default:
            dmgl_memory_write(&g_bus.memory, address, value);
            break;
    }
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
