/*
 * $Id: string_strspn.c,v 1.3 2005-02-03 16:56:17 obarthel Exp $
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

#ifndef _STRING_HEADERS_H
#include "string_headers.h"
#endif /* _STRING_HEADERS_H */

/****************************************************************************/

size_t
strspn(const char *s, const char *set)
{
	const char * start = s;
	size_t result = 0;
	char c;

	assert(s != NULL && set != NULL);

	#if defined(CHECK_FOR_NULL_POINTERS)
	{
		if(s == NULL || set == NULL)
		{
			__set_errno(EFAULT);
			goto out;
		}
	}
	#endif /* CHECK_FOR_NULL_POINTERS */

	while((c = (*s)) != '\0')
	{
		/* Is this character found in the set? */
		if(strchr(set,c) == NULL)
			break; /* Stop if it's not in there. */

		s++;
	}

	result = (size_t)(s - start);

 out:

	return(result);
}