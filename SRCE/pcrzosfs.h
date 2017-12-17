


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
/****************************************************************************
* The pcrzosfs.h provides structures, types and variables needed to handle  *
* some peculiarities of the classic z/OS file system.  Particularly there   *
* are structures and types needed to handle PDS (or PDSE files).  Also, the *
* PARM filed in the JCL behaves differently then the familiar Unix/Linux and*
* Windows command line and there are some issues that need to be resolved.  *
*                                                            ZA 7/08/2013   *
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* #define _XOPEN_SOURCE */
#include <fnmatch.h>
/* #define _POSIX_SOURCE */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/modes.h>
#include <dirent.h>

/* There are three ways to supply a file name to C in z/OS
fopen ("file.dsn", "r") - HLQ is added, much like in TSO
fopen ("\'myhlq.file.dsn\'", "r") - HLQ is present in the filename
fopen ("dd:ddname", "r") - needs a valid DD allocation
fopen ("dd:MYDD(B)","r");
*/

/* Each PDS directory record is of 256 bytes
 * +----------------+------+------+------+------+----------------+
 * + logical record |Member|Member|......|Member|  Unused        +
 * + length - short |elmnt1|elmnt2|      |elmntn|                +
 * +----------------+------+------+------+------+----------------+
 *  (logical record len includes itslef)
 *
 * each member element looks like the following
 * +--------+-------+----+-----------------------------------+
 * + Member |TTR    |info|                                   +
 * + Name   |       |byte|  User Data TTRN's (halfwords)     +
 * + 8 bytes|3 bytes|    |                                   +
 * +--------+-------+----+-----------------------------------+
*/

#define RECLEN  256
#define TTRLEN  3      /* TTR's are 3 bytes long */
/*
 * bit 0 of the info-byte is '1' if the member is an alias,
 * 0 otherwise. ALIAS_MASK is used to extract this information
*/
#define ALIAS_MASK ((unsigned int) 0x80)
/*
 * The number of user data half-words is in bits 3-7 of the info byte.
 * TTRN_MASK is used to extract this number and doubled (for half words)
*/
#define TTRN_MASK ((unsigned int) 0x1F)
#define MEMBLEN     8
#define TRUE        1
#define FALSE       0
#define DIRECTORY_END "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"

/* define z/OS file types */
#define __ZOS_NOFILE      0
#define __ZOS_PDS         8
#define __ZOS_PDSE        9
#define __ZOS_PDS_MEMBER  10
#define __ZOS_PS          16
#define __ZOS_VSAM_KSDS   24
#define __ZOS_VSAM_ESDS   25
#define __ZOS_VSAM_RRDS   26
#define __ZOS_HFS         32

#define __ZOS_FUNCTION pcre2grep(handle, frtype, pds_n_member,\
            (filenames > FN_DEFAULT ||\
            (filenames == FN_DEFAULT &&\
            !only_one_at_top))? pds_n_member:NULL);

/* member element within PDS directory record */
struct MEMBER_ELEMENT
{
    char member_name [MEMBLEN];
    char ttrmem [TTRLEN];
    unsigned char info_byte;
};

/* Distiled member information for our purpose */
typedef struct PDS_MEMBER
{
    char member[8+1];   /* member name                    */
    short int  alias;   /* TRUE if alias, FALSE otherwise */
}PDS_MEMBER;

/* Prototypes */
int readpds(PDS_MEMBER * pds, FILE * fp);
FILE * openpds (char * pds);
int identifyzosfiletype (FILE * filename);
int travelonpdsdir (char * pathname, BOOL only_one_at_top);

/* any other functions are work in progress and are not exposed */

static FILE *fp;

int readpds
(
    PDS_MEMBER * pds,   /* pointer to current member information  */
    FILE    * fp        /* PDS directory FILE pointer      */
)
{
    static struct MEMBER_ELEMENT *member;
    static char   directory_record[256];
    static short  ttrns;
    static short  data_cnt = 2;     /* first useful; byte number  */
    static int    blocks_read = 0;  /* Directory blocks read total */
    static short  count = 0;
    int rc;
    int i;

    if (   data_cnt >= count  )
    {
       i = fread(directory_record,256,1,fp);
       if (i == 1)           /* Was 1 block of 256 bytes read?  */
       {
           blocks_read += 1;       /* increment block count           */
           memcpy ( & count, directory_record, 2 );
           data_cnt = 2;
       }
       else
       {
          int ferr = ferror(fp);
          fprintf(stderr,
             "ZOSFS-001 - directory read error after block %d, read: %d ferror(): %d\n",
             blocks_read, i, ferr);
             rc = EXIT_FAILURE;
       };
    };

    if (memcmp((directory_record+data_cnt),DIRECTORY_END,8) == 0 )
    {
        rc = EOF;    /* It is like logical EOF      */
    }
    else
    {
        member = (struct MEMBER_ELEMENT *) (directory_record + data_cnt);
        memset(pds,'\0',sizeof(PDS_MEMBER));
        strncpy(pds->member, member->member_name, MEMBLEN);

        if ( (member->info_byte & 0x80) == FALSE )
        {
            pds->alias = FALSE;
        }
        else
        {
            pds->alias = TRUE;
        };

        i = 2 * (member->info_byte & 0x1f); /* Number of ttrns */
        data_cnt += sizeof(struct MEMBER_ELEMENT) + i; /* next element offset  */
        rc = EXIT_SUCCESS;
    };
    return(rc);
}

