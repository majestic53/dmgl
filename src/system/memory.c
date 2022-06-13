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

#include <memory.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t dmgl_memory_checksum(const dmgl_memory_t *memory)
{
    return dmgl_mapper_checksum(&memory->mapper);
}

bool dmgl_memory_has_bootloader(const dmgl_memory_t *memory)
{
    return dmgl_bootloader_enabled(&memory->bootloader);
}

dmgl_error_e dmgl_memory_initialize(dmgl_memory_t *memory, const dmgl_t *context)
{
    dmgl_error_e result;

    if((result = dmgl_bootloader_initialize(&memory->bootloader, context->bootloader.data, context->bootloader.length)) != DMGL_SUCCESS) {
        goto exit;
    }

    if((result = dmgl_mapper_initialize(&memory->mapper, context->cartridge.data, context->cartridge.length)) != DMGL_SUCCESS) {
        goto exit;
    }

exit:
    return result;
}

uint8_t dmgl_memory_read(const dmgl_memory_t *memory, uint16_t address)
{
    uint8_t result = 0;

    switch(address) {
        case 0x0000 ... 0x00FF:

            if(dmgl_bootloader_enabled(&memory->bootloader)) {
                result = dmgl_bootloader_read(&memory->bootloader, address);
            } else {
                result = dmgl_mapper_read(&memory->mapper, address);
            }
            break;
        case 0x8000 ... 0x9FFF:
            result = memory->video[address - 0x8000];
            break;
        case 0xC000 ... 0xDFFF:
            result = memory->internal[address - 0xC000];
            break;
        case 0xE000 ... 0xFDFF:
            result = memory->internal[address - 0xE000];
            break;
        case 0xFE00 ... 0xFE9F:
            result = memory->sprite[address - 0xFE00];
            break;
        case 0xFF80 ... 0xFFFE:
            result = memory->high[address - 0xFF80];
            break;
        default:
            result = dmgl_mapper_read(&memory->mapper, address);
            break;
    }

    return result;
}

const char *dmgl_memory_title(const dmgl_memory_t *memory)
{
    return dmgl_mapper_title(&memory->mapper);
}

void dmgl_memory_uninitialize(dmgl_memory_t *memory)
{
    dmgl_mapper_uninitialize(&memory->mapper);
    dmgl_bootloader_uninitialize(&memory->bootloader);
    memset(memory, 0, sizeof(*memory));
}

void dmgl_memory_write(dmgl_memory_t *memory, uint16_t address, uint8_t value)
{

    switch(address) {
        case 0x8000 ... 0x9FFF:
            memory->video[address - 0x8000] = value;
            break;
        case 0xC000 ... 0xDFFF:
            memory->internal[address - 0xC000] = value;
            break;
        case 0xE000 ... 0xFDFF:
            memory->internal[address - 0xE000] = value;
            break;
        case 0xFE00 ... 0xFE9F:
            memory->sprite[address - 0xFE00] = value;
            break;
        case 0xFF80 ... 0xFFFE:
            memory->high[address - 0xFF80] = value;
            break;
        case 0xFF50:
            dmgl_bootloader_disable(&memory->bootloader);
            break;
        default:
            dmgl_mapper_write(&memory->mapper, address, value);
            break;
    }
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
