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

#include <processor.h>
#include <test.h>

typedef struct {
    dmgl_processor_t processor;
    dmgl_processor_t expected;

    /* TODO */

} dmgl_test_processor_t;

static dmgl_test_processor_t g_test_processor = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void dmgl_bus_write(uint16_t address, uint8_t value)
{
    /* TODO */
}

static inline void dmgl_test_initialize(void)
{
    memset(&g_test_processor, 0, sizeof(g_test_processor));
}

static inline dmgl_error_e dmgl_test_match(void)
{
    dmgl_error_e result;

    /* TODO: CHECK EACH MEMBER MATCHES BETWEEN PROCESSOR AND EXPECTED */
    result = DMGL_SUCCESS;
    /* ---- */

    return result;
}

static dmgl_error_e dmgl_test_processor_cycle(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_initialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO: SET EXPECTED */

    if(DMGL_ASSERT((dmgl_processor_initialize(&g_test_processor.processor, false, 0x00) == DMGL_SUCCESS)
            && (dmgl_test_match() == DMGL_SUCCESS))) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();

    /* TODO: SET EXPECTED */

    if(DMGL_ASSERT((dmgl_processor_initialize(&g_test_processor.processor, false, 0x01) == DMGL_SUCCESS)
            && (dmgl_test_match() == DMGL_SUCCESS))) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();

    /* TODO: SET EXPECTED */

    if(DMGL_ASSERT((dmgl_processor_initialize(&g_test_processor.processor, true, 0x00) == DMGL_SUCCESS)
            && (dmgl_test_match() == DMGL_SUCCESS))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_read(void)
{
    uint8_t data = 0x00;
    dmgl_error_e result = DMGL_SUCCESS;

    for(uint32_t address = 0x0000; address <= 0xFFFF; ++address, ++data) {
        dmgl_test_initialize();

        switch(address) {
            case 0xFF0F:
                g_test_processor.processor.interrupt.flag.raw = 0xEF;

                if(DMGL_ASSERT(dmgl_processor_read(&g_test_processor.processor, address) == 0xEF)) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            case 0xFFFF:
                g_test_processor.processor.interrupt.enable.raw = 0xEF;

                if(DMGL_ASSERT(dmgl_processor_read(&g_test_processor.processor, address) == 0xEF)) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            default:

                if(DMGL_ASSERT(dmgl_processor_read(&g_test_processor.processor, address) == 0x00)) {
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

static dmgl_error_e dmgl_test_processor_uninitialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    dmgl_processor_uninitialize(&g_test_processor.processor);

    if(DMGL_ASSERT(dmgl_test_match() == DMGL_SUCCESS)) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_write(void)
{
    uint8_t data = 0x00;
    dmgl_error_e result = DMGL_SUCCESS;

    for(uint32_t address = 0x0000; address <= 0xFFFF; ++address, ++data) {
        dmgl_test_initialize();
        dmgl_processor_write(&g_test_processor.processor, address, 0xEF);

        switch(address) {
            case 0xFF0F:

                if(DMGL_ASSERT(g_test_processor.processor.interrupt.flag.raw == 0xEF)) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            case 0xFFFF:

                if(DMGL_ASSERT(g_test_processor.processor.interrupt.enable.raw == 0xEF)) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            default:

                if(DMGL_ASSERT(dmgl_test_match() == DMGL_SUCCESS)) {
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
        dmgl_test_processor_cycle, dmgl_test_processor_initialize, dmgl_test_processor_read, dmgl_test_processor_uninitialize,
        dmgl_test_processor_write,
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
