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
#include <processor.h>
#include <test.h>

typedef struct {
    dmgl_processor_t processor;
    dmgl_processor_t expected;

    struct {
        uint16_t address;
        uint8_t value[0x10000];
    } bus;
} dmgl_test_processor_t;

static dmgl_test_processor_t g_test_processor = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t dmgl_bus_read(uint16_t address)
{
    g_test_processor.bus.address = address;

    return g_test_processor.bus.value[address];
}

void dmgl_bus_write(uint16_t address, uint8_t value)
{
    g_test_processor.bus.address = address;
    g_test_processor.bus.value[address] = value;
}

static inline void dmgl_test_initialize(void)
{
    memset(&g_test_processor, 0, sizeof(g_test_processor));
}

static inline dmgl_error_e dmgl_test_match(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    if(DMGL_ASSERT(g_test_processor.processor.cycle == g_test_processor.expected.cycle)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.checksum == g_test_processor.expected.checksum)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.has_bootloader == g_test_processor.expected.has_bootloader)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.bank.af.high == g_test_processor.expected.bank.af.high)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.bank.af.carry == g_test_processor.expected.bank.af.carry)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.bank.af.half_carry == g_test_processor.expected.bank.af.half_carry)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.bank.af.subtract == g_test_processor.expected.bank.af.subtract)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.bank.af.zero == g_test_processor.expected.bank.af.zero)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.bank.af.low == g_test_processor.expected.bank.af.low)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.bank.bc.high == g_test_processor.expected.bank.bc.high)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.bank.bc.low == g_test_processor.expected.bank.bc.low)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.bank.de.high == g_test_processor.expected.bank.de.high)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.bank.de.low == g_test_processor.expected.bank.de.low)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.bank.hl.high == g_test_processor.expected.bank.hl.high)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.bank.hl.low == g_test_processor.expected.bank.hl.low)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.bank.pc.word == g_test_processor.expected.bank.pc.word)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.bank.sp.word == g_test_processor.expected.bank.sp.word)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.halt.bug == g_test_processor.expected.halt.bug)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.halt.enabled == g_test_processor.expected.halt.enabled)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.stop.code == g_test_processor.expected.stop.code)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.stop.enabled == g_test_processor.expected.stop.enabled)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.instruction.address.word == g_test_processor.expected.instruction.address.word)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.instruction.operand.high == g_test_processor.expected.instruction.operand.high)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.instruction.operand.low == g_test_processor.expected.instruction.operand.low)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.instruction.opcode == g_test_processor.expected.instruction.opcode)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.instruction.cycle == g_test_processor.expected.instruction.cycle)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.instruction.extended == g_test_processor.expected.instruction.extended)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.interrupt.address.word == g_test_processor.expected.interrupt.address.word)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.interrupt.enable.vblank == g_test_processor.expected.interrupt.enable.vblank)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.interrupt.enable.screen == g_test_processor.expected.interrupt.enable.screen)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.interrupt.enable.timer == g_test_processor.expected.interrupt.enable.timer)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.interrupt.enable.serial == g_test_processor.expected.interrupt.enable.serial)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.interrupt.enable.button == g_test_processor.expected.interrupt.enable.button)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.interrupt.enable.raw == g_test_processor.expected.interrupt.enable.raw)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.interrupt.flag.vblank == g_test_processor.expected.interrupt.flag.vblank)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.interrupt.flag.screen == g_test_processor.expected.interrupt.flag.screen)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.interrupt.flag.timer == g_test_processor.expected.interrupt.flag.timer)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.interrupt.flag.serial == g_test_processor.expected.interrupt.flag.serial)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.interrupt.flag.button == g_test_processor.expected.interrupt.flag.button)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.interrupt.flag.raw == g_test_processor.expected.interrupt.flag.raw)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.interrupt.cycle == g_test_processor.expected.interrupt.cycle)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.interrupt.enabling == g_test_processor.expected.interrupt.enabling)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    if(DMGL_ASSERT(g_test_processor.processor.interrupt.enabled == g_test_processor.expected.interrupt.enabled)) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    return result;
}

