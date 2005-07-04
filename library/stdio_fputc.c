/*
 * $Id: stdio_fputc.c,v 1.7 2005-05-29 09:56:09 obarthel Exp $
 *
 * :ts=4
 *
 * Portable ISO 'C' (1994) runtime library for the Amiga computer
 * Copyright (c) 2002-2005 by Olaf Barthel <olsen@sourcery.han.de>
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

#ifndef _STDLIB_NULL_POINTER_CHECK_H
#include "stdlib_null_pointer_check.h"
#endif /* _STDLIB_NULL_POINTER_CHECK_H */

/****************************************************************************/

#ifndef _STDIO_HEADERS_H
#include "stdio_headers.h"
#endif /* _STDIO_HEADERS_H */

/****************************************************************************/

int
__fputc_check(FILE *stream)
{
	struct iob * file = (struct iob *)stream;
	int result = EOF;

	assert( stream != NULL );

	#if defined(CHECK_FOR_NULL_POINTERS)
	{
		if(stream == NULL)
		{
			__set_errno(EFAULT);
			goto out;
		}
	}
	#endif /* CHECK_FOR_NULL_POINTERS */

	assert( __is_valid_iob(file) );
	assert( FLAG_IS_SET(file->iob_Flags,IOBF_IN_USE) );
	assert( file->iob_BufferSize > 0 );

	if(FLAG_IS_CLEAR(file->iob_Flags,IOBF_IN_USE))
	{
		SHOWMSG("this file is not even in use");

		SET_FLAG(file->iob_Flags,IOBF_ERROR);

		__set_errno(EBADF);

		goto out;
	}

	if(FLAG_IS_CLEAR(file->iob_Flags,IOBF_WRITE))
	{
		SHOWMSG("this stream is not write enabled");

		SET_FLAG(file->iob_Flags,IOBF_ERROR);

		__set_errno(EBADF);

		goto out;
	}

	if(__iob_read_buffer_is_valid(file) && __drop_iob_read_buffer(file) < 0)
	{
		SHOWMSG("couldn't get rid of the read buffer.");
		goto out;
	}

	result = OK;

 out:

	return(result);
}

/****************************************************************************/

int
__fputc(int c,FILE *stream,int buffer_mode)
{
	struct iob * file = (struct iob *)stream;
	int result = EOF;

	assert( stream != NULL );

	assert( FLAG_IS_SET(file->iob_Flags,IOBF_IN_USE) );
	assert( file->iob_BufferSize > 0 );

	if(__iob_write_buffer_is_full(file) && __flush_iob_write_buffer(file) < 0)
		goto out;

	file->iob_Buffer[file->iob_BufferWriteBytes++] = c;

	if((buffer_mode == IOBF_BUFFER_MODE_NONE || (buffer_mode == IOBF_BUFFER_MODE_LINE && c == '\n')) && __flush_iob_write_buffer(file) < 0)
	{
		/* Pretend that the last character was not written. */
		file->iob_BufferWriteBytes--;
		goto out;
	}

	/* Clip everything but the least significant eight bits. This
	   also takes care of the sign. Thus, a -1 (== EOF) always comes
	   out as 255. */
	result = (c & 255);

 out:

	return(result);
}

/****************************************************************************/

int
fputc(int c,FILE *stream)
{
	struct iob * file = (struct iob *)stream;
	int result = EOF;

	assert( stream != NULL );

	if(__check_abort_enabled)
		__check_abort();

	assert( FLAG_IS_SET(file->iob_Flags,IOBF_IN_USE) );

	flockfile(stream);

	if(__fputc_check(stream) < 0)
		goto out;

	result = __fputc(c,stream,(file->iob_Flags & IOBF_BUFFER_MODE));

 out:

	funlockfile(stream);

	return(result);
}