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

#include <mbc0.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

dmgl_error_e dmgl_mbc0_initialize(const dmgl_cartridge_t *cartridge, void **context)
{
    *context = NULL;

    return DMGL_SUCCESS;
}

uint8_t dmgl_mbc0_read(const dmgl_cartridge_t *cartridge, void *context, uint16_t address)
{
    uint8_t result = 0xFF;

    switch(address) {
        case 0x0000 ... 0x3FFF:
            result = dmgl_cartridge_rom_read(cartridge, 0, address - 0x0000);
            break;
        case 0x4000 ... 0x7FFF:
            result = dmgl_cartridge_rom_read(cartridge, 1, address - 0x4000);
            break;
        case 0xA000 ... 0xBFFF:
            result = dmgl_cartridge_ram_read(cartridge, 0, address - 0xA000);
            break;
        default:
            break;
    }

    return result;
}

void dmgl_mbc0_reset(void *context)
{
    return;
}

void dmgl_mbc0_uninitialize(void *context)
{
    return;
}

void dmgl_mbc0_write(dmgl_cartridge_t *cartridge, void *context, uint16_t address, uint8_t value)
{

    switch(address) {
        case 0xA000 ... 0xBFFF:
            dmgl_cartridge_ram_write(cartridge, 0, address - 0xA000, value);
            break;
        default:
            break;
    }
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
