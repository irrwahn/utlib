/*
 * testsupp.h
 *
 * Copyright 2017 Urban Wallasch <irrwahn35@freenet.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */

#include <stdio.h>

#define MESG(...) do { \
    fprintf( stderr, "%4d %8s (%s:%s:%d) ", id__, "[Info]", __FILE__, __func__, __LINE__ ); \
    fprintf( stderr, __VA_ARGS__ ); fprintf( stderr, "\n" ); \
  } while (0)

#define PASS(...) do { \
    fprintf( stderr, "%4d %8s (%s:%s:%d) ", id__, "[Ok]", __FILE__, __func__, __LINE__ ); \
    fprintf( stderr, __VA_ARGS__ ); fprintf( stderr, "\n" ); \
  } while (0)

#define FAIL(...) do { \
    fprintf( stderr, "%4d %8s (%s:%s:%d) ", id__, "[FAILED]", __FILE__, __func__, __LINE__ ); \
    fprintf( stderr, __VA_ARGS__ ); fprintf( stderr, "\n" ); \
  } while (0)

#undef REGISTER
#define REGISTER(X) int (X)(int id__)

/* EOF */
