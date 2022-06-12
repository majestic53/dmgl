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

#include <mapper.h>
#include <test.h>

typedef struct {
    dmgl_mapper_t mapper;

    struct {
        const dmgl_cartridge_t *cartridge;
        const uint8_t *data;
        size_t length;
        dmgl_error_e status;
        const char *title;
        dmgl_cartridge_e type;
        bool initialized;
    } cartridge;

    struct {
        const dmgl_cartridge_t *cartridge;
        void *context;
        dmgl_error_e status;
        uint16_t address;
        uint8_t value;
        bool initialized;
    } mbc;
} dmgl_test_mapper_t;

static dmgl_test_mapper_t g_test_mapper = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

dmgl_error_e dmgl_cartridge_initialize(dmgl_cartridge_t *cartridge, const uint8_t *data, size_t length)
{
    g_test_mapper.cartridge.cartridge = cartridge;
    g_test_mapper.cartridge.data = data;
    g_test_mapper.cartridge.length = length;
    g_test_mapper.cartridge.initialized = true;

    return g_test_mapper.cartridge.status;
}

const char *dmgl_cartridge_title(const dmgl_cartridge_t *cartridge)
{
    g_test_mapper.cartridge.cartridge = cartridge;

    return g_test_mapper.cartridge.title;
}

dmgl_cartridge_e dmgl_cartridge_type(const dmgl_cartridge_t *cartridge)
{
    g_test_mapper.cartridge.cartridge = cartridge;

    return g_test_mapper.cartridge.type;
}

void dmgl_cartridge_uninitialize(dmgl_cartridge_t *cartridge)
{
    g_test_mapper.cartridge.cartridge = cartridge;
    g_test_mapper.cartridge.initialized = false;
}

dmgl_error_e dmgl_mbc_initialize(const dmgl_cartridge_t *cartridge, void **context)
{
    g_test_mapper.mbc.cartridge = cartridge;
    g_test_mapper.mbc.context = *context;
    g_test_mapper.mbc.initialized = true;

    return g_test_mapper.mbc.status;
}

uint8_t dmgl_mbc_read(const dmgl_cartridge_t *cartridge, void *context, uint16_t address)
{
    g_test_mapper.cartridge.cartridge = cartridge;
    g_test_mapper.mbc.context = context;
    g_test_mapper.mbc.address = address;

    return g_test_mapper.mbc.value;
}

void dmgl_mbc_uninitialize(void *context)
{
    g_test_mapper.mbc.context = context;
    g_test_mapper.mbc.initialized = false;
}

void dmgl_mbc_write(dmgl_cartridge_t *cartridge, void *context, uint16_t address, uint8_t value)
{
    g_test_mapper.cartridge.cartridge = cartridge;
    g_test_mapper.mbc.context = context;
    g_test_mapper.mbc.address = address;
    g_test_mapper.mbc.value = value;
}

dmgl_error_e dmgl_mbc0_initialize(const dmgl_cartridge_t *cartridge, void **context)
{
    return dmgl_mbc_initialize(cartridge, context);
}

uint8_t dmgl_mbc0_read(const dmgl_cartridge_t *cartridge, void *context, uint16_t address)
{
    return dmgl_mbc_read(cartridge, context, address);
}

void dmgl_mbc0_uninitialize(void *context)
{
    dmgl_mbc_uninitialize(context);
}

void dmgl_mbc0_write(dmgl_cartridge_t *cartridge, void *context, uint16_t address, uint8_t value)
{
    dmgl_mbc_write(cartridge, context, address, value);
}

static inline void dmgl_test_initialize(void)
{
    memset(&g_test_mapper, 0, sizeof(g_test_mapper));
}

