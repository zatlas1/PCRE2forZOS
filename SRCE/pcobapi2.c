/*
THE z/OS specific functionality
-------------------------------

Contributed by:   Ze'ev Atlas.

Copyright (c) 2015-2017 Ze'ev Atlas.
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
* The PCOBAPI2 provides old style z/OS COBOL with the ability to call       *
* traditional z/OS short (8 characters) names.  In this first version I     *
* converted the API man page to simple C program and created short names    *
* manually.  Subsequent conversion scripts might generate this program      *
* automatically from my database.                            ZA 07/19/2015  *
*                                                                           *
* I also provide here some old COBOL oriented string manipulation and       *
* memory allocation functions                                ZA 07/19/2015  *
*                                                                           *
* Basically, this is an adjusted version of the PCOBAPI as appeared in the  *
* ported old PCRE (still available) - adjusted to PCRE2      ZA 07/19/2015  *
****************************************************************************/
/*PCRE2API(3)
NAME
       PCRE2 - Perl-compatible regular expressions (revised API)

/*PCRE 16-BIT API, PCRE 32-BIT API as well as ASCII are specifically not
         supported.  The z/OS port is meant to support native EBCDIC
         environment only!    */

#ifdef HAVE_CONFIG_H
#include "CONFIG.h"
#endif
#include <stdlib.h>
#include "posixh.h"
#include "pcre2.h"
#include "pcrzconh.h"

     /*PCRE2  is  a  new API for PCRE. This program provides API between LE
       languages and its functions. See the pcre2 document for an overview
       of  all  the PCRE2 documentation.*/
/*PCRE2_GENERAL_INFO_FUNCTIONS*/
extern     int PCRE2_CALL_CONVENTION CONFIG2(uint32_t, void *);

/* Functions for manipulating contexts. */

/*PCRE2_GENERAL_CONTEXT_FUNCTIONS*/
extern    pcre2_general_context PCRE2_CALL_CONVENTION
  *GENCNTCP(pcre2_general_context *);
extern     pcre2_general_context PCRE2_CALL_CONVENTION
  *GENCNTCR(void *(*)(PCRE2_SIZE, void *),
    void (*)(void *, void *), void *);
extern     void PCRE2_CALL_CONVENTION
  GENCNTFR(pcre2_general_context *);

/*PCRE2_COMPILE_CONTEXT_FUNCTIONS*/
extern     pcre2_compile_context PCRE2_CALL_CONVENTION
  *CMPCNTCP(pcre2_compile_context *);
extern     pcre2_compile_context PCRE2_CALL_CONVENTION
  *CMPCNTCR(pcre2_general_context *);
extern     void PCRE2_CALL_CONVENTION
  CMPCNTFR(pcre2_compile_context *);
extern     int PCRE2_CALL_CONVENTION
  SETBSR(pcre2_compile_context *, uint32_t);
extern     int PCRE2_CALL_CONVENTION
  SETCHRTB(pcre2_compile_context *, const unsigned char *);
extern     int PCRE2_CALL_CONVENTION
  SETCEXOP(pcre2_compile_context *, uint32_t);
extern     int PCRE2_CALL_CONVENTION
  SETMXPLN(pcre2_compile_context *, PCRE2_SIZE);
extern     int PCRE2_CALL_CONVENTION
  SETNEWLN(pcre2_compile_context *, uint32_t);
extern     int PCRE2_CALL_CONVENTION
  SETPRNLM(pcre2_compile_context *, uint32_t);
extern     int PCRE2_CALL_CONVENTION
  SETCMRCG(pcre2_compile_context *,
    int (*)(uint32_t, void *), void *);

/*PCRE2_MATCH_CONTEXT_FUNCTIONS*/
extern     pcre2_match_context PCRE2_CALL_CONVENTION
  *MTCCNTCP(pcre2_match_context *);
extern     pcre2_match_context PCRE2_CALL_CONVENTION
  *MTCCNTCR(pcre2_general_context *);
extern     void PCRE2_CALL_CONVENTION
  MTCCNTFR(pcre2_match_context *);
extern     int PCRE2_CALL_CONVENTION
  SETCALOT(pcre2_match_context *,
    int (*)(pcre2_callout_block *, void *), void *);
extern     int PCRE2_CALL_CONVENTION
  SETDEPLM(pcre2_match_context *, uint32_t);
extern     int PCRE2_CALL_CONVENTION
  SETHEPLM(pcre2_match_context *, uint32_t);
extern     int PCRE2_CALL_CONVENTION
  SETMTCLM(pcre2_match_context *, uint32_t);
extern     int PCRE2_CALL_CONVENTION
  SETOFFLM(pcre2_match_context *, PCRE2_SIZE);
