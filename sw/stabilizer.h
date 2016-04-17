#include "config.h"

#ifndef __STABILIZER_H__
#define __STABILIZER_H__

// so that we can control throttle and develop through serial port
void SerialControlTask(void *params);

// main stabilization task
void StabilizerTask(void *params);

#endif
