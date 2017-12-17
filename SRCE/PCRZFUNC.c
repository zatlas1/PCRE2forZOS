
#ifndef HAVE_CONFIG_H
#   define HAVE_CONFIG_H 1
#endif
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#ifdef NATIVE_ZOS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PCRZCONH.h"

static unsigned char CHARSET_IBM1047[] = "IBM-1047";

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

/* The functions herein do not use Reg Exp because Reg Exp is not yet
available.  They help to prepare COBOL fixed length character strings for Reg
Exp manipulation.  COBOL character strings are fundamentally different then C
(and thus also C++, Java, C# and virtually any other language) character
strings in that they are fixed length or length terminated (i.e. the length of
the string is known either in compile time or in run time via some variable.)
That length is retrievable by means of querying the prefix variable or using
the LENGTH OF special register.  There is no terminator character (null or
otherwise.)  COBOL notion of fixed length or length terminated character
strings relate directly to the notion of fixed length records - record length
is pre-defined and known in similar manner and there is no record terminator
character.
Most other languages use the null terminated notation (i.e. the end-of-string
is signaled by the null character.  Regular Expression as defined and implemented
by Perl, Posix and PCRE depend on the string being null terminated.
To add to the complication, C on z/OS adds record terminator to the end of the
record even on fixed length records, but on text records, at least from the
standard input, the last spaces are chopped.  I coined that type as space
terminated records and strings.
The functions herein provide the means to query a character string and convert
it to the desired format.
Currently, these functions compile only under NATIVE_ZOS as I suspect that nobody
else will want or need them, however, it is pretty simple to remove this
limitation                                                                     */

extern int pcrz_what_term_str (char * str, int maxlen);
extern int pcrz_is_space_term_str (char * str, int maxlen);
extern int pcrz_is_null_term_str (char * str, int maxlen);
extern int pcrz_space_to_null_term_str (char * str, int maxlen);
extern int pcrz_null_to_space_term_str (char * str, int maxlen);

extern int pcrz_codeset_init (ZCSSTRCT ** handle_ptr,
                      char * codeset_name,
                      size_t codeset_name_size,
                      size_t pattern_size,
                      size_t subject_size);
extern int pcrz_codeset_convert_pattern (ZCSSTRCT *zcsstrct_ptr,
         char * input_pattern, size_t pattern_length,
         char ** output_pattern, size_t ** out_pattern_length);
extern int pcrz_codeset_convert_subject (ZCSSTRCT *zcsstrct_ptr,
         char * input_subject, size_t subject_length,
         char ** output_subject, size_t ** out_subject_length);
extern void pcrz_codeset_release (ZCSSTRCT *zcsstrct_ptr);
int pcrz_codeset_convert (ZCSSTRCT *zcsstrct_ptr, char func,
         char * input_subject, size_t subject_length,
         char ** output_subject, size_t ** out_subject_length);
/* to make a length terminated into null terminated use COBOL (or PL/I
equivalent) technique of concatenating the sting with null:
           STRING YOUR-LENGTH-TERMINATED_STRING, LOW-VALUE
                  DELIMITED BY SIZE INTO YOUR-TARGET-STRING
*/

/* This function examines the presumably COBOL character string as is.  If it
is not null terminated, then if there is at least one space in the end it will
return -2 for space terminated and if the last character is not space it will
return -3 for length terminated.  The user may disregard the difference between
length and space terminated, depending on the application.  If the string is
null  terminated, then the function will return -1.
Note: maxlen is one more then the null terminated string length to allow for
the null terminator.  For the others it is the fixed length of the string.  */
int pcrz_what_term_str (char * str, int maxlen)
{
   int i;
   /* is it null terminated? */
   for (i=0; i<maxlen; i++)
   {
      if (str [i] == CHAR_NULL)
      {
         return PCRZ_NULL_TERMINATED; /* null terminated */
      }
   }
   /* no! then choose among length terminated variants*/
   if (str [maxlen-1] ==  CHAR_SPACE)
   {
      return PCRZ_SPACE_TERMINATED; /*space terminated*/
   }
   else
   {
      return PCRZ_LENGTH_TERMINATED; /*length terminated */
   }
}

/* This function examines the presumably COBOL character string as is.  If it
is not null terminated, then it looks for the last non-space character and
returns the actual length of the string to that character.  If the last
character is not space it will return -3 for length terminated.  If the string
is null terminated, then the function will return -1                        */
int pcrz_is_space_term_str (char * str, int maxlen)
{
   int i;
   for (i=0; i<maxlen; i++)
   {
      if (str [i] == CHAR_NULL)
      {
         return PCRZ_NULL_TERMINATED; /* no, it is null terminated */
      }
   }
   for (i=maxlen-1; i>=0; i--)
   {
      if (str [i] !=  CHAR_SPACE)
      {
         return i+1; /* length of spaec terminated */
      }
   }
   return PCRZ_LENGTH_TERMINATED; /*length terminated */
}

