#include "config.h"
#include "stabilizer.h"
#include "mpu6050.h"
#include "kf.h"
#include "pid.h"

void SerialControlTask(void *params) {
    TickType_t last_wake_time;
    TickType_t tick_diff;
    sensor_value_t sensor_value;
    angles_t angles;
    angles_t gyro_angles;
    kalman_filter_t kf_pitch;
    kalman_filter_t kf_roll;
    pid_data_t pitch_pid;
    char cmd[10];
    int cmd_length;
    int32_t throttle_value = 0;
    int32_t m1, m2, m3, m4;
    float dt;

    last_wake_time = xTaskGetTickCount();
    tick_diff = xTaskGetTickCount();

    mpu6050_update_readings(&sensor_value);

    // calculate pitch and row from accelerometer
    double z2 = pow((double) sensor_value.az, 2.0);
    double sqrt_x2_z2 = pow((double) sensor_value.ax, 2.0) + z2;
    double sqrt_y2_z2 = pow((double) sensor_value.ay, 2.0) + z2;
    angles.pitch = atan2((double) sensor_value.ay, sqrt(sqrt_x2_z2));
    angles.roll = atan2((double) -sensor_value.ax, sqrt(sqrt_y2_z2));

    kalman_reset(&kf_pitch, angles.pitch);
    kalman_reset(&kf_roll, angles.roll);

    // these are parameters that require tuning per hardware platform
    float q_angle = 0.0001f; // decrease this if estimate is slow
    float q_bias = 2.0f; // increate this is estimate is drifting
    float r_measure = 0.003f; // decrease this if estimate is slow

    // set kalman filter variance values
    kalman_set_variances(&kf_pitch, q_angle, q_bias, r_measure);
    kalman_set_variances(&kf_roll, q_angle,q_bias, r_measure);

    // PID updates
    float k_p = 100.0f;
    float k_i = 1.00f;
    float k_d = 0.00f;
    double desired_pitch = 0.0f;

    pid_init(&pitch_pid, k_p, k_i, k_d);

	while(1)
	{
        tick_diff = last_wake_time;
        vTaskDelayUntil(&last_wake_time, 1/portTICK_PERIOD_MS); // 1kHz
        tick_diff = last_wake_time - tick_diff;

        // get time elapsed
        dt = (float) tick_diff * portTICK_PERIOD_MS / 1000.0f;

        mpu6050_update_readings(&sensor_value);

        // calculate pitch and row from accelerometer
        z2 = pow((double) sensor_value.az, 2.0);
        sqrt_x2_z2 = pow((double) sensor_value.ax, 2.0) + z2;
        sqrt_y2_z2 = pow((double) sensor_value.ay, 2.0) + z2;
        angles.pitch = atan2((double) sensor_value.ay, sqrt(sqrt_x2_z2));
        angles.roll = atan2((double) -sensor_value.ax, sqrt(sqrt_y2_z2));

        gyro_angles.pitch += (double) sensor_value.gx / GYRO_SENSITIVITY * dt;
        gyro_angles.roll += (double) sensor_value.gy / GYRO_SENSITIVITY * dt;

        kalman_update(&kf_pitch, angles.pitch, sensor_value.gx / GYRO_SENSITIVITY, dt);
        kalman_update(&kf_roll, angles.roll, sensor_value.gy / GYRO_SENSITIVITY, dt);

        pid_update(&pitch_pid, desired_pitch, kalman_get_angle(&kf_pitch), sensor_value.gx / GYRO_SENSITIVITY);

        cmd_length = GetCmd(cmd, 10);
        if (cmd_length > 0 && cmd_length < 10) {
            // update throttle values
            led_toggle(ORANGE);

            throttle_value = (uint8_t) atoi(cmd);

            if (throttle_value > MAX_THROTTLE_VALUE)
                throttle_value = MAX_THROTTLE_VALUE;

            if (throttle_value == 0) {
                m1 = m2 = m3 = m4 = 0;
            } else {
                // calculate throttle
                m1 = 0;
                m2 = 0;
                m3 = motors_correct_throttle(throttle_value - pid_get_value(&pitch_pid));
                m4 = 0;
            }

            motors_set_m1(m1);
            motors_set_m2(m2);
            motors_set_m3(m3);
            motors_set_m4(m4);

            // update sensor values
            UART_PRINT("%f %f %f %f %f %f %f %f %f %f %f %f %u %u %u %u\r\n",
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
                kalman_get_angle(&kf_roll),
                m1,
                m2,
                m3,
                m4);
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
