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
#include <test.h>

typedef struct {

    /* TODO */

} dmgl_test_memory_t;

static dmgl_test_memory_t g_test_memory = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void dmgl_bootloader_disable(dmgl_bootloader_t *bootloader)
{
    /* TODO */
}

bool dmgl_bootloader_enabled(const dmgl_bootloader_t *bootloader)
{
    /* TODO */
    return false;
    /* ---- */
}

dmgl_error_e dmgl_bootloader_initialize(dmgl_bootloader_t *bootloader, const uint8_t *data, size_t length)
{
    /* TODO */
    return DMGL_SUCCESS;
    /* ---- */
}

uint8_t dmgl_bootloader_read(const dmgl_bootloader_t *bootloader, uint16_t address)
{
    /* TODO */
    return 0;
    /* ---- */
}

void dmgl_bootloader_uninitialize(dmgl_bootloader_t *bootloader)
{
    /* TODO */
}

dmgl_error_e dmgl_mapper_initialize(dmgl_mapper_t *mapper, const uint8_t *data, size_t length)
{
    /* TODO */
    return DMGL_SUCCESS;
    /* ---- */
}

uint8_t dmgl_mapper_read(const dmgl_mapper_t *mapper, uint16_t address)
{
    /* TODO */
    return 0;
    /* ---- */
}

const char *dmgl_mapper_title(const dmgl_mapper_t *mapper)
{
    /* TODO */
    return NULL;
    /* ---- */
}

void dmgl_mapper_uninitialize(dmgl_mapper_t *mapper)
{
    /* TODO */
}

void dmgl_mapper_write(dmgl_mapper_t *mapper, uint16_t address, uint8_t value)
{
    /* TODO */
}

static inline void dmgl_test_initialize(void)
{
    memset(&g_test_memory, 0, sizeof(g_test_memory));
}

static dmgl_error_e dmgl_test_memory_initialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_memory_read(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_memory_title(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_memory_uninitialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_memory_write(void)
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
        dmgl_test_memory_initialize, dmgl_test_memory_read, dmgl_test_memory_title, dmgl_test_memory_uninitialize,
        dmgl_test_memory_write,
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
