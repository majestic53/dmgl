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

#include <cartridge.h>
#include <test.h>

typedef struct {
    dmgl_cartridge_t cartridge;
    dmgl_cartridge_header_t *header;

    struct {
        const void *data;
        uint8_t value;
        size_t begin;
        size_t end;
    } checksum;

    struct {
        uint8_t *bank[2];
        uint8_t data[8 * 1024];
        bool allocate_bank;
        bool allocate_data;
    } ram;

    struct {
        const uint8_t *bank[2];
        uint8_t data[16 * 1024];
        bool allocate_bank;
    } rom;
} dmgl_test_cartridge_t;

static dmgl_test_cartridge_t g_test_cartridge = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t dmgl_checksum(const void *data, size_t begin, size_t end)
{
    g_test_cartridge.checksum.data = data;
    g_test_cartridge.checksum.begin = begin;
    g_test_cartridge.checksum.end = end;

    return g_test_cartridge.checksum.value;
}

void *dmgl_buffer_allocate(size_t length)
{
    void *result = NULL;

    switch(length) {
        case 8:
            result = g_test_cartridge.ram.allocate_bank ? g_test_cartridge.ram.bank : NULL;
            break;
        case 8 * 1024:
            result = g_test_cartridge.ram.allocate_data ? g_test_cartridge.ram.data : NULL;
            break;
        case 16:
            result = g_test_cartridge.rom.allocate_bank ? g_test_cartridge.rom.bank : NULL;
            break;
        default:
            break;
    }

    return result;
}

void dmgl_buffer_free(void *buffer)
{
    return;
}

static inline void dmgl_test_initialize(void)
{
    memset(&g_test_cartridge, 0, sizeof(g_test_cartridge));

    for(size_t index = 0; index < 2; ++index) {
        g_test_cartridge.ram.bank[index] = g_test_cartridge.ram.data;
        g_test_cartridge.rom.bank[index] = g_test_cartridge.rom.data;
    }

    g_test_cartridge.cartridge.ram.bank = g_test_cartridge.ram.bank;
    g_test_cartridge.cartridge.ram.count = 2;
    g_test_cartridge.cartridge.rom.bank = g_test_cartridge.rom.bank;
    g_test_cartridge.cartridge.rom.count = 2;
    g_test_cartridge.header = (dmgl_cartridge_header_t *)&g_test_cartridge.rom.data[0x0100];
}

