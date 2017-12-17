/**************************************************
 *       Perl-Compatible Regular Expressions      *
 **************************************************
---------------------------------------------------------------
Redistribution and use in source and binary forms, with or
without modification, are permitted provided that the following
conditions are met:

 1. Redistributions of source code must retain the above
 copyright notice, this list of conditions and the following
 disclaimer.

 2. Redistributions in binary form must reproduce the above
 copyright notice, this list of conditions and the following
 disclaimer in the documentation and/or other materials
 provided with the distribution.

 3. Neither the name of the University of Cambridge nor the
 names of its contributors may be used to endorse or promote
 products derived from this software without specific prior
 written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------*/
/* This header file is to be used by client C programs that want to use
 the traditional POSIX compliant CALL sequences, yet to get PCRE2
 functionality.  Non-C programs that want to achieve the same effect
 may either call the PCRE2 routine names directly or create an API
 module that wrap the PCRE2 names with the traditional namea. Be aware
 though that the latter solution may cause undue conflicts in 'bind'
 time.
 I've decided to change the POSIX API compliant PCRE2 functions to
 prevent such conflict, but by doing so I eliminated the simple
 traditional call sequnce.  This header restores the simple traditional
 call sequence for C programs, but the API for other languages is not
 yet available and is in low priority.
 I will try to provide such an API and full, less confusing
 documentation of the issue in a subsequent release of PCRE2 for z/OS
 This is the second version attempt.  It provides both the #pragma map
 directives and the decision whether to use them (and the accompanied
 headers, or the posix standard header, in one package.  It replaces 
 the PC2PSXPG.h and makes it obsolete.
 */
#ifdef COMPILE_WITH_PCRE2
/* This header contains a few pragma statements to make the compiler
 use the PCRE2 equivalents of the traditional POSIX functions
 PC2PSXAP.h replaced the obsolete PC2PSXPG.h */
#include "pcre2.h"
#include "posixh.h"
#pragma map (regfree, "PREGFREE")
#pragma map (regerror, "PREGERR")
#pragma map (regexec, "PREGEXEC")
#pragma map (regcomp, "PREGCOMP")
static unsigned char COMPILE_OPTION[] = "COMPILE WITH PCRE2";
#else
static unsigned char COMPILE_OPTION[] = "COMPILE WITH POSIX";
#include <regex.h>
#endif