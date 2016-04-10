#include "config.h"

#ifndef __STABILIZER_H__
#define __STABILIZER_H__

typedef struct {
    double pitch;
    double roll;
    double yaw;
} angles_t;

// so that we can control throttle and develop through serial port
void SerialControlTask(void *params);

// main stabilization task
void StabilizerTask(void *params);

#endif
