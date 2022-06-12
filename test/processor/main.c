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

    /* TODO */

} dmgl_test_processor_t;

static dmgl_test_processor_t g_test_processor = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static inline void dmgl_test_initialize(void)
{
    memset(&g_test_processor, 0, sizeof(g_test_processor));
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

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_read(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_uninitialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_write(void)
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