extern     int PCRE2_CALL_CONVENTION
  SETRCNLM(pcre2_match_context *, uint32_t);
extern     int PCRE2_CALL_CONVENTION
  SETRCNMM(pcre2_match_context *,
    void *(*)(PCRE2_SIZE, void *), void (*)(void *, void *), void *);

/*PCRE2_CONVERT_CONTEXT_FUNCTIONS */
extern     pcre2_convert_context PCRE2_CALL_CONVENTION
  *CNVCTXCP(pcre2_convert_context *);
extern     pcre2_convert_context PCRE2_CALL_CONVENTION
  *CNVCTXCR(pcre2_general_context *);
extern     void PCRE2_CALL_CONVENTION
  CNVCTXFR(pcre2_convert_context *);
extern     int PCRE2_CALL_CONVENTION
  SETGLBES(pcre2_convert_context *, uint32_t);
extern     int PCRE2_CALL_CONVENTION
  SETGLBSP(pcre2_convert_context *, uint32_t);

/* Functions concerned with compiling a pattern to PCRE internal code. */

/*PCRE2_COMPILE_FUNCTIONS*/
extern     pcre2_code PCRE2_CALL_CONVENTION
  *COMPILE2(PCRE2_SPTR, PCRE2_SIZE, uint32_t, int *, PCRE2_SIZE *,
    pcre2_compile_context *);
extern     void PCRE2_CALL_CONVENTION
  CODEFREE(pcre2_code *);
extern     pcre2_code PCRE2_CALL_CONVENTION
  *CODECOPY(const pcre2_code *);
extern     pcre2_code PCRE2_CALL_CONVENTION
  *CODECOPT(const pcre2_code *);

/* Functions that give information about a compiled pattern. */

/*PCRE2_PATTERN_INFO_FUNCTIONS*/
extern     int PCRE2_CALL_CONVENTION
  PATTERN2(const pcre2_code *, uint32_t, void *);
extern     int PCRE2_CALL_CONVENTION
  CALOTENM(const pcre2_code *,
    int (*)(pcre2_callout_enumerate_block *, void *), void *);

/* Functions for running a match and inspecting the result. */

/*PCRE2_MATCH_FUNCTIONS*/
extern     pcre2_match_data PCRE2_CALL_CONVENTION
  *MATCHDCR(uint32_t, pcre2_general_context *);
extern     pcre2_match_data PCRE2_CALL_CONVENTION
  *MATCDCFP(const pcre2_code *,
    pcre2_general_context *);
extern     int PCRE2_CALL_CONVENTION
  DFAMATC2(const pcre2_code *, PCRE2_SPTR, PCRE2_SIZE, PCRE2_SIZE,
    uint32_t, pcre2_match_data *, pcre2_match_context *, int *, PCRE2_SIZE);
extern     int PCRE2_CALL_CONVENTION
  MATCH2(const pcre2_code *, PCRE2_SPTR, PCRE2_SIZE, PCRE2_SIZE,
    uint32_t, pcre2_match_data *, pcre2_match_context *);
extern     void PCRE2_CALL_CONVENTION
  pcre2_match_data_free(pcre2_match_data *);
extern     PCRE2_SPTR PCRE2_CALL_CONVENTION
  GETMARK(pcre2_match_data *);
extern     uint32_t PCRE2_CALL_CONVENTION
  GETOVECN(pcre2_match_data *);
extern     PCRE2_SIZE PCRE2_CALL_CONVENTION
  *GETOVEPT(pcre2_match_data *);
extern     PCRE2_SIZE PCRE2_CALL_CONVENTION
  GETSTART(pcre2_match_data *);

/* Convenience functions for handling matched substrings. */

/*PCRE2_SUBSTRING_FUNCTIONS*/
extern     int PCRE2_CALL_CONVENTION
  SUBCPNAM(pcre2_match_data *, PCRE2_SPTR, PCRE2_UCHAR *,
    PCRE2_SIZE *);
extern     int PCRE2_CALL_CONVENTION
  SUBCPNUM(pcre2_match_data *, uint32_t, PCRE2_UCHAR *,
    PCRE2_SIZE *);
extern     void PCRE2_CALL_CONVENTION
  SUBSTFR(PCRE2_UCHAR *);
extern     int PCRE2_CALL_CONVENTION
  SUBGTNAM(pcre2_match_data *, PCRE2_SPTR, PCRE2_UCHAR **,
    PCRE2_SIZE *);
extern     int PCRE2_CALL_CONVENTION
  SUBGTNUM(pcre2_match_data *, uint32_t, PCRE2_UCHAR **,
    PCRE2_SIZE *);
