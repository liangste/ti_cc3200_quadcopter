#ifndef __MPU_6050_H__
#define __MPU_6050_H__

// reset MPU 6050
bool mpu6050_reset();

// detects whether MPU 6050 is reacheable and present on board
bool mpu6050_detect();

// perform a self test
bool mpu6050_selftest();

// initialize to various settings
bool mpu6050_init();

#endif // __MPU_6050_H__
