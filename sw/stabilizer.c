#include "config.h"
#include "stabilizer.h"
#include "mpu6050.h"

void StabilizerTask(void *params) {
    TickType_t last_wake_time;
    sensor_value_t sensor_value;
    char cmd[128];
    int cmd_length;

    INFO("Cmd format");
    NOTE("  1 - get raw sensor readings");

	while(1)
	{
        vTaskDelayUntil(&last_wake_time, 1/portTICK_PERIOD_MS); // 1kHz

        cmd_length = GetCmd(cmd, 128);
        if (cmd_length > 0 && cmd_length < 128) {
            if (cmd[0] == '1') {
                mpu6050_update_readings(&sensor_value);
                UART_PRINT("%f %f %f %f %f %f\r\n",
                sensor_value.ax / ACCEL_SENSITIVITY,
                sensor_value.ay / ACCEL_SENSITIVITY,
                sensor_value.az / ACCEL_SENSITIVITY,
                sensor_value.gx / GYRO_SENSITIVITY,
                sensor_value.gy / GYRO_SENSITIVITY,
                sensor_value.gz / GYRO_SENSITIVITY);
            }
        }
	}
}