FILE * openpds (char * pds)
{
   static FILE *fp;
   static char pds_name [59]; /* ' + 44 bytes file name + '(' + +/- + 8 + ')' + ' + null */
   memset(pds_name,'\0',sizeof(pds_name));
   sprintf(pds_name,"%s",pds);

   /*
   * Open the pds in binary read mode. The PDS directory will be read one
   * record at a time until either the end of the directory or end-of-file
   * is detected.
   */

   fp = fopen(pds_name,"rb");
   if (fp == NULL)
   {
       fprintf(stderr, "ZOSFS-002 - could not open %s\n", pds_name);
       return(NULL);
   };

   return (fp);
}

/* fclose (fp); */

int identifyzosfiletype (FILE * filename)
{
   fldata_t fileinfo;
   int rc;
   static char local_fname [FILENAME_MAX];
   rc = fldata(filename, local_fname, &fileinfo);
   if (rc != 0)
   {
       fprintf(stderr, "ZOSFS-003 - fldata failed\n");
       return -1;
   }
   else
   {
   /* Note: if this is a file name (rather than DDNAME, it is
      returned quoted
   */
      if ((fileinfo.__dsorgPO == 1 && fileinfo.__dsorgPDSE == 0) &&
          fileinfo.__dsorgPDSdir == 1)
      {
          rc = __ZOS_PDS;
      }
      else if ((fileinfo.__dsorgPO == 1 && fileinfo.__dsorgPDSE == 1) &&
          fileinfo.__dsorgPDSdir == 1)
      {
          rc = __ZOS_PDSE;
      }
      else if (fileinfo.__dsorgPS == 1)
      {
          rc = __ZOS_PS;
      }
      else if ((fileinfo.__dsorgPO == 1 || fileinfo.__dsorgPDSE == 1) &&
          fileinfo.__dsorgPDSmem == 1)
      {
          rc = __ZOS_PDS_MEMBER;
      }
      else if(fileinfo.__vsamtype == __ESDS)
      {
          rc = __ZOS_VSAM_ESDS;
      }
      else if(fileinfo.__vsamtype == __KSDS)
      {
          rc = __ZOS_VSAM_KSDS;
      }
      else if(fileinfo.__vsamtype == __RRDS)
      {
          rc = __ZOS_VSAM_RRDS;
      }
      else if (fileinfo.__dsorgHFS == 1)
      {
          rc = __ZOS_HFS;
      }
      else
      {
          rc = __ZOS_NOFILE;
      }
   };
   return rc;
}

int travelonpdsdir (char * pathname, BOOL only_one_at_top)
{
   int c;
   int e;
   int rc;
   char * mem;
   char pds_n_member [59];
   /* duplicating some enum's to allow for a general puurpos function */
   enum { FN_NONE, FN_DEFAULT, FN_MATCH_ONLY, FN_NOMATCH_ONLY,
          FN_FORCE };
   enum { FR_PLAIN, FR_LIBZ, FR_LIBBZ2 };
   void *handle;
   int frtype;
   FILE * mypds;
   FILE * membfile;
   PDS_MEMBER member_attributes;
   mypds = openpds(pathname);
   if (mypds == NULL)
   {
      fprintf(stderr, "ZOSFS-004: error openning %s as a PDS/E\n", pathname);
      fclose (mypds);
      return -1;
   }
   else
   {
      c = readpds (&member_attributes, mypds);
      while (c != EXIT_FAILURE && c != EOF)
      {
          strcpy (pds_n_member, pathname);
          e = strlen(pds_n_member);
          if (pds_n_member[0] == '\'')
          {
             e--;
          }
          mem = &pds_n_member[e];
          strcpy (mem, "("); mem ++;
          pcrz_space_to_null_term_str(member_attributes.member, 9);
          /*ZSPC2NUL (member_attributes.member, 9);*/
          strcpy (mem, member_attributes.member);
          e = strlen(pds_n_member);
          mem = &pds_n_member[e];
          strcpy (mem, ")"); mem ++;
          if (pds_n_member[0]=='\'')
          {
             strcpy (mem, "\'");
          }
          membfile = fopen(pds_n_member, "r");
          handle = (void *)membfile;
          frtype = FR_PLAIN;
          if (handle == NULL)
            {
            if (!silent)
              fprintf(stderr, "ZOSFS-005: Failed to open member %s: %s\n", pds_n_member,
                strerror(errno));
            return 2;
            }

          /* Now grep the member file */
          printf ("ZOSFS-006: PDS member %s \n", pds_n_member);
          rc = __ZOS_FUNCTION;

          /* Close in an appropriate manner. */
          fclose (membfile);

          c = readpds (&member_attributes, mypds);
      }
   }
   return rc;
}
/*autoconv-0001 C:\projects\pcre2port\pcrzosfs.h converted to PCRZOSFS*/
