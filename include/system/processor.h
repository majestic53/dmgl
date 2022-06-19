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

#ifndef DMGL_PROCESSOR_H_
#define DMGL_PROCESSOR_H_

#include <common.h>

typedef union {

    struct {
        uint8_t vblank : 1;
        uint8_t screen : 1;
        uint8_t timer : 1;
        uint8_t serial : 1;
        uint8_t button : 1;
    };

    uint8_t raw;
} dmgl_processor_interrupt_t;

typedef union {

    struct {

        union {

            struct {
                uint8_t : 4;
                uint8_t carry : 1;
                uint8_t half_carry : 1;
                uint8_t subtract : 1;
                uint8_t zero : 1;
            };

            uint8_t low;
        };

        uint8_t high;
    };

    uint16_t word;
    uint32_t dword;
} dmgl_processor_register_t;

typedef struct {
    uint8_t cycle;

    struct {
        dmgl_processor_register_t af;
        dmgl_processor_register_t bc;
        dmgl_processor_register_t de;
        dmgl_processor_register_t hl;
        dmgl_processor_register_t pc;
        dmgl_processor_register_t sp;
    } bank;

    struct {
        bool bug;
        bool enabled;
    } halt;

    struct {
        dmgl_processor_register_t address;
        dmgl_processor_register_t operand;
        uint8_t opcode;
        uint8_t cycle;
        bool extended;
    } instruction;

    struct {
        dmgl_processor_register_t address;
        dmgl_processor_interrupt_t enable;
        dmgl_processor_interrupt_t flag;
        uint8_t cycle;
        bool enabled;
    } interrupt;

    struct {
        uint8_t code;
        bool enabled;
    } stop;
} dmgl_processor_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

dmgl_error_e dmgl_processor_clock(dmgl_processor_t *processor);

dmgl_error_e dmgl_processor_initialize(dmgl_processor_t *processor, bool has_bootloader, uint8_t checksum);

uint8_t dmgl_processor_read(const dmgl_processor_t *processor, uint16_t address);

void dmgl_processor_uninitialize(dmgl_processor_t *processor);

void dmgl_processor_write(dmgl_processor_t *processor, uint16_t address, uint8_t value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMGL_PROCESSOR_H_ */
