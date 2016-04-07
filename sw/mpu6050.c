// Standard includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

// Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_i2c.h"
#include "rom.h"
#include "rom_map.h"
#include "interrupt.h"
#include "prcm.h"
#include "utils.h"
#include "uart.h"
#include "i2c.h"

// Common interface includes
#include "common.h"
#include "uart_if.h"
#include "i2c_if.h"

// FreeRTOS includes
#include "osi.h"
#include "FreeRTOS.h"
#include "task.h"

// local includes
#include "pinmux.h"
#include "mpu6050.h"
#include "led.h"
#include "config.h"

/*/////////////////////////////////////////////////////////////////////////////
    Variables
/////////////////////////////////////////////////////////////////////////////*/

unsigned char g_i2c_read_buf[I2C_BUF_SIZE];
unsigned char g_i2c_write_buf[I2C_BUF_SIZE];

/*/////////////////////////////////////////////////////////////////////////////
    Function Prototypes
/////////////////////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////////////////////
    Function Definitions
/////////////////////////////////////////////////////////////////////////////*/

bool mpu6050_reset() {
    unsigned u;

    if (SUCCESS < I2C_IF_Open(I2C_MASTER_MODE_FST)) {
        FAIL("Failed to open I2C in Fast Mode");
        return false;
    }

    u = 0;
    g_i2c_write_buf[u++] = PWR_MGMT_1_REG;
    g_i2c_write_buf[u++] = 0x80; // reset

    if (SUCCESS < I2C_IF_Write(MPU6050_I2C_ADDR, &g_i2c_write_buf, u, 1)) {
        FAIL("I2C_IF_ReadFrom Failed");
        return false;
    }

    return true;
}

bool mpu6050_detect() {
    unsigned char reg_addr = WHOAMI_REG;

    if (SUCCESS < I2C_IF_Open(I2C_MASTER_MODE_FST)) {
        FAIL("Failed to open I2C in Fast Mode");
        return false;
    }

    if (SUCCESS < I2C_IF_ReadFrom(MPU6050_I2C_ADDR, &reg_addr, 1, &g_i2c_read_buf, 1)) {
        FAIL("I2C_IF_ReadFrom Failed");
        return false;
    }

    if (g_i2c_read_buf[0] == WHOAMI_VAL) {
        INFO("WHOAMI_VAL Matches");
    } else {
        FAIL("WHOAMI_VAL does not match");
        return false;
    }

    return true;
}

int32_t _sign_extend(int16_t sint) {
    int32_t value = (0x0000FFFF & sint);
    int32_t mask = 0x00008000;
    int32_t sign = (mask & sint) >> 15;
    if (sign == 1)
        value += 0xFFFF0000;
    return value;
}

bool _check_tolerance(char* comp, float val) {
    if (fabs(val) < TRIM_TOLERANCE) {
        UART_PRINT("%s(%f) within tolerance\r\n", comp, val);
        return true;
    } else {
        UART_PRINT("%s(%f) NOT within tolerance\r\n", comp, val);
        return false;
    }
}

