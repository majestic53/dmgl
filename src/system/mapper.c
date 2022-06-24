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
 * @file mapper.c
 * @brief Mapper subsystem.
 */

#include <mbc0.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t dmgl_mapper_checksum(const dmgl_mapper_t *mapper)
{
    return dmgl_cartridge_checksum(&mapper->cartridge);
}

dmgl_error_e dmgl_mapper_initialize(dmgl_mapper_t *mapper, const uint8_t *data, size_t length)
{
    dmgl_error_e result;
    dmgl_cartridge_e type;
    const dmgl_mapper_handler_t handler[] = {
        { dmgl_mbc0_initialize, dmgl_mbc0_read, dmgl_mbc0_reset, dmgl_mbc0_uninitialize, dmgl_mbc0_write, },
        };

    if((result = dmgl_cartridge_initialize(&mapper->cartridge, data, length)) != DMGL_SUCCESS) {
        goto exit;
    }

    if((result = handler[type = dmgl_cartridge_type(&mapper->cartridge)].initialize(&mapper->cartridge, &mapper->context)) != DMGL_SUCCESS) {
        goto exit;
    }

    memcpy(&mapper->handler, &handler[type], sizeof(handler[type]));

exit:
    return result;
}

uint8_t dmgl_mapper_read(const dmgl_mapper_t *mapper, uint16_t address)
{
    return mapper->handler.read(&mapper->cartridge, mapper->context, address);
}

void dmgl_mapper_reset(dmgl_mapper_t *mapper)
{
    dmgl_cartridge_reset(&mapper->cartridge);
    mapper->handler.reset(mapper->context);
}

const char *dmgl_mapper_title(const dmgl_mapper_t *mapper)
{
    return dmgl_cartridge_title(&mapper->cartridge);
}

void dmgl_mapper_uninitialize(dmgl_mapper_t *mapper)
{

    if(mapper->handler.uninitialize) {
        mapper->handler.uninitialize(mapper->context);
    }

    dmgl_cartridge_uninitialize(&mapper->cartridge);
    memset(mapper, 0, sizeof(*mapper));
}

void dmgl_mapper_write(dmgl_mapper_t *mapper, uint16_t address, uint8_t value)
{
    mapper->handler.write(&mapper->cartridge, mapper->context, address, value);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
