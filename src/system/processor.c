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
 * @brief Execute processor AND instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_and(dmgl_processor_t *processor)
{
    bool result = false;

    switch(processor->instruction.cycle) {
        case 0:

            switch(processor->instruction.opcode) {
                case 0xA0: /* B */
                    processor->instruction.operand.low = processor->bank.bc.high;
                    break;
                case 0xA1: /* C */
                    processor->instruction.operand.low = processor->bank.bc.low;
                    break;
                case 0xA2: /* D */
                    processor->instruction.operand.low = processor->bank.de.high;
                    break;
                case 0xA3: /* E */
                    processor->instruction.operand.low = processor->bank.de.low;
                    break;
                case 0xA4: /* H */
                    processor->instruction.operand.low = processor->bank.hl.high;
                    break;
                case 0xA5: /* L */
                    processor->instruction.operand.low = processor->bank.hl.low;
                    break;
                case 0xA7: /* A */
                    processor->instruction.operand.low = processor->bank.af.high;
                    break;
                default:
                    result = true;
                    break;
            }
            break;
        case 1:

            switch(processor->instruction.opcode) {
                case 0xA6: /* (HL) */
                    processor->instruction.operand.low = dmgl_bus_read(processor->bank.hl.word);
                    break;
                case 0xE6: /* N */
                    processor->instruction.operand.low = dmgl_processor_fetch(processor);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    if(!result) {
        processor->bank.af.high &= processor->instruction.operand.low;
        processor->bank.af.carry = false;
        processor->bank.af.half_carry = true;
        processor->bank.af.subtract = false;
        processor->bank.af.zero = !processor->bank.af.high;
    }

    return result;
}

/*!
 * @brief Execute processor BIT instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_bit(dmgl_processor_t *processor)
{
    bool result = false;

    switch(processor->instruction.cycle) {
        case 0:

            switch(processor->instruction.opcode) {
                case 0x40: /* 0-7, B */
                case 0x48:
                case 0x50:
                case 0x58:
                case 0x60:
                case 0x68:
                case 0x70:
                case 0x78:
                    processor->instruction.operand.low = processor->bank.bc.high & (1 << ((processor->instruction.opcode - 0x40) / 0x08));
                    break;
                case 0x41: /* 0-7, C */
                case 0x49:
                case 0x51:
                case 0x59:
                case 0x61:
                case 0x69:
                case 0x71:
                case 0x79:
                    processor->instruction.operand.low = processor->bank.bc.low & (1 << ((processor->instruction.opcode - 0x41) / 0x08));
                    break;
                case 0x42: /* 0-7, D */
                case 0x4A:
                case 0x52:
                case 0x5A:
                case 0x62:
                case 0x6A:
                case 0x72:
                case 0x7A:
                    processor->instruction.operand.low = processor->bank.de.high & (1 << ((processor->instruction.opcode - 0x42) / 0x08));
                    break;
                case 0x43: /* 0-7, E */
                case 0x4B:
                case 0x53:
                case 0x5B:
                case 0x63:
                case 0x6B:
                case 0x73:
                case 0x7B:
                    processor->instruction.operand.low = processor->bank.de.low & (1 << ((processor->instruction.opcode - 0x43) / 0x08));
                    break;
                case 0x44: /* 0-7, H */
                case 0x4C:
                case 0x54:
                case 0x5C:
                case 0x64:
                case 0x6C:
                case 0x74:
                case 0x7C:
                    processor->instruction.operand.low = processor->bank.hl.high & (1 << ((processor->instruction.opcode - 0x44) / 0x08));
                    break;
                case 0x45: /* 0-7, L */
                case 0x4D:
                case 0x55:
                case 0x5D:
                case 0x65:
                case 0x6D:
                case 0x75:
                case 0x7D:
                    processor->instruction.operand.low = processor->bank.hl.low & (1 << ((processor->instruction.opcode - 0x45) / 0x08));
                    break;
                case 0x47: /* 0-7, A */
                case 0x4F:
                case 0x57:
                case 0x5F:
                case 0x67:
                case 0x6F:
                case 0x77:
                case 0x7F:
                    processor->instruction.operand.low = processor->bank.af.high & (1 << ((processor->instruction.opcode - 0x47) / 0x08));
                    break;
                default:
                    result = true;
                    break;
            }
            break;
        case 1:

            switch(processor->instruction.opcode) {
                case 0x46: /* 0-7, (HL) */
                case 0x4E:
                case 0x56:
                case 0x5E:
                case 0x66:
                case 0x6E:
                case 0x76:
                case 0x7E:
                    processor->instruction.operand.low = dmgl_bus_read(processor->bank.hl.word) & (1 << ((processor->instruction.opcode - 0x46) / 0x08));
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    if(!result) {
        processor->bank.af.half_carry = true;
        processor->bank.af.subtract = false;
        processor->bank.af.zero = !processor->instruction.operand.low;
    }

    return result;
}

/*!
 * @brief Execute processor CCF instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_ccf(dmgl_processor_t *processor)
{
    processor->bank.af.carry = !processor->bank.af.carry;
    processor->bank.af.half_carry = false;
    processor->bank.af.subtract = false;

    return false;
}

/*!
 * @brief Execute processor CPL instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_cpl(dmgl_processor_t *processor)
{
    processor->bank.af.high = ~processor->bank.af.high;
    processor->bank.af.half_carry = true;
    processor->bank.af.subtract = true;

    return false;
}

/*!
 * @brief Execute processor DI instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_di(dmgl_processor_t *processor)
{
    processor->interrupt.enabled = false;
    processor->interrupt.enabling = 0;

    return false;
}

/*!
 * @brief Execute processor EI instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_ei(dmgl_processor_t *processor)
{
    processor->interrupt.enabling = 2;

    return false;
}

/*!
 * @brief Execute processor HALT instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_halt(dmgl_processor_t *processor)
{
    processor->halt.bug = (!processor->interrupt.enabled && (processor->interrupt.flag.raw & processor->interrupt.enable.raw & 0x1F));
    processor->halt.enabled = true;

    return false;
}

/*!
 * @brief Execute processor NOP instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_nop(dmgl_processor_t *processor)
{
    return false;
}

/*!
 * @brief Execute processor OR instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_or(dmgl_processor_t *processor)
{
    bool result = false;

    switch(processor->instruction.cycle) {
        case 0:

            switch(processor->instruction.opcode) {
                case 0xB0: /* B */
                    processor->instruction.operand.low = processor->bank.bc.high;
                    break;
                case 0xB1: /* C */
                    processor->instruction.operand.low = processor->bank.bc.low;
                    break;
                case 0xB2: /* D */
                    processor->instruction.operand.low = processor->bank.de.high;
                    break;
                case 0xB3: /* E */
                    processor->instruction.operand.low = processor->bank.de.low;
                    break;
                case 0xB4: /* H */
                    processor->instruction.operand.low = processor->bank.hl.high;
                    break;
                case 0xB5: /* L */
                    processor->instruction.operand.low = processor->bank.hl.low;
                    break;
                case 0xB7: /* A */
                    processor->instruction.operand.low = processor->bank.af.high;
                    break;
                default:
                    result = true;
                    break;
            }
            break;
        case 1:

            switch(processor->instruction.opcode) {
                case 0xB6: /* (HL) */
                    processor->instruction.operand.low = dmgl_bus_read(processor->bank.hl.word);
                    break;
                case 0xF6: /* N */
                    processor->instruction.operand.low = dmgl_processor_fetch(processor);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    if(!result) {
        processor->bank.af.high |= processor->instruction.operand.low;
        processor->bank.af.carry = false;
        processor->bank.af.half_carry = false;
        processor->bank.af.subtract = false;
        processor->bank.af.zero = !processor->bank.af.high;
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
        case 0xC1: /* BC */
            bank = &processor->bank.bc;
            break;
        case 0xD1: /* DE */
            bank = &processor->bank.de;
            break;
        case 0xE1: /* HL */
            bank = &processor->bank.hl;
            break;
        case 0xF1: /* AF */
            bank = &processor->bank.af;
            break;
    }

    switch(processor->instruction.cycle) {
        case 0:
            bank->low = dmgl_processor_pop(processor);

            if(processor->instruction.opcode == 0xF1) {
                bank->low &= 0xF0;
            }
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
        case 0xC5: /* BC */
            bank = &processor->bank.bc;
            break;
        case 0xD5: /* DE */
            bank = &processor->bank.de;
            break;
        case 0xE5: /* HL */
            bank = &processor->bank.hl;
            break;
        case 0xF5: /* AF */
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
 * @brief Execute processor RES instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_res(dmgl_processor_t *processor)
{
    bool result = false;

    switch(processor->instruction.cycle) {
        case 0:

            switch(processor->instruction.opcode) {
                case 0x80: /* 0-7, B */
                case 0x88:
                case 0x90:
                case 0x98:
                case 0xA0:
                case 0xA8:
                case 0xB0:
                case 0xB8:
                    processor->bank.bc.high &= ~(1 << ((processor->instruction.opcode - 0x80) / 0x08));
                    processor->instruction.operand.low = processor->bank.bc.high;
                    break;
                case 0x81: /* 0-7, C */
                case 0x89:
                case 0x91:
                case 0x99:
                case 0xA1:
                case 0xA9:
                case 0xB1:
                case 0xB9:
                    processor->bank.bc.low &= ~(1 << ((processor->instruction.opcode - 0x81) / 0x08));
                    processor->instruction.operand.low = processor->bank.bc.low;
                    break;
                case 0x82: /* 0-7, D */
                case 0x8A:
                case 0x92:
                case 0x9A:
                case 0xA2:
                case 0xAA:
                case 0xB2:
                case 0xBA:
                    processor->bank.de.high &= ~(1 << ((processor->instruction.opcode - 0x82) / 0x08));
                    processor->instruction.operand.low = processor->bank.de.high;
                    break;
                case 0x83: /* 0-7, E */
                case 0x8B:
                case 0x93:
                case 0x9B:
                case 0xA3:
                case 0xAB:
                case 0xB3:
                case 0xBB:
                    processor->bank.de.low &= ~(1 << ((processor->instruction.opcode - 0x83) / 0x08));
                    processor->instruction.operand.low = processor->bank.de.low;
                    break;
                case 0x84: /* 0-7, H */
                case 0x8C:
                case 0x94:
                case 0x9C:
                case 0xA4:
                case 0xAC:
                case 0xB4:
                case 0xBC:
                    processor->bank.hl.high &= ~(1 << ((processor->instruction.opcode - 0x84) / 0x08));
                    processor->instruction.operand.low = processor->bank.hl.high;
                    break;
                case 0x85: /* 0-7, L */
                case 0x8D:
                case 0x95:
                case 0x9D:
                case 0xA5:
                case 0xAD:
                case 0xB5:
                case 0xBD:
                    processor->bank.hl.low &= ~(1 << ((processor->instruction.opcode - 0x85) / 0x08));
                    processor->instruction.operand.low = processor->bank.hl.low;
                    break;
                case 0x87: /* 0-7, A */
                case 0x8F:
                case 0x97:
                case 0x9F:
                case 0xA7:
                case 0xAF:
                case 0xB7:
                case 0xBF:
                    processor->bank.af.high &= ~(1 << ((processor->instruction.opcode - 0x87) / 0x08));
                    processor->instruction.operand.low = processor->bank.af.high;
                    break;
                default:
                    result = true;
                    break;
            }
            break;
        case 1:

            switch(processor->instruction.opcode) {
                case 0x86: /* 0-7, (HL) */
                case 0x8E:
                case 0x96:
                case 0x9E:
                case 0xA6:
                case 0xAE:
                case 0xB6:
                case 0xBE:
                    processor->instruction.operand.low = dmgl_bus_read(processor->bank.hl.word);
                    result = true;
                    break;
                default:
                    break;
            }
            break;
        case 2:

            switch(processor->instruction.opcode) {
                case 0x86: /* 0-7, (HL) */
                case 0x8E:
                case 0x96:
                case 0x9E:
                case 0xA6:
                case 0xAE:
                case 0xB6:
                case 0xBE:
                    dmgl_bus_write(processor->bank.hl.word, processor->instruction.operand.low & ~(1 << ((processor->instruction.opcode - 0x86) / 0x08)));
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return result;
}

/*!
 * @brief Execute processor RL instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_rl(dmgl_processor_t *processor)
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
 * @brief Execute processor RR instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_rr(dmgl_processor_t *processor)
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
 * @brief Execute processor RRC instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_rrc(dmgl_processor_t *processor)
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
    processor->bank.af.carry = true;
    processor->bank.af.half_carry = false;
    processor->bank.af.subtract = false;

    return false;
}

/*!
 * @brief Execute processor SET instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_set(dmgl_processor_t *processor)
{
    bool result = false;

    switch(processor->instruction.cycle) {
        case 0:

            switch(processor->instruction.opcode) {
                case 0xC0: /* 0-7, B */
                case 0xC8:
                case 0xD0:
                case 0xD8:
                case 0xE0:
                case 0xE8:
                case 0xF0:
                case 0xF8:
                    processor->bank.bc.high |= (1 << ((processor->instruction.opcode - 0xC0) / 0x08));
                    processor->instruction.operand.low = processor->bank.bc.high;
                    break;
                case 0xC1: /* 0-7, C */
                case 0xC9:
                case 0xD1:
                case 0xD9:
                case 0xE1:
                case 0xE9:
                case 0xF1:
                case 0xF9:
                    processor->bank.bc.low |= (1 << ((processor->instruction.opcode - 0xC1) / 0x08));
                    processor->instruction.operand.low = processor->bank.bc.low;
                    break;
                case 0xC2: /* 0-7, D */
                case 0xCA:
                case 0xD2:
                case 0xDA:
                case 0xE2:
                case 0xEA:
                case 0xF2:
                case 0xFA:
                    processor->bank.de.high |= (1 << ((processor->instruction.opcode - 0xC2) / 0x08));
                    processor->instruction.operand.low = processor->bank.de.high;
                    break;
                case 0xC3: /* 0-7, E */
                case 0xCB:
                case 0xD3:
                case 0xDB:
                case 0xE3:
                case 0xEB:
                case 0xF3:
                case 0xFB:
                    processor->bank.de.low |= (1 << ((processor->instruction.opcode - 0xC3) / 0x08));
                    processor->instruction.operand.low = processor->bank.de.low;
                    break;
                case 0xC4: /* 0-7, H */
                case 0xCC:
                case 0xD4:
                case 0xDC:
                case 0xE4:
                case 0xEC:
                case 0xF4:
                case 0xFC:
                    processor->bank.hl.high |= (1 << ((processor->instruction.opcode - 0xC4) / 0x08));
                    processor->instruction.operand.low = processor->bank.hl.high;
                    break;
                case 0xC5: /* 0-7, L */
                case 0xCD:
                case 0xD5:
                case 0xDD:
                case 0xE5:
                case 0xED:
                case 0xF5:
                case 0xFD:
                    processor->bank.hl.low |= (1 << ((processor->instruction.opcode - 0xC5) / 0x08));
                    processor->instruction.operand.low = processor->bank.hl.low;
                    break;
                case 0xC7: /* 0-7, A */
                case 0xCF:
                case 0xD7:
                case 0xDF:
                case 0xE7:
                case 0xEF:
                case 0xF7:
                case 0xFF:
                    processor->bank.af.high |= (1 << ((processor->instruction.opcode - 0xC7) / 0x08));
                    processor->instruction.operand.low = processor->bank.af.high;
                    break;
                default:
                    result = true;
                    break;
            }
            break;
        case 1:

            switch(processor->instruction.opcode) {
                case 0xC6: /* 0-7, (HL) */
                case 0xCE:
                case 0xD6:
                case 0xDE:
                case 0xE6:
                case 0xEE:
                case 0xF6:
                case 0xFE:
                    processor->instruction.operand.low = dmgl_bus_read(processor->bank.hl.word);
                    result = true;
                    break;
                default:
                    break;
            }
            break;
        case 2:

            switch(processor->instruction.opcode) {
                case 0xC6: /* 0-7, (HL) */
                case 0xCE:
                case 0xD6:
                case 0xDE:
                case 0xE6:
                case 0xEE:
                case 0xF6:
                case 0xFE:
                    dmgl_bus_write(processor->bank.hl.word, processor->instruction.operand.low | (1 << ((processor->instruction.opcode - 0xC6) / 0x08)));
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return result;
}

/*!
 * @brief Execute processor SLA instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_sla(dmgl_processor_t *processor)
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
 * @brief Execute processor SRA instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_sra(dmgl_processor_t *processor)
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
 * @brief Execute processor SRL instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_srl(dmgl_processor_t *processor)
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
 * @brief Execute processor STOP instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_stop(dmgl_processor_t *processor)
{
    processor->stop.code = dmgl_processor_fetch(processor);
    processor->stop.enabled = true;

    return false;
}

/*!
 * @brief Execute processor SWAP instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_swap(dmgl_processor_t *processor)
{
    bool result = false;

    switch(processor->instruction.cycle) {
        case 0:

            switch(processor->instruction.opcode) {
                case 0x30: /* B */
                    processor->bank.bc.high = (processor->bank.bc.high << 4) | (processor->bank.bc.high >> 4);
                    processor->instruction.operand.low = processor->bank.bc.high;
                    break;
                case 0x31: /* C */
                    processor->bank.bc.low = (processor->bank.bc.low << 4) | (processor->bank.bc.low >> 4);
                    processor->instruction.operand.low = processor->bank.bc.low;
                    break;
                case 0x32: /* D */
                    processor->bank.de.high = (processor->bank.de.high << 4) | (processor->bank.de.high >> 4);
                    processor->instruction.operand.low = processor->bank.de.high;
                    break;
                case 0x33: /* E */
                    processor->bank.de.low = (processor->bank.de.low << 4) | (processor->bank.de.low >> 4);
                    processor->instruction.operand.low = processor->bank.de.low;
                    break;
                case 0x34: /* H */
                    processor->bank.hl.high = (processor->bank.hl.high << 4) | (processor->bank.hl.high >> 4);
                    processor->instruction.operand.low = processor->bank.hl.high;
                    break;
                case 0x35: /* L */
                    processor->bank.hl.low = (processor->bank.hl.low << 4) | (processor->bank.hl.low >> 4);
                    processor->instruction.operand.low = processor->bank.hl.low;
                    break;
                case 0x37: /* A */
                    processor->bank.af.high = (processor->bank.af.high << 4) | (processor->bank.af.high >> 4);
                    processor->instruction.operand.low = processor->bank.af.high;
                    break;
                default:
                    result = true;
                    break;
            }
            break;
        case 1:

            switch(processor->instruction.opcode) {
                case 0x36: /* (HL) */
                    processor->instruction.operand.low = dmgl_bus_read(processor->bank.hl.word);
                    result = true;
                    break;
                default:
                    break;
            }
        case 2:

            switch(processor->instruction.opcode) {
                case 0x36: /* (HL) */
                    processor->instruction.operand.low = (processor->instruction.operand.low << 4) | (processor->instruction.operand.low >> 4);
                    dmgl_bus_write(processor->bank.hl.word, processor->instruction.operand.low);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    if(!result) {
        processor->bank.af.carry = false;
        processor->bank.af.half_carry = false;
        processor->bank.af.subtract = false;
        processor->bank.af.zero = !processor->instruction.operand.low;
    }

    return result;
}

/*!
 * @brief Execute processor XOR instruction.
 * @param[in,out] processor Pointer to processor context
 * @return true on success, false otherwise
 */
static bool dmgl_processor_instruction_xor(dmgl_processor_t *processor)
{
    bool result = false;

    switch(processor->instruction.cycle) {
        case 0:

            switch(processor->instruction.opcode) {
                case 0xA8: /* B */
                    processor->instruction.operand.low = processor->bank.bc.high;
                    break;
                case 0xA9: /* C */
                    processor->instruction.operand.low = processor->bank.bc.low;
                    break;
                case 0xAA: /* D */
                    processor->instruction.operand.low = processor->bank.de.high;
                    break;
                case 0xAB: /* E */
                    processor->instruction.operand.low = processor->bank.de.low;
                    break;
                case 0xAC: /* H */
                    processor->instruction.operand.low = processor->bank.hl.high;
                    break;
                case 0xAD: /* L */
                    processor->instruction.operand.low = processor->bank.hl.low;
                    break;
                case 0xAF: /* A */
                    processor->instruction.operand.low = processor->bank.af.high;
                    break;
                default:
                    result = true;
                    break;
            }
            break;
        case 1:

            switch(processor->instruction.opcode) {
                case 0xAE: /* (HL) */
                    processor->instruction.operand.low = dmgl_bus_read(processor->bank.hl.word);
                    break;
                case 0xEE: /* N */
                    processor->instruction.operand.low = dmgl_processor_fetch(processor);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    if(!result) {
        processor->bank.af.high ^= processor->instruction.operand.low;
        processor->bank.af.carry = false;
        processor->bank.af.half_carry = false;
        processor->bank.af.subtract = false;
        processor->bank.af.zero = !processor->bank.af.high;
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
            dmgl_processor_instruction_and, dmgl_processor_instruction_and, dmgl_processor_instruction_and, dmgl_processor_instruction_and,
            dmgl_processor_instruction_and, dmgl_processor_instruction_and, dmgl_processor_instruction_and, dmgl_processor_instruction_and,
            /* A8 */
            dmgl_processor_instruction_xor, dmgl_processor_instruction_xor, dmgl_processor_instruction_xor, dmgl_processor_instruction_xor,
            dmgl_processor_instruction_xor, dmgl_processor_instruction_xor, dmgl_processor_instruction_xor, dmgl_processor_instruction_xor,
            /* B0 */
            dmgl_processor_instruction_or, dmgl_processor_instruction_or, dmgl_processor_instruction_or, dmgl_processor_instruction_or,
            dmgl_processor_instruction_or, dmgl_processor_instruction_or, dmgl_processor_instruction_or, dmgl_processor_instruction_or,
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
            NULL, dmgl_processor_instruction_push, dmgl_processor_instruction_and, NULL,
            /* E8 */
            NULL, NULL, NULL, NULL,
            NULL, NULL, dmgl_processor_instruction_xor, NULL,
            /* F0 */
            NULL, dmgl_processor_instruction_pop, NULL, dmgl_processor_instruction_di,
            NULL, dmgl_processor_instruction_push, dmgl_processor_instruction_or, NULL,
            /* F8 */
            NULL, NULL, NULL, dmgl_processor_instruction_ei,
            NULL, NULL, NULL, NULL,
        }, instruction_extended[] = {
            /* 00 */
            dmgl_processor_instruction_rlc, dmgl_processor_instruction_rlc, dmgl_processor_instruction_rlc, dmgl_processor_instruction_rlc,
            dmgl_processor_instruction_rlc, dmgl_processor_instruction_rlc, dmgl_processor_instruction_rlc, dmgl_processor_instruction_rlc,
            /* 08 */
            dmgl_processor_instruction_rrc, dmgl_processor_instruction_rrc, dmgl_processor_instruction_rrc, dmgl_processor_instruction_rrc,
            dmgl_processor_instruction_rrc, dmgl_processor_instruction_rrc, dmgl_processor_instruction_rrc, dmgl_processor_instruction_rrc,
            /* 10 */
            dmgl_processor_instruction_rl, dmgl_processor_instruction_rl, dmgl_processor_instruction_rl, dmgl_processor_instruction_rl,
            dmgl_processor_instruction_rl, dmgl_processor_instruction_rl, dmgl_processor_instruction_rl, dmgl_processor_instruction_rl,
            /* 18 */
            dmgl_processor_instruction_rr, dmgl_processor_instruction_rr, dmgl_processor_instruction_rr, dmgl_processor_instruction_rr,
            dmgl_processor_instruction_rr, dmgl_processor_instruction_rr, dmgl_processor_instruction_rr, dmgl_processor_instruction_rr,
            /* 20 */
            dmgl_processor_instruction_sla, dmgl_processor_instruction_sla, dmgl_processor_instruction_sla, dmgl_processor_instruction_sla,
            dmgl_processor_instruction_sla, dmgl_processor_instruction_sla, dmgl_processor_instruction_sla, dmgl_processor_instruction_sla,
            /* 28 */
            dmgl_processor_instruction_sra, dmgl_processor_instruction_sra, dmgl_processor_instruction_sra, dmgl_processor_instruction_sra,
            dmgl_processor_instruction_sra, dmgl_processor_instruction_sra, dmgl_processor_instruction_sra, dmgl_processor_instruction_sra,
            /* 30 */
            dmgl_processor_instruction_swap, dmgl_processor_instruction_swap, dmgl_processor_instruction_swap, dmgl_processor_instruction_swap,
            dmgl_processor_instruction_swap, dmgl_processor_instruction_swap, dmgl_processor_instruction_swap, dmgl_processor_instruction_swap,
            /* 38 */
            dmgl_processor_instruction_srl, dmgl_processor_instruction_srl, dmgl_processor_instruction_srl, dmgl_processor_instruction_srl,
            dmgl_processor_instruction_srl, dmgl_processor_instruction_srl, dmgl_processor_instruction_srl, dmgl_processor_instruction_srl,
            /* 40 */
            dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit,
            dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit,
            /* 48 */
            dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit,
            dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit,
            /* 50 */
            dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit,
            dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit,
            /* 58 */
            dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit,
            dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit,
            /* 60 */
            dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit,
            dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit,
            /* 68 */
            dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit,
            dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit,
            /* 70 */
            dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit,
            dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit,
            /* 78 */
            dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit,
            dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit, dmgl_processor_instruction_bit,
            /* 80 */
            dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res,
            dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res,
            /* 88 */
            dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res,
            dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res,
            /* 90 */
            dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res,
            dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res,
            /* 98 */
            dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res,
            dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res,
            /* A0 */
            dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res,
            dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res,
            /* A8 */
            dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res,
            dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res,
            /* B0 */
            dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res,
            dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res,
            /* B8 */
            dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res,
            dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res, dmgl_processor_instruction_res,
            /* C0 */
            dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set,
            dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set,
            /* C8 */
            dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set,
            dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set,
            /* D0 */
            dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set,
            dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set,
            /* D8 */
            dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set,
            dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set,
            /* E0 */
            dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set,
            dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set,
            /* E8 */
            dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set,
            dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set,
            /* F0 */
            dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set,
            dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set,
            /* F8 */
            dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set,
            dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set, dmgl_processor_instruction_set,
        };

    switch(processor->instruction.cycle) {
        case 0:

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

    if(!processor->instruction.cycle) {
        processor->instruction.address.word = processor->bank.pc.word;
        processor->instruction.opcode = dmgl_processor_fetch(processor);
        processor->instruction.extended = (processor->instruction.opcode == 0xCB);
        processor->instruction.operand.word = 0x0000;

        if(processor->halt.bug) {
            processor->halt.bug = false;
            --processor->bank.pc.word;
        }

        if(processor->interrupt.enabling) {

            if(!--processor->interrupt.enabling) {
                processor->interrupt.enabled = true;
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
            processor->instruction.extended = (processor->instruction.opcode == 0xCB);
            processor->instruction.operand.word = 0x0000;
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
    processor->instruction.extended = (processor->instruction.opcode == 0xCB);
    processor->instruction.operand.word = 0x0000;
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
