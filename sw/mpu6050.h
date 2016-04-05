#ifndef __MPU_6050_H__
#define __MPU_6050_H__

// detects whether MPU 6050 is reacheable and present on board
bool mpu6050_detect();

// initialize to various settings
bool mpu6050_init();

#endif // __MPU_6050_H__
