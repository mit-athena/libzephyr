/* This file is part of the Project Athena Zephyr Notification System.
 * It contains code for the "zpopnotify" command.
 *
 *	Created by:	Robert French
 *
 *	$Source: /afs/dev.mit.edu/source/repository/athena/lib/zephyr/clients/zpopnotify/zpopnotify.c,v $
 *	$Author: jtkohl $
 *
 *	Copyright (c) 1987,1988 by the Massachusetts Institute of Technology.
 *	For copying and distribution information, see the file
 *	"mit-copyright.h". 
 */

#include <zephyr/mit-copyright.h>

#include <zephyr/zephyr.h>
#include <netdb.h>
#include <string.h>
#include <sys/param.h>			/* for MAXHOSTNAMELEN */

#ifndef lint
static char rcsid_zpopnotify_c[] = "$Header: /afs/dev.mit.edu/source/repository/athena/lib/zephyr/clients/zpopnotify/zpopnotify.c,v 1.4 1988-06-27 17:02:06 jtkohl Exp $";
#endif lint

#define MAIL_CLASS "MAIL"
#define MAIL_INSTANCE "POP"

main(argc,argv)
	int argc;
	char *argv[];
{
	char *rindex();
	
	ZNotice_t notice;
	struct hostent *hent;
	int retval;
	char *whoami,*ptr,myhost[MAXHOSTNAMELEN],mysender[BUFSIZ];
	char *lines[2];
	
	whoami = argv[0];

	if ((retval = ZInitialize()) != ZERR_NONE) {
		com_err(whoami,retval,"while initializing");
		exit(1);
	} 

	if (argc != 2) {
		usage(whoami);
		exit(1);
	}

	ptr = (char *)rindex(argv[1],'/');
	if (ptr)
		ptr++;
	else
		ptr = argv[1];

	if (gethostname(myhost,MAXHOSTNAMELEN) == -1) {
		com_err(whoami,errno,"Can't get hostname");
		exit(1);
	}
	myhost[MAXHOSTNAMELEN-1] = '\0';

	if (!(hent = gethostbyname(myhost))) {
		com_err(whoami,errno,"Can't get canonical hostname");
		exit(1);
	}

	(void) strncpy(myhost,hent->h_name,MAXHOSTNAMELEN);
	myhost[MAXHOSTNAMELEN-1] = '\0';

	lines[0] = myhost;
	lines[1] = "You have new mail.";
	
	(void) strcpy(mysender,"pop@");
	(void) strcat(mysender,ZGetRealm());

	(void) bzero((char *)&notice, sizeof(notice));
	notice.z_kind = UNSAFE;
	notice.z_class = MAIL_CLASS;
	notice.z_class_inst = MAIL_INSTANCE;
	notice.z_opcode = "";
	notice.z_sender = mysender;
	notice.z_recipient = ptr;
	notice.z_default_format = "From Post Office $1:\n$2";
	
	if ((retval = ZSendList(&notice,lines,2,ZNOAUTH)) != ZERR_NONE) {
		com_err(whoami,retval,"while sending notice");
		exit(1);
	} 
}

usage(whoami)
	char *whoami;
{
	printf("Usage: %s username\n",whoami);
}
