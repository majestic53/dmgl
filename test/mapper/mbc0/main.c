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
#include <test.h>

typedef struct {
    dmgl_cartridge_t cartridge;
    void *context;

    struct {
        const dmgl_cartridge_t *cartridge;
        size_t index;
        uint16_t address;
        uint8_t value;
    } ram;

    struct {
        const dmgl_cartridge_t *cartridge;
        size_t index;
        uint16_t address;
        uint8_t value;
    } rom;
} dmgl_test_mbc0_t;

static dmgl_test_mbc0_t g_test_mbc0 = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t dmgl_cartridge_ram_read(const dmgl_cartridge_t *cartridge, size_t index, uint16_t address)
{
    g_test_mbc0.ram.cartridge = cartridge;
    g_test_mbc0.ram.index = index;
    g_test_mbc0.ram.address = address;

    return g_test_mbc0.ram.value;
}

void dmgl_cartridge_ram_write(dmgl_cartridge_t *cartridge, size_t index, uint16_t address, uint8_t value)
{
    g_test_mbc0.ram.cartridge = cartridge;
    g_test_mbc0.ram.index = index;
    g_test_mbc0.ram.address = address;
    g_test_mbc0.ram.value = value;
}

uint8_t dmgl_cartridge_rom_read(const dmgl_cartridge_t *cartridge, size_t index, uint16_t address)
{
    g_test_mbc0.rom.cartridge = cartridge;
    g_test_mbc0.rom.index = index;
    g_test_mbc0.rom.address = address;

    return g_test_mbc0.rom.value;
}

static inline void dmgl_test_initialize(void)
{
    memset(&g_test_mbc0, 0, sizeof(g_test_mbc0));
}

static dmgl_error_e dmgl_test_mbc0_initialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_mbc0.context = (void *)1;

    if(DMGL_ASSERT((dmgl_mbc0_initialize(&g_test_mbc0.cartridge, &g_test_mbc0.context) == DMGL_SUCCESS)
            && (g_test_mbc0.context == NULL))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_mbc0_read(void)
{
    uint8_t data = 0x00;
    dmgl_error_e result = DMGL_SUCCESS;

    for(uint32_t address = 0x0000; address <= 0xFFFF; ++address, ++data) {
        uint8_t value;

        dmgl_test_initialize();

        switch(address) {
            case 0x0000 ... 0x3FFF:
            case 0x4000 ... 0x7FFF:
                g_test_mbc0.rom.value = data;
                break;
            case 0xA000 ... 0xBFFF:
                g_test_mbc0.ram.value = data;
                break;
            default:
                break;
        }

        value = dmgl_mbc0_read(&g_test_mbc0.cartridge, g_test_mbc0.context, address);

        switch(address) {
            case 0x0000 ... 0x3FFF:

                if(DMGL_ASSERT((value == data)
                        && (g_test_mbc0.ram.cartridge == NULL)
                        && (g_test_mbc0.ram.index == 0)
                        && (g_test_mbc0.ram.address == 0x0000)
                        && (g_test_mbc0.ram.value == 0x00)
                        && (g_test_mbc0.rom.cartridge == &g_test_mbc0.cartridge)
                        && (g_test_mbc0.rom.index == 0)
                        && (g_test_mbc0.rom.address == address - 0x0000)
                        && (g_test_mbc0.rom.value == data))) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            case 0x4000 ... 0x7FFF:

                if(DMGL_ASSERT((value == data)
                        && (g_test_mbc0.ram.cartridge == NULL)
                        && (g_test_mbc0.ram.index == 0)
                        && (g_test_mbc0.ram.address == 0x0000)
                        && (g_test_mbc0.ram.value == 0x00)
                        && (g_test_mbc0.rom.cartridge == &g_test_mbc0.cartridge)
                        && (g_test_mbc0.rom.index == 1)
                        && (g_test_mbc0.rom.address == address - 0x4000)
                        && (g_test_mbc0.rom.value == data))) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            case 0xA000 ... 0xBFFF:

                if(DMGL_ASSERT((value == data)
                        && (g_test_mbc0.ram.cartridge == &g_test_mbc0.cartridge)
                        && (g_test_mbc0.ram.index == 0)
                        && (g_test_mbc0.ram.address == address - 0xA000)
                        && (g_test_mbc0.ram.value == data)
                        && (g_test_mbc0.rom.cartridge == NULL)
                        && (g_test_mbc0.rom.index == 0)
                        && (g_test_mbc0.rom.address == 0x0000)
                        && (g_test_mbc0.rom.value == 0x00))) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            default:

                if(DMGL_ASSERT((value == 0xFF)
                        && (g_test_mbc0.ram.cartridge == NULL)
                        && (g_test_mbc0.ram.index == 0)
                        && (g_test_mbc0.ram.address == 0x0000)
                        && (g_test_mbc0.ram.value == 0x00)
                        && (g_test_mbc0.rom.cartridge == NULL)
                        && (g_test_mbc0.rom.index == 0)
                        && (g_test_mbc0.rom.address == 0x0000)
                        && (g_test_mbc0.rom.value == 0x00))) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
        }
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_mbc0_reset(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    dmgl_mbc0_reset(&g_test_mbc0.context);

    if(DMGL_ASSERT(g_test_mbc0.context == NULL)) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_mbc0_uninitialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    dmgl_mbc0_uninitialize(&g_test_mbc0.context);

    if(DMGL_ASSERT(g_test_mbc0.context == NULL)) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_mbc0_write(void)
{
    uint8_t data = 0x00;
    dmgl_error_e result = DMGL_SUCCESS;

    for(uint32_t address = 0x0000; address <= 0xFFFF; ++address, ++data) {
        dmgl_test_initialize();
        dmgl_mbc0_write(&g_test_mbc0.cartridge, g_test_mbc0.context, address, data);

        switch(address) {
            case 0xA000 ... 0xBFFF:

                if(DMGL_ASSERT((g_test_mbc0.ram.cartridge == &g_test_mbc0.cartridge)
                        && (g_test_mbc0.ram.index == 0)
                        && (g_test_mbc0.ram.address == address - 0xA000)
                        && (g_test_mbc0.ram.value == data)
                        && (g_test_mbc0.rom.cartridge == NULL)
                        && (g_test_mbc0.rom.index == 0)
                        && (g_test_mbc0.rom.address == 0x0000)
                        && (g_test_mbc0.rom.value == 0x00))) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            default:

                if(DMGL_ASSERT((g_test_mbc0.ram.cartridge == NULL)
                        && (g_test_mbc0.ram.index == 0)
                        && (g_test_mbc0.ram.address == 0x0000)
                        && (g_test_mbc0.ram.value == 0x00)
                        && (g_test_mbc0.rom.cartridge == NULL)
                        && (g_test_mbc0.rom.index == 0)
                        && (g_test_mbc0.rom.address == 0x0000)
                        && (g_test_mbc0.rom.value == 0x00))) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
        }
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

int main(void)
{
    dmgl_error_e result = DMGL_SUCCESS;
    const dmgl_test_cb tests[] = {
        dmgl_test_mbc0_initialize, dmgl_test_mbc0_read, dmgl_test_mbc0_reset, dmgl_test_mbc0_uninitialize,
        dmgl_test_mbc0_write,
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
