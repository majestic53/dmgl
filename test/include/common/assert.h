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
 * @file assert.h
 * @brief Common test assert.
 */

#ifndef DMGL_ASSERT_H_
#define DMGL_ASSERT_H_

/*!
 * @brief Assert on conditional macro.
 * @param[in] _CONDITION_ Constant pointer to conditional string
 * @return DMGL_SUCCESS on success, DMGL_FAILURE otherwise
 */
#define DMGL_ASSERT(_CONDITION_) \
    (_CONDITION_) ? DMGL_SUCCESS : dmgl_assert(# _CONDITION_, __FILE__, __FUNCTION__, __LINE__)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Assert on conditional.
 * @param[in] condition Constant pointer to conditional string
 * @param[in] file Constant pointer to file string
 * @param[in] function Constant pointer to function string
 * @param[in] line File line
 * @return DMGL_FAILURE
 */
dmgl_error_e dmgl_assert(const char *condition, const char *file, const char *function, size_t line)
{
    fprintf(stderr, "Assert failed -- %s (%s:%s@%zu)\n", condition, function, file, line);

    return DMGL_FAILURE;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMGL_ASSERT_H_ */
