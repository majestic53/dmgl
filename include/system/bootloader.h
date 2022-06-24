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
 * @file bootloader.h
 * @brief Bootloader subsystem.
 */

#ifndef DMGL_BOOTLOADER_H_
#define DMGL_BOOTLOADER_H_

#include <common.h>

/*!
 * @struct dmgl_bootloader_t
 * @brief Bootloader subsystem context.
 */
typedef struct {
    const uint8_t *data;    /*!< Bootloader data */
    bool enabled;           /*!< Bootloader enabled flag */
} dmgl_bootloader_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Disable bootloader subsystem.
 * @param[in,out] bootloader Pointer to bootloader subsystem context
 */
void dmgl_bootloader_disable(dmgl_bootloader_t *bootloader);

/*!
 * @brief Query bootloader subsystem enabled flag.
 * @param[in] bootloader Constant pointer to bootloader subsystem context
 * @return Bootloader subsystem enabled flag
 */
bool dmgl_bootloader_enabled(const dmgl_bootloader_t *bootloader);

/*!
 * @brief Initialize bootloader subsystem.
 * @param[in,out] bootloader Pointer to bootloader subsystem context
 * @param[in] data Pointer to bootloader data
 * @param[in] length Bootloader data length, in bytes
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
dmgl_error_e dmgl_bootloader_initialize(dmgl_bootloader_t *bootloader, const uint8_t *data, size_t length);

/*!
 * @brief Read byte from bootloader subsystem.
 * @param[in] bootloader Constant pointer to bootloader subsystem context
 * @param[in] address Byte address
 * @return Byte value
 */
uint8_t dmgl_bootloader_read(const dmgl_bootloader_t *bootloader, uint16_t address);

/*!
 * @brief Reset bootloader subsystem.
 * @param[in,out] bootloader Pointer to bootloader subsystem context
 */
void dmgl_bootloader_reset(dmgl_bootloader_t *bootloader);

/*!
 * @brief Uninitialize bootloader subsystem.
 * @param[in,out] bootloader Pointer to bootloader subsystem context
 */
void dmgl_bootloader_uninitialize(dmgl_bootloader_t *bootloader);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMGL_BOOTLOADER_H_ */
