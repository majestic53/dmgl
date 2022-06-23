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

#ifndef DMGL_BOOTLOADER_H_
#define DMGL_BOOTLOADER_H_

#include <common.h>

typedef struct {
    const uint8_t *data;
    bool enabled;
} dmgl_bootloader_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void dmgl_bootloader_disable(dmgl_bootloader_t *bootloader);

bool dmgl_bootloader_enabled(const dmgl_bootloader_t *bootloader);

dmgl_error_e dmgl_bootloader_initialize(dmgl_bootloader_t *bootloader, const uint8_t *data, size_t length);

uint8_t dmgl_bootloader_read(const dmgl_bootloader_t *bootloader, uint16_t address);

void dmgl_bootloader_reset(dmgl_bootloader_t *bootloader);

void dmgl_bootloader_uninitialize(dmgl_bootloader_t *bootloader);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMGL_BOOTLOADER_H_ */
