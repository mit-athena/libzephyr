/* This file is part of the Project Athena Zephyr Notification System.
 * It contains source for ZPeekPacket function.
 *
 *	Created by:	Robert French
 *
 *	$Id: ZPeekPkt.c,v 1.13 1999-01-22 23:19:20 ghudson Exp $
 *
 *	Copyright (c) 1987 by the Massachusetts Institute of Technology.
 *	For copying and distribution information, see the file
 *	"mit-copyright.h". 
 */

#ifndef lint
static char rcsid_ZPeekPacket_c[] = "$Id: ZPeekPkt.c,v 1.13 1999-01-22 23:19:20 ghudson Exp $";
#endif

#include <internal.h>

Code_t ZPeekPacket(buffer, ret_len, from)
    char **buffer;
    int *ret_len;
    struct sockaddr_in *from;
{
    Code_t retval;
    struct _Z_InputQ *nextq;
    
    if ((retval = Z_WaitForComplete()) != ZERR_NONE)
	return (retval);

    nextq =Z_GetFirstComplete();

    *ret_len = nextq->packet_len;
    
    if (!(*buffer = (char *) malloc((unsigned) *ret_len)))
	return (ENOMEM);

    (void) memcpy(*buffer, nextq->packet, *ret_len);

    if (from)
	*from = nextq->from;
	
    return (ZERR_NONE);
}
