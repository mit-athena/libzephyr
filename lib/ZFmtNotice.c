/* This file is part of the Project Athena Zephyr Notification System.
 * It contains source for the ZFormatNotice function.
 *
 *	Created by:	Robert French
 *
 *	$Id: ZFmtNotice.c,v 1.17 1999-01-22 23:19:07 ghudson Exp $
 *
 *	Copyright (c) 1987,1988 by the Massachusetts Institute of Technology.
 *	For copying and distribution information, see the file
 *	"mit-copyright.h". 
 */

#ifndef lint
static char rcsid_ZFormatNotice_c[] = "$Id: ZFmtNotice.c,v 1.17 1999-01-22 23:19:07 ghudson Exp $";
#endif

#include <internal.h>

Code_t ZFormatNotice(notice, buffer, ret_len, cert_routine)
    register ZNotice_t *notice;
    char **buffer;
    int *ret_len;
    Z_AuthProc cert_routine;
{
    char header[Z_MAXHEADERLEN];
    int hdrlen;
    Code_t retval;

    if ((retval = Z_FormatHeader(notice, header, sizeof(header), &hdrlen, 
				 cert_routine)) != ZERR_NONE)
	return (retval);

    *ret_len = hdrlen+notice->z_message_len;

    /* Length can never be zero, don't have to worry about malloc(0). */
    if (!(*buffer = (char *) malloc((unsigned)*ret_len)))
	return (ENOMEM);

    (void) memcpy(*buffer, header, hdrlen);
    (void) memcpy(*buffer+hdrlen, notice->z_message, notice->z_message_len);

    return (ZERR_NONE);
}

Code_t ZNewFormatNotice(notice, buffer, ret_len, cert_routine)
    register ZNotice_t *notice;
    char **buffer;
    int *ret_len;
    Z_AuthProc cert_routine;
{
    char header[Z_MAXHEADERLEN];
    int hdrlen;
    Code_t retval;

    if ((retval = Z_NewFormatHeader(notice, header, sizeof(header), &hdrlen, 
				 cert_routine)) != ZERR_NONE)
	return (retval);

    *ret_len = hdrlen+notice->z_message_len;

    /* Length can never be zero, don't have to worry about malloc(0). */
    if (!(*buffer = (char *) malloc((unsigned)*ret_len)))
	return (ENOMEM);

    (void) memcpy(*buffer, header, hdrlen);
    (void) memcpy(*buffer+hdrlen, notice->z_message, notice->z_message_len);

    return (ZERR_NONE);
}