extern     int PCRE2_CALL_CONVENTION
  SUBLNNAM(pcre2_match_data *, PCRE2_SPTR, PCRE2_SIZE *);
extern     int PCRE2_CALL_CONVENTION
  SUBLNNUM(pcre2_match_data *, uint32_t, PCRE2_SIZE *);
extern     int PCRE2_CALL_CONVENTION
  SUBNMSCN(const pcre2_code *, PCRE2_SPTR, PCRE2_SPTR *,
    PCRE2_SPTR *);
extern     int PCRE2_CALL_CONVENTION
  SUBNMFNM(const pcre2_code *, PCRE2_SPTR);
extern     void PCRE2_CALL_CONVENTION
  SUBLSTFR(PCRE2_SPTR *);
extern     int PCRE2_CALL_CONVENTION
  SUBLSTGT(pcre2_match_data *, PCRE2_UCHAR ***, PCRE2_SIZE **);

/* Functions for serializing / deserializing compiled patterns. */

/*PCRE2_SERIALIZE_FUNCTIONS*/
extern     int32_t PCRE2_CALL_CONVENTION
  SERIAENC(const pcre2_code **, int32_t, uint8_t **,
    PCRE2_SIZE *, pcre2_general_context *);
extern     int32_t PCRE2_CALL_CONVENTION
  SERIADCD(pcre2_code **, int32_t, const uint8_t *,
    pcre2_general_context *);
extern     int32_t PCRE2_CALL_CONVENTION
  SERGTNMC(const uint8_t *);
extern     void PCRE2_CALL_CONVENTION
  SERIAFR(uint8_t *);

/* Convenience function for match + substitute. */

/*PCRE2_SUBSTITUTE_FUNCTION*/
extern     int PCRE2_CALL_CONVENTION
  SUBSTIT2(const pcre2_code *, PCRE2_SPTR, PCRE2_SIZE, PCRE2_SIZE,
    uint32_t, pcre2_match_data *, pcre2_match_context *, PCRE2_SPTR,
    PCRE2_SIZE, PCRE2_UCHAR *, PCRE2_SIZE *);


/* Functions for converting pattern source strings. */

/* PCRE2_CONVERT_FUNCTIONS */
extern     int PCRE2_CALL_CONVENTION
  PATTCONV(PCRE2_SPTR, PCRE2_SIZE, uint32_t, PCRE2_UCHAR **, \
    PCRE2_SIZE *, pcre2_convert_context *); \
extern     void PCRE2_CALL_CONVENTION
  CNVPATFR(PCRE2_UCHAR *);


/* Functions for JIT processing */

/*PCRE2_JIT_FUNCTIONS*/
extern     int PCRE2_CALL_CONVENTION
  JITCOMP2(pcre2_code *, uint32_t);
extern     int PCRE2_CALL_CONVENTION
  JITMATC2(const pcre2_code *, PCRE2_SPTR, PCRE2_SIZE, PCRE2_SIZE,
    uint32_t, pcre2_match_data *, pcre2_match_context *);
/*
extern     void PCRE2_CALL_CONVENTION
  pcre2_jit_free_unused_memory(pcre2_general_context *);
extern     pcre2_jit_stack PCRE2_CALL_CONVENTION
  *pcre2_jit_stack_create(PCRE2_SIZE, PCRE2_SIZE, pcre2_general_context *);
extern     void PCRE2_CALL_CONVENTION
  pcre2_jit_stack_assign(pcre2_match_context *, pcre2_jit_callback, void *);
extern     void PCRE2_CALL_CONVENTION
  pcre2_jit_stack_free(pcre2_jit_stack *);
*/


/* Other miscellaneous functions. */

/*PCRE2_OTHER_FUNCTIONS*/
extern     int PCRE2_CALL_CONVENTION
  GETERROR(int, PCRE2_UCHAR *, PCRE2_SIZE);
extern     const uint8_t PCRE2_CALL_CONVENTION
  *MAKETAB2(pcre2_general_context *);

extern    void FREEMAIN (void* ptr);

extern    void * GETMAIN (size_t size);

extern    int ZWHTTRM (char * str, int maxlen);

extern    int ZISSPTRM (char * str, int maxlen);

extern    int ZISNLTRM (char * str, int maxlen);

extern    int ZSPC2NUL (char * str, int maxlen);

extern    int ZNUL2SPC (char * str, int maxlen);

/*  pcrz_codeset_init <=> ZCSINIT             */
extern int ZCSINIT (ZCSSTRCT ** handle_ptr,
                      char * codeset_name,
                      size_t pattern_size,
                      size_t subject_size);
