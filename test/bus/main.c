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

#include <bus.h>
#include <memory.h>
#include <processor.h>
#include <test.h>

typedef struct {

    struct {
        const dmgl_memory_t *memory;
        const dmgl_t *context;
        const char *title;
        dmgl_error_e status;
        uint16_t address;
        uint8_t value;
        bool has_bootloader;
        uint8_t checksum;
        bool initialized;
        bool reset;
    } memory;

    struct {
        const dmgl_processor_t *processor;
        dmgl_error_e status;
        uint16_t address;
        uint8_t value;
        bool has_bootloader;
        uint8_t checksum;
        bool initialized;
        bool reset;
        bool clock;
    } processor;
} dmgl_test_bus_t;

static dmgl_test_bus_t g_test_bus = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t dmgl_memory_checksum(const dmgl_memory_t *memory)
{
    g_test_bus.memory.memory = memory;

    return g_test_bus.memory.checksum;
}

bool dmgl_memory_has_bootloader(const dmgl_memory_t *memory)
{
    g_test_bus.memory.memory = memory;

    return g_test_bus.memory.has_bootloader;
}

dmgl_error_e dmgl_memory_initialize(dmgl_memory_t *memory, const dmgl_t *context)
{
    g_test_bus.memory.memory = memory;
    g_test_bus.memory.context = context;
    g_test_bus.memory.initialized = true;

    return g_test_bus.memory.status;
}

uint8_t dmgl_memory_read(const dmgl_memory_t *memory, uint16_t address)
{
    g_test_bus.memory.memory = memory;
    g_test_bus.memory.address = address;

    return g_test_bus.memory.value;
}

void dmgl_memory_reset(dmgl_memory_t *memory)
{
    g_test_bus.memory.reset = true;
}

const char *dmgl_memory_title(const dmgl_memory_t *memory)
{
    g_test_bus.memory.memory = memory;

    return g_test_bus.memory.title;
}

void dmgl_memory_uninitialize(dmgl_memory_t *memory)
{
    g_test_bus.memory.memory = memory;
    g_test_bus.memory.initialized = false;
}

void dmgl_memory_write(dmgl_memory_t *memory, uint16_t address, uint8_t value)
{
    g_test_bus.memory.memory = memory;
    g_test_bus.memory.address = address;
    g_test_bus.memory.value = value;
}

dmgl_error_e dmgl_processor_clock(dmgl_processor_t *processor)
{
    g_test_bus.processor.processor = processor;
    g_test_bus.processor.clock = true;

    return g_test_bus.processor.status;
}

void dmgl_processor_initialize(dmgl_processor_t *processor, bool has_bootloader, uint8_t checksum)
{
    g_test_bus.processor.processor = processor;
    g_test_bus.processor.has_bootloader = has_bootloader;
    g_test_bus.processor.checksum = checksum;
    g_test_bus.processor.initialized = true;
}

uint8_t dmgl_processor_read(const dmgl_processor_t *processor, uint16_t address)
{
    g_test_bus.processor.processor = processor;
    g_test_bus.processor.address = address;

    return g_test_bus.processor.value;
}

void dmgl_processor_reset(dmgl_processor_t *processor)
{
    g_test_bus.processor.reset = true;
}

void dmgl_processor_uninitialize(dmgl_processor_t *processor)
{
    g_test_bus.processor.processor = processor;
    g_test_bus.processor.initialized = false;
}

void dmgl_processor_write(dmgl_processor_t *processor, uint16_t address, uint8_t value)
{
    g_test_bus.processor.processor = processor;
    g_test_bus.processor.address = address;
    g_test_bus.processor.value = value;
}

static inline void dmgl_test_initialize(void)
{
    memset(&g_test_bus, 0, sizeof(g_test_bus));
}

