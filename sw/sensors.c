#include "config.h"

#define MPU6050_I2C_ADDR        0x68
#define MPU6050_WHOAMI          0x68
#define I2C_BUF_SIZE            32
#define MPU6050_LSB_PER_DEG     (float)(16.4F)   // FSR = 2000 deg/s
#define MPU6050_LSB_PER_G       (float)(4096.0F) // FSR = 8g

static unsigned char i2cWriteBuf[I2C_BUF_SIZE];
static unsigned char i2cDataBuf[I2C_BUF_SIZE];

static int mpu_write_and_check(unsigned char reg_addr, unsigned char new_val) {
    unsigned char old_val;
    int ret = 0;
    ret |= I2C_IF_ReadFrom(MPU6050_I2C_ADDR, &reg_addr, 1, &old_val, 1);
    i2cWriteBuf[0] = reg_addr;
    i2cWriteBuf[1] = new_val;
    ret |= I2C_IF_Write(MPU6050_I2C_ADDR, &i2cWriteBuf[0], 2, 1);
    ret |= I2C_IF_ReadFrom(MPU6050_I2C_ADDR, &reg_addr, 1, &i2cDataBuf[0], 1);
    if (old_val != new_val)
        UART_PRINT("[X] write(0x%02x) to reg 0x%02x FAILED,  before(0x%02x) after(0x%02x)\r\n",
            new_val, reg_addr, old_val, i2cDataBuf[0]);
    return ret;
}

void sensors_init(int* status) {
    unsigned char reg_data, reg_addr;

    I2C_IF_Open(I2C_MASTER_MODE_FST);

    reg_addr = 0x75;
    I2C_IF_ReadFrom(MPU6050_I2C_ADDR, &reg_addr, 1, &reg_data, 1);

    if (reg_data == MPU6050_WHOAMI) {
        int fail = 0;
        UART_PRINT("[!] MPU6050 found\r\n");
        fail |= mpu_write_and_check(0x6b, 0x01);    // CLK select
        fail |= mpu_write_and_check(0x19, 0x02);    // 1kHz output rate, 500Hz gyro sample
        fail |= mpu_write_and_check(0x1A, 0x01);    // LPF config, BW=184, 188Hz
        fail |= mpu_write_and_check(0x1B, 0x18);    // gyro FSR = 2000/s
        fail |= mpu_write_and_check(0x1C, 0x10);    // accl FSR = 8g
        fail |= mpu_write_and_check(0x23, 0x00);    // disable FIFO
        if (fail) {
            UART_PRINT("[X] Failed to write to MPU6050");
            *status = -1;
        }
        else
            g_sensorInitialized = 1;
    } else {
        UART_PRINT("[X] could not find MPU6050\r\n");
        *status = -1;
    }
}

void sensors_update(struct float3x * accl3x, struct float3x * gyro3x) {
    if (!g_sensorInitialized)
        return;

    unsigned char reg = 0x3b; // start of MPU6050 Accl data

    I2C_IF_ReadFrom(MPU6050_I2C_ADDR, &reg, 1, &i2cDataBuf[0], 14);
    accl3x->x = (short)(i2cDataBuf[0] << 8 | i2cDataBuf[1]) / MPU6050_LSB_PER_G;
    accl3x->y = (short)(i2cDataBuf[2] << 8 | i2cDataBuf[3]) / MPU6050_LSB_PER_G;
    accl3x->z = (short)(i2cDataBuf[4] << 8 | i2cDataBuf[5]) / MPU6050_LSB_PER_G;
    gyro3x->x = (short)(i2cDataBuf[8] << 8 | i2cDataBuf[9]) / MPU6050_LSB_PER_DEG;
    gyro3x->y = (short)(i2cDataBuf[10] << 8 | i2cDataBuf[11]) / MPU6050_LSB_PER_DEG;
    gyro3x->z = (short)(i2cDataBuf[12] << 8 | i2cDataBuf[13]) / MPU6050_LSB_PER_DEG;

    g_sensorConnected = 1;
}