/*  pcrz_codeset_convert_pattern <=> ZCSCONVP */
extern int ZCSCONVP (ZCSSTRCT *zcsstrct_ptr,
         char * input_pattern, size_t pattern_length,
         char ** output_pattern, size_t ** out_pattern_length);
/*  pcrz_codeset_convert_subject <=> ZCSCONVS */
extern int ZCSCONVS (ZCSSTRCT *zcsstrct_ptr,
         char * input_subject, size_t subject_length,
         char ** output_subject, size_t ** out_subject_length);
/*  pcrz_codeset_release <=> ZCSRLSE          */
extern void ZCSRLSE (ZCSSTRCT *zcsstrct_ptr);

extern int PREGCOMP(regex_t *, const char *, int);

extern int PREGEXEC(const regex_t *, const char *, size_t,
                     regmatch_t *, int);

extern size_t PREGERR(int, const regex_t *, char *, size_t);

extern void PREGFREE(regex_t *);

/*PCRE2 NATIVE API BASIC FUNCTIONS*/

/*        =>COMPILE2 */
pcre2_code * PCRE2_CALL_CONVENTION
COMPILE2(PCRE2_SPTR pattern, PCRE2_SIZE patlen, uint32_t options,
   int *errorptr, PCRE2_SIZE *erroroffset, pcre2_compile_context *ccontext)
{
    return pcre2_compile(pattern, patlen,
         options, errorptr, erroroffset,
         ccontext);
}

/*       = part of COMPILE2 - supported as is via PGMNAME(LONGMIXED) */
void PCRE2_CALL_CONVENTION
CODEFREE(pcre2_code *code)
{
    return pcre2_code_free(code);
}

/*       = part of COMPILE2 - supported as is via PGMNAME(LONGMIXED) */
pcre2_code * PCRE2_CALL_CONVENTION
CODECOPY(const pcre2_code *code)
{
    return pcre2_code_copy(code);
}

/*       = part of COMPILE2 - supported as is via PGMNAME(LONGMIXED) */
pcre2_code * PCRE2_CALL_CONVENTION
CODECOPT(const pcre2_code *code)
{
    return pcre2_code_copy_with_tables(code);
}

/*       = part of MATCHDA2 - supported as is via PGMNAME(LONGMIXED) */
pcre2_match_data * PCRE2_CALL_CONVENTION
MATCHDCR(uint32_t oveccount, pcre2_general_context *gcontext)
{
    return pcre2_match_data_create(oveccount,
        gcontext);
}

/*       = part of MATCHDA2 - supported as is via PGMNAME(LONGMIXED) */
pcre2_match_data * PCRE2_CALL_CONVENTION
MATCDCFP(const pcre2_code *code,
  pcre2_general_context *gcontext)
{
    return pcre2_match_data_create_from_pattern(code,
        gcontext);
}

/*        =>MATCH2  */
int PCRE2_CALL_CONVENTION
MATCH2(const pcre2_code *code, PCRE2_SPTR subject, PCRE2_SIZE length,
  PCRE2_SIZE start_offset, uint32_t options, pcre2_match_data *match_data,
  pcre2_match_context *mcontext)
{
    return pcre2_match(code, subject,
         length, start_offset,
         options, match_data,
         mcontext);
}

/*        =>DFAMATC2  */
int PCRE2_CALL_CONVENTION
DFAMATC2(const pcre2_code *code, PCRE2_SPTR subject, PCRE2_SIZE length,
  PCRE2_SIZE start_offset, uint32_t options, pcre2_match_data *match_data,
  pcre2_match_context *mcontext, int *workspace, PCRE2_SIZE wscount)
{
    return pcre2_dfa_match(code, subject,
         length, start_offset,
         options, match_data,
         mcontext,
         workspace, wscount);
}

/*       = part of MATCHDA2 - supported as is via PGMNAME(LONGMIXED) */
void PCRE2_CALL_CONVENTION
MATCHDFR(pcre2_match_data *match_data)
{
    return pcre2_match_data_free(match_data);
}

/*PCRE2 NATIVE API AUXILIARY MATCH FUNCTIONS*/

/*       = part of MATCHDA2 - supported as is via PGMNAME(LONGMIXED) */
PCRE2_SPTR PCRE2_CALL_CONVENTION
GETMARK(pcre2_match_data *match_data)
{
    return pcre2_get_mark(match_data);
}

/*       = part of MATCHDA2 - supported as is via PGMNAME(LONGMIXED) */
uint32_t PCRE2_CALL_CONVENTION
GETOVECN(pcre2_match_data *match_data)
{
    return pcre2_get_ovector_count(match_data);
}

