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

#ifndef DMGL_MAPPER_H_
#define DMGL_MAPPER_H_

#include <cartridge.h>

typedef struct {
    dmgl_error_e (*initialize)(const dmgl_cartridge_t *, void **);
    uint8_t (*read)(const dmgl_cartridge_t *, void *, uint16_t);
    void (*uninitialize)(void *);
    void (*write)(dmgl_cartridge_t *, void *, uint16_t, uint8_t);
} dmgl_mapper_handler_t;

typedef struct {
    dmgl_cartridge_t cartridge;
    dmgl_mapper_handler_t handler;
    void *context;
} dmgl_mapper_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t dmgl_mapper_checksum(const dmgl_mapper_t *mapper);

dmgl_error_e dmgl_mapper_initialize(dmgl_mapper_t *mapper, const uint8_t *data, size_t length);

uint8_t dmgl_mapper_read(const dmgl_mapper_t *mapper, uint16_t address);

const char *dmgl_mapper_title(const dmgl_mapper_t *mapper);

void dmgl_mapper_uninitialize(dmgl_mapper_t *mapper);

void dmgl_mapper_write(dmgl_mapper_t *mapper, uint16_t address, uint8_t value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMGL_MAPPER_H_ */
