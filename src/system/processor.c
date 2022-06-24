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
 * @file processor.c
 * @brief Processor subsystem.
 */

#include <bus.h>
#include <processor.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Processor instruction callback.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
typedef bool (*dmgl_processor_instruction_cb)(dmgl_processor_t *processor);

/*!
 * @brief Fetch processor byte at PC and increment PC.
 * @param[in,out] processor Pointer to processor context
 * @return Byte value
 */
static uint8_t dmgl_processor_fetch(dmgl_processor_t *processor)
{
    return dmgl_bus_read(processor->bank.pc.word++);
}

/*!
 * @brief Pop processor byte from SP and increment SP.
 * @param[in,out] processor Pointer to processor context
 * @return Byte value
 */
static uint8_t dmgl_processor_pop(dmgl_processor_t *processor)
{
    return dmgl_bus_read(processor->bank.sp.word++);
}

/*!
 * @brief Decrement processor SP and Push byte to SP.
 * @param[in,out] processor Pointer to processor context
 * @param[in] value Byte value
 */
static void dmgl_processor_push(dmgl_processor_t *processor, uint8_t value)
{
    dmgl_bus_write(--processor->bank.sp.word, value);
}

/*!
 * @brief Execute processor CCF instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_ccf(dmgl_processor_t *processor)
{
    bool result = true;

    switch(processor->instruction.cycle) {
        case 0:
            processor->bank.af.carry = !processor->bank.af.carry;
            processor->bank.af.half_carry = false;
            processor->bank.af.subtract = false;
            result = false;
            break;
        default:
            break;
    }

    return result;
}

/*!
 * @brief Execute processor CPL instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_cpl(dmgl_processor_t *processor)
{
    bool result = true;

    switch(processor->instruction.cycle) {
        case 0:
            processor->bank.af.high = ~processor->bank.af.high;
            processor->bank.af.half_carry = true;
            processor->bank.af.subtract = true;
            result = false;
            break;
        default:
            break;
    }

    return result;
}

/*!
 * @brief Execute processor DI instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_di(dmgl_processor_t *processor)
{
    bool result = true;

    switch(processor->instruction.cycle) {
        case 0:
            processor->interrupt.enabled = false;
            processor->interrupt.enabling = 0;
            result = false;
            break;
        default:
            break;
    }

    return result;
}

/*!
 * @brief Execute processor EI instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_ei(dmgl_processor_t *processor)
{
    bool result = true;

    switch(processor->instruction.cycle) {
        case 0:
            processor->interrupt.enabling = 2;
            result = false;
            break;
        default:
            break;
    }

    return result;
}

/*!
 * @brief Execute processor HALT instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_halt(dmgl_processor_t *processor)
{
    bool result = true;

    switch(processor->instruction.cycle) {
        case 0:
            processor->halt.bug = (!processor->interrupt.enabled && (processor->interrupt.flag.raw & processor->interrupt.enable.raw & 0x1F));
            processor->halt.enabled = true;
            result = false;
            break;
        default:
            break;
    }

    return result;
}

/*!
 * @brief Execute processor NOP instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
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

/*!
 * @brief Execute processor POP instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_pop(dmgl_processor_t *processor)
{
    bool result = true;
    dmgl_processor_register_t *bank = NULL;

    switch(processor->instruction.opcode) {
        case 0xC1:
            bank = &processor->bank.bc;
            break;
        case 0xD1:
            bank = &processor->bank.de;
            break;
        case 0xE1:
            bank = &processor->bank.hl;
            break;
        case 0xF1:
            bank = &processor->bank.af;
            break;
    }

    switch(processor->instruction.cycle) {
        case 0:
            bank->low = dmgl_processor_pop(processor);
            break;
        case 1:
            bank->high = dmgl_processor_pop(processor);
            result = false;
            break;
        default:
            break;
    }

    return result;
}

/*!
 * @brief Execute processor PUSH instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_push(dmgl_processor_t *processor)
{
    bool result = true;
    dmgl_processor_register_t *bank = NULL;

    switch(processor->instruction.opcode) {
        case 0xC5:
            bank = &processor->bank.bc;
            break;
        case 0xD5:
            bank = &processor->bank.de;
            break;
        case 0xE5:
            bank = &processor->bank.hl;
            break;
        case 0xF5:
            bank = &processor->bank.af;
            break;
    }

    switch(processor->instruction.cycle) {
        case 0:
            break;
        case 1:
            dmgl_processor_push(processor, bank->high);
            break;
        case 2:
            dmgl_processor_push(processor, bank->low);
            result = false;
            break;
        default:
            break;
    }

    return result;
}

/*!
 * @brief Execute processor RLC instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_rlc(dmgl_processor_t *processor)
{
    bool result = true;

    switch(processor->instruction.cycle) {

        /* TODO */

        default:
            break;
    }

    return result;
}

