/*
 * Copyright (C) 2011 ADChips Inc. (http://www.adc.co.kr)
 * eagle_timer_api.c
 *
 * API for EAGLE Non Kernel Timer
 *
 * You can use these API for your timer based driver
 *
 *
 * Revision
 * V1.0: 2011/09/09
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "eagle_timer_api.h"

static int timer_dev = -1;

int eagle_timer_open (void)
{
    int handle;

    if ((handle = open(TIMER_DEV_NAME, O_RDWR)) < 0) {
        printf("Open Error %s\n", TIMER_DEV_NAME);
        return -1;
    }

    timer_dev = handle;

    return 0;
}

void eagle_timer_close (void)
{
    close(timer_dev);
}

int eagle_timer_config(int timer_num, unsigned int interval_usec)
{
    struct timer_config t_conf;

	if((timer_num != 2) && (timer_num != 3)) {
		printf("Error!, Timer Number that can be used is only 2 or 3.\n");
	}

    t_conf.timer = timer_num;
    t_conf.interval_usec = interval_usec;

    return ioctl(timer_dev, TIMER_CONFIG, &t_conf);
}

void eagle_timer_run(int timer)
{
    ioctl(timer_dev, TIMER_RUN, timer);
}

void eagle_timer_stop(int timer)
{
    ioctl(timer_dev, TIMER_STOP, timer);
}

void eagle_timer_int_enable(int timer)
{
    ioctl(timer_dev, TIMER_INT_ENABLE, timer);
}

void eagle_timer_int_disable(int timer)
{
    ioctl(timer_dev, TIMER_INT_DISABLE, timer);
}

/* Return Global Count Variable */
unsigned int eagle_timer_read_count(int timer)
{
    return ioctl(timer_dev, TIMER_READ_COUNT, timer);
}

/* Clear Global Count Variable */
void eagle_timer_count_clear(int timer)
{
    ioctl(timer_dev, TIMER_COUNT_CLEAR, timer);
}

/* Return Timer Current Count Register */
unsigned int eagle_timer_read_current_count(int timer)
{
    return ioctl(timer_dev, TIMER_READ_CUR_COUNT, timer);
}

