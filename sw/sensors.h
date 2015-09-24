#ifndef __GYRO_H__
#define __GYRO_H__

uint32_t g_sensorInitialized;
uint32_t g_sensorConnected;

struct float3x {
    float x;
    float y;
    float z;
};

struct short3x {
    short x;
    short y;
    short z;
};

void sensors_init(int*);
void sensors_update(struct float3x*, struct float3x*);
#define FREQ2TIX(X) ((unsigned int)((configTICK_RATE_HZ/X)))

#endif
