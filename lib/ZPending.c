/* This file is part of the Project Athena Zephyr Notification System.
 * It contains source for the ZPending function.
 *
 *	Created by:	Robert French
 *
 *	$Id: ZPending.c,v 1.8 1999-01-22 23:19:20 ghudson Exp $
 *
 *	Copyright (c) 1987 by the Massachusetts Institute of Technology.
 *	For copying and distribution information, see the file
 *	"mit-copyright.h". 
 */

#ifndef lint
static char rcsid_ZPending_c[] = "$Id: ZPending.c,v 1.8 1999-01-22 23:19:20 ghudson Exp $";
#endif

#include <internal.h>

int ZPending()
{
	int retval;
	
	if (ZGetFD() < 0) {
		errno = ZERR_NOPORT;
		return (-1);
	}
	
	if ((retval = Z_ReadEnqueue()) != ZERR_NONE) {
		errno = retval;
		return (-1);
	} 
	
	return(ZQLength());
}