/*       = part of MATCHDA2 - supported as is via PGMNAME(LONGMIXED) */
PCRE2_SIZE * PCRE2_CALL_CONVENTION
GETOVEPT(pcre2_match_data *match_data)
{
    return pcre2_get_ovector_pointer(match_data);
}

/*       = part of MATCHDA2 - supported as is via PGMNAME(LONGMIXED) */
PCRE2_SIZE PCRE2_CALL_CONVENTION
GETSTART(pcre2_match_data *match_data)
{
    return pcre2_get_startchar(match_data);
}

/*PCRE2 NATIVE API GENERAL CONTEXT FUNCTIONS*/

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
pcre2_general_context * PCRE2_CALL_CONVENTION
GENCNTCR(void *(*private_malloc)(size_t, void *),
void (*private_free)(void *, void *), void *memory_data)
{
    return pcre2_general_context_create(
         private_malloc,
         private_free, memory_data);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
pcre2_general_context * PCRE2_CALL_CONVENTION
GENCNTCP(pcre2_general_context *gcontext)
{
    return pcre2_general_context_copy(
         gcontext);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
void PCRE2_CALL_CONVENTION
GENCNTFR(pcre2_general_context *gcontext)
{
    return pcre2_general_context_free(gcontext);
}

/*PCRE2 NATIVE API COMPILE CONTEXT FUNCTIONS*/

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
pcre2_compile_context * PCRE2_CALL_CONVENTION
CMPCNTCR(
         pcre2_general_context *gcontext)
{
    return pcre2_compile_context_create(
         gcontext);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
pcre2_compile_context * PCRE2_CALL_CONVENTION
CMPCNTCP(pcre2_compile_context *ccontext)
{
    return pcre2_compile_context_copy(
         ccontext);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
void PCRE2_CALL_CONVENTION
CMPCNTFR(pcre2_compile_context *ccontext)
{
    return pcre2_compile_context_free(ccontext);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SETBSR(pcre2_compile_context *ccontext, uint32_t value)
{
    return pcre2_set_bsr(ccontext,
         value);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SETMXPLN(pcre2_compile_context *ccontext, PCRE2_SIZE length)
{
    return pcre2_set_max_pattern_length(ccontext,
  length);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SETCHRTB(pcre2_compile_context *ccontext,
const unsigned char *tables)
{
    return pcre2_set_character_tables(ccontext,
         tables);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SETNEWLN(pcre2_compile_context *ccontext, uint32_t newline)
{
    return pcre2_set_newline(ccontext,
         newline);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SETPRNLM(pcre2_compile_context *ccontext, uint32_t limit)
{
    return pcre2_set_parens_nest_limit(ccontext,
         limit);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SETCEXOP(pcre2_compile_context *ccontext, uint32_t options)
{
    return pcre2_set_compile_extra_options(ccontext, options);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SETCMRCG(pcre2_compile_context *ccontext,
int (*guard)(uint32_t, void *), void *user_data)
{
    return pcre2_set_compile_recursion_guard(ccontext,
         guard, user_data);
}

/*PCRE2 NATIVE API MATCH CONTEXT FUNCTIONS*/

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
pcre2_match_context * PCRE2_CALL_CONVENTION
MTCCNTCR(pcre2_general_context *gcontext)
{
     return pcre2_match_context_create(
         gcontext);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
pcre2_match_context * PCRE2_CALL_CONVENTION
MTCCNTCP(pcre2_match_context *mcontext)
{
     return pcre2_match_context_copy(
         mcontext);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
void PCRE2_CALL_CONVENTION
MTCCNTFR(pcre2_match_context *mcontext)
{
     return pcre2_match_context_free(mcontext);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SETCALOT(pcre2_match_context *mcontext,
int (*callout)(pcre2_callout_block *, void *), void *callout_data)
{
     return pcre2_set_callout(mcontext,
         callout,
         callout_data);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SETOFFLM(pcre2_match_context *mcontext, PCRE2_SIZE limit)
{
     return pcre2_set_offset_limit(mcontext,
         limit);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SETMTCLM(pcre2_match_context *mcontext, uint32_t limit)
{
     return pcre2_set_match_limit(mcontext,
         limit);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SETHEPLM(pcre2_match_context *mcontext, uint32_t limit)
{
     return pcre2_set_heap_limit(mcontext, limit);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SETDEPLM(pcre2_match_context *mcontext, uint32_t limit)
{
     return pcre2_set_depth_limit(mcontext, limit);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SETRCNLM(pcre2_match_context *mcontext, uint32_t limit)
{
     return pcre2_set_recursion_limit(mcontext,
         limit);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SETRCNMM(pcre2_match_context *mcontext,
void *(*mymalloc)(size_t, void *), void (*myfree)(void *, void *),
void *mydata)
{
     return pcre2_set_recursion_memory_management(
         mcontext,
         mymalloc,
         myfree, mydata);
}

/* PCRE2 NATIVE API CONVERT CONTEXT FUNCTIONS*/

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
pcre2_convert_context * PCRE2_CALL_CONVENTION
CNVCTXCP(pcre2_convert_context *ccontext)
{
     return pcre2_convert_context_copy(ccontext);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
pcre2_convert_context * PCRE2_CALL_CONVENTION
CNVCTXCR(pcre2_general_context *gcontext)
{
     return pcre2_convert_context_create(gcontext);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
void PCRE2_CALL_CONVENTION
CNVCTXFR(pcre2_convert_context *ccontext)
{
     return pcre2_convert_context_free(ccontext);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SETGLBES(pcre2_convert_context *ccontext, uint32_t escape)
{
     return pcre2_set_glob_escape(ccontext, escape);
}

/*       = part of CONTEXT2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SETGLBSP(pcre2_convert_context *ccontext, uint32_t separator)
{
     return pcre2_set_glob_separator(ccontext, separator);
}

/*PCRE2 NATIVE API STRING EXTRACTION FUNCTIONS*/

/*       = part of SUBSTRI2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SUBCPNAM(pcre2_match_data *match_data, PCRE2_SPTR stringname,
  PCRE2_UCHAR *buffer, PCRE2_SIZE *sizeptr)
{
     return pcre2_substring_copy_byname(match_data,
         stringname, buffer, sizeptr);
}

/*       = part of SUBSTRI2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SUBCPNUM(pcre2_match_data *match_data,
  uint32_t stringnumber, PCRE2_UCHAR *buffer, PCRE2_SIZE *sizeptr)
{
     return pcre2_substring_copy_bynumber(match_data,
         stringnumber, buffer,
         sizeptr);
}

/*       = part of SUBSTRI2 - supported as is via PGMNAME(LONGMIXED) */
void PCRE2_CALL_CONVENTION
SUBSTFR(PCRE2_UCHAR *string)
{
     return pcre2_substring_free(string);
}

/*       = part of SUBSTRI2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SUBGTNAM(pcre2_match_data *match_data,
  PCRE2_SPTR stringname, PCRE2_UCHAR **stringptr, PCRE2_SIZE *sizeptr)
{
     return pcre2_substring_get_byname(match_data,
         stringname, stringptr, sizeptr);
}

/*       = part of SUBSTRI2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SUBGTNUM(pcre2_match_data *match_data,
  uint32_t stringnumber, PCRE2_UCHAR **stringptr, PCRE2_SIZE *sizeptr)
{
     return pcre2_substring_get_bynumber(match_data,
         stringnumber, stringptr,
         sizeptr);
}

/*       = part of SUBSTRI2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SUBLNNAM(pcre2_match_data *match_data,
  PCRE2_SPTR stringname, PCRE2_SIZE *sizeptr)
{
     return pcre2_substring_length_byname(match_data,
         stringname, sizeptr);
}

/*       = part of SUBSTRI2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SUBLNNUM(pcre2_match_data *match_data,
  uint32_t stringnumber, PCRE2_SIZE *sizeptr)
{
     return pcre2_substring_length_bynumber(match_data,
         stringnumber, sizeptr);
}

/*       = part of SUBSTRI2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SUBNMSCN(const pcre2_code *code, PCRE2_SPTR stringname,
  PCRE2_SPTR *firstptr, PCRE2_SPTR *lastptr)
{
     return pcre2_substring_nametable_scan(code,
         stringname, firstptr, lastptr);
}

/*       = part of SUBSTRI2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SUBNMFNM(const pcre2_code *code,
  PCRE2_SPTR stringname)
{
     return pcre2_substring_number_from_name(code,
         stringname);
}

/*       = part of SUBSTRI2 - supported as is via PGMNAME(LONGMIXED) */

void PCRE2_CALL_CONVENTION
SUBLSTFR(PCRE2_SPTR *list)
{
     return pcre2_substring_list_free(list);
}

/*       = part of SUBSTRI2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
SUBLSTGT(pcre2_match_data *match_data, PCRE2_UCHAR ***listptr,
  PCRE2_SIZE **lengthsptr)
{
     return pcre2_substring_list_get(match_data,
         listptr, lengthsptr);
}

/*PCRE2 NATIVE API STRING SUBSTITUTION FUNCTION*/
/*        =>SUBSTIT2 */
int PCRE2_CALL_CONVENTION
SUBSTIT2(const pcre2_code *code, PCRE2_SPTR subject, PCRE2_SIZE length,
  PCRE2_SIZE start_offset, uint32_t options, pcre2_match_data *match_data,
  pcre2_match_context *mcontext, PCRE2_SPTR replacement, PCRE2_SIZE rlength,
  PCRE2_UCHAR *buffer, PCRE2_SIZE *blength)
{
     return pcre2_substitute(code, subject,
         length, start_offset,
         options, match_data,
         mcontext, replacement,
         rlength, buffer,
         blength);
}

/*PCRE2 NATIVE API CONVERT_FUNCTIONS */

/*       = part of CONVERT2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
PATTCONV(PCRE2_SPTR pattern, PCRE2_SIZE plength, uint32_t options,
  PCRE2_UCHAR **buffptr, PCRE2_SIZE *bufflenptr,
  pcre2_convert_context *ccontext)
{
     return pcre2_pattern_convert(pattern, plength, options,
  buffptr, bufflenptr,
  ccontext);
}

/*       = part of CONVERT2 - supported as is via PGMNAME(LONGMIXED) */
void PCRE2_CALL_CONVENTION
CNVPATFR(PCRE2_UCHAR *converted)
{
 pcre2_converted_pattern_free(converted);
}


/*PCRE2 NATIVE API JIT FUNCTIONS*/
/*        =>JITCOMP2 - available but not supportet in z/OS */
int PCRE2_CALL_CONVENTION
JITCOMP2(pcre2_code *code, uint32_t options)
{
     return pcre2_jit_compile(code, options);
}

/*        =>JITMATC2 - available but not supportet in z/OS */
int PCRE2_CALL_CONVENTION
JITMATC2(const pcre2_code *code, PCRE2_SPTR subject, PCRE2_SIZE length,
  PCRE2_SIZE start_offset, uint32_t options, pcre2_match_data *match_data,
  pcre2_match_context *mcontext)
{
     return pcre2_jit_match(code, subject,
         length, start_offset,
         options, match_data,
         mcontext);
}

/*       = part of JITMISC2 - not supportet in z/OS
       void pcre2_jit_free_unused_memory(pcre2_general_context *gcontext);*/

/*       = part of JITMISC2 - not supportet in z/OS
       pcre2_jit_stack *pcre2_jit_stack_create(PCRE2_SIZE startsize,
         PCRE2_SIZE maxsize, pcre2_general_context *gcontext);*/

/*       = part of JITMISC2 - not supportet in z/OS
       void pcre2_jit_stack_assign(pcre2_match_context *mcontext,
         pcre2_jit_callback callback_function, void *callback_data);*/

/*       = part of JITMISC2 - not supportet in z/OS
       void pcre2_jit_stack_free(pcre2_jit_stack *jit_stack);*/

/*PCRE2 NATIVE API SERIALIZATION FUNCTIONS*/

/*       = part of SERIALI2 - supported as is via PGMNAME(LONGMIXED) */
int32_t PCRE2_CALL_CONVENTION
SERIADCD(pcre2_code **codes, int32_t number_of_codes,
   const uint8_t *bytes, pcre2_general_context *gcontext)
{
     return pcre2_serialize_decode(codes,
         number_of_codes, bytes,
         gcontext);
}

/*       = part of SERIALI2 - supported as is via PGMNAME(LONGMIXED) */
int32_t PCRE2_CALL_CONVENTION
SERIAENC(const pcre2_code **codes, int32_t number_of_codes,
   uint8_t **serialized_bytes, PCRE2_SIZE *serialized_size,
   pcre2_general_context *gcontext)
{
     return pcre2_serialize_encode(codes,
         number_of_codes, serialized_bytes,
         serialized_size, gcontext);
}

/*       = part of SERIALI2 - supported as is via PGMNAME(LONGMIXED) */
void PCRE2_CALL_CONVENTION
SERIAFR(uint8_t *bytes)
{
     return pcre2_serialize_free(bytes);
}

/*       = part of SERIALI2 - supported as is via PGMNAME(LONGMIXED) */
int32_t PCRE2_CALL_CONVENTION
SERGTNMC(const uint8_t *bytes)
{
     return pcre2_serialize_get_number_of_codes(bytes);
}

/*PCRE2 NATIVE API AUXILIARY FUNCTIONS*/

/*       = part of ERROR2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
GETERROR(int enumber, PCRE2_UCHAR *buffer, PCRE2_SIZE size)
{
     return pcre2_get_error_message(enumber, buffer,
         size);
}

/*        =>MAKETAB2 */
const uint8_t * PCRE2_CALL_CONVENTION
MAKETAB2(pcre2_general_context *gcontext)
{
     return pcre2_maketables(gcontext);
}

/*        =>PATTERN2 */
int PCRE2_CALL_CONVENTION
PATTERN2(const pcre2_code *code, uint32_t what, void *where)

{
     return pcre2_pattern_info(code, what, where);
}
/*       = part of PATTERN2 - supported as is via PGMNAME(LONGMIXED) */
int PCRE2_CALL_CONVENTION
CALOTENM(const pcre2_code *code,
  int (*callback)(pcre2_callout_enumerate_block *, void *), void *callout_data)
{
     return pcre2_callout_enumerate(code,
         callback,
         callout_data);
}

/*        =>CONFIG2 */
int CONFIG2(uint32_t what, void *where)
{
     return pcre2_config(what, where);
}

/* experimental implementation of some of the above as COBOL front end to
   native C
   I chose the mainframe names GETMAIN and FREEMAIN since these are the names
   that native z/OS users are familiar with, especially in the CICS
   environment, but the implementation is simply via the malloc and free
   functions.*/

void FREEMAIN (void* ptr)
{
        free (ptr);
}

void * GETMAIN (size_t size)
{
    return malloc (size);
}

/*SYNOPSIS FOR PCRZ API*/

/*     = part of pcrzfunc - supported as is via PGMNAME(LONGMIXED) */

int ZWHTTRM (char * str, int maxlen)
{
       return pcrz_what_term_str (str, maxlen);
}

int ZISSPTRM (char * str, int maxlen)
{
       return pcrz_is_space_term_str (str, maxlen);
}

int ZISNLTRM (char * str, int maxlen)
{
       return pcrz_is_null_term_str (str, maxlen);
}

int ZSPC2NUL (char * str, int maxlen)
{
       return pcrz_space_to_null_term_str (str, maxlen);
}

int ZNUL2SPC (char * str, int maxlen)
{
       return pcrz_null_to_space_term_str (str, maxlen);
}

/*  pcrz_codeset_init <=> ZCSINIT             */
int ZCSINIT (ZCSSTRCT ** handle_ptr,
                      char * codeset_name,
                      size_t pattern_size,
                      size_t subject_size)
{
       return pcrz_codeset_init (handle_ptr,
                      codeset_name,
                      pattern_size,
                      subject_size);
}

/*  pcrz_codeset_convert_pattern <=> ZCSCONVP */
int ZCSCONVP (ZCSSTRCT *zcsstrct_ptr,
         char * input_pattern, size_t pattern_length,
         char ** output_pattern, size_t ** out_pattern_length)
{
       return pcrz_codeset_convert_pattern (zcsstrct_ptr,
                input_pattern, pattern_length,
                output_pattern, out_pattern_length);
}

/*  pcrz_codeset_convert_subject <=> ZCSCONVS */
int ZCSCONVS (ZCSSTRCT *zcsstrct_ptr,
         char * input_subject, size_t subject_length,
         char ** output_subject, size_t ** out_subject_length)
{
       return pcrz_codeset_convert_subject (zcsstrct_ptr,
                input_subject, subject_length,
                output_subject, out_subject_length);
}

/*  pcrz_codeset_release <=> ZCSRLSE          */
void ZCSRLSE (ZCSSTRCT *zcsstrct_ptr)
{
       pcrz_codeset_release (zcsstrct_ptr);
}

/*SYNOPSIS OF POSIX API*/

/*note: native z/OS C has <regex.h> and these functions.  Do not issue
#include <regex.h> in any program that intend to use PCRE POSIX*/
/* Decided to transform names to eliminate the conflict above!
#define NATIVE_ZOS_POSIX
in the OPTFILE member will make the COBAPI2 to override the PCRE2 calls
and direct them to the native z/OS, POSIX compliant functions */
#ifdef NATIVE_ZOS_POSIX
/* 10.22.
I do no longer provide any override for POSIX through PCOBAPI.c.  Any
override, for any desired result should be achieved via using the
PC2PSXAP.h header and manipulating the COMPILE_WITH_PCRE2 macro.
int PREGCOMP(regex_t *preg, const char *pattern,
            int cflags)
{
       return regcomp(preg, pattern,
            cflags);
}

int PREGEXEC(regex_t *preg, const char *string,
            size_t nmatch, regmatch_t pmatch[], int eflags)
{
       return regexec(preg, string,
            nmatch, pmatch  , eflags);
}

size_t PREGERR(int errcode, const regex_t *preg,
            char *errbuf, size_t errbuf_size)
{
       return regerror(errcode, preg,
            errbuf, errbuf_size);
}

void PREGFREE(regex_t *preg)
{
       return regfree(preg);
}*/
#endif
/**/
