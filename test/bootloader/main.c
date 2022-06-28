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
 * @file main.c
 * @brief Bootloader test application.
 */

#include <bootloader.h>
#include <test.h>

/*!
 * @struct dmgl_test_bootloader_t
 * @brief Bootloader test context.
 */
typedef struct {
    dmgl_bootloader_t bootloader;                       /*!< Bootloader test context */
    uint8_t data[256];                                  /*!< Bootloader data */

    struct {
        const void *data;                               /*!< Bootloader checksum data */
        uint8_t value;                                  /*!< Bootloader checksum value */
        size_t begin;                                   /*!< Bootloader checksum begin index */
        size_t end;                                     /*!< Bootloader checksum end index */
    } checksum;                                         /*!< Bootloader checksum */
} dmgl_test_bootloader_t;

static dmgl_test_bootloader_t g_test_bootloader = {};   /*!< Bootloader test context */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t dmgl_checksum(const void *data, size_t begin, size_t end)
{
    g_test_bootloader.checksum.data = data;
    g_test_bootloader.checksum.begin = begin;
    g_test_bootloader.checksum.end = end;

    return g_test_bootloader.checksum.value;
}

dmgl_error_e dmgl_error_set(const char *file, const char *function, size_t line, const char *format, ...)
{
    return DMGL_FAILURE;
}

/*!
 * @brief Initilalize test context.
 */
static inline void dmgl_test_initialize(void)
{
    memset(&g_test_bootloader, 0, sizeof(g_test_bootloader));
}

/*!
 * @brief Test bootloader disable.
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
static dmgl_error_e dmgl_test_bootloader_disable(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    dmgl_bootloader_disable(&g_test_bootloader.bootloader);

    if(DMGL_ASSERT(g_test_bootloader.bootloader.enabled == false)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_bootloader.bootloader.enabled = true;
    dmgl_bootloader_disable(&g_test_bootloader.bootloader);

    if(DMGL_ASSERT(g_test_bootloader.bootloader.enabled == false)) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

/*!
 * @brief Test bootloader enabled.
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
static dmgl_error_e dmgl_test_bootloader_enabled(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    if(DMGL_ASSERT(dmgl_bootloader_enabled(&g_test_bootloader.bootloader) == false)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_bootloader.bootloader.enabled = true;

    if(DMGL_ASSERT(dmgl_bootloader_enabled(&g_test_bootloader.bootloader) == true)) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

/*!
 * @brief Test bootloader initialize.
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
static dmgl_error_e dmgl_test_bootloader_initialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    if(DMGL_ASSERT((dmgl_bootloader_initialize(&g_test_bootloader.bootloader, NULL, 0) == DMGL_SUCCESS)
            && (g_test_bootloader.bootloader.data == NULL)
            && (g_test_bootloader.bootloader.enabled == false))) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();

    if(DMGL_ASSERT((dmgl_bootloader_initialize(&g_test_bootloader.bootloader, NULL, 0) == DMGL_SUCCESS)
            && (g_test_bootloader.bootloader.data == NULL)
            && (g_test_bootloader.bootloader.enabled == false))) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();

    if(DMGL_ASSERT((dmgl_bootloader_initialize(&g_test_bootloader.bootloader, g_test_bootloader.data, 1) == DMGL_FAILURE)
            && (g_test_bootloader.bootloader.data == NULL)
            && (g_test_bootloader.bootloader.enabled == false))) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();

    if(DMGL_ASSERT((dmgl_bootloader_initialize(&g_test_bootloader.bootloader, g_test_bootloader.data, 256) == DMGL_FAILURE)
            && (g_test_bootloader.bootloader.data == NULL)
            && (g_test_bootloader.bootloader.enabled == false)
            && (g_test_bootloader.checksum.data == g_test_bootloader.data)
            && (g_test_bootloader.checksum.begin == 0)
            && (g_test_bootloader.checksum.end == 255))) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_bootloader.checksum.value = 0x92;

    if(DMGL_ASSERT((dmgl_bootloader_initialize(&g_test_bootloader.bootloader, g_test_bootloader.data, 256) == DMGL_SUCCESS)
            && (g_test_bootloader.bootloader.data == g_test_bootloader.data)
            && (g_test_bootloader.bootloader.enabled == true)
            && (g_test_bootloader.checksum.data == g_test_bootloader.data)
            && (g_test_bootloader.checksum.begin == 0)
            && (g_test_bootloader.checksum.end == 255))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

/*!
 * @brief Test bootloader read.
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
static dmgl_error_e dmgl_test_bootloader_read(void)
{
    uint8_t data = 0x00;
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_bootloader.bootloader.data = g_test_bootloader.data;

    for(uint32_t address = 0x0000; address <= 0xFFFF; ++address, ++data) {
        uint8_t expected = 0xFF;

        for(int enabled = false; enabled <= true; ++enabled) {
            g_test_bootloader.bootloader.enabled = enabled;

            switch(address) {
                case 0x0000 ... 0x00FF:
                    g_test_bootloader.data[address] = data;
                    expected = enabled ? data : 0xFF;
                    break;
                default:
                    break;
            }

            if(DMGL_ASSERT(dmgl_bootloader_read(&g_test_bootloader.bootloader, address) == expected)) {
                result = DMGL_FAILURE;
                goto exit;
            }
        }
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

/*!
 * @brief Test bootloader reset.
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
static dmgl_error_e dmgl_test_bootloader_reset(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_bootloader.bootloader.data = NULL;
    dmgl_bootloader_reset(&g_test_bootloader.bootloader);

    if(DMGL_ASSERT(g_test_bootloader.bootloader.enabled == false)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_bootloader.bootloader.data = g_test_bootloader.data;
    dmgl_bootloader_reset(&g_test_bootloader.bootloader);

    if(DMGL_ASSERT(g_test_bootloader.bootloader.enabled == true)) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

/*!
 * @brief Test bootloader uninitialize.
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
static dmgl_error_e dmgl_test_bootloader_uninitialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_bootloader.bootloader.data = g_test_bootloader.data;
    g_test_bootloader.bootloader.enabled = true;
    dmgl_bootloader_uninitialize(&g_test_bootloader.bootloader);

    if(DMGL_ASSERT((g_test_bootloader.bootloader.data == NULL)
            && (g_test_bootloader.bootloader.enabled == false))) {
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
        dmgl_test_bootloader_disable, dmgl_test_bootloader_enabled, dmgl_test_bootloader_initialize, dmgl_test_bootloader_read,
        dmgl_test_bootloader_reset, dmgl_test_bootloader_uninitialize,
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
