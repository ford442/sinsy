/* ----------------------------------------------------------------- */
/*           The HMM-Based Speech Synthesis Engine "hts_engine API"  */
/*           developed by HTS Working Group                          */
/*           http://hts-engine.sourceforge.net/                      */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2001-2015  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/*                2001-2008  Tokyo Institute of Technology           */
/*                           Interdisciplinary Graduate School of    */
/*                           Science and Engineering                 */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the HTS working group nor the names of its  */
/*   contributors may be used to endorse or promote products derived */
/*   from this software without specific prior written permission.   */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

#ifndef HTS_MISC_C
#define HTS_MISC_C

#ifdef __cplusplus
#define HTS_MISC_C_START extern "C" {
#define HTS_MISC_C_END   }
#else
#define HTS_MISC_C_START
#define HTS_MISC_C_END
#endif                          /* __cplusplus */

HTS_MISC_C_START;

#include <stdlib.h>             /* for exit(),calloc(),free() */
#include <string.h>             /* for str*() */
#include <stdarg.h>             /* for va_* */

#include "HTS_hidden.h"

/* HTS_show_copyright: show copyright */
void HTS_show_copyright(void)
{
   HTS_fprintf(stdout, "\n");
   HTS_fprintf(stdout, "The HMM-Based Speech Synthesis Engine \"hts_engine API\"\n");
   HTS_fprintf(stdout, "Version %s (%s)\n", HTS_VERSION, HTS_DATE);
   HTS_fprintf(stdout, "Copyright (C) 2001-2015 Nagoya Institute of Technology\n");
   HTS_fprintf(stdout, "              2001-2008 Tokyo Institute of Technology\n");
   HTS_fprintf(stdout, "All rights reserved.\n");
   HTS_fprintf(stdout, "\n");
}

/* HTS_get_copyright: get copyright */
const char *HTS_get_copyright(void)
{
   const char *str = "The HMM-Based Speech Synthesis Engine \"hts_engine API\" "
       "Version " HTS_VERSION " (" HTS_DATE "), "
       "Copyright (C) 2001-2015 Nagoya Institute of Technology, "
       "2001-2008 Tokyo Institute of Technology, " "All rights reserved.";

   return str;
}

/* HTS_error: output error message */
void HTS_error(int error, char *message, ...)
{
   va_list arg;

   HTS_fflush(stdout);
   HTS_fprintf(stderr, "\nError: ");
   va_start(arg, message);
   HTS_vfprintf(stderr, message, arg);
   va_end(arg);
   HTS_fflush(stderr);

   if (error > 0)
      exit(error);
}

/* HTS_fopen: wrapper for fopen */
FILE *HTS_fopen(const char *path, const char *mode)
{
   FILE *fp = fopen(path, mode);

   if (fp == NULL) {
      HTS_error(1, "HTS_fopen: Cannot open %s.\n", path);
      return NULL;
   }

   return fp;
}

/* HTS_fclose: wrapper for fclose */
void HTS_fclose(FILE * fp)
{
   fclose(fp);
}

/* HTS_fgetc: wrapper for fgetc */
char HTS_fgetc(FILE * fp)
{
   char c = fgetc(fp);

   if (feof(fp)) {
      HTS_error(1, "HTS_fgetc: Unexpected end of file.\n");
   }

   return c;
}

/* HTS_fgets: wrapper for fgets */
char *HTS_fgets(char *buff, int n, FILE * fp)
{
   char *s = fgets(buff, n, fp);

   if (s == NULL && feof(fp))
      return NULL;

   if (s == NULL && !feof(fp)) {
      HTS_error(1, "HTS_fgets: Failed to read from stream.\n");
      return NULL;
   }

   return s;
}

