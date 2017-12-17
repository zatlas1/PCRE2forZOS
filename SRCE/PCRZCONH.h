
/*
THE z/OS specific functionality
-------------------------------

Contributed by:   Ze'ev Atlas.

Copyright (c) 2012, Ze'ev Atlas.
All rights reserved.


THE "BSD" LICENCE
-----------------

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of the University of Cambridge nor the name of Google
      Inc. nor the names of their contributors may be used to endorse or
      promote products derived from this software without specific prior
      written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.                                          */
/****************************************************************************
* This section  provides means to handle the EBCDIC horror within the       *
* context of PCRE2 and its REXX interface.  Read more about the problem and *
* solution in the PCRE2 Native z/OS Port documentation.                     *
****************************************************************************/

#include "langinfo.h"
#include "locale.h"
#include <iconv.h>
#include "interna2.h"
#include "pcre2.h"

#ifndef CHAR_NULL
#define CHAR_NULL 0x00
#endif
#define PCRZ_NULL_TERMINATED -1
#define PCRZ_SPACE_TERMINATED -2
#define PCRZ_LENGTH_TERMINATED -3

#define PCRZ_MAX_PATTERN_SIZE 2048
#define PCRZ_MAX_SUBJECT_SIZE 8192
#define PCRZ_MAX_CODESET_SIZE 16


typedef struct ZCSSTRCT
{
    unsigned char charset_locale [16];
    iconv_t cd; /* conversion descriptor */
    size_t pattern_size; /* Max size converted pattern */
    size_t subject_size; /* Max size converted subject */
    char* pattern_1047; /* pointer to allocated space for conveted pattern */
    char* subject_1047; /* pointer to allocated space for conveted subject */
}ZCSSTRCT;
