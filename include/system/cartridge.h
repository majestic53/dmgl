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
 * @file cartridge.h
 * @brief Cartridge subsystem.
 */

#ifndef DMGL_CARTRIDGE_H_
#define DMGL_CARTRIDGE_H_

#include <common.h>

/*!
 * @enum dmgl_cartridge_e
 * @brief Cartridge types.
 */
typedef enum {
    DMGL_CARTRIDGE_MBC0 = 0,    /*!< MBC0 cartridge type */
    DMGL_CARTRIDGE_MAX,         /*!< Max cartridge type */
} dmgl_cartridge_e;

/*!
 * @struct dmgl_cartridge_header_t
 * @brief Cartridge header.
 */
typedef struct {
    uint8_t entry[4];           /*!< Entry string */
    uint8_t logo[48];           /*!< Logo string */
    uint8_t title[11];          /*!< Title string */
    uint8_t manufacturer[4];    /*!< Manufacturer string */
    uint8_t cgb;                /*!< CGB flag */
    uint8_t licensee[2];        /*!< Licensee string */
    uint8_t sgb;                /*!< SGB flag */
    uint8_t type;               /*!< Cartridge type */
    uint8_t rom;                /*!< ROM type */
    uint8_t ram;                /*!< RAM type */
    uint8_t destination;        /*!< Destination code */
    uint8_t licensee_old;       /*!< Licensee code */
    uint8_t version;            /*!< Mask version */
    uint8_t checksum;           /*!< Header checksum */
    uint16_t checksum_global;   /*!< Global checksum */
} dmgl_cartridge_header_t;

/*!
 * @struct dmgl_cartridge_t
 * @brief Cartridge subsystem context.
 */
typedef struct {

    struct {
        uint8_t **bank;         /*!< Cartridge RAM banks */
        size_t count;           /*!< Cartridge RAM bank count */
    } ram;                      /*!< Cartridge RAM */

    struct {
        const uint8_t **bank;   /*!< Cartridge ROM banks */
        size_t count;           /*!< Cartridge ROM bank count */
    } rom;                      /*!< Cartridge ROM */
} dmgl_cartridge_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Query cartridge checksum.
 * @param[in] cartridge Constant pointer to cartridge subsystem context
 * @return Cartridge checksum value
 */
uint8_t dmgl_cartridge_checksum(const dmgl_cartridge_t *cartridge);

/*!
 * @brief Initialize cartridge subsystem.
 * @param[in,out] cartridge Pointer to cartridge subsystem context
 * @param[in] data Pointer to cartridge data
 * @param[in] length Cartridge data length, in bytes
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
dmgl_error_e dmgl_cartridge_initialize(dmgl_cartridge_t *cartridge, const uint8_t *data, size_t length);

/*!
 * @brief Query cartridge RAM bank count.
 * @param[in] cartridge Constant pointer to cartridge subsystem context
 * @return Cartridge RAM bank count.
 */
size_t dmgl_cartridge_ram_count(const dmgl_cartridge_t *cartridge);

/*!
 * @brief Read byte from cartridge subsystem RAM bank.
 * @param[in] cartridge Constant pointer to cartridge subsystem context
 * @param[in] index RAM bank index
 * @param[in] address Byte address
 * @return Byte value
 */
uint8_t dmgl_cartridge_ram_read(const dmgl_cartridge_t *cartridge, size_t index, uint16_t address);

/*!
 * @brief Write byte to cartridge subsystem RAM bank.
 * @param[in,out] cartridge Pointer to cartridge subsystem context
 * @param[in] index RAM bank index
 * @param[in] address Byte address
 * @param[in] value Byte value
 */
void dmgl_cartridge_ram_write(dmgl_cartridge_t *cartridge, size_t index, uint16_t address, uint8_t value);

/*!
 * @brief Reset cartridge subsystem.
 * @param[in,out] cartridge Pointer to cartridge subsystem context
 */
void dmgl_cartridge_reset(dmgl_cartridge_t *cartridge);

/*!
 * @brief Query cartridge ROM bank count.
 * @param[in] cartridge Constant pointer to cartridge subsystem context
 * @return Cartridge ROM bank count.
 */
size_t dmgl_cartridge_rom_count(const dmgl_cartridge_t *cartridge);

/*!
 * @brief Read byte from cartridge subsystem ROM bank.
 * @param[in] cartridge Constant pointer to cartridge subsystem context
 * @param[in] index ROM bank index
 * @param[in] address Byte address
 * @return Byte value
 */
uint8_t dmgl_cartridge_rom_read(const dmgl_cartridge_t *cartridge, size_t index, uint16_t address);

/*!
 * @brief Query cartridge title string.
 * @param[in] cartridge Constant pointer to cartridge subsystem context
 * @return Constant pointer to cartridge title string
 */
const char *dmgl_cartridge_title(const dmgl_cartridge_t *cartridge);

/*!
 * @brief Query cartridge type.
 * @param[in] cartridge Constant pointer to cartridge subsystem context
 * @return Cartridge type.
 */
dmgl_cartridge_e dmgl_cartridge_type(const dmgl_cartridge_t *cartridge);

/*!
 * @brief Uninitialize cartridge subsystem.
 * @param[in,out] cartridge Pointer to cartridge subsystem context
 */
void dmgl_cartridge_uninitialize(dmgl_cartridge_t *cartridge);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMGL_CARTRIDGE_H_ */
