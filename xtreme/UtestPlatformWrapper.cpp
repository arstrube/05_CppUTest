/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of icdts contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <string.h>
#include <stdio.h>
#include <time.h>

#define FILE char

#include "UtestPlatformWrapper.h"

static char file_buffer[80];
static tm   tm_stub;

#undef  stdout
#define stdout file_buffer

struct tm* localtime_stub(const time_t*)
{
    return &tm_stub;
}

FILE* fopen_stub (const char*, const char*)
{
    return (FILE*)file_buffer;
}

int fputs_stub (const char* str, FILE* fp)
{
    strcpy(fp, str);
    return 0;
}

int fflush_stub (FILE* fp)
{
    *fp = '\0';
    return 0;
}

int fclose_stub (FILE*)
{
    return 0;
}

char* file_spy()
{
    return file_buffer;
}

#define time(tm)                   0
#define localtime(x)               localtime_stub((x))
#define fopen(fname, flag)         fopen_stub((fname), (flag))
#define fputs(str, fp)             fputs_stub((str), ((fp)))
#define fclose(fp)                 fclose_stub((fp))
#define fflush(fp)                 fflush_stub((fp))

#include "../src/Platforms/Gcc/UtestPlatform.cpp"
