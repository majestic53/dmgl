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

#include <processor.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

dmgl_error_e dmgl_processor_clock(dmgl_processor_t *processor)
{
    dmgl_error_e result;

    /* TODO */
    result = DMGL_SUCCESS;
    /* ---- */

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
            processor->interrupt.flag.raw = value;
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