static dmgl_error_e dmgl_test_processor_clock(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_processor.processor.cycle = 3;

    if(DMGL_ASSERT((dmgl_processor_clock(&g_test_processor.processor) == DMGL_SUCCESS)
            && (g_test_processor.processor.cycle == 0))) {
        result = DMGL_FAILURE;
        goto exit;
    }

    for(uint32_t cycle = 0; cycle <= 2; ++cycle) {

        if(DMGL_ASSERT((dmgl_processor_clock(&g_test_processor.processor) == DMGL_SUCCESS)
                && (g_test_processor.processor.cycle == cycle + 1))) {
            result = DMGL_FAILURE;
            goto exit;
        }
    }

    if(DMGL_ASSERT((dmgl_processor_clock(&g_test_processor.processor) == DMGL_SUCCESS)
            && (g_test_processor.processor.cycle == 0))) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_initialize(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_processor.bus.value[0x0100] = 0xEF;
    g_test_processor.expected.bank.af.word = 0x0180;
    g_test_processor.expected.bank.bc.word = 0x0013;
    g_test_processor.expected.bank.de.word = 0x00D8;
    g_test_processor.expected.bank.hl.word = 0x014D;
    g_test_processor.expected.bank.pc.word = 0x0101;
    g_test_processor.expected.bank.sp.word = 0xFFFE;
    g_test_processor.expected.instruction.address.word = 0x0100;
    g_test_processor.expected.instruction.opcode = 0xEF;
    g_test_processor.expected.interrupt.flag.raw = 0xE1;
    dmgl_processor_initialize(&g_test_processor.processor, false, 0x00);

    if(DMGL_ASSERT(dmgl_test_match() == DMGL_SUCCESS)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_processor.bus.value[0x0100] = 0xEF;
    g_test_processor.expected.checksum = 0x01;
    g_test_processor.expected.bank.af.word = 0x01B0;
    g_test_processor.expected.bank.bc.word = 0x0013;
    g_test_processor.expected.bank.de.word = 0x00D8;
    g_test_processor.expected.bank.hl.word = 0x014D;
    g_test_processor.expected.bank.pc.word = 0x0101;
    g_test_processor.expected.bank.sp.word = 0xFFFE;
    g_test_processor.expected.instruction.address.word = 0x0100;
    g_test_processor.expected.instruction.opcode = 0xEF;
    g_test_processor.expected.interrupt.flag.raw = 0xE1;
    dmgl_processor_initialize(&g_test_processor.processor, false, 0x01);

    if(DMGL_ASSERT(dmgl_test_match() == DMGL_SUCCESS)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_processor.bus.value[0x0000] = 0xEF;
    g_test_processor.expected.has_bootloader = true;
    g_test_processor.expected.bank.pc.word = 0x0001;
    g_test_processor.expected.instruction.opcode = 0xEF;
    dmgl_processor_initialize(&g_test_processor.processor, true, 0x00);

    if(DMGL_ASSERT(dmgl_test_match() == DMGL_SUCCESS)) {
        result = DMGL_FAILURE;
        goto exit;
    }

exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_instruction_ccf(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_instruction_cpl(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_instruction_di(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_instruction_ei(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_instruction_halt(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_instruction_nop(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_instruction_pop(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_instruction_push(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_instruction_scf(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_instruction_stop(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();

    /* TODO */

//exit:
    DMGL_TEST_RESULT(result);

    return result;
}

static dmgl_error_e dmgl_test_processor_interrupt(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    for(dmgl_interrupt_e interrupt = 0; interrupt < DMGL_INTERRUPT_MAX; ++interrupt) {
        dmgl_test_initialize();
        g_test_processor.bus.value[0x0040 + (0x0008 * interrupt)] = interrupt + 1;
        g_test_processor.processor.halt.enabled = true;
        g_test_processor.processor.stop.enabled = true;
        g_test_processor.processor.bank.pc.word = 0xABCD;
        g_test_processor.processor.bank.sp.word = 0xFFFE;
        g_test_processor.processor.interrupt.enable.raw = (1 << interrupt);
        g_test_processor.processor.interrupt.flag.raw = (1 << interrupt);
        g_test_processor.processor.interrupt.enabled = true;
        g_test_processor.expected.bank.pc.word = 0xABCD;
        g_test_processor.expected.interrupt.enable.raw = (1 << interrupt);
        g_test_processor.expected.interrupt.flag.raw = (1 << interrupt);
        g_test_processor.expected.interrupt.enabled = true;

        for(uint32_t cycle = 0; cycle < 5; ++cycle) {
            g_test_processor.processor.cycle = 3;

            if(DMGL_ASSERT(dmgl_processor_clock(&g_test_processor.processor) == DMGL_SUCCESS)) {
                result = DMGL_FAILURE;
                goto exit;
            }

            switch(cycle) {
                case 0:
                    g_test_processor.expected.bank.sp.word = 0xFFFE;
                    g_test_processor.expected.stop.enabled = true;
                    g_test_processor.expected.interrupt.enable.raw = 1 << interrupt;
                    g_test_processor.expected.interrupt.flag.raw = 1 << interrupt;
                    g_test_processor.expected.interrupt.cycle = cycle + 1;
                    g_test_processor.expected.interrupt.enabled = true;
                    break;
                case 1:
                    g_test_processor.expected.bank.sp.word = 0xFFFE;
                    g_test_processor.expected.stop.enabled = interrupt != DMGL_INTERRUPT_BUTTON;
                    g_test_processor.expected.interrupt.address.word = 0x0040 + (0x0008 * interrupt);
                    g_test_processor.expected.interrupt.flag.raw = 0;
                    g_test_processor.expected.interrupt.cycle = cycle + 1;
                    g_test_processor.expected.interrupt.enabled = false;
                    break;
                case 2:
                    g_test_processor.expected.bank.sp.word = 0xFFFD;
                    g_test_processor.expected.interrupt.cycle = cycle + 1;

                    if(DMGL_ASSERT((g_test_processor.bus.address == 0xFFFD)
                            && (g_test_processor.bus.value[g_test_processor.bus.address] == 0xAB))) {
                        result = DMGL_FAILURE;
                        goto exit;
                    }
                    break;
                case 3:
                    g_test_processor.expected.bank.sp.word = 0xFFFC;
                    g_test_processor.expected.interrupt.cycle = cycle + 1;

                    if(DMGL_ASSERT((g_test_processor.bus.address == 0xFFFC)
                            && (g_test_processor.bus.value[g_test_processor.bus.address] == 0xCD))) {
                        result = DMGL_FAILURE;
                        goto exit;
                    }
                    break;
                case 4:
                    g_test_processor.expected.bank.pc.word = 0x0040 + (0x0008 * interrupt) + 1;
                    g_test_processor.expected.instruction.address.word = 0x0040 + (0x0008 * interrupt);
                    g_test_processor.expected.instruction.opcode = interrupt + 1;
                    g_test_processor.expected.interrupt.cycle = 0;
                    break;
                default:
                    break;
            }

            if(DMGL_ASSERT(dmgl_test_match() == DMGL_SUCCESS)) {
                result = DMGL_FAILURE;
                goto exit;
            }
        }
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
                g_test_processor.processor.interrupt.flag.raw = 0x1F;

                if(DMGL_ASSERT(dmgl_processor_read(&g_test_processor.processor, address) == 0x1F)) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            case 0xFFFF:
                g_test_processor.processor.interrupt.enable.raw = 0x1F;

                if(DMGL_ASSERT(dmgl_processor_read(&g_test_processor.processor, address) == 0x1F)) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            default:

                if(DMGL_ASSERT(dmgl_processor_read(&g_test_processor.processor, address) == 0xFF)) {
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

static dmgl_error_e dmgl_test_processor_reset(void)
{
    dmgl_error_e result = DMGL_SUCCESS;

    dmgl_test_initialize();
    g_test_processor.bus.value[0x0100] = 0xEF;
    g_test_processor.expected.bank.af.word = 0x0180;
    g_test_processor.expected.bank.bc.word = 0x0013;
    g_test_processor.expected.bank.de.word = 0x00D8;
    g_test_processor.expected.bank.hl.word = 0x014D;
    g_test_processor.expected.bank.pc.word = 0x0101;
    g_test_processor.expected.bank.sp.word = 0xFFFE;
    g_test_processor.expected.instruction.address.word = 0x0100;
    g_test_processor.expected.instruction.opcode = 0xEF;
    g_test_processor.expected.interrupt.flag.raw = 0xE1;
    dmgl_processor_reset(&g_test_processor.processor);

    if(DMGL_ASSERT(dmgl_test_match() == DMGL_SUCCESS)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_processor.bus.value[0x0100] = 0xEF;
    g_test_processor.processor.checksum = 0x01;
    g_test_processor.expected.checksum = 0x01;
    g_test_processor.expected.bank.af.word = 0x01B0;
    g_test_processor.expected.bank.bc.word = 0x0013;
    g_test_processor.expected.bank.de.word = 0x00D8;
    g_test_processor.expected.bank.hl.word = 0x014D;
    g_test_processor.expected.bank.pc.word = 0x0101;
    g_test_processor.expected.bank.sp.word = 0xFFFE;
    g_test_processor.expected.instruction.address.word = 0x0100;
    g_test_processor.expected.instruction.opcode = 0xEF;
    g_test_processor.expected.interrupt.flag.raw = 0xE1;
    dmgl_processor_reset(&g_test_processor.processor);

    if(DMGL_ASSERT(dmgl_test_match() == DMGL_SUCCESS)) {
        result = DMGL_FAILURE;
        goto exit;
    }

    dmgl_test_initialize();
    g_test_processor.bus.value[0x0000] = 0xEF;
    g_test_processor.processor.has_bootloader = true;
    g_test_processor.expected.has_bootloader = true;
    g_test_processor.expected.bank.pc.word = 0x0001;
    g_test_processor.expected.instruction.opcode = 0xEF;
    dmgl_processor_reset(&g_test_processor.processor);

    if(DMGL_ASSERT(dmgl_test_match() == DMGL_SUCCESS)) {
        result = DMGL_FAILURE;
        goto exit;
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
        dmgl_processor_write(&g_test_processor.processor, address, 0x1F);

        switch(address) {
            case 0xFF0F:

                if(DMGL_ASSERT(g_test_processor.processor.interrupt.flag.raw == 0xFF)) {
                    result = DMGL_FAILURE;
                    goto exit;
                }
                break;
            case 0xFFFF:

                if(DMGL_ASSERT(g_test_processor.processor.interrupt.enable.raw == 0x1F)) {
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
        dmgl_test_processor_clock, dmgl_test_processor_initialize, dmgl_test_processor_instruction_ccf, dmgl_test_processor_instruction_cpl,
        dmgl_test_processor_instruction_di, dmgl_test_processor_instruction_ei, dmgl_test_processor_instruction_halt, dmgl_test_processor_instruction_nop,
        dmgl_test_processor_instruction_pop, dmgl_test_processor_instruction_push, dmgl_test_processor_instruction_scf, dmgl_test_processor_instruction_stop,
        dmgl_test_processor_interrupt, dmgl_test_processor_read, dmgl_test_processor_reset, dmgl_test_processor_uninitialize,
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
