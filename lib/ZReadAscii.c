/* This file is part of the Project Athena Zephyr Notification System.
 * It contains source for the ZParseNotice function.
 *
 *	Created by:	Robert French
 *
 *	$Source: /afs/dev.mit.edu/source/repository/athena/lib/zephyr/lib/ZReadAscii.c,v $
 *	$Author: rfrench $
 *
 *	Copyright (c) 1987 by the Massachusetts Institute of Technology.
 *	For copying and distribution information, see the file
 *	"mit-copyright.h". 
 */
/* $Header: /afs/dev.mit.edu/source/repository/athena/lib/zephyr/lib/ZReadAscii.c,v 1.4 1987-06-29 03:13:49 rfrench Exp $ */

#include <zephyr/mit-copyright.h>

#include <zephyr/zephyr_internal.h>

int ZReadAscii(ptr,len,field,num)
	char *ptr;
	int len;
	char *field;
	int num;
{
	int i;
	char bfr[3];

	for (i=0;i<num;i++) {
		if (*ptr == ' ') {
			ptr++;
			if (--len < 1)
				return (ZERR_BADFIELD);
		} 
		if (ptr[0] == '0' && ptr[1] == 'x') {
			ptr += 2;
			len -= 2;
			if (len < 1)
				return (ZERR_BADFIELD);
		} 
		bfr[0] = ptr[0];
		bfr[1] = ptr[1];
		bfr[2] = '\0';
		if (!bfr[0] || !bfr[1])
			return (ZERR_BADFIELD);
		sscanf(bfr,"%x",field+i);
		ptr += 2;
		len -= 2;
		if (len < 1)
			return (ZERR_BADFIELD);
	}

	if (*ptr)
		return (ZERR_BADFIELD);

	return (ZERR_NONE);
}