static dmgl_error_e dmgl_test_cartridge_checksum(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_cartridge.header->checksum = 0xEF;

    if(DMGL_ASSERT(dmgl_cartridge_checksum(&g_test_cartridge.cartridge) == 0xEF)) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_initialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    if(DMGL_ASSERT(dmgl_cartridge_initialize(&g_test_cartridge.cartridge, (const uint8_t *)1, 0) == DMGL_FAILURE)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_cartridge.checksum.value = 0xEF;

    if(DMGL_ASSERT(dmgl_cartridge_initialize(&g_test_cartridge.cartridge, *g_test_cartridge.rom.bank, 2 * 16 * 1024) == DMGL_FAILURE)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_cartridge.checksum.value = 0xEF;
    g_test_cartridge.header->checksum = g_test_cartridge.checksum.value;
    g_test_cartridge.header->cgb = 0xC0;

    if(DMGL_ASSERT(dmgl_cartridge_initialize(&g_test_cartridge.cartridge, *g_test_cartridge.rom.bank, 2 * 16 * 1024) == DMGL_FAILURE)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_cartridge.checksum.value = 0xEF;
    g_test_cartridge.header->checksum = g_test_cartridge.checksum.value;
    g_test_cartridge.header->type = DMGL_CARTRIDGE_MAX;

    if(DMGL_ASSERT(dmgl_cartridge_initialize(&g_test_cartridge.cartridge, *g_test_cartridge.rom.bank, 2 * 16 * 1024) == DMGL_FAILURE)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_cartridge.checksum.value = 0xEF;
    g_test_cartridge.header->checksum = g_test_cartridge.checksum.value;
    g_test_cartridge.header->rom = 20;

    if(DMGL_ASSERT(dmgl_cartridge_initialize(&g_test_cartridge.cartridge, *g_test_cartridge.rom.bank, 2 * 16 * 1024) == DMGL_FAILURE)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_cartridge.checksum.value = 0xEF;
    g_test_cartridge.header->checksum = g_test_cartridge.checksum.value;
    g_test_cartridge.header->ram = 20;

    if(DMGL_ASSERT(dmgl_cartridge_initialize(&g_test_cartridge.cartridge, *g_test_cartridge.rom.bank, 2 * 16 * 1024) == DMGL_FAILURE)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_cartridge.checksum.value = 0xEF;
    g_test_cartridge.header->checksum = g_test_cartridge.checksum.value;
    g_test_cartridge.header->rom = 1;

    if(DMGL_ASSERT(dmgl_cartridge_initialize(&g_test_cartridge.cartridge, *g_test_cartridge.rom.bank, 2 * 16 * 1024) == DMGL_FAILURE)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_cartridge.checksum.value = 0xEF;
    g_test_cartridge.header->checksum = g_test_cartridge.checksum.value;

    if(DMGL_ASSERT(dmgl_cartridge_initialize(&g_test_cartridge.cartridge, *g_test_cartridge.rom.bank, 2 * 16 * 1024) == DMGL_FAILURE)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_cartridge.checksum.value = 0xEF;
    g_test_cartridge.header->checksum = g_test_cartridge.checksum.value;
    g_test_cartridge.ram.allocate_bank = true;

    if(DMGL_ASSERT(dmgl_cartridge_initialize(&g_test_cartridge.cartridge, *g_test_cartridge.rom.bank, 2 * 16 * 1024) == DMGL_FAILURE)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_cartridge.checksum.value = 0xEF;
    g_test_cartridge.header->checksum = g_test_cartridge.checksum.value;
    g_test_cartridge.ram.allocate_bank = true;
    g_test_cartridge.ram.allocate_data = true;

    if(DMGL_ASSERT(dmgl_cartridge_initialize(&g_test_cartridge.cartridge, *g_test_cartridge.rom.bank, 2 * 16 * 1024) == DMGL_FAILURE)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_cartridge.checksum.value = 0xEF;
    g_test_cartridge.header->checksum = g_test_cartridge.checksum.value;
    g_test_cartridge.ram.allocate_bank = true;
    g_test_cartridge.ram.allocate_data = true;
    g_test_cartridge.rom.allocate_bank = true;

    if(DMGL_ASSERT(dmgl_cartridge_initialize(&g_test_cartridge.cartridge, *g_test_cartridge.rom.bank, 2 * 16 * 1024) == DMGL_SUCCESS)) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_ram_count(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    if(DMGL_ASSERT(dmgl_cartridge_ram_count(&g_test_cartridge.cartridge) == 2)) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_ram_read(void)
{
    uint8_t data = 0x00;
    dmgl_error_e result = DMGL_SUCCESS;

    for(size_t index = 0; index < 2; ++index) {

        for(uint16_t address = 0x0000; address <= 0x1FFF; ++address, ++data) {
            dmgl_test_initialize();
            g_test_cartridge.ram.data[address] = data;

            if(DMGL_ASSERT(dmgl_cartridge_ram_read(&g_test_cartridge.cartridge, index, address) == data)) {
                result = DMGL_FAILURE;
                goto exit;
            }
        }
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_ram_write(void)
{
    uint8_t data = 0x00;
    dmgl_error_e result = DMGL_SUCCESS;

    for(size_t index = 0; index < 2; ++index) {

        for(uint16_t address = 0x0000; address <= 0x1FFF; ++address, ++data) {
            dmgl_test_initialize();
            dmgl_cartridge_ram_write(&g_test_cartridge.cartridge, index, address, data);

            if(DMGL_ASSERT(g_test_cartridge.ram.data[address] == data)) {
                result = DMGL_FAILURE;
                goto exit;
            }
        }
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_reset(void)
{
    uint8_t data = 0x00;
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    for(size_t index = 0; index < 2; ++index) {

        for(uint16_t address = 0x0000; address <= 0x1FFF; ++address, ++data) {
            g_test_cartridge.cartridge.ram.bank[index][address] = data;
        }
    }

    dmgl_cartridge_reset(&g_test_cartridge.cartridge);

    for(size_t index = 0; index < 2; ++index) {

        for(uint16_t address = 0x0000; address <= 0x1FFF; ++address, ++data) {

            if(DMGL_ASSERT(g_test_cartridge.cartridge.ram.bank[index][address] == 0xFF)) {
                result = DMGL_FAILURE;
                goto exit;
            }
        }
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_rom_count(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    if(DMGL_ASSERT(dmgl_cartridge_rom_count(&g_test_cartridge.cartridge) == 2)) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_rom_read(void)
{
    uint8_t data = 0x00;
    dmgl_error_e result = DMGL_SUCCESS;

    for(size_t index = 0; index < 2; ++index) {

        for(uint16_t address = 0x0000; address <= 0x3FFF; ++address, ++data) {
            dmgl_test_initialize();
            g_test_cartridge.rom.data[address] = data;

            if(DMGL_ASSERT(dmgl_cartridge_rom_read(&g_test_cartridge.cartridge, index, address) == data)) {
                result = DMGL_FAILURE;
                goto exit;
            }
        }
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_title(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    memcpy(g_test_cartridge.header->title, "Test", strlen("Test"));

    if(DMGL_ASSERT(!strcmp(dmgl_cartridge_title(&g_test_cartridge.cartridge), "Test"))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_type(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    for(dmgl_cartridge_e type = 0; type < DMGL_CARTRIDGE_MAX; ++type) {
        dmgl_test_initialize();
        g_test_cartridge.header->type = type;

        if(DMGL_ASSERT(dmgl_cartridge_type(&g_test_cartridge.cartridge) == type)) {
            result = DMGL_FAILURE;
            goto exit;
        }
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_uninitialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    dmgl_cartridge_uninitialize(&g_test_cartridge.cartridge);

    if(DMGL_ASSERT((g_test_cartridge.cartridge.ram.bank == NULL)
            && (g_test_cartridge.cartridge.ram.count == 0)
            && (g_test_cartridge.cartridge.rom.bank == NULL)
            && (g_test_cartridge.cartridge.rom.count == 0))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

int main(void)
{
    dmgl_error_e result = DMGL_SUCCESS;
    const dmgl_test_cb tests[] = {
        dmgl_test_cartridge_checksum, dmgl_test_cartridge_initialize, dmgl_test_cartridge_ram_count, dmgl_test_cartridge_ram_read,
        dmgl_test_cartridge_ram_write, dmgl_test_cartridge_reset, dmgl_test_cartridge_rom_count, dmgl_test_cartridge_rom_read,
        dmgl_test_cartridge_title, dmgl_test_cartridge_type, dmgl_test_cartridge_uninitialize,
        };

    for(int index = 0; index < (sizeof(tests) / sizeof(*(tests))); ++index) {

        if(tests[index]() == DMGL_FAILURE) {
            result = DMGL_FAILURE;
        }
    }

    return result;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
