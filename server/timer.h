/* This file is part of the Project Athena Zephyr Notification System.
 * It contains definitions used by timer.c
 *
 *	Created by:	John T. Kohl
 *	Derived from timer_manager_.h by Ken Raeburn
 *
 *	$Source: /afs/dev.mit.edu/source/repository/athena/lib/zephyr/server/timer.h,v $
 *	$Author: raeburn $
 *	$Header: /afs/dev.mit.edu/source/repository/athena/lib/zephyr/server/timer.h,v 1.6 1990-11-13 17:06:23 raeburn Exp $
 *
 */

/*
 * timer_manager_ -- routines for handling timers in login_shell
 * (and elsewhere)
 *
 * Copyright 1986 Student Information Processing Board,
 * Massachusetts Institute of Technology
 *
 * written by Ken Raeburn

Permission to use, copy, modify, and distribute this
software and its documentation for any purpose and without
fee is hereby granted, provided that the above copyright
notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting
documentation, and that the name of M.I.T. and the Student
Information Processing Board not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.
M.I.T. and the Student Information Processing Board
make no representations about the suitability of
this software for any purpose.  It is provided "as is"
without express or implied warranty.

 */

typedef struct _timer {
	struct _timer 	*next;		/*  Next one to go off.. */
	struct _timer   *prev;		/*  Previous one to go off.. */
	/* time for timer to go off, absolute time */
	long 	alarm_time;
	/* procedure to call when timer goes off */
	void 	(*func)(void*);
	/* argument for that procedure */
	void *	arg;
} *timer;

#define ALARM_TIME(x) ((x)->alarm_time)
#define ALARM_FUNC(x) ((x)->func)
#define ALARM_NEXT(x) ((x)->next)
#define ALARM_PREV(x) ((x)->prev)
#define ALARM_ARG(x)  ((x)->arg)
#define TIMER_SIZE sizeof(struct _timer)

#ifdef mips
#define time_t long /* sigh */
#endif
extern "C" time_t time(time_t*);
#define NOW (time((time_t *)NULL))
typedef void (*timer_proc) (void *);
extern timer timer_set_rel(long, timer_proc, void*);
extern timer timer_set_abs(long, timer_proc, void*);
extern void timer_reset(timer), timer_process(void);

#define	timer_when(x)	ALARM_TIME(x)

extern long nexttimo;			/* Unix time of next timout */
