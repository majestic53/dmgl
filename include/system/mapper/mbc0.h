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
 * @file mbc0.h
 * @brief MBC0 mapper subsystem.
 */

#ifndef DMGL_MBC0_H_
#define DMGL_MBC0_H_

#include <mapper.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Initialize MBC0 mapper subsystem.
 * @param[in] cartridge Pointer to cartridge subsystem context
 * @param[out] context Pointer to context pointer
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
dmgl_error_e dmgl_mbc0_initialize(const dmgl_cartridge_t *cartridge, void **context);

/*!
 * @brief Read byte from MBC0 mapper subsystem.
 * @param[in] cartridge Constant pointer to cartridge subsystem context
 * @param[in,out] context Pointer to context
 * @param[in] address Byte address
 * @return Byte value
 */
uint8_t dmgl_mbc0_read(const dmgl_cartridge_t *cartridge, void *context, uint16_t address);

/*!
 * @brief Reset MBC0 mapper subsystem.
 * @param[in,out] context Pointer to context
 */
void dmgl_mbc0_reset(void *context);

/*!
 * @brief Uninitialize MBC0 mapper subsystem.
 * @param[in,out] context Pointer to context
 */
void dmgl_mbc0_uninitialize(void *context);

/*!
 * @brief Write byte to MBC0 mapper subsystem.
 * @param[in,out] cartridge Pointer to cartridge subsystem context
 * @param[in,out] context Pointer to context
 * @param[in] address Byte address
 * @param[in] value Byte value
 */
void dmgl_mbc0_write(dmgl_cartridge_t *cartridge, void *context, uint16_t address, uint8_t value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMGL_MBC0_H_ */
