#ifndef __TIME_DELAY__
#define __TIME_DELAY__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <sys/time.h>
#include <errno.h>

extern void microseconds_sleep(unsigned long uSec);
extern void milliseconds_sleep(unsigned long mSec);
extern void seconds_sleep(unsigned long seconds);

#ifdef __cplusplus
}
#endif

#endif
