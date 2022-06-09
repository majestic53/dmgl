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

    /* TODO */

} dmgl_test_mbc0_t;

static dmgl_test_mbc0_t g_test_mbc0 = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t dmgl_cartridge_ram_read(const dmgl_cartridge_t *cartridge, size_t index, uint16_t address)
{
    /* TODO */
    return 0;
    /* ---- */
}

void dmgl_cartridge_ram_write(dmgl_cartridge_t *cartridge, size_t index, uint16_t address, uint8_t value)
{
    /* TODO */
}

uint8_t dmgl_cartridge_rom_read(const dmgl_cartridge_t *cartridge, size_t index, uint16_t address)
{
    /* TODO */
    return 0;
    /* ---- */
}

static inline void dmgl_test_initialize(void)
{
    memset(&g_test_mbc0, 0, sizeof(g_test_mbc0));
}

static dmgl_error_e dmgl_test_mbc0_initialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_mbc0_read(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_mbc0_uninitialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_mbc0_write(void)
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
        dmgl_test_mbc0_initialize, dmgl_test_mbc0_read, dmgl_test_mbc0_uninitialize, dmgl_test_mbc0_write,
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