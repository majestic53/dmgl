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
 * @file dmgl.h
 * @brief DMGL interface.
 */

#ifndef DMGL_H_
#define DMGL_H_

/*!
 * @enum dmgl_error_e
 * @brief DMGL error types.
 */
typedef enum {
    DMGL_FAILURE = -1,  /*!< Failure, query dmgl_error for details */
    DMGL_SUCCESS,       /*!< Success */
    DMGL_COMPLETE,      /*!< Internal type */
    DMGL_MAX,           /*!< Max error type */
} dmgl_error_e;

/*!
 * @struct dmgl_t
 * @brief DMGL context.
 */
typedef struct {

    struct {
        void *data;     /*!< Bootloader data */
        int length;     /*!< Bootloader data length, in bytes */
    } bootloader;       /*!< Bootloader context */

    struct {
        void *data;     /*!< Cartridge data */
        int length;     /*!< Cartridge data length, in bytes */
    } cartridge;        /*!< Cartridge context */

    struct {
        int scale;      /*!< Window scale [1x-8x] */
    } window;           /*!< Window context */
} dmgl_t;

/*!
 * @struct dmgl_version_t
 * @brief DMGL version context.
 */
typedef struct {
    int major;          /*!< Major version */
    int minor;          /*!< Minor version */
    int patch;          /*!< Patch version */
} dmgl_version_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Run DMGL with context.
 * @param[in] context Constant pointer to DMGL context
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
dmgl_error_e dmgl(const dmgl_t *context);

/*!
 * @brief Query DMGL error string.
 * @return Constant pointer to DMGL error string
 */
const char *dmgl_error(void);

/*!
 * @brief Query DMGL version.
 * @return Constant pointer to DMGL version
 */
const dmgl_version_t *dmgl_version(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMGL_H_ */
