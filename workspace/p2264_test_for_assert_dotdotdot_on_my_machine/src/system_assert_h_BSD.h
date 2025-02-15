// THIS FILE IS MODIFIED for P2264 (WG21) N2621 & successors (WG14)
// original file modified and tested on MacOS, but OK, because BSD license
// Peter Sommerlad.


/*-
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)assert.h	8.2 (Berkeley) 1/21/94
 * $FreeBSD: src/include/assert.h,v 1.4 2002/03/23 17:24:53 imp Exp $
 */

#include <sys/cdefs.h>
#ifdef __cplusplus
#include <stdlib.h>
#endif /* __cplusplus */

/*
 * Unlike other ANSI header files, <assert.h> may usefully be included
 * multiple times, with and without NDEBUG defined.
 */

#undef assert
#undef __assert

#ifdef NDEBUG
#define	assert(...)	((void)0)
#else

#ifndef CHECK_SINGLE_ASSERT_ARGUMENT_PASSED_TO_ASSERT_DEFINED
#define CHECK_SINGLE_ASSERT_ARGUMENT_PASSED_TO_ASSERT_DEFINED
#ifdef __cplusplus
#else
// should better use (_Bool){__VA_ARGS__}
//inline int __check_single_argument_passed_to_assert(int b) { return b; }
/* need to have one extern declaration of inline function introduced
// to prevent linker errors. did not patch my libc for that.
*/
//extern int __check_single_argument_passed_to_assert(int b);

#endif
#endif /* CHECK_SINGLE_ASSERT_ARGUMENT_PASSED_TO_ASSERT_DEFINED */

#ifdef __cplusplus
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wexcess-initializers"
#endif


#ifndef __GNUC__

__BEGIN_DECLS
#ifndef __cplusplus
void abort(void) __dead2;
#endif /* !__cplusplus */
int  printf(const char * __restrict, ...);
__END_DECLS

#ifdef __cplusplus
#define assert(...)  \
    ((void) (bool( __VA_ARGS__) ? ((void)sizeof(bool(__VA_ARGS__))) : __assert (#__VA_ARGS__, __FILE__, __LINE__)))
#else
#define assert(...)  \
		_Pragma("GCC diagnostic push")\
		_Pragma("GCC diagnostic error \"-Wexcess-initializers\"")\
    ((void) ((_Bool){ __VA_ARGS__} ? ((void)0) : __assert (#__VA_ARGS__, __FILE__, __LINE__)))\
        _Pragma("GCC diagnostic pop")
#endif

#define __assert(e, file, line) \
    ((void)printf ("%s:%u: failed assertion `%s'\n", file, line, e), abort())

#else /* __GNUC__ */

__BEGIN_DECLS
void __assert_rtn(const char *, const char *, int, const char *) __dead2 __disable_tail_calls;
#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && ((__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__-0) < 1070)
void __eprintf(const char *, const char *, unsigned, const char *) __dead2;
#endif
__END_DECLS

#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && ((__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__-0) < 1070)
#define __assert(e, file, line) \
    __eprintf ("%s:%u: failed assertion `%s'\n", file, line, e)
#else
/* 8462256: modified __assert_rtn() replaces deprecated __eprintf() */
#define __assert(e, file, line) \
    __assert_rtn ((const char *)-1L, file, line, e)
#endif

#if __DARWIN_UNIX03
#ifdef __cplusplus
#define	assert(...) \
    (__builtin_expect(!bool( __VA_ARGS__), 0) ? __assert_rtn(__func__, __FILE__, __LINE__, #__VA_ARGS__) : (void)sizeof(bool(__VA_ARGS__)))
#else
#define	assert(...) \
	_Pragma("GCC diagnostic push")\
	_Pragma("GCC diagnostic error \"-Wexcess-initializers\"")\
    ((void)(__builtin_expect(!(_Bool){ __VA_ARGS__}, 0) ? __assert_rtn(__func__, __FILE__, __LINE__, #__VA_ARGS__) : (void)0))\
	_Pragma("GCC diagnostic pop")
#endif
#else /* !__DARWIN_UNIX03 */
#ifdef __cplusplus
#define assert(...)  \
    (__builtin_expect(!bool(__VA_ARGS__), 0) ? __assert (#__VA_ARGS__, __FILE__, __LINE__) : (void)sizeof(bool(__VA_ARGS__)))
#else
#define assert(...)  \
		_Pragma("GCC diagnostic push")\
		_Pragma("GCC diagnostic error \"-Wexcess-initializers\"")\
    ((void)(__builtin_expect(!(_Bool){__VA_ARGS__}, 0) ? __assert (#__VA_ARGS__, __FILE__, __LINE__) : (void)0))\
    _Pragma("GCC diagnostic pop")
#endif
#endif /* __DARWIN_UNIX03 */

#endif /* __GNUC__ */
#ifdef __cplusplus
#else
// not sure if that works wrt macro expansions....
// It doesn't, because we cannot enable the #pragma locally for a macro expansion.
//#pragma GCC diagnostic pop
#endif

#endif /* NDEBUG */

#ifndef _ASSERT_H_
#define _ASSERT_H_

#ifndef __cplusplus
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define static_assert _Static_assert
#endif /* __STDC_VERSION__ */
#endif /* !__cplusplus */

#endif /* _ASSERT_H_ */
