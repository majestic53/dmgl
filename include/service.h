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
 * @file service.h
 * @brief Service interface.
 */

#ifndef DMGL_SERVICE_H_
#define DMGL_SERVICE_H_

#include <common.h>

/*!
 * @enum dmgl_button_e
 * @brief DMGL button types.
 */
typedef enum {
    DMGL_BUTTON_A = 0,      /*!< A button type */
    DMGL_BUTTON_B,          /*!< B button type */
    DMGL_BUTTON_START,      /*!< Start button type */
    DMGL_BUTTON_SELECT,     /*!< Select button type */
    DMGL_BUTTON_RIGHT,      /*!< Right button type */
    DMGL_BUTTON_LEFT,       /*!< Left button type */
    DMGL_BUTTON_UP,         /*!< Up button type */
    DMGL_BUTTON_DOWN,       /*!< Down button type */
    DMGL_BUTTON_MAX,        /*!< Max button type */
} dmgl_button_e;

/*!
 * @enum dmgl_color_e
 * @brief DMGL color types.
 */
typedef enum {
    DMGL_COLOR_BLACK = 0,   /*!< Black color type */
    DMGL_COLOR_GREY_DARK,   /*!< Dark-grey color type */
    DMGL_COLOR_GREY_LIGHT,  /*!< Light-grey color type */
    DMGL_COLOR_WHITE,       /*!< Write color type */
    DMGL_COLOR_MAX,         /*!< Max color type */
} dmgl_color_e;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Query service interface button state.
 * @param[in] button Button type
 * @return Button state (true=pressed, false=released)
 */
bool dmgl_service_button(dmgl_button_e button);

/*!
 * @brief Initialize service interface.
 * @param[in] context Constant pointer to DMGL context
 * @param[in] title Constant pointer to service title string
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
dmgl_error_e dmgl_service_initialize(const dmgl_t *context, const char *title);

/*!
 * @brief Set service interface pixel.
 * @param[in] color Pixel color
 * @param[in] x Pixel x-coordinate
 * @param[in] y Pixel y-coordinate
 */
void dmgl_service_pixel(dmgl_color_e color, uint8_t x, uint8_t y);

/*!
 * @brief Poll service interface.
 * @return DMGL_SUCCESS or DMGL_COMPLETE on success, DMGL_FAILURE otherwise
 */
dmgl_error_e dmgl_service_poll(void);

/*!
 * @brief Sync service interface.
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
dmgl_error_e dmgl_service_sync(void);

/*!
 * @brief Uninitialize service interface.
 */
void dmgl_service_uninitialize(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMGL_SERVICE_H_ */