/* This function examines the presumably COBOL character string as is.  If it
is null terminated then its length is returned.  Otherewise, if there is at
least one space in the end it will return -2 for space terminated and if the
last character is not space it will return -3 for length terminated.  The user
may disregard the difference between length and space terminated, depending on
the application.
Note: maxlen is one more then the null terminated string length to allow for
the null terminator.  For the others it is the fixed length of the string.  */
int pcrz_is_null_term_str (char * str, int maxlen)
{
   int i;
   for (i=0; i<maxlen; i++)
   {
      if (str [i] == CHAR_NULL)
      {
         return i; /* length of null terminated */
      }
   }
   /* no! tehn choose among length terminated variants*/
   if (str [maxlen-1] ==  CHAR_SPACE)
   {
      return PCRZ_SPACE_TERMINATED; /*space terminated*/
   }
   else
   {
      return PCRZ_LENGTH_TERMINATED; /*length terminated */
   }
}

/* This function examines the presumably COBOL character string.  If it is
already null terminated, only the null terminated string part is examined
farther.  The function then looks for the last non-space character, marks the
next character as null and returns the actual length of the null terminated
string.  If the last non-space character is the last character (no room for
null character, then the function will return -3 for length terminated.
Note: maxlen is one more then the null terminated string length to allow for
the null terminator.  For the others it is the fixed length of the string.  */
int pcrz_space_to_null_term_str (char * str, int maxlen)
{
   int i;
   int j;
   j = maxlen;
   for (i=0; i<maxlen; i++)
   {
      if (str [i] == CHAR_NULL)
      {
         j= i;
         break;
      }
   }
   for (i=j-1; i>=0; i--)
   {
      if (str [i] !=  CHAR_SPACE)
      {
         if (i+1 == maxlen)
         {
            return PCRZ_LENGTH_TERMINATED; /*length terminated */
         }
         else
         {
            str [i+1] = CHAR_NULL;
            return i; /* length of spaec terminated */
         }
      }
   }
}

/* This function examines the presumably COBOL character string.  If it is null
terminated, the null character and all characters after it to maxlen are
replaced by space.  The function then looks for the last non-space character
and returns the actual length of the space terminated string.  If the last
non-space character is the last character, then the function will return -3 for
length terminated.
Note: maxlen is one more then the null terminated string length to allow for
the null terminator.  For the others it is the fixed length of the string.  */
int pcrz_null_to_space_term_str (char * str, int maxlen)
{
   int i;
   int j;
   for (i=0; i<maxlen; i++)
   {
      if (str [i] == CHAR_NULL)
      {
         for (j=1; j < maxlen; j++)
         {
            str [j] =  CHAR_SPACE;
         }
      }
   }
   if (str [maxlen - 1] !=  CHAR_SPACE)
   {
      return PCRZ_LENGTH_TERMINATED; /*length terminated */
   }
   for (i=maxlen-1; i>=0; i--)
   {
      if (str [i] !=  CHAR_SPACE)
      {
          return i; /* length of spaec terminated */
      }
   }
}


/****************************************************************************
* This section  provides means to handle the EBCDIC horror within the       *
* context of PCRE2 and its REXX interface.  Read more about the problem and *
* solution in the PCRE2 Native z/OS Port documentation.                     *
****************************************************************************/
/* provided functionalities:
*  1. Determine the native (or supplied) input CODESET.
*  2. Initializie conversion. (to, from)
*  3. Allocate two blocks of memory for the IBM1047 intermediate
*     strings, one for the pattern and one for the subject string.
*  4. Front end conversion, converting pattern and subject strings
*     from the local codeset to IBM1047
*  5. Releasing the formerly initialized environment and deallocating
*     allocated memory.
*  6. Appropriate structure to point to all the above, via the
*     PCRZCONH header file, that defines the ZCSSTRCT structure.
*  Obviously the package should be smart enough to notice whether the
*  local codeset is IBM1047 and do nothing.
*  The LE language (COBOL) and REXX user does not need to know the
*  intertnal structures or even the the structure of ZCSSTRCT.
*  The current documentation would change to reflect what was learned
*  about the EBCDIC horror, accurate explanation on the compiled
*  version, and so on.  Appropriate changes would be introduced to
*  the functions that try to determine the compiler and locale
*  settings.
*/

/*
*  pcrz_codeset_init <=> ZCSINIT
*  1. Determines the native (or supplied) input CODESET.
*  2. Initializie conversion. (to, from)
*  3. Allocate two blocks of memory for the IBM1047 intermediate
*     strings, one for the pattern and one for the subject string.
*  codeset_name is optional and provides the input CODESET,
*  defaults to local codeset. Note however, that if your emulator
*  defaults to something different then the installation locale,
*  you have to override the locale by supplying your codeset.
*  Note that the internal CODESET is always IBM1047 which is the IBM
*  C compiler default and the way the binaries are distributed.
*  pattern_size and subject_size are the estimated max sizes for
*  the largest pattern and largest subject string, default to 2048 and
*  8192 corr.
*  The returned handle should be preserved.
*/

