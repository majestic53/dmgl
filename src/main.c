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

#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dmgl.h>

static const struct option OPTION[] = {
    { "bootloader", required_argument, NULL, 'b' },
    { "help", no_argument, NULL, 'h' },
    { "version", no_argument, NULL, 'v' },
    { NULL, 0, NULL, 0 },
    };

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static dmgl_error_e read_file(const char *base, const char *path, uint8_t **data, size_t *length)
{
    int file_length;
    FILE *file = NULL;
    dmgl_error_e result = DMGL_SUCCESS;

    if(!(file = fopen(path, "rb"))) {
        fprintf(stderr, "%s: File does not exist -- %s\n", base, path);
        result = DMGL_FAILURE;
        goto exit;
    }

    fseek(file, 0, SEEK_END);

    if((file_length = ftell(file)) <= 0) {
        fprintf(stderr, "%s: File is empty -- %s\n", base, path);
        result = DMGL_FAILURE;
        goto exit;
    }

    fseek(file, 0, SEEK_SET);
    *length = file_length;

    if((*data = calloc(*length, sizeof(uint8_t))) == NULL) {
        fprintf(stderr, "%s: Failed to allocate buffer -- %.2f KB (%zu bytes)\n", base, *length / 1024.f, *length);
        result = DMGL_FAILURE;
        goto exit;
    }

    if(fread(*data, sizeof(uint8_t), *length, file) != *length) {
        fprintf(stderr, "%s: Failed to read file -- %s\n", base, path);
        result = DMGL_FAILURE;
        goto exit;
    }

exit:

    if(file) {
        fclose(file);
    }

    return result;
}

static void show_help(const char *base)
{
    size_t flag = 0;

    fprintf(stdout, "Usage: %s [options] file...\n\n", base);
    fprintf(stdout, "Options:\n");

    while(OPTION[flag].name) {
        char message[22] = {};
        const char *description[] = {
            "Specify bootloader path", "Show help information", "Show version information",
            };

        snprintf(message, sizeof(message), "   -%c, --%s", OPTION[flag].val, OPTION[flag].name);

        for(size_t index = strlen(message); index < sizeof(message); ++index) {
            message[index] = ' ';
        }

        fprintf(stdout, "%s%s\n", message, description[flag]);
        ++flag;
    }
}

static void show_version(void)
{
    const dmgl_version_t *version = dmgl_version();

    fprintf(stdout, "%u.%u-%X\n", version->major, version->minor, version->patch);
}

int main(int argc, char *argv[])
{
    int option, option_index;
    uint8_t *bootloader = NULL, *cartridge = NULL;
    size_t bootloader_length = 0, cartridge_length = 0;
    dmgl_t context = {};
    dmgl_error_e result = DMGL_SUCCESS;

    opterr = 1;

    while((option = getopt_long(argc, argv, "b:hv", OPTION, &option_index)) != -1) {

        switch(option) {
            case 'b':

                if(bootloader) {
                    fprintf(stderr, "%s: Redefined bootloader path -- %s\n", argv[0], optarg);
                    goto exit;
                }

                if((result = read_file(argv[0], optarg, &bootloader, &bootloader_length)) != DMGL_SUCCESS) {
                    goto exit;
                }

                context.bootloader.data = bootloader;
                context.bootloader.length = bootloader_length;
                break;
            case 'h':
                show_help(argv[0]);
                goto exit;
            case 'v':
                show_version();
                goto exit;
            case '?':
            default:
                result = DMGL_FAILURE;
                goto exit;
        }
    }

    for(option = optind; option < argc; ++option) {

        if(cartridge) {
            fprintf(stderr, "%s: Redefined cartridge path -- %s\n", argv[0], argv[option]);
            goto exit;
        }

        if((result = read_file(argv[0], argv[option], &cartridge, &cartridge_length)) != DMGL_SUCCESS) {
            goto exit;
        }

        context.cartridge.data = cartridge;
        context.cartridge.length = cartridge_length;
    }

    if(!cartridge) {
        fprintf(stderr, "%s: Undefined cartridge path\n", argv[0]);
        goto exit;
    }

    if((result = dmgl(&context)) != DMGL_SUCCESS) {
        fprintf(stderr, "%s: %s\n", argv[0], dmgl_error());
        goto exit;
    }

exit:
    free(bootloader);
    free(cartridge);

    return result;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
