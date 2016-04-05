/*
    - file name gyro.c is very misleading, this is because the only i2c devices
    working on main2.0 was the gyroscope...
    - But this file actually does general i2c functions including an
    accelerometer
*/

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

#define I2C_BASE                I2CA0_BASE
#define UART_PRINT              Report
#define GYRO_I2C_ADDR           0x68
#define ACCL_I2C_ADDR           0x1D
#define I2C_BUF_SIZE            256
#define WHOAMI_VAL              0x68
#define WHOAMI_REG              0x75
#define PI                      3.14159265

/*/////////////////////////////////////////////////////////////////////////////
    Variables
/////////////////////////////////////////////////////////////////////////////*/

unsigned char g_i2c_buf[I2C_BUF_SIZE];

/*/////////////////////////////////////////////////////////////////////////////
    Function Prototypes
/////////////////////////////////////////////////////////////////////////////*/

void sensors_init();
void sensors_reset();
void sensors_configure();

/*/////////////////////////////////////////////////////////////////////////////
    Function Definitions
/////////////////////////////////////////////////////////////////////////////*/

bool mpu6050_detect() {
    unsigned char reg_addr = WHOAMI_REG;

    if (SUCCESS < I2C_IF_Open(I2C_MASTER_MODE_FST)) {
        FAIL("Failed to open I2C in Fast Mode");
        return false;
    }

    if (SUCCESS < I2C_IF_ReadFrom(GYRO_I2C_ADDR, &reg_addr, 1, &g_i2c_buf, 1)) {
        FAIL("I2C_IF_ReadFrom Failed");
        return false;
    }

    if (g_i2c_buf[0] == WHOAMI_VAL) {
        INFO("Found MPU 6050 Sensor");
    } else {
        FAIL("Could not find MPU 6050 Sensor!");
        return false;
    }

    return true;
}

bool mpu6050_init() {
    INFO("Initializing MPU 6050 Sensor");

    if (!mpu6050_detect()) {
        FAIL("Could not find MPU 6050 Sensor");
        return false;
    }

    return true;
}
