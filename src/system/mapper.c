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

#include <mapper.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

dmgl_error_e dmgl_mapper_initialize(dmgl_mapper_t *mapper, const uint8_t *data, size_t length)
{
    dmgl_error_e result;

    /* TODO: INITIALIZE CARTRIDGE, ALLOCATE MAPPER (MBC) */
    result = DMGL_SUCCESS;
    /* ---- */

//exit:
    return result;
}

uint8_t dmgl_mapper_read(const dmgl_mapper_t *mapper, uint16_t address)
{
    /* TODO: READ FROM MAPPER (MBC) */
    return 0;
    /* ---- */
}

const char *dmgl_mapper_title(const dmgl_mapper_t *mapper)
{
    /* TODO */
    return "DMGL";
    /* ---- */
}

void dmgl_mapper_uninitialize(dmgl_mapper_t *mapper)
{
    /* TODO: FREE MAPPER (MBC), UNINITIALIZE CARTRIDGE */

    memset(mapper, 0, sizeof(*mapper));
}

void dmgl_mapper_write(const dmgl_mapper_t *mapper, uint16_t address, uint8_t value)
{
     /* TODO: WRITE TO MAPPER (MBC) */
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