int pcrz_codeset_init (ZCSSTRCT ** handle_ptr, char * codeset_name,
                      size_t codeset_name_size,
                      size_t pattern_size,
                      size_t subject_size)
{
    char *codeset;
    int rc;
    ZCSSTRCT * zcsstrct_ptr;

    /* Allocate the return block */
    zcsstrct_ptr = (ZCSSTRCT *) malloc (sizeof (ZCSSTRCT));
    if (zcsstrct_ptr == NULL)
    {
        fprintf(stderr,
             "ZCSINIT-001: failed to allocate ZCSSTRCT block\n");
        return(-1);
    }

    /* Initialize the structure to empty */
    *handle_ptr = zcsstrct_ptr;
    memcpy(zcsstrct_ptr->charset_locale, " ", 2);
    zcsstrct_ptr->cd           = NULL;
    zcsstrct_ptr->pattern_size = 0;
    zcsstrct_ptr->subject_size = 0;
    zcsstrct_ptr->pattern_1047 = NULL;
    zcsstrct_ptr->subject_1047 = NULL;


    /* Determine the codeset name */
    if (codeset_name != NULL)
    {
        codeset = codeset_name;
        if (codeset_name_size == PCRE2_ZERO_TERMINATED)
        {
           if (strlen(codeset) > PCRZ_MAX_CODESET_SIZE - 1)
           {
              fprintf(stderr,
              "ZCSINIT-002: CODESET name length %d too large\n",
                          strlen(codeset));
              free (zcsstrct_ptr);
              zcsstrct_ptr = NULL;
              return(-1);
           }
        }
        else
        {
           if (codeset_name_size == PCRZ_SPACE_TERMINATED)
           {
              rc = pcrz_space_to_null_term_str ((char *) codeset_name,
                             PCRZ_MAX_CODESET_SIZE);
              if (rc == PCRZ_LENGTH_TERMINATED)
              {
                 fprintf(stderr,
                 "ZCSINIT-003: CODESET name too long\n");
                 free (zcsstrct_ptr);
                 zcsstrct_ptr = NULL;
                 return(-1);
              }
           }
           else
           {
              if (codeset_name_size > PCRZ_MAX_CODESET_SIZE - 1)
              {
                 fprintf(stderr,
                 "ZCSINIT-004: CODESET name size %d too large\n",
                             codeset_name_size);
                 free (zcsstrct_ptr);
                 zcsstrct_ptr = NULL;
                 return(-1);
              }
              else
              {
                 codeset[codeset_name_size] = CHAR_NULL;
              }
           }
        }
    }
    else
    {
        setlocale(LC_ALL, "");
        codeset = nl_langinfo(CODESET);
    }

    strcpy(zcsstrct_ptr->charset_locale, codeset);

    /* Do we need conversion? */
    if (strcmp(codeset, CHARSET_IBM1047) != 0)
    /* Yes, open and allocate descriptor */
    {
        if ((zcsstrct_ptr->cd = iconv_open(CHARSET_IBM1047, codeset))
           == (iconv_t)(-1))
        {
            fprintf(stderr,
            "ZCSINIT-005: Cannot open converter from %s to %s\n",
                codeset, CHARSET_IBM1047);
            free (zcsstrct_ptr);
            zcsstrct_ptr = NULL;
            return(-1);
        }
    }

    if (pattern_size == 0)
    {
       zcsstrct_ptr->pattern_size = PCRZ_MAX_PATTERN_SIZE;
    }
    else
    {
       zcsstrct_ptr->pattern_size = pattern_size;
    }

    if (subject_size == 0)
    {
       zcsstrct_ptr->subject_size = PCRZ_MAX_SUBJECT_SIZE;
    }
    else
    {
       zcsstrct_ptr->subject_size = subject_size;
    }

    /* Allocate space for converted pattern */
    zcsstrct_ptr->pattern_1047 = (char *)
                  malloc (zcsstrct_ptr->pattern_size);
    if (zcsstrct_ptr->pattern_1047 == NULL)
    {
        fprintf(stderr,
        "ZCSINIT-006: failed to allocate space for converted pattern\n");
        free (zcsstrct_ptr->cd );
        free (zcsstrct_ptr);
        zcsstrct_ptr = NULL;
        return(-1);
    }

    /* Allocate space for converted subject */
    zcsstrct_ptr->subject_1047 = (char *)
                  malloc ( zcsstrct_ptr->subject_size);
    if (zcsstrct_ptr->subject_1047 == NULL)
    {
        fprintf(stderr,
        "ZCSINIT-007: failed to allocate space for converted subject\n");
        free (zcsstrct_ptr->cd );
        free (zcsstrct_ptr->pattern_1047);
        free (zcsstrct_ptr);
        zcsstrct_ptr = NULL;
        return(-1);
    }

    return (0);
}

