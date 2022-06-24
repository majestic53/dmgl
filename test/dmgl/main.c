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
 * @brief DMGL test application.
 */

#include <test.h>

/*!
 * @struct dmgl_test_t
 * @brief DMGL test context.
 */
typedef struct {
    const char *error;                  /*!< DMGL error string */
    dmgl_version_t version;             /*!< DMGL version */

    struct {
        const dmgl_t *context;          /*!< DMGL bus context */
        const char *title;              /*!< DMGL bus title string */
        bool initialized;               /*!< DMGL bus intitialized flag */

        struct {
            dmgl_error_e clock;         /*!< DMGL bus clock status */
            dmgl_error_e initialize;    /*!< DMGL bus initialize status */
        } status;                       /*!< DMGL bus status */
    } bus;                              /*!< DMGL bus */

    struct {
        const dmgl_t *context;          /*!< DMGL service context */
        const char *title;              /*!< DMGL service title string */
        bool initialized;               /*!< DMGL service intitialized flag */

        struct {
            dmgl_error_e initialize;    /*!< DMGL service initialize status */
            dmgl_error_e poll;          /*!< DMGL service poll status */
            dmgl_error_e sync;          /*!< DMGL service sync status */
        } status;                       /*!< DMGL service status */
    } service;                          /*!< DMGL service */
} dmgl_test_t;

static dmgl_test_t g_test = {};         /*!< DMGL test context */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

dmgl_error_e dmgl_bus_clock(void)
{
    return g_test.bus.status.clock;
}

dmgl_error_e dmgl_bus_initialize(const dmgl_t *context)
{
    g_test.bus.context = context;
    g_test.bus.initialized = true;

    return g_test.bus.status.initialize;
}

const char *dmgl_bus_title(void)
{
    return g_test.bus.title;
}

void dmgl_bus_uninitialize(void)
{
    g_test.bus.initialized = false;
}

const char *dmgl_error(void)
{
    return g_test.error;
}

dmgl_error_e dmgl_service_initialize(const dmgl_t *context, const char *title)
{
    g_test.service.context = context;
    g_test.service.title = title;
    g_test.service.initialized = true;

    return g_test.service.status.initialize;
}

dmgl_error_e dmgl_service_poll(void)
{
    return g_test.service.status.poll;
}

dmgl_error_e dmgl_service_sync(void)
{
    return g_test.service.status.sync;
}

void dmgl_service_uninitialize(void)
{
    g_test.service.initialized = false;
}

const dmgl_version_t *dmgl_version(void)
{
    return &g_test.version;
}

/*!
 * @brief Initilalize test context.
 */
static inline void dmgl_test_initialize(void)
{
    memset(&g_test, 0, sizeof(g_test));
    g_test.version.major = DMGL_MAJOR;
    g_test.version.minor = DMGL_MINOR;
    g_test.version.patch = DMGL_PATCH;
}

/*!
 * @brief Test DMGL.
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
static dmgl_error_e dmgl_test(void)
{
    dmgl_t context = {};
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test.bus.status.initialize = DMGL_FAILURE;

    if(DMGL_ASSERT((dmgl(&context) == DMGL_FAILURE)
            && (g_test.bus.context == &context)
            && (g_test.bus.initialized == false)
            && (g_test.service.context == NULL)
            && (g_test.service.title == NULL)
            && (g_test.service.initialized == false))) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test.bus.title = "Test";
    g_test.service.status.initialize = DMGL_FAILURE;

    if(DMGL_ASSERT((dmgl(&context) == DMGL_FAILURE)
            && (g_test.bus.context == &context)
            && (g_test.bus.initialized == false)
            && (g_test.service.context == &context)
            && (g_test.service.title == g_test.bus.title)
            && (g_test.service.initialized == false))) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test.bus.title = "Test";
    g_test.service.status.poll = DMGL_FAILURE;

    if(DMGL_ASSERT((dmgl(&context) == DMGL_FAILURE)
            && (g_test.bus.context == &context)
            && (g_test.bus.initialized == false)
            && (g_test.service.context == &context)
            && (g_test.service.title == g_test.bus.title)
            && (g_test.service.initialized == false))) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test.bus.title = "Test";
    g_test.service.status.poll = DMGL_COMPLETE;

    if(DMGL_ASSERT((dmgl(&context) == DMGL_SUCCESS)
            && (g_test.bus.context == &context)
            && (g_test.bus.initialized == false)
            && (g_test.service.context == &context)
            && (g_test.service.title == g_test.bus.title)
            && (g_test.service.initialized == false))) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test.bus.title = "Test";
    g_test.bus.status.clock = DMGL_FAILURE;

    if(DMGL_ASSERT((dmgl(&context) == DMGL_FAILURE)
            && (g_test.bus.context == &context)
            && (g_test.bus.initialized == false)
            && (g_test.service.context == &context)
            && (g_test.service.title == g_test.bus.title)
            && (g_test.service.initialized == false))) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test.bus.title = "Test";
    g_test.bus.status.clock = DMGL_COMPLETE;
    g_test.service.status.sync = DMGL_FAILURE;

    if(DMGL_ASSERT((dmgl(&context) == DMGL_FAILURE)
            && (g_test.bus.context == &context)
            && (g_test.bus.initialized == false)
            && (g_test.service.context == &context)
            && (g_test.service.title == g_test.bus.title)
            && (g_test.service.initialized == false))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

/*!
 * @brief Test DMGL error.
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
static dmgl_error_e dmgl_error_test(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test.error = "Test";

    if(DMGL_ASSERT(!strcmp(dmgl_error(), "Test"))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

/*!
 * @brief Test DMGL version.
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
static dmgl_error_e dmgl_version_test(void)
{
    dmgl_error_e result = DMGL_SUCCESS;
    const dmgl_version_t *version = NULL;

    dmgl_test_initialize();

    if(DMGL_ASSERT((version = dmgl_version())
            && (version->major == DMGL_MAJOR)
            && (version->minor == DMGL_MINOR)
            && (version->patch == DMGL_PATCH))) {
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
        dmgl_test, dmgl_error_test, dmgl_version_test,
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
