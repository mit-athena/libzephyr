#ifndef __HM_H__
#define __HM_H__
/* This file is part of the Project Athena Zephyr Notification System.
 * It contains the hostmanager header file.
 *
 *      Created by:     David C. Jedlinsky
 *
 *      $Source: /afs/dev.mit.edu/source/repository/athena/lib/zephyr/zhm/zhm.h,v $
 *      $Author: opus $
 *      $Header: /afs/dev.mit.edu/source/repository/athena/lib/zephyr/zhm/zhm.h,v 1.1 1987-06-29 20:38:59 opus Exp $
 *
 *      Copyright (c) 1987 by the Massachusetts Institute of Technology.
 *      For copying and distribution information, see the file
 *      "mit-copyright.h". 
 */

#include <zephyr/mit-copyright.h>
#include <zephyr/zephyr.h>

#ifdef DEBUG
#define DPR(a) fprintf(stderr, a); fflush(stderr)
#define DPR2(a,b) fprintf(stderr, a, b); fflush(stderr)
#else
#define DPR(a)
#define DPR2(a,b)
#endif

#define ever (;;)
#define Zperr(e) fprintf(stderr, "Error = %d\n", e)

#define SERV_TIMEOUT 20
#define NOTICE_TIMEOUT 5
#define BOOTING 1
#define NOTICES 2

extern char *malloc();

#endif !__HM_H__
