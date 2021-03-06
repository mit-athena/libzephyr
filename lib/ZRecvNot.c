/* This file is part of the Project Athena Zephyr Notification System.
 * It contains source for ZReceiveNotice function.
 *
 *	Created by:	Robert French
 *
 *	$Id: ZRecvNot.c,v 1.15 1999-01-22 23:19:21 ghudson Exp $
 *
 *	Copyright (c) 1987,1988 by the Massachusetts Institute of Technology.
 *	For copying and distribution information, see the file
 *	"mit-copyright.h". 
 */

#ifndef lint
static char rcsid_ZReceiveNotice_c[] = "$Id: ZRecvNot.c,v 1.15 1999-01-22 23:19:21 ghudson Exp $";
#endif

#include <internal.h>

Code_t ZReceiveNotice(notice, from)
    ZNotice_t *notice;
    struct sockaddr_in *from;
{
    char *buffer;
    struct _Z_InputQ *nextq;
    int len, auth;
    Code_t retval;

    if ((retval = Z_WaitForComplete()) != ZERR_NONE)
	return (retval);

    nextq = Z_GetFirstComplete();

    len = nextq->packet_len;
    
    if (!(buffer = (char *) malloc((unsigned) len)))
	return (ENOMEM);

    if (from)
	*from = nextq->from;
    
    (void) memcpy(buffer, nextq->packet, len);

    auth = nextq->auth;
    Z_RemQueue(nextq);
    
    if ((retval = ZParseNotice(buffer, len, notice)) != ZERR_NONE)
	return (retval);
    notice->z_checked_auth = auth;
    return ZERR_NONE;
}
