/*
 * $Id: stdio.h,v 1.2 2004-08-07 09:15:33 obarthel Exp $
 *
 * :ts=4
 *
 * Portable ISO 'C' (1994) runtime library for the Amiga computer
 * Copyright (c) 2002-2004 by Olaf Barthel <olsen@sourcery.han.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   - Neither the name of Olaf Barthel nor the names of contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _STDIO_H
#define _STDIO_H

/****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****************************************************************************/

#ifndef _STDDEF_H
#include <stddef.h>
#endif /* _STDDEF_H */

#ifndef _STDARG_H
#include <stdarg.h>
#endif /* _STDARG_H */

/****************************************************************************/

/* 'End of file' indicator returned by, for example, fgetc() */
#define EOF (-1)

/****************************************************************************/

/*
 * Maximum number of files that can be open at a time. This number does not
 * correspond to a real limitation for this 'C' runtime library and is
 * included solely for ISO 'C' (1994) compliance.
 */
#define FOPEN_MAX 8

/****************************************************************************/

/* Maximum size of a file/path name. */
#define FILENAME_MAX 256

/****************************************************************************/

/* Default size for all standard I/O file buffers. */
#define BUFSIZ 8192

/****************************************************************************/

/* File buffering modes for use with setvbuf() */

#define _IOFBF 0	/* Full buffering (flush when buffer is full) */

#define _IOLBF 1	/* Line buffering (flush when buffer is full or when
					   a line feed character is written) */

#define _IONBF 2	/* Perform no buffering at all */

/****************************************************************************/

/* Positioning modes for use with fseek() */

#define SEEK_SET 0	/* New position is absolute */
#define SEEK_CUR 1	/* New position is relative to current file position */
#define SEEK_END 2	/* New position is relative to end of file */

/****************************************************************************/

/* Used by fgetpos() and fsetpos() */

#if defined(__GNUC__)
typedef long long fpos_t;
#else
typedef long fpos_t;
#endif /* __GNUC__ */

/****************************************************************************/

/*
 * This is part of the internal 'FILE' structure; this is guaranteed not to
 * change in future library releases. However, the private portion of this
 * data structure may change.
 */
typedef struct
{
	unsigned long	flags;				/* See below for some of the public
										   flag bits defined; this is by no
										   means a complete list, though! */
	unsigned char *	buffer;				/* Points to the first byte of the
										   buffer; this could be NULL! */
	long			size;				/* How many bytes will fit into the
										   buffer; this could be 0! */
	long			position;			/* Current buffer position, which is
										   usually a number between 0 and
										   size-1 */
	long			num_read_bytes;		/* How many bytes can be read from
										   the buffer; this can be 0! */
	long			num_write_bytes;	/* How many bytes have been copied
										   to the buffer which have not been
										   written back yet; this can be 0! */

	/* Private fields follow... */
} FILE;

/****************************************************************************/

/* Field and flag definitions for the getc/putc macros below. */
#define __FILE_BUFFER_MASK	3		/* Masks off the buffering mode */
#define __FILE_EOF			(1<<2)	/* EOF reached */
#define __FILE_READABLE		(1<<3)	/* File is readable */
#define __FILE_WRITABLE		(1<<4)	/* File is writable */
#define __FILE_IN_USE		(1<<5)	/* File is in use */
#define __FILE_ERROR		(1<<6)	/* Error detected */

/****************************************************************************/

/*
 * Maximum file name buffer size for use with tmpfile() and tmpnam();
 * note that the maximum file name length is shorter by one character
 */
#define L_tmpnam 10

/* Maximum number of unique file names tmpnam() can generate */
#define TMP_MAX 0x3ffff

/****************************************************************************/

/* Anchor for the buffered standard I/O streams */
extern struct iob ** __iob;

/****************************************************************************/

/* The three standard I/O streams */
#define stdin	((FILE *)__iob[0])
#define stdout	((FILE *)__iob[1])
#define stderr	((FILE *)__iob[2])

/****************************************************************************/

extern void perror(const char * s);

/****************************************************************************/

extern FILE *fopen(const char *filename, const char *mode);
extern int fclose(FILE *stream);
extern int fflush(FILE *stream);
extern FILE *freopen(const char *filename, const char *mode, FILE *stream);
extern int setvbuf(FILE *stream,char *buf,int bufmode,size_t size);
extern void setbuf(FILE *stream, char *buf);

/****************************************************************************/

extern int fseek(FILE *stream, long int offset, int wherefrom);
extern long int ftell(FILE *stream);
extern void rewind(FILE *stream);
extern int fgetpos(FILE *stream, fpos_t *pos);
extern int fsetpos(FILE *stream, fpos_t *pos);

/****************************************************************************/

extern int fgetc(FILE *stream);
extern int getc(FILE *stream);
extern int getchar(void);
extern int ungetc(int c,FILE *stream);

/****************************************************************************/

extern int fputc(int c,FILE *stream);
extern int putc(int c,FILE *stream);
extern int putchar(int c);

