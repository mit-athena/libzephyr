/* This file is part of the Project Athena Zephyr Notification System.
 * It contains source for the ZCheckIfNotice function.
 *
 *	Created by:	Robert French
 *
 *	$Source: /afs/dev.mit.edu/source/repository/athena/lib/zephyr/lib/ZCkIfNot.c,v $
 *	$Author: probe $
 *
 *	Copyright (c) 1987,1988 by the Massachusetts Institute of Technology.
 *	For copying and distribution information, see the file
 *	"mit-copyright.h". 
 */
/* $Header: /afs/dev.mit.edu/source/repository/athena/lib/zephyr/lib/ZCkIfNot.c,v 1.12 1993-09-24 16:18:57 probe Exp $ */

#ifndef lint
static char rcsid_ZCheckIfNotice_c[] = "$Id: ZCkIfNot.c,v 1.12 1993-09-24 16:18:57 probe Exp $";
#endif

#include <zephyr/zephyr_internal.h>

Code_t ZCheckIfNotice(notice, from, predicate, args)
    ZNotice_t *notice;
    struct sockaddr_in *from;
    register int (*predicate)();
    char *args;
{
    ZNotice_t tmpnotice;
    Code_t retval;
    register char *buffer;
    register struct _Z_InputQ *qptr;

    if ((retval = Z_ReadEnqueue()) != ZERR_NONE)
	return (retval);
	
    qptr = Z_GetFirstComplete();
    
    while (qptr) {
	if ((retval = ZParseNotice(qptr->packet, qptr->packet_len, 
				   &tmpnotice)) != ZERR_NONE)
	    return (retval);
	if ((*predicate)(&tmpnotice, args)) {
	    if (!(buffer = (char *) malloc((unsigned) qptr->packet_len)))
		return (ENOMEM);
	    _BCOPY(qptr->packet, buffer, qptr->packet_len);
	    if (from)
		*from = qptr->from;
	    if ((retval = ZParseNotice(buffer, qptr->packet_len, 
				       notice)) != ZERR_NONE) {
		free(buffer);
		return (retval);
	    }
	    Z_RemQueue(qptr);
	    return (ZERR_NONE);
	} 
	qptr = Z_GetNextComplete(qptr);
    }

    return (ZERR_NONOTICE);
}
