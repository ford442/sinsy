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

#include "HTS_engine.h"
#include "HTS_hidden.h"

/* HTS_show_copyright: show copyright */
void HTS_show_copyright(void)
{
   fprintf(stdout, "\n");
   fprintf(stdout, "The HMM-Based Speech Synthesis Engine \"hts_engine API\"\n");
   fprintf(stdout, "Version %s (%s)\n", HTS_VERSION, HTS_DATE);
   fprintf(stdout, "Copyright (C) 2001-2015 Nagoya Institute of Technology\n");
   fprintf(stdout, "              2001-2008 Tokyo Institute of Technology\n");
   fprintf(stdout, "All rights reserved.\n");
   fprintf(stdout, "\n");
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
void HTS_error(int error, const char *message, ...)
{
   va_list arg;

   fflush(stdout);
   fprintf(stderr, "\nError: ");
   va_start(arg, message);
   vfprintf(stderr, message, arg);
   va_end(arg);
   fflush(stderr);

   if (error > 0)
      exit(error);
}

/* HTS_fopen_from_fn: wrapper for fopen */
HTS_File *HTS_fopen_from_fn(const char *name, const char *opt)
{
   HTS_File *fp = (HTS_File *) calloc(1, sizeof(HTS_File));
   fp->type = 1;
   fp->pointer = fopen(name, opt);
   if (fp->pointer == NULL) {
      HTS_error(1, "HTS_fopen: Cannot open %s.\n", name);
      free(fp);
      return NULL;
   }
   return fp;
}

/* HTS_fclose: wrapper for fclose */
void HTS_fclose(HTS_File * fp)
{
   if (fp->type == 1)
      fclose((FILE *) fp->pointer);
   free(fp);
}

/* HTS_fgetc: wrapper for fgetc */
int HTS_fgetc(HTS_File * fp)
{
   if (fp->type == 1)
      return fgetc((FILE *) fp->pointer);
   return 0;
}

/* HTS_feof: wrapper for feof */
int HTS_feof(HTS_File * fp)
{
   if (fp->type == 1)
      return feof((FILE *) fp->pointer);
   return 0;
}

/* HTS_fseek: wrapper for fseek */
int HTS_fseek(HTS_File * fp, long offset, int origin)
{
   if (fp->type == 1)
      return fseek((FILE *) fp->pointer, offset, origin);
   return 0;
}

/* HTS_ftell: wrapper for ftell */
size_t HTS_ftell(HTS_File * fp)
{
#if defined(__ANDROID__) || defined(__EMSCRIPTEN__)
   if (fp->type == 1)
      return (size_t) ftell((FILE *) fp->pointer);
#else
   fpos_t pos;

   if (fp->type == 1) {
      if (fgetpos((FILE *) fp->pointer, &pos) != 0)
         return 0;
#ifdef __FreeBSD__
      return (size_t) pos;
#else
      return (size_t) pos.__pos;
#endif                          /* __FreeBSD__ */
   }
#endif                          /* __ANDROID__ || __EMSCRIPTEN__ */
   return 0;
}

/* HTS_fread_little_endian: fread with byteswap */
size_t HTS_fread_little_endian(void *buf, size_t size, size_t n, HTS_File * fp)
{
   if (fp->type == 1)
      return fread(buf, size, n, (FILE *) fp->pointer);
   return 0;
}

/* HTS_fwrite_little_endian: fwrite with byteswap */
size_t HTS_fwrite_little_endian(const void *buf, size_t size, size_t n, FILE * fp)
{
   return fwrite(buf, size, n, fp);
}

/* HTS_get_pattern_token: get pattern token (single/double quote can be used) */
HTS_Boolean HTS_get_pattern_token(HTS_File * fp, char *buff)
{
   char c;
   HTS_Boolean squote = HTS_FALSE, dquote = HTS_FALSE;
   size_t i = 0;

   c = HTS_fgetc(fp);
   while (HTS_feof(fp) != 1 && (c == ' ' || c == '\n'))
      c = HTS_fgetc(fp);

   if (c == '\'') {
      c = HTS_fgetc(fp);
      squote = HTS_TRUE;
   } else if (c == '\"') {
      c = HTS_fgetc(fp);
      dquote = HTS_TRUE;
   }

   if (HTS_feof(fp))
      return HTS_FALSE;

   while (HTS_feof(fp) != 1) {
      buff[i++] = c;
      c = HTS_fgetc(fp);
      if (squote && c == '\'')
         break;
      if (dquote && c == '\"')
         break;
      if (!squote && !dquote && (c == ' ' || c == '\n'))
         break;
   }
   buff[i] = '\0';
   return HTS_TRUE;
}

/* HTS_get_token_from_fp: get token from file pointer (separators are space,tab,line break) */
HTS_Boolean HTS_get_token_from_fp(HTS_File * fp, char *buff)
{
   char c;
   size_t i = 0;

   c = HTS_fgetc(fp);
   while (!HTS_feof(fp) && (c == ' ' || c == '\n' || c == '\t'))
      c = HTS_fgetc(fp);

   if (HTS_feof(fp))
      return HTS_FALSE;

   while (!HTS_feof(fp) && c != ' ' && c != '\n' && c != '\t') {
      buff[i++] = c;
      c = HTS_fgetc(fp);
   }
   buff[i] = '\0';
   return HTS_TRUE;
}

/* HTS_calloc: wrapper for calloc */
void *HTS_calloc(const size_t num, const size_t size)
{
   void *p = calloc(num, size);
   if (p == NULL)
      HTS_error(1, "HTS_calloc: Cannot allocate memory.\n");
   return p;
}

/* HTS_strdup: wrapper for strdup */
char *HTS_strdup(const char *string)
{
   char *p = strdup(string);
   if (p == NULL)
      HTS_error(1, "HTS_strdup: Cannot duplicate string.\n");
   return p;
}

/* HTS_free: wrapper for free */
void HTS_free(void *p)
{
   free(p);
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