bool mpu6050_selftest() {
    unsigned u;
    uint8_t xa_test, ya_test, za_test;
    uint8_t xg_test, yg_test, zg_test;
    float ft_xa, ft_ya, ft_za;
    float ft_xg, ft_yg, ft_zg;
    float d_xa, d_ya, d_za;
    float d_xg, d_yg, d_zg;
    unsigned char reg_addr;
    sensor_value_t sensor_value_old;
    sensor_value_t sensor_value_new;

    if (SUCCESS < I2C_IF_Open(I2C_MASTER_MODE_FST)) {
        FAIL("Failed to open I2C in Fast Mode");
        return false;
    }

    reg_addr = SELF_TEST_X;
    if (SUCCESS < I2C_IF_ReadFrom(MPU6050_I2C_ADDR, &reg_addr, 1, &g_i2c_read_buf, 4)) {
        FAIL("I2C_IF_ReadFrom 4-byte Failed");
        return false;
    }

    xg_test = g_i2c_read_buf[0] & 0x1F;
    xa_test = (g_i2c_read_buf[0] & 0xE0) >> 3;

    yg_test = g_i2c_read_buf[1] & 0x1F;
    ya_test = (g_i2c_read_buf[1] & 0xE0) >> 3;

    zg_test = g_i2c_read_buf[2] & 0x1F;
    za_test = (g_i2c_read_buf[2] & 0xE0) >> 3;

    za_test |= g_i2c_read_buf[3] & 0x03;
    ya_test |= (g_i2c_read_buf[3] >> 2) & 0x03;
    xa_test |= (g_i2c_read_buf[3] >> 4) & 0x03;

    ft_xg = (xg_test == 0)? 0 : CALC_FT_G(xg_test);
    ft_yg = (yg_test == 0)? 0 : -CALC_FT_G(yg_test);
    ft_zg = (zg_test == 0)? 0 : CALC_FT_G(zg_test);
    ft_xa = (xa_test == 0)? 0 : CALC_FT_A(xa_test);
    ft_ya = (ya_test == 0)? 0 : CALC_FT_A(ya_test);
    ft_za = (za_test == 0)? 0 : CALC_FT_A(za_test);

    // power management stuff
    u = 0;
    g_i2c_write_buf[u++] = PWR_MGMT_1_REG;
    g_i2c_write_buf[u++] = 0x09;
    if (SUCCESS < I2C_IF_Write(MPU6050_I2C_ADDR, &g_i2c_write_buf, u, 1)) {
        FAIL("I2C_IF_Write to PWR_MGMT_1_REG Failed");
        return false;
    }

    // turn on self test
    u = 0;
    g_i2c_write_buf[u++] = GYRO_CONFIG;
    g_i2c_write_buf[u++] = 0xE0; // turn on self-test, FSR set to 250
    if (SUCCESS < I2C_IF_Write(MPU6050_I2C_ADDR, &g_i2c_write_buf, u, 1)) {
        FAIL("I2C_IF_Write to GYRO_CONFIG Failed");
        return false;
    }

    u = 0;
    g_i2c_write_buf[u++] = ACCEL_CONFIG;
    g_i2c_write_buf[u++] = 0xF0; // turn on self-test, FSR set to 8
    if (SUCCESS < I2C_IF_Write(MPU6050_I2C_ADDR, &g_i2c_write_buf, u, 1)) {
        FAIL("I2C_IF_Write to ACCEL_CONFIG Failed");
        return false;
    }

    // add small delay for self-test to complete
    MAP_UtilsDelay((90*80*1000)); // 30msec

    mpu6050_update_readings(&sensor_value_old);

    // turn off self test
    u = 0;
    g_i2c_write_buf[u++] = GYRO_CONFIG;
    g_i2c_write_buf[u++] = 0x00; // turn off self-test, leave FSR
    if (SUCCESS < I2C_IF_Write(MPU6050_I2C_ADDR, &g_i2c_write_buf, u, 1)) {
        FAIL("I2C_IF_Write to GYRO_CONFIG Failed");
        return false;
    }

    u = 0;
    g_i2c_write_buf[u++] = ACCEL_CONFIG;
    g_i2c_write_buf[u++] = 0x10; // turn off self-test, leave FSR
    if (SUCCESS < I2C_IF_Write(MPU6050_I2C_ADDR, &g_i2c_write_buf, u, 1)) {
        FAIL("I2C_IF_Write to ACCEL_CONFIG Failed");
        return false;
    }

    MAP_UtilsDelay((90*80*1000)); // 30msec

    mpu6050_update_readings(&sensor_value_new);

    sensor_value_old.ax -= sensor_value_new.ax;
    sensor_value_old.ay -= sensor_value_new.ay;
    sensor_value_old.az -= sensor_value_new.az;
    sensor_value_old.gx -= sensor_value_new.gx;
    sensor_value_old.gy -= sensor_value_new.gy;
    sensor_value_old.gz -= sensor_value_new.gz;

    d_xa = (sensor_value_old.ax - ft_xa) / ft_xa * 100.0;
    d_ya = (sensor_value_old.ay - ft_ya) / ft_ya * 100.0;
    d_za = (sensor_value_old.az - ft_za) / ft_za * 100.0;
    d_xg = (sensor_value_old.gx - ft_xg) / ft_xg * 100.0;
    d_yg = (sensor_value_old.gy - ft_yg) / ft_yg * 100.0;
    d_zg = (sensor_value_old.gz - ft_zg) / ft_zg * 100.0;

    INFO("Testing for self test tolerances");
    if (!_check_tolerance("Accelerometer.x-axis", d_xa)
        || !_check_tolerance("Accelerometer.y-axis", d_ya)
        || !_check_tolerance("Accelerometer.z-axis", d_za)
        || !_check_tolerance("Gyroscope.x-axis", d_xg)
        || !_check_tolerance("Gyroscope.y-axis", d_yg)
        || !_check_tolerance("Gyroscope.z-axis", d_zg)) {
        FAIL("Tolerance check failed");
        return false;
    }

    return true;
}