static dmgl_error_e dmgl_test_mapper_initialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_mapper.cartridge.status = DMGL_FAILURE;

    if(DMGL_ASSERT(dmgl_mapper_initialize(&g_test_mapper.mapper, (const uint8_t *)1, 256) == DMGL_FAILURE)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    for(dmgl_cartridge_e type = 0; type < DMGL_CARTRIDGE_MAX; ++type) {
        const dmgl_mapper_handler_t handler[] = {
            { dmgl_mbc0_initialize, dmgl_mbc0_read, dmgl_mbc0_uninitialize, dmgl_mbc0_write, },
            };

        dmgl_test_initialize();
        g_test_mapper.cartridge.type = type;
        g_test_mapper.mapper.handler.initialize = dmgl_mbc_initialize;
        g_test_mapper.mbc.status = DMGL_FAILURE;

        if(DMGL_ASSERT(dmgl_mapper_initialize(&g_test_mapper.mapper, (const uint8_t *)1, 256) == DMGL_FAILURE)) {
            result = DMGL_FAILURE;
            goto exit;
        }

        dmgl_test_initialize();
        g_test_mapper.cartridge.type = type;
        g_test_mapper.mapper.handler.initialize = dmgl_mbc_initialize;
        g_test_mapper.mbc.status = DMGL_SUCCESS;

        if(DMGL_ASSERT((dmgl_mapper_initialize(&g_test_mapper.mapper, (const uint8_t *)1, 256) == DMGL_SUCCESS)
                && (g_test_mapper.cartridge.cartridge == &g_test_mapper.mapper.cartridge)
                && (g_test_mapper.cartridge.data == (const uint8_t *)1)
                && (g_test_mapper.cartridge.length == 256)
                && (g_test_mapper.cartridge.initialized == true)
                && (g_test_mapper.mbc.cartridge == &g_test_mapper.mapper.cartridge)
                && (g_test_mapper.mbc.context == g_test_mapper.mapper.context)
                && (g_test_mapper.mbc.initialized == true)
                && (g_test_mapper.mapper.handler.initialize == handler[type].initialize)
                && (g_test_mapper.mapper.handler.read == handler[type].read)
                && (g_test_mapper.mapper.handler.uninitialize == handler[type].uninitialize)
                && (g_test_mapper.mapper.handler.write == handler[type].write))) {
            result = DMGL_FAILURE;
            goto exit;
        }
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_mapper_read(void)
{
    uint8_t data = 0x00;
    dmgl_error_e result = DMGL_SUCCESS;

    for(uint32_t address = 0x0000; address <= 0xFFFF; ++address, ++data) {
        dmgl_test_initialize();
        g_test_mapper.mapper.context = (void *)1;
        g_test_mapper.mapper.handler.read = dmgl_mbc_read;
        g_test_mapper.mbc.value = data;

        if(DMGL_ASSERT((dmgl_mapper_read(&g_test_mapper.mapper, address) == data)
                && (g_test_mapper.cartridge.cartridge == &g_test_mapper.mapper.cartridge)
                && (g_test_mapper.mbc.context == g_test_mapper.mapper.context)
                && (g_test_mapper.mbc.address == address))) {
            result = DMGL_FAILURE;
            goto exit;
        }
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_mapper_title(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_mapper.cartridge.title = "Test";

    if(DMGL_ASSERT(!strcmp(dmgl_mapper_title(&g_test_mapper.mapper), "Test")
            && (g_test_mapper.cartridge.cartridge == &g_test_mapper.mapper.cartridge))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_mapper_uninitialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_mapper.cartridge.initialized = true;
    g_test_mapper.mbc.initialized = true;
    g_test_mapper.mapper.context = (void *)1;
    g_test_mapper.mapper.handler.uninitialize = dmgl_mbc_uninitialize;
    dmgl_mapper_uninitialize(&g_test_mapper.mapper);

    if(DMGL_ASSERT((g_test_mapper.cartridge.cartridge == &g_test_mapper.mapper.cartridge)
            && (g_test_mapper.cartridge.initialized == false)
            && (g_test_mapper.mbc.context == (void *)1)
            && (g_test_mapper.mapper.context == NULL)
            && (g_test_mapper.mbc.initialized == false))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_mapper_write(void)
{
    uint8_t data = 0x00;
    dmgl_error_e result = DMGL_SUCCESS;

    for(uint32_t address = 0x0000; address <= 0xFFFF; ++address, ++data) {
        dmgl_test_initialize();
        g_test_mapper.mapper.context = (void *)1;
        g_test_mapper.mapper.handler.write = dmgl_mbc_write;
        dmgl_mapper_write(&g_test_mapper.mapper, address, data);

        if(DMGL_ASSERT((g_test_mapper.cartridge.cartridge == &g_test_mapper.mapper.cartridge)
                && (g_test_mapper.mbc.context == g_test_mapper.mapper.context)
                && (g_test_mapper.mbc.address == address)
                && (g_test_mapper.mbc.value == data))) {
            result = DMGL_FAILURE;
            goto exit;
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
        dmgl_test_mapper_initialize, dmgl_test_mapper_read, dmgl_test_mapper_title, dmgl_test_mapper_uninitialize,
        dmgl_test_mapper_write,
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
