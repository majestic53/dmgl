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
 * @file memory.h
 * @brief Memory subsystem.
 */

#ifndef DMGL_MEMORY_H_
#define DMGL_MEMORY_H_

#include <bootloader.h>
#include <mapper.h>

/*!
 * @struct dmgl_memory_t
 * @brief Memory subsystem context.
 */
typedef struct {
    dmgl_bootloader_t bootloader;   /*!< Bootloader subsystem context */
    dmgl_mapper_t mapper;           /*!< Mapper subsystem context */
    uint8_t high[0x7F];             /*!< High RAM [FF80-FFFE] */
    uint8_t internal[0x2000];       /*!< Internal RAM [C000-DFFF], Mirrored [E000-FDFF] */
    uint8_t sprite[0xA0];           /*!< Sprite RAM [FE00-FE9F] */
    uint8_t video[0x2000];          /*!< Video RAM [8000-9FFF] */
} dmgl_memory_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Query memory checksum.
 * @param[in] memory Constant pointer to memory subsystem context
 * @return Memory checksum value
 */
uint8_t dmgl_memory_checksum(const dmgl_memory_t *memory);

/*!
 * @brief Query memory bootloader status.
 * @param[in] memory Constant pointer to memory subsystem context
 * @return Memory bootloader status
 */
bool dmgl_memory_has_bootloader(const dmgl_memory_t *memory);

/*!
 * @brief Initialize memory subsystem.
 * @param[in,out] memory Pointer to memory subsystem context
 * @param[in] context Constant pointer to DMGL context
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
dmgl_error_e dmgl_memory_initialize(dmgl_memory_t *memory, const dmgl_t *context);

/*!
 * @brief Read byte from memory subsystem.
 * @param[in] memory Constant pointer to memory subsystem context
 * @param[in] address Byte address
 * @return Byte value
 */
uint8_t dmgl_memory_read(const dmgl_memory_t *memory, uint16_t address);

/*!
 * @brief Reset memory subsystem.
 * @param[in,out] memory Pointer to memory subsystem context
 */
void dmgl_memory_reset(dmgl_memory_t *memory);

/*!
 * @brief Query memory title string.
 * @param[in] memory Constant pointer to memory subsystem context
 * @return Constant pointer to memory title string
 */
const char *dmgl_memory_title(const dmgl_memory_t *memory);

/*!
 * @brief Uninitialize memory subsystem.
 * @param[in,out] memory Pointer to memory subsystem context
 */
void dmgl_memory_uninitialize(dmgl_memory_t *memory);

/*!
 * @brief Write byte to memory subsystem.
 * @param[in,out] memory Pointer to memory subsystem context
 * @param[in] address Byte address
 * @param[in] value Byte value
 */
void dmgl_memory_write(dmgl_memory_t *memory, uint16_t address, uint8_t value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMGL_MEMORY_H_ */
