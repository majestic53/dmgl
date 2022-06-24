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
 * @file processor.h
 * @brief Processor subsystem.
 */

#ifndef DMGL_PROCESSOR_H_
#define DMGL_PROCESSOR_H_

#include <common.h>

/*!
 * @struct dmgl_processor_interrupt_t
 * @brief Processor subsystem interrupt.
 */
typedef union {

    struct {
        uint8_t vblank : 1;                 /*!< Vertical-blank interrupt flag (V) */
        uint8_t screen : 1;                 /*!< Screen interrupt flag (L) */
        uint8_t timer : 1;                  /*!< Timer interrupt flag (T) */
        uint8_t serial : 1;                 /*!< Serial interrupt flag (S) */
        uint8_t button : 1;                 /*!< Button interrupt flag (J) */
    };

    uint8_t raw;                            /*!< Raw interrupt flags */
} dmgl_processor_interrupt_t;

/*!
 * @struct dmgl_processor_register_t
 * @brief Processor subsystem register.
 */
typedef union {

    struct {

        union {

            struct {
                uint8_t : 4;                /*!< Unused flags */
                uint8_t carry : 1;          /*!< Carry flag (C) */
                uint8_t half_carry : 1;     /*!< Half-carry flag (H) */
                uint8_t subtract : 1;       /*!< Subtract flag (N) */
                uint8_t zero : 1;           /*!< Zero flag (Z) */
            };

            uint8_t low;                    /*!< Low byte */
        };

        uint8_t high;                       /*!< High byte */
    };

    uint16_t word;                          /*!< Word */
    uint32_t dword;                         /*!< Double-word */
} dmgl_processor_register_t;

/*!
 * @struct dmgl_processor_t
 * @brief Processor subsystem context.
 */
typedef struct {
    uint8_t cycle;                          /*!< Processor cycle counter */
    uint8_t checksum;                       /*!< Processor cartridge checksum */
    bool has_bootloader;                    /*!< Processor bootloader flag */

    struct {
        dmgl_processor_register_t af;       /*!< Processor AF register */
        dmgl_processor_register_t bc;       /*!< Processor BC register */
        dmgl_processor_register_t de;       /*!< Processor DE register */
        dmgl_processor_register_t hl;       /*!< Processor HL register */
        dmgl_processor_register_t pc;       /*!< Processor PC register */
        dmgl_processor_register_t sp;       /*!< Processor SP register */
    } bank;                                 /*!< Processor registers */

    struct {
        bool bug;                           /*!< Processor halt-bug flag */
        bool enabled;                       /*!< Processor halt enabled flag */
    } halt;                                 /*!< Processor halt */

    struct {
        dmgl_processor_register_t address;  /*!< Processor instruction address register */
        dmgl_processor_register_t operand;  /*!< Processor instruction operand register */
        uint8_t opcode;                     /*!< Processor instruction opcode */
        uint8_t cycle;                      /*!< Processor instruction cycle counter */
        bool extended;                      /*!< Processor instruction extended flag */
    } instruction;                          /*!< Processor instruction */

    struct {
        dmgl_processor_register_t address;  /*!< Processor interrupt address register */
        dmgl_processor_interrupt_t enable;  /*!< Processor interrupt enable register (IE) [FF0F] */
        dmgl_processor_interrupt_t flag;    /*!< Processor interrupt flag register (IF) [FFFF] */
        uint8_t cycle;                      /*!< Processor interrupt cycle counter */
        uint8_t enabling;                   /*!< Processor interrupt enabling counter */
        bool enabled;                       /*!< Processor interrupt enabled flag */
    } interrupt;                            /*!< Processor interrupt */

    struct {
        uint8_t code;                       /*!< Processor stop code */
        bool enabled;                       /*!< Processor stop enabled flag */
    } stop;                                 /*!< Processor stop */
} dmgl_processor_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Clock processor subsystem.
 * @param[in,out] processor Pointer to processor subsystem context
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
dmgl_error_e dmgl_processor_clock(dmgl_processor_t *processor);

/*!
 * @brief Initialize processor subsystem.
 * @param[in,out] processor Pointer to processor subsystem context
 * @param[in] has_bootloader Bootloader flag
 * @param[in] checksum Cartridge checksum
 */
void dmgl_processor_initialize(dmgl_processor_t *processor, bool has_bootloader, uint8_t checksum);

/*!
 * @brief Read byte from processor subsystem.
 * @param[in] processor Constant pointer to processor subsystem context
 * @param[in] address Byte address
 * @return Byte value
 */
uint8_t dmgl_processor_read(const dmgl_processor_t *processor, uint16_t address);

/*!
 * @brief Reset processor subsystem.
 * @param[in,out] processor Pointer to processor subsystem context
 */
void dmgl_processor_reset(dmgl_processor_t *processor);

/*!
 * @brief Uninitialize processor subsystem.
 * @param[in,out] processor Pointer to processor subsystem context
 */
void dmgl_processor_uninitialize(dmgl_processor_t *processor);

/*!
 * @brief Write byte to processor subsystem.
 * @param[in,out] processor Pointer to processor subsystem context
 * @param[in] address Byte address
 * @param[in] value Byte value
 */
void dmgl_processor_write(dmgl_processor_t *processor, uint16_t address, uint8_t value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMGL_PROCESSOR_H_ */
