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

#ifndef DMGL_TEST_H_
#define DMGL_TEST_H_

#include <assert.h>
#include <mock.h>

#define DMGL_TEST_RESULT(_RESULT_) \
    fprintf(((_RESULT_) != DMGL_SUCCESS) ? stderr : stdout, "[%s%s%s] %s\n", \
        ((_RESULT_) != DMGL_SUCCESS) ? "\x1b[91m" : "\x1b[92m", \
        ((_RESULT_) != DMGL_SUCCESS) ? "FAIL" : "PASS", \
        "\x1b[0m", __FUNCTION__)

typedef dmgl_error_e (*dmgl_test_cb)(void);

#endif /* DMGL_TEST_H_ */
