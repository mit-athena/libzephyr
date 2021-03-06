/* This file is part of the Project Athena Zephyr Notification System.
 * It is one of the source files comprising zwgc, the Zephyr WindowGram
 * client.
 *
 *      Created by:     Marc Horowitz <marc@athena.mit.edu>
 *
 *      $Id: character_class.c,v 1.8 1999-01-22 23:20:13 ghudson Exp $
 *
 *      Copyright (c) 1989 by the Massachusetts Institute of Technology.
 *      For copying and distribution information, see the file
 *      "mit-copyright.h".
 */

#include <sysdep.h>

#if (!defined(lint) && !defined(SABER))
static const char rcsid_character_class_c[] = "$Id: character_class.c,v 1.8 1999-01-22 23:20:13 ghudson Exp $";
#endif

#include <zephyr/mit-copyright.h>
#include <zephyr/zephyr.h>
#include "character_class.h"

/* 
 * It may look like we are passing the cache by value, but since it's
 * really an array we are passing by reference.  C strikes again....
 */

static character_class cache;

/* character_class */
char * string_to_character_class(str)
     string str;
{
    int i;

    (void) memset(cache, 0, sizeof(cache));

    for (i=0; i<strlen(str); i++)
      cache[str[i]] = 1;

    return(cache);
}