/*!
 * @brief Execute processor SCF instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_scf(dmgl_processor_t *processor)
{
    bool result = true;

    switch(processor->instruction.cycle) {
        case 0:
            processor->bank.af.carry = true;
            processor->bank.af.half_carry = false;
            processor->bank.af.subtract = false;
            result = false;
            break;
        default:
            break;
    }

    return result;
}

/*!
 * @brief Execute processor STOP instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_stop(dmgl_processor_t *processor)
{
    bool result = true;

    switch(processor->instruction.cycle) {
        case 0:
            processor->stop.enabled = true;
            break;
        case 1:
            processor->stop.code = dmgl_processor_fetch(processor);
            result = false;
            break;
        default:
            break;
    }

    return result;
}

/*!
 * @brief Execute processor instruction.
 * @param[in,out] processor Pointer to processor context
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
static dmgl_error_e dmgl_processor_instruction(dmgl_processor_t *processor)
{
    dmgl_error_e result = DMGL_SUCCESS;
    const dmgl_processor_instruction_cb instruction[] = {
            /* 00 */
            dmgl_processor_instruction_nop, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 08 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 10 */
            dmgl_processor_instruction_stop, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 18 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 20 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 28 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, dmgl_processor_instruction_cpl,
            /* 30 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, dmgl_processor_instruction_scf,
            /* 38 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, dmgl_processor_instruction_ccf,
            /* 40 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 48 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 50 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 58 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 60 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 68 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 70 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, dmgl_processor_instruction_halt, NULL,
            /* 78 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 80 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 88 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 90 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 98 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* A0 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* A8 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* B0 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* B8 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* C0 */
            NULL, dmgl_processor_instruction_pop, NULL, NULL,
            NULL, dmgl_processor_instruction_push, NULL, NULL,
            /* C8 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* D0 */
            NULL, dmgl_processor_instruction_pop, NULL, NULL,
            NULL, dmgl_processor_instruction_push, NULL, NULL,
            /* D8 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* E0 */
            NULL, dmgl_processor_instruction_pop, NULL, NULL,
            NULL, dmgl_processor_instruction_push, NULL, NULL,
            /* E8 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* F0 */
            NULL, dmgl_processor_instruction_pop, NULL, dmgl_processor_instruction_di,
            NULL, dmgl_processor_instruction_push, NULL, NULL,
            /* F8 */
            NULL, NULL, NULL, dmgl_processor_instruction_ei,
            NULL, NULL, NULL, NULL,
        }, instruction_extended[] = {
            /* 00 */
            dmgl_processor_instruction_rlc, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 08 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 10 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 18 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 20 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 28 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 30 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 38 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 40 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 48 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 50 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 58 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 60 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 68 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 70 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 78 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 80 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 88 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 90 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* 98 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* A0 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* A8 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* B0 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* B8 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* C0 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* C8 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* D0 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* D8 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* E0 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* E8 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* F0 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
            /* F8 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,
        };

    switch(processor->instruction.cycle) {
        case 0:

            if(processor->instruction.extended) {
                processor->instruction.opcode = dmgl_processor_fetch(processor);
                ++processor->instruction.cycle;
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

    if(!processor->instruction.cycle) {
        processor->instruction.address.word = processor->bank.pc.word;
        processor->instruction.opcode = dmgl_processor_fetch(processor);

        if(processor->halt.bug) {
            processor->halt.bug = false;
            --processor->bank.pc.word;
        }

        if(processor->interrupt.enabling) {

            if(!--processor->interrupt.enabling) {
                processor->interrupt.enabled = false;
            }
        }
    }

    return result;
}

/*!
 * @brief Service processor interrupt.
 * @param[in,out] processor Pointer to processor context
 */
static void dmgl_processor_interrupt(dmgl_processor_t *processor)
{

    switch(processor->interrupt.cycle) {
        case 0:
            ++processor->interrupt.cycle;
            break;
        case 1:

            for(dmgl_interrupt_e interrupt = 0; interrupt < DMGL_INTERRUPT_MAX; ++interrupt) {
                int mask = (1 << interrupt);

                if((processor->interrupt.flag.raw & mask) && (processor->interrupt.enable.raw & mask)) {

                    if(interrupt == DMGL_INTERRUPT_BUTTON) {
                        processor->stop.enabled = false;
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
            dmgl_processor_push(processor, processor->bank.pc.high);
            ++processor->interrupt.cycle;
            break;
        case 3:
            dmgl_processor_push(processor, processor->bank.pc.low);
            ++processor->interrupt.cycle;
            break;
        case 4:
            processor->bank.pc.word = processor->interrupt.address.word;
            processor->instruction.address.word = processor->bank.pc.word;
            processor->instruction.opcode = dmgl_processor_fetch(processor);
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

        if(processor->halt.enabled && (processor->interrupt.flag.raw & processor->interrupt.enable.raw & 0x1F)) {
            processor->halt.enabled = false;
        }

        if(!processor->instruction.cycle) {

            if((processor->interrupt.enabled && (processor->interrupt.flag.raw & processor->interrupt.enable.raw & 0x1F))
                    || processor->interrupt.cycle) {
                dmgl_processor_interrupt(processor);
            } else if(!processor->halt.enabled && !processor->stop.enabled) {

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

void dmgl_processor_initialize(dmgl_processor_t *processor, bool has_bootloader, uint8_t checksum)
{
    processor->checksum = checksum;
    processor->has_bootloader = has_bootloader;
    dmgl_processor_reset(processor);
}

uint8_t dmgl_processor_read(const dmgl_processor_t *processor, uint16_t address)
{
    uint8_t result = 0xFF;

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

void dmgl_processor_reset(dmgl_processor_t *processor)
{

    if(!processor->has_bootloader) {
        processor->bank.af.high = 0x01;
        processor->bank.af.carry = processor->checksum ? 1 : 0;
        processor->bank.af.half_carry = processor->checksum ? 1 : 0;
        processor->bank.af.zero = 1;
        processor->bank.bc.word = 0x0013;
        processor->bank.de.word = 0x00D8;
        processor->bank.hl.word = 0x014D;
        processor->bank.pc.word = 0x0100;
        processor->bank.sp.word = 0xFFFE;
        processor->interrupt.flag.raw = 0xE1;
    }

    processor->instruction.address.word = processor->bank.pc.word;
    processor->instruction.opcode = dmgl_processor_fetch(processor);
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
