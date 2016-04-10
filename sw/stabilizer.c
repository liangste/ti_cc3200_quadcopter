#include "config.h"
#include "stabilizer.h"
#include "mpu6050.h"

void SerialControlTask(void *params) {
    TickType_t last_wake_time;
    sensor_value_t sensor_value;
    char cmd[10];
    int cmd_length;
    int throttle_value = 0;

    INFO("Cmd format");
    NOTE("  1 - get raw sensor readings");

    last_wake_time = xTaskGetTickCount();

	while(1)
	{
        vTaskDelayUntil(&last_wake_time, 100/portTICK_PERIOD_MS/3); // 30hz

        cmd_length = GetCmd(cmd, 10);
        if (cmd_length > 0 && cmd_length < 10) {
            // update throttle values
            led_toggle(ORANGE);

            throttle_value = (uint32_t) atoi(cmd);

            if (throttle_value > 100)
                throttle_value = 100;

            motors_set_m1(throttle_value);
            motors_set_m2(throttle_value);
            motors_set_m3(throttle_value);
            motors_set_m4(throttle_value);

            // update sensor values
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

void StabilizerTask(void *params) {
    TickType_t last_wake_time;
    sensor_value_t sensor_value;

    last_wake_time = xTaskGetTickCount();

	while(1)
	{
        vTaskDelayUntil(&last_wake_time, 2/portTICK_PERIOD_MS); // 2ms = 500Hz
	}
}
