
/*
THE z/OS specific functionality
-------------------------------

Contributed by:   Ze'ev Atlas.

Copyright (c) 2013 Ze'ev Atlas.
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

static unsigned char CHARSET_IBM1047[] = "IBM1047";
static unsigned char CHARSET_IBM1140[] = "IBM1140";
static unsigned char CHARSET_IBM285[] = "IBM285";
static unsigned char CHARSET_posix_bc[] = "posix-bc";
static unsigned char CHARSET_IBMUNKN[] = "unknown";

/* This function resembles the Perl check for codepage and returns one
   the four major EBCDIC versions (3 recognized by Perl and one more)
/* my %ebcdic = ( 176 => 'cp37', 95 => 'cp1047', 106 => 'posix-bc' );*/

const char * pcrz_cpversion ()
{
  char CHAR_circumflex = '�';   /*<TAG>*/
  if ((int) CHAR_circumflex == 176)     /* Common with CP424, etc. */
  {
    return CHARSET_IBM1140;
  }
  else if ((int) CHAR_circumflex == 95) /* Common with CP500, CP875,
                                           IBM1026, etc. */
  {
    return CHARSET_IBM1047;
  }
  else if ((int) CHAR_circumflex == 106)
  {
    return CHARSET_posix_bc;
  }
  else if ((int) CHAR_circumflex == 186)
  {
    return CHARSET_IBM285;
  }
  else
  {
    return CHARSET_IBMUNKN;
  }
}
/*autoconv-0001 C:\projects\pcre2port\pcrzoscs.h converted to PCRZOSCS*/
