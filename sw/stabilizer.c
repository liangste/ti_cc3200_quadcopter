#include "config.h"
#include "stabilizer.h"
#include "mpu6050.h"
#include "kf.h"

void SerialControlTask(void *params) {
    TickType_t last_wake_time;
    TickType_t tick_diff;
    sensor_value_t sensor_value;
    angles_t angles;
    angles_t gyro_angles;
    kalman_filter_t kf_pitch;
    kalman_filter_t kf_roll;
    char cmd[10];
    int cmd_length;
    int throttle_value = 0;
    float dt;

    last_wake_time = xTaskGetTickCount();
    tick_diff = xTaskGetTickCount();

    kalman_reset(&kf_pitch);
    kalman_reset(&kf_roll);

    // these are parameters that require tuning per hardware platform
    float q_angle = 0.0001f; // decrease this if estimate is slow
    float q_bias = 1.0f; // increate this is estimate is drifting
    float r_measure = 0.003f; // decrease this if estimate is slow

    // set kalman filter variance values
    kalman_set_variances(&kf_pitch, q_angle, q_bias, r_measure);
    kalman_set_variances(&kf_roll, q_angle,q_bias, r_measure);

	while(1)
	{
        tick_diff = last_wake_time;
        vTaskDelayUntil(&last_wake_time, 1/portTICK_PERIOD_MS); // 1kHz
        tick_diff = last_wake_time - tick_diff;

        // get time elapsed
        dt = (float) tick_diff * portTICK_PERIOD_MS / 1000.0f;

        mpu6050_update_readings(&sensor_value);

        // calculate pitch and row from accelerometer
        double z2 = pow((double) sensor_value.az, 2.0);
        double sqrt_x2_z2 = pow((double) sensor_value.ax, 2.0) + z2;
        double sqrt_y2_z2 = pow((double) sensor_value.ay, 2.0) + z2;
        angles.pitch = atan2((double) sensor_value.ay, sqrt(sqrt_x2_z2));
        angles.roll = atan2((double) -sensor_value.ax, sqrt(sqrt_y2_z2));

        gyro_angles.pitch += (double) sensor_value.gx / GYRO_SENSITIVITY * dt;
        gyro_angles.roll += (double) sensor_value.gy / GYRO_SENSITIVITY * dt;

        kalman_update(&kf_pitch, angles.pitch, sensor_value.gx / GYRO_SENSITIVITY, dt);
        kalman_update(&kf_roll, angles.roll, sensor_value.gy / GYRO_SENSITIVITY, dt);

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

            // update sensor values
            UART_PRINT("%f %f %f %f %f %f %f %f %f %f %f %f\r\n",
                sensor_value.ax / ACCEL_SENSITIVITY,
                sensor_value.ay / ACCEL_SENSITIVITY,
                sensor_value.az / ACCEL_SENSITIVITY,
                sensor_value.gx / GYRO_SENSITIVITY,
                sensor_value.gy / GYRO_SENSITIVITY,
                sensor_value.gz / GYRO_SENSITIVITY,
                angles.pitch,
                angles.roll,
                gyro_angles.pitch,
                gyro_angles.roll,
                kalman_get_angle(&kf_pitch),
                kalman_get_angle(&kf_roll));
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
