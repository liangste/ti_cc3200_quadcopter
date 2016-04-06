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
    Defines
/////////////////////////////////////////////////////////////////////////////*/

#define MPU6050_I2C_ADDR        0x68
#define I2C_BUF_SIZE            256

#define WHOAMI_VAL              0x68
#define PWR_MGMT_2_DEFAULT_VAL  0x40
#define SENSOR_DATA_SIZE        14

// registers
#define SELF_TEST_X             0x0D
#define SELF_TEST_Y             0x0E
#define SELF_TEST_Z             0x0F
#define SELF_TEST_A             0x10
#define GYRO_CONFIG             0x1B
#define ACCEL_CONFIG            0x1C
#define SENSOR_DATA_BASE        0x3B
#define WHOAMI_REG              0x75
#define PWR_MGMT_1_REG          0x6B
#define PWR_MGMT_2_REG          0x6C

// accoridng to section 4 of "MPU-6000/MPU-6050 Register Maps and Description"
#define CALC_FT_G(a)            (25.0 * 131 * pow(1.046, a - 1))
#define CALC_FT_A(a)            (4096 * .34 * pow(0.92/0.34, (a - 1) / 30.0))

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

bool mpu6050_selftest() {
    unsigned u;
    unsigned char xa_test, ya_test, za_test;
    unsigned char xg_test, yg_test, zg_test;
    double ft_xa, ft_ya, ft_za;
    double ft_xg, ft_yg, ft_zg;
    double d_xa, d_ya, d_za;
    double d_xg, d_yg, d_zg;
    unsigned char reg_addr;
    uint16_t ax, ay, az;
    uint16_t gx, gy, gz;

    if (SUCCESS < I2C_IF_Open(I2C_MASTER_MODE_FST)) {
        FAIL("Failed to open I2C in Fast Mode");
        return false;
    }

    // power management stuff
    u = 0;
    g_i2c_write_buf[u++] = PWR_MGMT_1_REG;
    g_i2c_write_buf[u++] = 0x09;
    if (SUCCESS < I2C_IF_Write(MPU6050_I2C_ADDR, &g_i2c_write_buf, u, 1)) {
        FAIL("I2C_IF_Write to PWR_MGMT_1_REG Failed");
        return false;
    }

    // turn on all self test registers
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

    ft_zg = (xg_test == 0)? 0 : CALC_FT_G(xg_test);
    ft_yg = (yg_test == 0)? 0 : -CALC_FT_G(yg_test);
    ft_zg = (zg_test == 0)? 0 : CALC_FT_G(zg_test);
    ft_xa = (xa_test == 0)? 0 : CALC_FT_A(xg_test);
    ft_ya = (ya_test == 0)? 0 : CALC_FT_A(yg_test);
    ft_za = (za_test == 0)? 0 : CALC_FT_A(zg_test);

    // add small delay for self-test to complete
    MAP_UtilsDelay((90*80*1000*8)); //240msec

    reg_addr = SENSOR_DATA_BASE;
    if (SUCCESS < I2C_IF_ReadFrom(MPU6050_I2C_ADDR, &reg_addr, 1, &g_i2c_read_buf, SENSOR_DATA_SIZE)) {
        FAIL("I2C_IF_ReadFrom 14-byte Failed");
        return false;
    }

    ax = (g_i2c_read_buf[0] << 8) | g_i2c_read_buf[1];
    ay = (g_i2c_read_buf[2] << 8) | g_i2c_read_buf[3];
    az = (g_i2c_read_buf[4] << 8) | g_i2c_read_buf[5];
    gx = (g_i2c_read_buf[8] << 8) | g_i2c_read_buf[9];
    gy = (g_i2c_read_buf[10] << 8) | g_i2c_read_buf[11];
    gz = (g_i2c_read_buf[12] << 8) | g_i2c_read_buf[13];

    // turn on all self test registers
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

    reg_addr = SENSOR_DATA_BASE;
    if (SUCCESS < I2C_IF_ReadFrom(MPU6050_I2C_ADDR, &reg_addr, 1, &g_i2c_read_buf, SENSOR_DATA_SIZE)) {
        FAIL("I2C_IF_ReadFrom 14-byte Failed");
        return false;
    }

    ax -= (g_i2c_read_buf[0] << 8) | g_i2c_read_buf[1];
    ay -= (g_i2c_read_buf[2] << 8) | g_i2c_read_buf[3];
    az -= (g_i2c_read_buf[4] << 8) | g_i2c_read_buf[5];
    gx -= (g_i2c_read_buf[8] << 8) | g_i2c_read_buf[9];
    gy -= (g_i2c_read_buf[10] << 8) | g_i2c_read_buf[11];
    gz -= (g_i2c_read_buf[12] << 8) | g_i2c_read_buf[13];

    d_xa = (ax - ft_xa) / ft_xa;
    d_ya = (ay - ft_ya) / ft_ya;
    d_za = (az - ft_za) / ft_za;
    d_xg = (gx - ft_xg) / ft_xg;
    d_yg = (gy - ft_yg) / ft_yg;
    d_zg = (gz - ft_zg) / ft_zg;

    UART_PRINT("d_xa = %f\r\n", d_xa);
    UART_PRINT("d_ya = %f\r\n", d_ya);
    UART_PRINT("d_za = %f\r\n", d_za);
    UART_PRINT("d_xg = %f\r\n", d_xg);
    UART_PRINT("d_yg = %f\r\n", d_yg);
    UART_PRINT("d_zg = %f\r\n", d_zg);
}

bool mpu6050_init() {
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


    return true;
}
