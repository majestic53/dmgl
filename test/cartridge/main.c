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

    struct {
        const void *data;
        uint8_t value;
        size_t begin;
        size_t end;
    } checksum;

    /* TODO */

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
    /* TODO */
    return NULL;
    /* ---- */
}

void dmgl_buffer_free(void *buffer)
{
    /* TODO */
}

static inline void dmgl_test_initialize(void)
{
    memset(&g_test_cartridge, 0, sizeof(g_test_cartridge));
}

static dmgl_error_e dmgl_test_cartridge_initialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_ram_count(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_cartridge.cartridge.ram.count = 4;

    if(DMGL_ASSERT(dmgl_cartridge_ram_count(&g_test_cartridge.cartridge) == 4)) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_ram_read(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_ram_write(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_rom_count(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_cartridge.cartridge.rom.count = 4;

    if(DMGL_ASSERT(dmgl_cartridge_rom_count(&g_test_cartridge.cartridge) == 4)) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_rom_read(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_title(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_type(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_cartridge_uninitialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

int main(void)
{
    dmgl_error_e result = DMGL_SUCCESS;
    const dmgl_test_cb tests[] = {
        dmgl_test_cartridge_initialize, dmgl_test_cartridge_ram_count, dmgl_test_cartridge_ram_read, dmgl_test_cartridge_ram_write,
        dmgl_test_cartridge_rom_count, dmgl_test_cartridge_rom_read, dmgl_test_cartridge_title, dmgl_test_cartridge_type,
        dmgl_test_cartridge_uninitialize,
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
