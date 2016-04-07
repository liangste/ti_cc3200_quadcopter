#ifndef __MPU_6050_H__
#define __MPU_6050_H__

/*/////////////////////////////////////////////////////////////////////////////
    Defines
/////////////////////////////////////////////////////////////////////////////*/

#define MPU6050_I2C_ADDR        0x68
#define I2C_BUF_SIZE            256

#define WHOAMI_VAL              0x68
#define PWR_MGMT_2_DEFAULT_VAL  0x40
#define SENSOR_DATA_SIZE        14
#define TRIM_TOLERANCE          14.0
#define ACCEL_SENSITIVITY       8192.0
#define GYRO_SENSITIVITY        65.5

// registers
#define SELF_TEST_X             0x0D
#define SELF_TEST_Y             0x0E
#define SELF_TEST_Z             0x0F
#define SELF_TEST_A             0x10
#define SMPLRT_DIV_REG          0x19
#define CONFIG_REG              0x1A
#define GYRO_CONFIG             0x1B
#define ACCEL_CONFIG            0x1C
#define SENSOR_DATA_BASE        0x3B
#define WHOAMI_REG              0x75
#define PWR_MGMT_1_REG          0x6B
#define PWR_MGMT_2_REG          0x6C

// accoridng to section 4 of "MPU-6000/MPU-6050 Register Maps and Description"
#define CALC_FT_G(a)            (25.0 * 131 * pow(1.046, a - 1))
#define CALC_FT_A(a)            (4096 * .34 * pow(0.92/0.34, (a - 1) / 30.0))

typedef struct {
    int16_t ax;
    int16_t ay;
    int16_t az;
    int16_t gx;
    int16_t gy;
    int16_t gz;
} sensor_value_t;

// reset MPU 6050
bool mpu6050_reset();

// detects whether MPU 6050 is reacheable and present on board
bool mpu6050_detect();

// perform a self test
bool mpu6050_selftest();

// initialize to various settings
bool mpu6050_init();

// update acceleromter and gyroscope readings
bool mpu6050_update_readings(sensor_value_t* val);

#endif // __MPU_6050_H__
