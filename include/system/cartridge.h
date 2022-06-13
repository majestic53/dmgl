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

#ifndef DMGL_CARTRIDGE_H_
#define DMGL_CARTRIDGE_H_

#include <common.h>

typedef enum {
    DMGL_CARTRIDGE_MBC0 = 0,
    DMGL_CARTRIDGE_MAX,
} dmgl_cartridge_e;

typedef struct {
    uint8_t entry[4];
    uint8_t logo[48];
    uint8_t title[11];
    uint8_t manufacturer[4];
    uint8_t cgb;
    uint8_t licensee[2];
    uint8_t sgb;
    uint8_t type;
    uint8_t rom;
    uint8_t ram;
    uint8_t destination;
    uint8_t licensee_old;
    uint8_t version;
    uint8_t checksum;
    uint16_t checksum_global;
} dmgl_cartridge_header_t;

typedef struct {

    struct {
        uint8_t **bank;
        size_t count;
    } ram;

    struct {
        const uint8_t **bank;
        size_t count;
    } rom;
} dmgl_cartridge_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t dmgl_cartridge_checksum(const dmgl_cartridge_t *cartridge);

dmgl_error_e dmgl_cartridge_initialize(dmgl_cartridge_t *cartridge, const uint8_t *data, size_t length);

size_t dmgl_cartridge_ram_count(const dmgl_cartridge_t *cartridge);

uint8_t dmgl_cartridge_ram_read(const dmgl_cartridge_t *cartridge, size_t index, uint16_t address);

void dmgl_cartridge_ram_write(dmgl_cartridge_t *cartridge, size_t index, uint16_t address, uint8_t value);

size_t dmgl_cartridge_rom_count(const dmgl_cartridge_t *cartridge);

uint8_t dmgl_cartridge_rom_read(const dmgl_cartridge_t *cartridge, size_t index, uint16_t address);

const char *dmgl_cartridge_title(const dmgl_cartridge_t *cartridge);

dmgl_cartridge_e dmgl_cartridge_type(const dmgl_cartridge_t *cartridge);

void dmgl_cartridge_uninitialize(dmgl_cartridge_t *cartridge);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMGL_CARTRIDGE_H_ */
