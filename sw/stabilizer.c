#include "config.h"
#include "stabilizer.h"
#include "mpu6050.h"

void SerialControlTask(void *params) {
    TickType_t last_wake_time;
    sensor_value_t sensor_value;
    angles_t angles;
    char cmd[10];
    int cmd_length;
    int throttle_value = 0;

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

            // apply throttle
            motors_set_m1(throttle_value);
            motors_set_m2(throttle_value);
            motors_set_m3(throttle_value);
            motors_set_m4(throttle_value);

            // calculate pitch and row from accelerometer
            double z2 = pow((double) sensor_value.az, 2.0);
            double sqrt_x2_z2 = pow((double) sensor_value.ax, 2.0) + z2;
            double sqrt_y2_z2 = pow((double) sensor_value.ay, 2.0) + z2;
            angles.pitch = atan2((double) sensor_value.ay, sqrt(sqrt_x2_z2));
            angles.roll = atan2((double) -sensor_value.ax, sqrt(sqrt_y2_z2));

            // update sensor values
            mpu6050_update_readings(&sensor_value);
            UART_PRINT("%f %f %f %f %f %f %f %f\r\n",
                sensor_value.ax / ACCEL_SENSITIVITY,
                sensor_value.ay / ACCEL_SENSITIVITY,
                sensor_value.az / ACCEL_SENSITIVITY,
                sensor_value.gx / GYRO_SENSITIVITY,
                sensor_value.gy / GYRO_SENSITIVITY,
                sensor_value.gz / GYRO_SENSITIVITY,
                angles.pitch,
                angles.roll
                );
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
