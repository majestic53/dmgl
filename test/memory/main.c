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
    dmgl_memory_t memory;

    struct {
        const dmgl_bootloader_t *bootloader;
        const uint8_t *data;
        size_t length;
        uint16_t address;
        uint8_t value;
        bool enabled;
        bool initialized;
        bool reset;
        dmgl_error_e status;
    } bootloader;

    struct {
        const dmgl_mapper_t *mapper;
        const uint8_t *data;
        size_t length;
        uint16_t address;
        uint8_t value;
        uint8_t checksum;
        bool initialized;
        bool reset;
        const char *title;
        dmgl_error_e status;
    } mapper;
} dmgl_test_memory_t;

static dmgl_test_memory_t g_test_memory = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void dmgl_bootloader_disable(dmgl_bootloader_t *bootloader)
{
    g_test_memory.bootloader.bootloader = bootloader;
    g_test_memory.bootloader.enabled = false;
}

bool dmgl_bootloader_enabled(const dmgl_bootloader_t *bootloader)
{
    g_test_memory.bootloader.bootloader = bootloader;

    return g_test_memory.bootloader.enabled;
}

dmgl_error_e dmgl_bootloader_initialize(dmgl_bootloader_t *bootloader, const uint8_t *data, size_t length)
{
    g_test_memory.bootloader.bootloader = bootloader;
    g_test_memory.bootloader.data = data;
    g_test_memory.bootloader.length = length;
    g_test_memory.bootloader.initialized = true;

    return g_test_memory.bootloader.status;
}

uint8_t dmgl_bootloader_read(const dmgl_bootloader_t *bootloader, uint16_t address)
{
    g_test_memory.bootloader.bootloader = bootloader;
    g_test_memory.bootloader.address = address;

    return g_test_memory.bootloader.value;
}

void dmgl_bootloader_reset(dmgl_bootloader_t *bootloader)
{
    g_test_memory.bootloader.reset = true;
}

void dmgl_bootloader_uninitialize(dmgl_bootloader_t *bootloader)
{
    g_test_memory.bootloader.bootloader = bootloader;
    g_test_memory.bootloader.initialized = false;
}

uint8_t dmgl_mapper_checksum(const dmgl_mapper_t *mapper)
{
    g_test_memory.mapper.mapper = mapper;

    return g_test_memory.mapper.checksum;
}

dmgl_error_e dmgl_mapper_initialize(dmgl_mapper_t *mapper, const uint8_t *data, size_t length)
{
    g_test_memory.mapper.mapper = mapper;
    g_test_memory.mapper.data = data;
    g_test_memory.mapper.length = length;
    g_test_memory.mapper.initialized = true;

    return g_test_memory.mapper.status;
}

uint8_t dmgl_mapper_read(const dmgl_mapper_t *mapper, uint16_t address)
{
    g_test_memory.mapper.mapper = mapper;
    g_test_memory.mapper.address = address;

    return g_test_memory.mapper.value;
}

void dmgl_mapper_reset(dmgl_mapper_t *mapper)
{
    g_test_memory.mapper.reset = true;
}

const char *dmgl_mapper_title(const dmgl_mapper_t *mapper)
{
    g_test_memory.mapper.mapper = mapper;

    return g_test_memory.mapper.title;
}

void dmgl_mapper_uninitialize(dmgl_mapper_t *mapper)
{
    g_test_memory.mapper.mapper = mapper;
    g_test_memory.mapper.initialized = false;
}

void dmgl_mapper_write(dmgl_mapper_t *mapper, uint16_t address, uint8_t value)
{
    g_test_memory.mapper.mapper = mapper;
    g_test_memory.mapper.address = address;
    g_test_memory.mapper.value = value;
}

static inline void dmgl_test_initialize(void)
{
    memset(&g_test_memory, 0, sizeof(g_test_memory));
}