/****************************************************************************/

extern char *fgets(char *s,int n,FILE *stream);
extern char *gets(char *s);

/****************************************************************************/

extern int fscanf(FILE *stream, const char *format, ...);
extern int scanf(const char *format, ...);
extern int sscanf(const char *s,const char *format, ...);

/****************************************************************************/

extern int fputs(const char *s, FILE *stream);
extern int puts(const char *s);

/****************************************************************************/

extern int fprintf(FILE *stream,const char *format,...);
extern int printf(const char *format, ...);
extern int sprintf(char *s, const char *format, ...);

/****************************************************************************/

extern int vfprintf(FILE *stream,const char *format,va_list arg);
extern int vprintf(const char *format,va_list arg);
extern int vsprintf(char *s, const char *format,va_list arg);

/****************************************************************************/

extern size_t fread(void *ptr,size_t element_size,size_t count,FILE *stream);
extern size_t fwrite(const void *ptr,size_t element_size,size_t count,FILE *stream);

/****************************************************************************/

extern int feof(FILE *stream);
extern int ferror(FILE *stream);
extern void clearerr(FILE *stream);

/****************************************************************************/

extern int rename(const char *oldname,const char *newname);
extern int remove(const char *filename);

/****************************************************************************/

extern FILE *tmpfile(void);
extern char *tmpnam(char *buf);

/****************************************************************************/

/*
 * A special buffer flush routine which returns the last character written
 * in case of success and EOF in case of failure. This is used by the
 * putc() macro defined below.
 */
extern int __flush(FILE *stream);

/****************************************************************************/

/*
 * fgetc() implemented as a "simple" macro; note that fgetc() does much more than
 * can be conveniently expressed as a macro!
 */
#define getc(f) \
	(((((FILE *)(f))->flags & (__FILE_IN_USE|__FILE_READABLE|__FILE_EOF)) == (__FILE_IN_USE|__FILE_READABLE) && \
	 (((FILE *)(f))->flags & __FILE_BUFFER_MASK) != _IONBF && \
	 ((FILE *)(f))->position < ((FILE *)(f))->num_read_bytes) ? \
	    ((FILE *)(f))->buffer[((FILE *)(f))->position++] : \
	    fgetc(f))

/****************************************************************************/

/*
 * fputc() implemented as a "simple" macro; note that fputc() does much more than
 * can be conveniently expressed as a macro!
 */
#define putc(c,f) \
	(((((FILE *)(f))->flags & (__FILE_IN_USE|__FILE_WRITABLE)) == (__FILE_IN_USE|__FILE_WRITABLE) && \
	 (((FILE *)(f))->flags & __FILE_BUFFER_MASK) != _IONBF && \
	 (((FILE *)(f))->num_write_bytes < ((FILE *)(f))->size)) ? \
	  (((FILE *)(f))->buffer[((FILE *)(f))->num_write_bytes++] = (c), \
	  (((((FILE *)(f))->flags & __FILE_BUFFER_MASK) == _IOLBF && \
	  ((FILE *)(f))->buffer[((FILE *)(f))->num_write_bytes-1] == '\n') ? \
	   __flush(f) : \
	   (((FILE *)(f))->buffer[((FILE *)(f))->num_write_bytes-1]))) : \
	   fputc((c),(f)))

/****************************************************************************/

/*
 * If requested, reimplement some of the file I/O routines as macros.
 */

#ifdef __C_MACROS__

/****************************************************************************/

#define getchar()		getc(stdin)
#define putchar(c)		putc((c),stdout)
#define clearerr(file)	((void)((file)->flags &= ~(__FILE_EOF|__FILE_ERROR)))
#define feof(file)		(((file)->flags & __FILE_EOF) != 0)
#define ferror(file)	(((file)->flags & __FILE_ERROR) != 0)

/****************************************************************************/

#endif /* __C_MACROS__ */

/****************************************************************************/

/* The following is not part of the ISO 'C' (1994) standard. */

/****************************************************************************/

#define MAXPATHLEN (4 * FILENAME_MAX)

/****************************************************************************/

extern FILE * fdopen(int file_descriptor, const char * type);
extern int fileno(FILE * file);
extern int asprintf(char **ret, const char *format, ...);
extern int snprintf(char *s,size_t size,const char *format,...);
extern int vsnprintf(char *s,size_t size,const char *format,va_list arg);
extern int pclose(FILE *stream);
extern FILE * popen(const char *command, const char *type);

/****************************************************************************/

extern int vasprintf(char **ret,const char *format,va_list arg);

/* This is the version for use with memory debugging; do not call
   it directly! */
extern int __vasprintf(const char *file,int line,char **ret,const char *format,va_list arg);

#ifdef __MEM_DEBUG
#define vasprintf(ret,format,arg) __vasprintf(__FILE__,__LINE__,(ret),(format),(arg))
#endif /* __MEM_DEBUG */

/****************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

/****************************************************************************/

#endif /* _STDIO_H */