bool mpu6050_update_readings(sensor_value_t* val) {
    uint8_t reg_addr;

    reg_addr = SENSOR_DATA_BASE;
    if (SUCCESS < I2C_IF_ReadFrom(MPU6050_I2C_ADDR, &reg_addr, 1, &g_i2c_read_buf, SENSOR_DATA_SIZE)) {
        FAIL("I2C_IF_ReadFrom 14-byte Failed");
        return false;
    }

    val->ax = (g_i2c_read_buf[0] << 8) | g_i2c_read_buf[1];
    val->ay = (g_i2c_read_buf[2] << 8) | g_i2c_read_buf[3];
    val->az = (g_i2c_read_buf[4] << 8) | g_i2c_read_buf[5];
    val->gx = (g_i2c_read_buf[8] << 8) | g_i2c_read_buf[9];
    val->gy = (g_i2c_read_buf[10] << 8) | g_i2c_read_buf[11];
    val->gz = (g_i2c_read_buf[12] << 8) | g_i2c_read_buf[13];
}

bool mpu6050_init() {
    unsigned u;

    INFO("Initializing MPU 6050 Sensor");

    if (!mpu6050_reset()) {
        FAIL("Could not reset MPU 6050 Sensor");
        return false;
    }

    if (!mpu6050_detect()) {
        FAIL("Could not find MPU 6050 Sensor");
        return false;
    }

    if (!mpu6050_selftest()) {
        FAIL("Self Test Failed");
        return false;
    }

    u = 0;
    g_i2c_write_buf[u++] = SMPLRT_DIV_REG;
    g_i2c_write_buf[u++] = 0x07; // div = 7 => yields 1kHz sample rate
    g_i2c_write_buf[u++] = 0x00; // config LPF to have highest BW
    g_i2c_write_buf[u++] = 0x08; // (+/-) 500 degree/sec
    g_i2c_write_buf[u++] = 0x08; // (+/-) 4g
    if (SUCCESS < I2C_IF_Write(MPU6050_I2C_ADDR, &g_i2c_write_buf, u, 1)) {
        FAIL("I2C_IF_Write to PWR_MGMT_1_REG Failed");
        return false;
    }

    MAP_UtilsDelay((90*80*1000)); // 30msec

    sensor_value_t sensor_value;
    mpu6050_update_readings(&sensor_value);

    INFO("Showing current MPU6050 sensor values");
    UART_PRINT("accel.x = %fg\r\n", sensor_value.ax / ACCEL_SENSITIVITY);
    UART_PRINT("accel.y = %fg\r\n", sensor_value.ay / ACCEL_SENSITIVITY);
    UART_PRINT("accel.z = %fg\r\n", sensor_value.az / ACCEL_SENSITIVITY);
    UART_PRINT("gyro.x = %f deg/s\r\n", sensor_value.gx / GYRO_SENSITIVITY);
    UART_PRINT("gyro.y = %f deg/s\r\n", sensor_value.gy / GYRO_SENSITIVITY);
    UART_PRINT("gyro.z = %f deg/s\r\n", sensor_value.gz / GYRO_SENSITIVITY);

    return true;
}
