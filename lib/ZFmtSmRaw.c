/* This file is part of the Project Athena Zephyr Notification System.
 * It contains source for the ZFormatSmallRawNotice function.
 *
 *	Created by:	Robert French
 *
 *	$Id: ZFmtSmRaw.c,v 1.8 1999-01-22 23:19:10 ghudson Exp $
 *
 *	Copyright (c) 1987 by the Massachusetts Institute of Technology.
 *	For copying and distribution information, see the file
 *	"mit-copyright.h". 
 */

#ifndef lint
static char rcsid_ZFormatRawNotice_c[] = "$Id: ZFmtSmRaw.c,v 1.8 1999-01-22 23:19:10 ghudson Exp $";
#endif

#include <internal.h>

Code_t ZFormatSmallRawNotice(notice, buffer, ret_len)
    ZNotice_t *notice;
    ZPacket_t buffer;
    int *ret_len;
{
    Code_t retval;
    int hdrlen;
    
    if ((retval = Z_FormatRawHeader(notice, buffer, Z_MAXHEADERLEN,
				    &hdrlen, NULL, NULL)) != ZERR_NONE)
	return (retval);

    *ret_len = hdrlen+notice->z_message_len;

    if (*ret_len > Z_MAXPKTLEN)
	return (ZERR_PKTLEN);

    (void) memcpy(buffer+hdrlen, notice->z_message, notice->z_message_len);

    return (ZERR_NONE);
}

Code_t ZNewFormatSmallRawNotice(notice, buffer, ret_len)
     ZNotice_t *notice;
     ZPacket_t buffer;
     int *ret_len;
{
  Code_t retval;
  int hdrlen;
  
  if ((retval = Z_AsciiFormatRawHeader(notice, buffer, Z_MAXHEADERLEN,
                                       &hdrlen, NULL, NULL, NULL, NULL)) 
      != ZERR_NONE)
    return (retval);
  
  *ret_len = hdrlen+notice->z_message_len;
  
  if (*ret_len > Z_MAXPKTLEN)
    return (ZERR_PKTLEN);
  
  (void) memcpy(buffer+hdrlen, notice->z_message, notice->z_message_len);
  
  return (ZERR_NONE);
}