/*-------------------------------------------------------------------*/

/*
*  pcrz_codeset_convert_pattern <=> ZCSCONVP
*  pcrz_codeset_convert_subject <=> ZCSCONVS
*  4. Front end conversion, converting pattern and subject strings
*     from the local codeset to IBM1047
*  This is done via two functions (to avoid user direct interuction
*  with the gory details:
*  int pcrz_codeset_convert_pattern (ZCSSTRCT zcsstrct_ptr,
*        char * input_pattern, size_t pattern_length);
*  int pcrz_codeset_convert_subject (ZCSSTRCT zcsstrct_ptr,
*        char * input_subject, size_t subject_length);
*  where zcsstrct_ptr is the structure returnde by ZCSINIT and
*  input_pattern is the input pattern, or input_subject which is
*  the input subject string.  The functions return pointer to
*  converted string and its length.
*/

int pcrz_codeset_convert_pattern (ZCSSTRCT *zcsstrct_ptr,
         char * input_pattern, size_t pattern_length,
         char ** output_pattern, size_t ** out_pattern_length)
{
   return
    pcrz_codeset_convert (zcsstrct_ptr, 'p',
         input_pattern, pattern_length,
         output_pattern, out_pattern_length);
}

/*-------------------------------------------------------------------*/

int pcrz_codeset_convert_subject (ZCSSTRCT *zcsstrct_ptr,
         char * input_subject, size_t subject_length,
         char ** output_subject, size_t ** out_subject_length)
{
   return
    pcrz_codeset_convert (zcsstrct_ptr, 's',
         input_subject, subject_length,
         output_subject, out_subject_length);
}

/*-------------------------------------------------------------------*/

int pcrz_codeset_convert (ZCSSTRCT *zcsstrct_ptr, char func,
         char * input_subject, size_t subject_length,
         char ** output_subject, size_t ** out_subject_length)
{
   size_t subj_len;
   size_t subj_len_save;
   size_t target_len;
   target_len = (func == 's' ?
              zcsstrct_ptr->subject_size :
              zcsstrct_ptr->pattern_size);
   char * subj_ptr;
   int rc;

   if (subject_length == 0)
   {
      subj_len = strlen(input_subject);
   }
   else
   {
      subj_len = subject_length;
   }
   if (subj_len > (func == 's' ?
              zcsstrct_ptr->subject_size -1  :
              zcsstrct_ptr->pattern_size -1 ))
   {
      fprintf(stderr,
      "ZCSCONVx-001: target length gt allocated block\n");
      return (-1);
   }
   subj_len_save = subj_len;

   if (zcsstrct_ptr->cd == NULL)
   {
      * output_subject = input_subject;
      * out_subject_length = subj_len_save;
      return (1); /* no conversion is necessary. use input */
   }

   subj_ptr = (func == 's' ?
              zcsstrct_ptr->subject_1047 :
              zcsstrct_ptr->pattern_1047);
   rc = iconv(zcsstrct_ptr->cd, &input_subject, &subj_len,
              &subj_ptr, &target_len);
   if (rc == -1)
   {
      fprintf(stderr,
      "ZCSCONVx-001: Error in converting subject string\n");
      return (-1);
   }
   * output_subject = (func == 's' ?
              zcsstrct_ptr->subject_1047 :
              zcsstrct_ptr->pattern_1047);
   * out_subject_length = subj_len_save;
   return (0); /* conversion successful, use zcsstrct_ptr->subject_1047 */
}

/*-------------------------------------------------------------------*/

/*
*  pcrz_codeset_release <=> ZCSRLSE
*  5. Releasing the formerly initialized environment and deallocating
*     allocated memory.
*  This is done via the function:
*  void pcrz_codeset_release (ZCSSTRCT zcsstrct_ptr);
*  where struct is the structure returnde by ZCSINIT and input_pattern
*  is the input pattern.
*/

void pcrz_codeset_release (ZCSSTRCT *zcsstrct_ptr)
{
   if (zcsstrct_ptr->subject_1047 != NULL)
   {
        free (zcsstrct_ptr->subject_1047);
        free (zcsstrct_ptr->pattern_1047);
   }

   if (zcsstrct_ptr->cd != NULL)
   {
        iconv_close(zcsstrct_ptr->cd);
   }

   free (zcsstrct_ptr);
}
#endif
/*autoconv-0001 C:\projects\pcre2port\pcrzfunc.c converted to PCRZFUNC*/