/* HTS_fread: wrapper for fread */
size_t HTS_fread(void *ptr, size_t size, size_t nmemb, FILE * fp)
{
   size_t count = fread(ptr, size, nmemb, fp);

   if (count != nmemb) {
      HTS_error(1, "HTS_fread: Unexpected end of file.\n");
      return 0;
   }

   return count;
}

/* HTS_fwrite: wrapper for fwrite */
size_t HTS_fwrite(const void *ptr, size_t size, size_t nmemb, FILE * fp)
{
   size_t count = fwrite(ptr, size, nmemb, fp);

   if (count != nmemb) {
      HTS_error(1, "HTS_fwrite: Cannot write to stream.\n");
      return 0;
   }

   return count;
}

/* HTS_fwrite_little_endian: fwrite with byteswap */
size_t HTS_fwrite_little_endian(const void *ptr, size_t size, size_t nmemb, FILE * fp)
{
   size_t i, j;
   const char *p = (const char *) ptr;
   char c;
   size_t count = 0;

   if (HTS_is_big_endian()) {
      for (i = 0; i < nmemb; i++) {
         for (j = 0; j < size; j++) {
            c = p[i * size + size - 1 - j];
            count += fwrite(&c, 1, 1, fp);
         }
      }
      count /= size;
   } else {
      count = fwrite(ptr, size, nmemb, fp);
   }

   if (count != nmemb) {
      HTS_error(1, "HTS_fwrite_little_endian: Cannot write to stream.\n");
      return 0;
   }

   return count;
}

/* HTS_fprintf: wrapper for fprintf */
int HTS_fprintf(FILE * fp, const char *message, ...)
{
   va_list arg;
   int result;

   va_start(arg, message);
   result = vfprintf(fp, message, arg);
   va_end(arg);

   return result;
}

/* HTS_vfprintf: wrapper for vfprintf */
int HTS_vfprintf(FILE * fp, const char *message, va_list arg)
{
   int result = vfprintf(fp, message, arg);

   return result;
}

/* HTS_fflush: wrapper for fflush */
void HTS_fflush(FILE * fp)
{
   fflush(fp);
}

/* HTS_fseek: wrapper for fseek */
int HTS_fseek(FILE * fp, long offset, int whence)
{
   int result = fseek(fp, offset, whence);

   if (result != 0) {
      HTS_error(1, "HTS_fseek: Failed to change stream position.\n");
      return 1;
   }

   return 0;
}

/* HTS_ftell: wrapper for ftell */
size_t HTS_ftell(FILE * fp)
{
#if defined(__ANDROID__) || defined(__EMSCRIPTEN__)
   return (size_t) ftell(fp);
#else
   fpos_t pos;

   if (fgetpos(fp, &pos) != 0)
      return 0;
#ifdef __FreeBSD__
   return (size_t) pos;
#else
   return (size_t) pos.__pos;
#endif                          /* __FreeBSD__ */
#endif                          /* __ANDROID__ || __EMSCRIPTEN__ */
}

/* HTS_calloc: wrapper for calloc */
void *HTS_calloc(size_t nmemb, size_t size)
{
   void *ptr = calloc(nmemb, size);

   if (ptr == NULL) {
      HTS_error(1, "HTS_calloc: Cannot allocate memory.\n");
      return NULL;
   }

   return ptr;
}

/* HTS_free: wrapper for free */
void HTS_free(void *ptr)
{
   free(ptr);
}

/* HTS_strdup: wrapper for strdup */
char *HTS_strdup(const char *s)
{
   char *p = strdup(s);

   if (p == NULL) {
      HTS_error(1, "HTS_strdup: Cannot duplicate string.\n");
      return NULL;
   }
   return p;
}

/* HTS_is_big_endian: check endian */
HTS_Boolean HTS_is_big_endian(void)
{
   int i = 1;
   char *p = (char *) &i;

   if (p[0] == 1)
      return HTS_FALSE;
   else
      return HTS_TRUE;
}

HTS_MISC_C_END;

#endif                          /* !HTS_MISC_C */
