/* This file is part of the Project Athena Zephyr Notification System.
 * It contains source for the ZSendNotice function.
 *
 *	Created by:	Robert French
 *
 *	$Source: /afs/dev.mit.edu/source/repository/athena/lib/zephyr/lib/ZSendNot.c,v $
 *	$Author: ghudson $
 *
 *	Copyright (c) 1987 by the Massachusetts Institute of Technology.
 *	For copying and distribution information, see the file
 *	"mit-copyright.h". 
 */
/* $Header: /afs/dev.mit.edu/source/repository/athena/lib/zephyr/lib/ZSendNot.c,v 1.12 1997-09-14 21:52:57 ghudson Exp $ */

#ifndef lint
static char rcsid_ZSendNotice_c[] = "$Header: /afs/dev.mit.edu/source/repository/athena/lib/zephyr/lib/ZSendNot.c,v 1.12 1997-09-14 21:52:57 ghudson Exp $";
#endif

#include <internal.h>

Code_t ZSendNotice(notice, cert_routine)
    ZNotice_t *notice;
    Z_AuthProc cert_routine;
{
    return(ZSrvSendNotice(notice, cert_routine, Z_XmitFragment));
}

Code_t ZSrvSendNotice(notice, cert_routine, send_routine)
    ZNotice_t *notice;
    Z_AuthProc cert_routine;
    Code_t (*send_routine)();
{    
    Code_t retval;
    ZNotice_t newnotice;
    char *buffer;
    int len;

    if ((retval = ZFormatNotice(notice, &buffer, &len, 
				cert_routine)) != ZERR_NONE)
	return (retval);

    if ((retval = ZParseNotice(buffer, len, &newnotice)) != ZERR_NONE)
	return (retval);
    
    retval = Z_SendFragmentedNotice(&newnotice, len, cert_routine,
				    send_routine);

    free(buffer);

    return (retval);
}
