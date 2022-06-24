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
 * @file mapper.h
 * @brief Mapper subsystem.
 */

#ifndef DMGL_MAPPER_H_
#define DMGL_MAPPER_H_

#include <cartridge.h>

/*!
 * @struct dmgl_mapper_handler_t
 * @brief Mapper subsystem handlers.
 */
typedef struct {
    dmgl_error_e (*initialize)(const dmgl_cartridge_t *, void **);  /*!< Mapper initialize handler */
    uint8_t (*read)(const dmgl_cartridge_t *, void *, uint16_t);    /*!< Mapper read handler */
    void (*reset)(void *);                                          /*!< Mapper reset handler */
    void (*uninitialize)(void *);                                   /*!< Mapper uninitialize handler */
    void (*write)(dmgl_cartridge_t *, void *, uint16_t, uint8_t);   /*!< Mapper write handler */
} dmgl_mapper_handler_t;

/*!
 * @struct dmgl_mapper_t
 * @brief Mapper subsystem context.
 */
typedef struct {
    dmgl_cartridge_t cartridge;                                     /*!< Cartridge subsystem context */
    dmgl_mapper_handler_t handler;                                  /*!< Mapper handlers */
    void *context;                                                  /*!< Mapper context */
} dmgl_mapper_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Query mapper checksum.
 * @param[in] mapper Constant pointer to mapper subsystem context
 * @return Mapper checksum value
 */
uint8_t dmgl_mapper_checksum(const dmgl_mapper_t *mapper);

/*!
 * @brief Initialize mapper subsystem.
 * @param[in,out] mapper Pointer to mapper subsystem context
 * @param[in] data Pointer to mapper data
 * @param[in] length Mapper data length, in bytes
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
dmgl_error_e dmgl_mapper_initialize(dmgl_mapper_t *mapper, const uint8_t *data, size_t length);

/*!
 * @brief Read byte from mapper subsystem.
 * @param[in] mapper Constant pointer to mapper subsystem context
 * @param[in] address Byte address
 * @return Byte value
 */
uint8_t dmgl_mapper_read(const dmgl_mapper_t *mapper, uint16_t address);

/*!
 * @brief Reset mapper subsystem.
 * @param[in,out] mapper Pointer to mapper subsystem context
 */
void dmgl_mapper_reset(dmgl_mapper_t *mapper);

/*!
 * @brief Query mapper title string.
 * @param[in] mapper Constant pointer to mapper subsystem context
 * @return Constant pointer to mapper title string
 */
const char *dmgl_mapper_title(const dmgl_mapper_t *mapper);

/*!
 * @brief Uninitialize mapper subsystem.
 * @param[in,out] mapper Pointer to mapper subsystem context
 */
void dmgl_mapper_uninitialize(dmgl_mapper_t *mapper);

/*!
 * @brief Write byte to mapper subsystem.
 * @param[in,out] mapper Pointer to mapper subsystem context
 * @param[in] address Byte address
 * @param[in] value Byte value
 */
void dmgl_mapper_write(dmgl_mapper_t *mapper, uint16_t address, uint8_t value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMGL_MAPPER_H_ */