static dmgl_error_e dmgl_test_memory_checksum(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_memory.mapper.checksum = 0xEF;

    if(DMGL_ASSERT((dmgl_memory_checksum(&g_test_memory.memory) == 0xEF)
            && (g_test_memory.mapper.mapper == &g_test_memory.memory.mapper))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_memory_has_bootloader(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    if(DMGL_ASSERT((dmgl_memory_has_bootloader(&g_test_memory.memory) == false)
            && (g_test_memory.bootloader.bootloader == &g_test_memory.memory.bootloader))) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_memory.bootloader.enabled = true;

    if(DMGL_ASSERT((dmgl_memory_has_bootloader(&g_test_memory.memory) == true)
            && (g_test_memory.bootloader.bootloader == &g_test_memory.memory.bootloader))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_memory_initialize(void)
{
    dmgl_t context = {};
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_memory.bootloader.status = DMGL_FAILURE;

    if(DMGL_ASSERT(dmgl_memory_initialize(&g_test_memory.memory, &context) == DMGL_FAILURE)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_memory.mapper.status = DMGL_FAILURE;

    if(DMGL_ASSERT(dmgl_memory_initialize(&g_test_memory.memory, &context) == DMGL_FAILURE)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    context.bootloader.data = (void *)1;
    context.bootloader.length = 256;
    context.cartridge.data = (void *)2;
    context.cartridge.length = 512;

    if(DMGL_ASSERT((dmgl_memory_initialize(&g_test_memory.memory, &context) == DMGL_SUCCESS)
            && (g_test_memory.bootloader.bootloader == &g_test_memory.memory.bootloader)
            && (g_test_memory.bootloader.data == (void *)1)
            && (g_test_memory.bootloader.length == 256)
            && (g_test_memory.bootloader.initialized == true)
            && (g_test_memory.mapper.mapper == &g_test_memory.memory.mapper)
            && (g_test_memory.mapper.data == (void *)2)
            && (g_test_memory.mapper.length == 512)
            && (g_test_memory.mapper.initialized == true))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_memory_read(void)
{
    uint8_t data = 0x00;
    dmgl_error_e result = DMGL_SUCCESS;

    for(uint32_t address = 0x0000; address <= 0xFFFF; ++address, ++data) {
        dmgl_test_initialize();

        switch(address) {
            case 0x0000 ... 0x00FF:
                g_test_memory.bootloader.enabled = true;
                g_test_memory.bootloader.value = data;

                if(DMGL_ASSERT((dmgl_memory_read(&g_test_memory.memory, address) == data)
                        && (g_test_memory.bootloader.bootloader == &g_test_memory.memory.bootloader)
                        && (g_test_memory.bootloader.address == address))) {
                    result = DMGL_FAILURE;
                    goto exit;
                }

                g_test_memory.bootloader.enabled = false;
                g_test_memory.mapper.value = data;

                if(DMGL_ASSERT((dmgl_memory_read(&g_test_memory.memory, address) == data)
                        && (g_test_memory.mapper.mapper == &g_test_memory.memory.mapper)
                        && (g_test_memory.mapper.address == address))) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            case 0x8000 ... 0x9FFF:
                g_test_memory.memory.video[address - 0x8000] = data;

                if(DMGL_ASSERT(dmgl_memory_read(&g_test_memory.memory, address) == data)) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            case 0xC000 ... 0xDFFF:
                g_test_memory.memory.internal[address - 0xC000] = data;

                if(DMGL_ASSERT(dmgl_memory_read(&g_test_memory.memory, address) == data)) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            case 0xE000 ... 0xFDFF:
                g_test_memory.memory.internal[address - 0xE000] = data;

                if(DMGL_ASSERT(dmgl_memory_read(&g_test_memory.memory, address) == data)) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            case 0xFE00 ... 0xFE9F:
                g_test_memory.memory.sprite[address - 0xFE00] = data;

                if(DMGL_ASSERT(dmgl_memory_read(&g_test_memory.memory, address) == data)) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            case 0xFF80 ... 0xFFFE:
                g_test_memory.memory.high[address - 0xFF80] = data;

                if(DMGL_ASSERT(dmgl_memory_read(&g_test_memory.memory, address) == data)) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            default:
                g_test_memory.mapper.value = data;

                if(DMGL_ASSERT((dmgl_memory_read(&g_test_memory.memory, address) == data)
                        && (g_test_memory.mapper.mapper == &g_test_memory.memory.mapper)
                        && (g_test_memory.mapper.address == address))) {
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

static dmgl_error_e dmgl_test_memory_reset(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    dmgl_memory_reset(&g_test_memory.memory);

    if(DMGL_ASSERT((g_test_memory.bootloader.reset == true)
            && (g_test_memory.mapper.reset == true))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_memory_title(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_memory.mapper.title = "Test";

    if(DMGL_ASSERT(!strcmp(dmgl_memory_title(&g_test_memory.memory), "Test")
            && (g_test_memory.mapper.mapper == &g_test_memory.memory.mapper))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_memory_uninitialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_memory.bootloader.initialized = true;
    g_test_memory.mapper.initialized = true;
    dmgl_memory_uninitialize(&g_test_memory.memory);

    if(DMGL_ASSERT((g_test_memory.bootloader.bootloader == &g_test_memory.memory.bootloader)
            && (g_test_memory.bootloader.initialized == false)
            && (g_test_memory.mapper.mapper == &g_test_memory.memory.mapper)
            && (g_test_memory.mapper.initialized == false))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_memory_write(void)
{
    uint8_t data = 0x00;
    dmgl_error_e result = DMGL_SUCCESS;

    for(uint32_t address = 0x0000; address <= 0xFFFF; ++address, ++data) {
        dmgl_test_initialize();
        g_test_memory.bootloader.enabled = true;
        dmgl_memory_write(&g_test_memory.memory, address, data);

        switch(address) {
            case 0x8000 ... 0x9FFF:

                if(DMGL_ASSERT(g_test_memory.memory.video[address - 0x8000] == data)) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            case 0xC000 ... 0xDFFF:

                if(DMGL_ASSERT(g_test_memory.memory.internal[address - 0xC000] == data)) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            case 0xE000 ... 0xFDFF:

                if(DMGL_ASSERT(g_test_memory.memory.internal[address - 0xE000] == data)) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            case 0xFE00 ... 0xFE9F:

                if(DMGL_ASSERT(g_test_memory.memory.sprite[address - 0xFE00] == data)) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            case 0xFF80 ... 0xFFFE:

                if(DMGL_ASSERT(g_test_memory.memory.high[address - 0xFF80] == data)) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            case 0xFF50:

                if(DMGL_ASSERT((g_test_memory.bootloader.enabled == false)
                        && (g_test_memory.bootloader.bootloader == &g_test_memory.memory.bootloader))) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            default:

                if(DMGL_ASSERT((g_test_memory.mapper.value == data)
                        && (g_test_memory.mapper.mapper == &g_test_memory.memory.mapper)
                        && (g_test_memory.mapper.address == address))) {
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
        dmgl_test_memory_checksum, dmgl_test_memory_has_bootloader, dmgl_test_memory_initialize, dmgl_test_memory_read,
        dmgl_test_memory_reset, dmgl_test_memory_title, dmgl_test_memory_uninitialize, dmgl_test_memory_write,
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
