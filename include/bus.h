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
 * @file bus.h
 * @brief Bus interface.
 */

#ifndef DMGL_BUS_H_
#define DMGL_BUS_H_

#include <common.h>

/*!
 * @enum dmgl_interrupt_e
 * @brief Bus interrupt types.
 */
typedef enum {
    DMGL_INTERRUPT_VBLANK = 0,  /*!< Vertical-blank interrupt type (V) */
    DMGL_INTERRUPT_SCREEN,      /*!< Screen interrupt type (L) */
    DMGL_INTERRUPT_TIMER,       /*!< Timer interrupt type (T) */
    DMGL_INTERRUPT_SERIAL,      /*!< Serial interrupt type (S) */
    DMGL_INTERRUPT_BUTTON,      /*!< Button interrupt type (J) */
    DMGL_INTERRUPT_MAX,         /*!< Max interrupt type */
} dmgl_interrupt_e;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Clock bus interface.
 * @return DMGL_SUCCESS or DMGL_COMPLETE on success, DMGL_FAILURE otherwise
 */
dmgl_error_e dmgl_bus_clock(void);

/*!
 * @brief Initialize bus interface.
 * @param[in] context Constant pointer to DMGL context
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
dmgl_error_e dmgl_bus_initialize(const dmgl_t *context);

/*!
 * @brief Interrupt bus interface.
 * @param[in] interrupt Byte interrupt type
 */
void dmgl_bus_interrupt(dmgl_interrupt_e interrupt);

/*!
 * @brief Read byte from bus interface.
 * @param[in] address Byte address
 * @return Byte value
 */
uint8_t dmgl_bus_read(uint16_t address);

/*!
 * @brief Reset bus interface.
 */
void dmgl_bus_reset(void);

/*!
 * @brief Query bus title string.
 * @return Constant pointer to bus title string
 */
const char *dmgl_bus_title(void);

/*!
 * @brief Uninitialize bus interface.
 */
void dmgl_bus_uninitialize(void);

/*!
 * @brief Write byte to bus interface.
 * @param[in] address Byte address
 * @param[in] value Byte value
 */
void dmgl_bus_write(uint16_t address, uint8_t value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMGL_BUS_H_ */
