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

#ifndef DMGL_H_
#define DMGL_H_

typedef enum {
    DMGL_FAILURE = -1,
    DMGL_SUCCESS,
    DMGL_COMPLETE,
    DMGL_MAX,
} dmgl_error_e;

typedef struct {

    struct {
        void *data;
        int length;
    } bootloader;

    struct {
        void *data;
        int length;
    } cartridge;
} dmgl_t;

typedef struct {
    int major;
    int minor;
    int patch;
} dmgl_version_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

dmgl_error_e dmgl(const dmgl_t *context);

const char *dmgl_error(void);

const dmgl_version_t *dmgl_version(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMGL_H_ */
