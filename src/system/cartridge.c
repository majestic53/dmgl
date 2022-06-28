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

/*!
 * @file cartridge.c
 * @brief Cartridge subsystem.
 */

#include <cartridge.h>

static const size_t RAM_COUNT[] = { 1, 1, 1, 4, 16, 8, };                   /*!< Supported cartridge RAM count */

static const size_t ROM_COUNT[] = { 2, 4, 8, 16, 32, 64, 128, 256, 512, };  /*!< Supported cartridge ROM count */

static const uint8_t TYPE[] = { 0, };                                       /*!< Supported cartridge types */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Validate cartridge data.
 * @param[in] data Constant pointer to cartridge data
 * @param[in] length Cartridge data length, in bytes
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
static dmgl_error_e dmgl_cartridge_validate(const uint8_t *data, size_t length)
{
    uint8_t checksum;
    dmgl_cartridge_e type;
    dmgl_error_e result = DMGL_SUCCESS;
    const dmgl_cartridge_header_t *header;

    if(length < (ROM_COUNT[0] * 16 * 1024)) {
        result = DMGL_ERROR("Cartridge length mismatch -- %.02f KB (%u bytes)", length / 1024.f, length);
        goto exit;
    }

    header = (const dmgl_cartridge_header_t *)&data[0x0100];

    if((checksum = dmgl_checksum(data, 0x0134, 0x014C)) != header->checksum) {
        result = DMGL_ERROR("Cartridge checksum mismatch -- %02X", checksum);
        goto exit;
    }

    if((header->cgb & 0xC0) == 0xC0) {
        result = DMGL_ERROR("Cartridge is CGB-only -- %02X", header->cgb);
        goto exit;
    }

    for(type = 0; type < DMGL_CARTRIDGE_MAX; ++type) {

        if(header->type == TYPE[type]) {
            break;
        }
    }

    if(type >= DMGL_CARTRIDGE_MAX) {
        result = DMGL_ERROR("Cartridge type is unsupported -- %u", header->type);
        goto exit;
    }

    if(header->rom >= (sizeof(ROM_COUNT) / sizeof(*ROM_COUNT))) {
        result = DMGL_ERROR("Cartridge ROM count is unsupported -- %u", header->rom);
        goto exit;
    }

    if(header->ram >= (sizeof(RAM_COUNT) / sizeof(*RAM_COUNT))) {
        result = DMGL_ERROR("Cartridge RAM count is unsupported -- %u", header->ram);
        goto exit;
    }

    if(length != (ROM_COUNT[header->rom] * 16 * 1024)) {
        result = DMGL_ERROR("Cartridge length mismatch -- %.02f KB (%u bytes)", length / 1024.f, length);
        goto exit;
    }

exit:
    return result;
}

uint8_t dmgl_cartridge_checksum(const dmgl_cartridge_t *cartridge)
{
    return ((const dmgl_cartridge_header_t *)&cartridge->rom.bank[0][0x0100])->checksum;
}

dmgl_error_e dmgl_cartridge_initialize(dmgl_cartridge_t *cartridge, const uint8_t *data, size_t length)
{
    size_t count, index;
    dmgl_error_e result;
    const dmgl_cartridge_header_t *header;

    if((result = dmgl_cartridge_validate(data, length)) != DMGL_SUCCESS) {
        goto exit;
    }

    header = (const dmgl_cartridge_header_t *)&data[0x0100];
    count = RAM_COUNT[header->ram];

    if((cartridge->ram.bank = (uint8_t **)dmgl_buffer_allocate(count * sizeof(*cartridge->ram.bank))) == NULL) {
        result = DMGL_ERROR("Cartridge failed to allocate RAM banks -- %zu", count);
        goto exit;
    }

    for(index = 0; index < count; ++index) {

        if((cartridge->ram.bank[index] = (uint8_t *)dmgl_buffer_allocate(8 * 1024 * sizeof(*cartridge->ram.bank[index]))) == NULL) {
            result = DMGL_ERROR("Cartridge failed to allocate RAM bank -- %zu", index);
            goto exit;
        }

        memset(cartridge->ram.bank[index], 0xFF, 8 * 1024 * sizeof(*cartridge->ram.bank[index]));
    }

    cartridge->ram.count = count;
    count = ROM_COUNT[header->rom];

    if((cartridge->rom.bank = (const uint8_t **)dmgl_buffer_allocate(count * sizeof(*cartridge->rom.bank))) == NULL) {
        result = DMGL_ERROR("Cartridge failed to allocate ROM banks -- %zu", count);
        goto exit;
    }

    for(index = 0; index < count; ++index) {
        cartridge->rom.bank[index] = data + (index * 16 * 1024);
    }

    cartridge->rom.count = count;

exit:
    return result;
}

size_t dmgl_cartridge_ram_count(const dmgl_cartridge_t *cartridge)
{
    return cartridge->ram.count;
}

uint8_t dmgl_cartridge_ram_read(const dmgl_cartridge_t *cartridge, size_t index, uint16_t address)
{
    return cartridge->ram.bank[index][address];
}

void dmgl_cartridge_ram_write(dmgl_cartridge_t *cartridge, size_t index, uint16_t address, uint8_t value)
{
    cartridge->ram.bank[index][address] = value;
}

void dmgl_cartridge_reset(dmgl_cartridge_t *cartridge)
{

    for(size_t index = 0; index < cartridge->ram.count; ++index) {
        memset(cartridge->ram.bank[index], 0xFF, 8 * 1024 * sizeof(*cartridge->ram.bank[index]));
    }
}

size_t dmgl_cartridge_rom_count(const dmgl_cartridge_t *cartridge)
{
    return cartridge->rom.count;
}

uint8_t dmgl_cartridge_rom_read(const dmgl_cartridge_t *cartridge, size_t index, uint16_t address)
{
    return cartridge->rom.bank[index][address];
}

const char *dmgl_cartridge_title(const dmgl_cartridge_t *cartridge)
{
    return (const char *)((const dmgl_cartridge_header_t *)&cartridge->rom.bank[0][0x0100])->title;
}

dmgl_cartridge_e dmgl_cartridge_type(const dmgl_cartridge_t *cartridge)
{
    dmgl_cartridge_e result;

    for(result = 0; result < DMGL_CARTRIDGE_MAX; ++result) {

        if(((const dmgl_cartridge_header_t *)&cartridge->rom.bank[0][0x0100])->type == TYPE[result]) {
            break;
        }
    }

    return result;
}

void dmgl_cartridge_uninitialize(dmgl_cartridge_t *cartridge)
{

    if(cartridge->rom.bank) {
        dmgl_buffer_free(cartridge->rom.bank);
    }

    if(cartridge->ram.bank) {

        for(size_t index = 0; index < cartridge->ram.count; ++index) {
            dmgl_buffer_free(cartridge->ram.bank[index]);
        }

        dmgl_buffer_free(cartridge->ram.bank);
    }

    memset(cartridge, 0, sizeof(*cartridge));
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
