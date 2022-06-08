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

#include <bootloader.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static dmgl_error_e dmgl_bootloader_validate(const uint8_t *data, size_t length)
{
    uint8_t checksum;
    dmgl_error_e result = DMGL_SUCCESS;

    if(length != 256) {
        result = DMGL_ERROR("Bootloader length mismatch -- %.02f KB (%u bytes)", length / 1024.f, length);
        goto exit;
    }

    if((checksum = dmgl_checksum(data, 0, 255)) != 0x92) {
        result = DMGL_ERROR("Bootloader checksum mismatch -- %02X", checksum);
        goto exit;
    }

exit:
    return result;
}

bool dmgl_bootloader_enabled(const dmgl_bootloader_t *bootloader)
{
    return bootloader->enabled;
}

void dmgl_bootloader_disable(dmgl_bootloader_t *bootloader)
{
    bootloader->enabled = false;
}

dmgl_error_e dmgl_bootloader_initialize(dmgl_bootloader_t *bootloader, const uint8_t *data, size_t length)
{
    dmgl_error_e result = DMGL_SUCCESS;

    if(data) {

        if((result = dmgl_bootloader_validate(data, length)) != DMGL_SUCCESS) {
            goto exit;
        }

        bootloader->data = data;
        bootloader->enabled = true;
    }

exit:
    return result;
}

uint8_t dmgl_bootloader_read(const dmgl_bootloader_t *bootloader, uint16_t address)
{
    uint8_t result = 0;

    switch(address) {
        case 0x0000 ... 0x00FF:
            result = bootloader->enabled ? bootloader->data[address - 0x0000] : 0x00;
            break;
        default:
            break;
    }

    return result;
}

void dmgl_bootloader_uninitialize(dmgl_bootloader_t *bootloader)
{
    memset(bootloader, 0, sizeof(*bootloader));
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
