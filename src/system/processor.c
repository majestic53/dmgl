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

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef bool (*dmgl_processor_instruction_cb)(dmgl_processor_t *processor);

static uint8_t dmgl_processor_fetch(dmgl_processor_t *processor)
{
    return dmgl_bus_read(processor->pc.word++);
}

/*static uint8_t dmgl_processor_pop(dmgl_processor_t *processor)
{
    return dmgl_bus_read(processor->sp.word++);
}*/

static void dmgl_processor_push(dmgl_processor_t *processor, uint8_t value)
{
    dmgl_bus_write(--processor->sp.word, value);
}

static bool dmgl_processor_instruction_nop(dmgl_processor_t *processor)
{
    bool result = true;

    switch(processor->instruction.cycle) {
        case 0:
            result = false;
            break;
        default:
            break;
    }

    return result;
}

static bool dmgl_processor_instruction_rlc(dmgl_processor_t *processor)
{
    bool result = true;

    switch(processor->instruction.cycle - 1) {

        /* TODO */

        default:
            break;
    }

    return result;
}

static dmgl_error_e dmgl_processor_instruction(dmgl_processor_t *processor)
{
    dmgl_error_e result = DMGL_SUCCESS;
    const dmgl_processor_instruction_cb instruction[] = {
            dmgl_processor_instruction_nop,

            /* TODO */

        }, instruction_extended[] = {
            dmgl_processor_instruction_rlc,

            /* TODO */
        };

    switch(processor->instruction.cycle) {
        case 0:
            processor->instruction.address.word = processor->pc.word;
            processor->instruction.opcode = dmgl_processor_fetch(processor);

            if((processor->instruction.extended = (processor->instruction.opcode == 0xCB))) {
                ++processor->instruction.cycle;
            } else if(!instruction[processor->instruction.opcode](processor)) {
                processor->instruction.cycle = 0;
            } else {
                ++processor->instruction.cycle;
            }
            break;
        case 1:

            if(processor->instruction.extended) {
                processor->instruction.opcode = dmgl_processor_fetch(processor);

                if(!instruction_extended[processor->instruction.opcode](processor)) {
                    processor->instruction.cycle = 0;
                } else {
                    ++processor->instruction.cycle;
                }
            } else if(!instruction[processor->instruction.opcode](processor)) {
                processor->instruction.cycle = 0;
            } else {
                ++processor->instruction.cycle;
            }
            break;
        default:

            if(processor->instruction.extended) {

                if(!instruction_extended[processor->instruction.opcode](processor)) {
                    processor->instruction.cycle = 0;
                } else {
                    ++processor->instruction.cycle;
                }
            } else if(!instruction[processor->instruction.opcode](processor)) {
                processor->instruction.cycle = 0;
            } else {
                ++processor->instruction.cycle;
            }
            break;
    }

    return result;
}

static void dmgl_processor_interrupt(dmgl_processor_t *processor)
{

    switch(processor->interrupt.cycle) {
        case 0:
            processor->halted = false;
            ++processor->interrupt.cycle;
            break;
        case 1:

            for(dmgl_interrupt_e interrupt = 0; interrupt < DMGL_INTERRUPT_MAX; ++interrupt) {
                int mask = (1 << interrupt);

                if((processor->interrupt.flag.raw & mask) && (processor->interrupt.enable.raw & mask)) {

                    if(interrupt == DMGL_INTERRUPT_BUTTON) {
                        processor->stopped = false;
                    }

                    processor->interrupt.address.word = 0x0040 + (0x0008 * interrupt);
                    processor->interrupt.flag.raw &= ~mask;
                    processor->interrupt.enabled = false;
                    break;
                }
            }

            ++processor->interrupt.cycle;
            break;
        case 2:
            dmgl_processor_push(processor, processor->pc.high);
            ++processor->interrupt.cycle;
            break;
        case 3:
            dmgl_processor_push(processor, processor->pc.low);
            ++processor->interrupt.cycle;
            break;
        case 4:
            processor->pc.word = processor->interrupt.address.word;
            processor->interrupt.cycle = 0;
            break;
        default:
            break;
    }
}

dmgl_error_e dmgl_processor_clock(dmgl_processor_t *processor)
{
    dmgl_error_e result = DMGL_SUCCESS;

    if(processor->cycle == 3) {

        if(!processor->instruction.cycle) {

            if((processor->interrupt.enabled && (processor->interrupt.flag.raw & processor->interrupt.enable.raw & 0x1F))
                    || processor->interrupt.cycle) {
                dmgl_processor_interrupt(processor);
            } else if(!processor->halted && !processor->stopped) {

                if((result = dmgl_processor_instruction(processor)) != DMGL_SUCCESS) {
                    goto exit;
                }
            }
        } else if((result = dmgl_processor_instruction(processor)) != DMGL_SUCCESS) {
            goto exit;
        }

        processor->cycle = 0;
    } else {
        ++processor->cycle;
    }

exit:
    return result;
}

dmgl_error_e dmgl_processor_initialize(dmgl_processor_t *processor, bool has_bootloader, uint8_t checksum)
{

    if(!has_bootloader) {
        processor->af.high = 0x01;
        processor->af.carry = checksum ? 1 : 0;
        processor->af.half_carry = checksum ? 1 : 0;
        processor->af.zero = 1;
        processor->bc.word = 0x0013;
        processor->de.word = 0x00D8;
        processor->hl.word = 0x014D;
        processor->pc.word = 0x0100;
        processor->sp.word = 0xFFFE;
        processor->interrupt.flag.raw = 0xE1;
    }

    processor->cycle = 3;

    return DMGL_SUCCESS;
}

uint8_t dmgl_processor_read(const dmgl_processor_t *processor, uint16_t address)
{
    uint8_t result = 0;

    switch(address) {
        case 0xFF0F:
            result = processor->interrupt.flag.raw;
            break;
        case 0xFFFF:
            result = processor->interrupt.enable.raw;
            break;
        default:
            break;
    }

    return result;
}

void dmgl_processor_uninitialize(dmgl_processor_t *processor)
{
    memset(processor, 0, sizeof(*processor));
}

void dmgl_processor_write(dmgl_processor_t *processor, uint16_t address, uint8_t value)
{

    switch(address) {
        case 0xFF0F:
            processor->interrupt.flag.raw = value | 0xE0;
            break;
        case 0xFFFF:
            processor->interrupt.enable.raw = value;
            break;
        default:
            break;
    }
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