static dmgl_error_e dmgl_test_bus_clock(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_bus.processor.status = DMGL_FAILURE;

    if(DMGL_ASSERT(dmgl_bus_clock() == DMGL_FAILURE)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();

    if(DMGL_ASSERT((dmgl_bus_clock() == DMGL_SUCCESS)
            && (g_test_bus.processor.processor != NULL)
            && (g_test_bus.processor.clock == true))) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();

    for(uint32_t cycle = 0; cycle < (4194304 / 60) - 1; ++cycle) {

        if(DMGL_ASSERT(dmgl_bus_clock() == DMGL_SUCCESS)) {
            result = DMGL_FAILURE;
            goto exit;
        }
    }

    if(DMGL_ASSERT(dmgl_bus_clock() == DMGL_COMPLETE)) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_bus_initialize(void)
{
    dmgl_t context = {};
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_bus.memory.status = DMGL_FAILURE;

    if(DMGL_ASSERT(dmgl_bus_initialize(&context) == DMGL_FAILURE)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_bus.memory.has_bootloader = true;
    g_test_bus.memory.checksum = 0xEF;
    context.bootloader.data = (uint8_t *)1;

    if(DMGL_ASSERT((dmgl_bus_initialize(&context) == DMGL_SUCCESS)
            && (g_test_bus.memory.memory != NULL)
            && (g_test_bus.memory.context == &context)
            && (g_test_bus.memory.initialized == true)
            && (g_test_bus.processor.processor != NULL)
            && (g_test_bus.processor.has_bootloader == g_test_bus.memory.has_bootloader)
            && (g_test_bus.processor.checksum == g_test_bus.memory.checksum)
            && (g_test_bus.processor.initialized == true))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_bus_interrupt(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    for(dmgl_interrupt_e set = 0; set < DMGL_INTERRUPT_MAX; ++set) {

        for(dmgl_interrupt_e get = 0; get < DMGL_INTERRUPT_MAX; ++get) {
            dmgl_test_initialize();
            g_test_bus.processor.value = 1 << get;
            dmgl_bus_interrupt(set);

            if(DMGL_ASSERT((g_test_bus.processor.processor != NULL)
                    && (g_test_bus.processor.address == 0xFF0F)
                    && (g_test_bus.processor.value == ((1 << set) | (1 << get))))) {
                result = DMGL_FAILURE;
                goto exit;
            }
        }
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_bus_read(void)
{
    uint8_t data = 0x00;
    dmgl_error_e result = DMGL_SUCCESS;

    for(uint32_t address = 0x0000; address <= 0xFFFF; ++address, ++data) {
        dmgl_test_initialize();

        switch(address) {
            case 0xFF0F:
            case 0xFFFF:
                g_test_bus.processor.value = data;

                if(DMGL_ASSERT((dmgl_bus_read(address) == data)
                        && (g_test_bus.processor.processor != NULL)
                        && (g_test_bus.processor.address == address))) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            default:
                g_test_bus.memory.value = data;

                if(DMGL_ASSERT((dmgl_bus_read(address) == data)
                        && (g_test_bus.memory.memory != NULL)
                        && (g_test_bus.memory.address == address))) {
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

static dmgl_error_e dmgl_test_bus_reset(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    dmgl_bus_reset();

    if(DMGL_ASSERT((g_test_bus.memory.reset == true)
            && (g_test_bus.processor.reset == true))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_bus_title(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_bus.memory.title = "Test";

    if(DMGL_ASSERT(!strcmp(dmgl_bus_title(), "Test"))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_bus_uninitialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_bus.memory.initialized = true;
    g_test_bus.processor.initialized = true;
    dmgl_bus_uninitialize();

    if(DMGL_ASSERT((g_test_bus.memory.memory != NULL)
            && (g_test_bus.memory.initialized == false)
            && (g_test_bus.processor.processor != NULL)
            && (g_test_bus.processor.initialized == false))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_bus_write(void)
{
    uint8_t data = 0x00;
    dmgl_error_e result = DMGL_SUCCESS;

    for(uint32_t address = 0x0000; address <= 0xFFFF; ++address, ++data) {
        dmgl_test_initialize();
        dmgl_bus_write(address, data);

        switch(address) {
            case 0xFF0F:
            case 0xFFFF:

                if(DMGL_ASSERT((g_test_bus.processor.processor != NULL)
                        && (g_test_bus.processor.address == address)
                        && (g_test_bus.processor.value == data))) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            default:

                if(DMGL_ASSERT((g_test_bus.memory.memory != NULL)
                        && (g_test_bus.memory.address == address)
                        && (g_test_bus.memory.value == data))) {
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
        dmgl_test_bus_clock, dmgl_test_bus_initialize, dmgl_test_bus_interrupt, dmgl_test_bus_read,
        dmgl_test_bus_reset, dmgl_test_bus_title, dmgl_test_bus_uninitialize, dmgl_test_bus_write,
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
