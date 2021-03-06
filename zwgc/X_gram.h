/* This file is part of the Project Athena Zephyr Notification System.
 * It is one of the source files comprising zwgc, the Zephyr WindowGram
 * client.
 *
 *      Created by:     Marc Horowitz <marc@athena.mit.edu>
 *
 *      $Id: X_gram.h,v 1.8 1999-01-22 23:20:10 ghudson Exp $
 *
 *      Copyright (c) 1989 by the Massachusetts Institute of Technology.
 *      For copying and distribution information, see the file
 *      "mit-copyright.h".
 */


#include <zephyr/mit-copyright.h>

#ifndef x_gram_TYPE
#define x_gram_TYPE

#include <X11/Xlib.h>
#include <sys/time.h>

typedef struct _xblock {
   unsigned long fgcolor;
   Font fid;
   int x,y;
   int x1,y1,x2,y2; /* bounds of block.  used for cut and paste. */
   int strindex;
   int strlen;
} xblock;

typedef struct _x_gram {
   unsigned long bgcolor;
   int numblocks;
   xblock *blocks;
   char *text;
   struct _x_gram *below,*above;
   Window w;
   struct timeval can_die;
} x_gram;

typedef struct _xauxblock {
   int align;
   XFontStruct *font;
   char *str;
   int len;
   int width;
} xauxblock;

typedef struct _xmode {
   int bold;
   int italic;
   int size;
   int align;
   int expcolor;
   unsigned long color;
   char *substyle;
   char *font;
} xmode;

typedef struct _xlinedesc {
   int startblock;
   int numblock;
   int lsize;
   int csize;
   int rsize;
   int ascent;
   int descent;
} xlinedesc;

/* alignment values: */
#define LEFTALIGN   0
#define CENTERALIGN 1
#define RIGHTALIGN  2

extern void x_gram_init();
extern void x_gram_create();
extern void x_gram_expose();
extern void xshow();
extern void xcut();

#endif
