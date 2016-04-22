#include "config.h"
#include "stabilizer.h"
#include "mpu6050.h"
#include "kf.h"
#include "pid.h"

extern bool g_wifiConnected;
extern bool g_wifiDataAvailable;
extern struct UdpCmd g_udpCmdRecvStruct;

void SerialControlTask(void *params) {
    TickType_t last_wake_time;
    TickType_t tick_diff;
    sensor_value_t sensor_value;
    angles_t angles;
    angles_t gyro_angles;
    kalman_filter_t kf_pitch;
    kalman_filter_t kf_roll;
    pid_data_t pitch_pid;
    pid_data_t roll_pid;
    char cmd[10];
    int cmd_length;
    int32_t throttle_value = 0;
    int32_t m1, m2, m3, m4;
    float dt;

    last_wake_time = xTaskGetTickCount();
    tick_diff = xTaskGetTickCount();

    mpu6050_update_readings(&sensor_value);

    mpu6050_calculate_accel_angles(&sensor_value, &angles);

    kalman_reset(&kf_pitch, angles.pitch);
    kalman_reset(&kf_roll, angles.roll);

    // these are parameters that require tuning per hardware platform
    float q_angle = 0.0001f; // decrease this if estimate is slow
    float q_bias = 0.2f; // increate this is estimate is drifting
    float r_measure = 0.003f; // decrease this if estimate is slow

    // set kalman filter variance values
    kalman_set_variances(&kf_pitch, q_angle, q_bias, r_measure);
    kalman_set_variances(&kf_roll, q_angle,q_bias, r_measure);

    // PID updates
    float k_p = 20.0f;
    float k_i = 20.0f;
    float k_d = 0.001f;

    float r_k_p = 20.0f;
    float r_k_i = 5.0f;
    float r_k_d = 0.001f;

    double desired_pitch = 0.0f;
    double desired_roll = 0.0f;

    pid_init(&pitch_pid, k_p, k_i, k_d);
    pid_init(&roll_pid, r_k_p, r_k_i, r_k_d);

	while(1)
	{
        tick_diff = last_wake_time;
        vTaskDelayUntil(&last_wake_time, 1/portTICK_PERIOD_MS); // 1kHz
        tick_diff = last_wake_time - tick_diff;

        // get time elapsed
        dt = (float) tick_diff * portTICK_PERIOD_MS / 1000.0f;

        mpu6050_update_readings(&sensor_value);

        mpu6050_calculate_accel_angles(&sensor_value, &angles);

        double gyro_pitch_rate = (double) sensor_value.gx / GYRO_SENSITIVITY;
        double gyro_roll_rate = (double) sensor_value.gy / GYRO_SENSITIVITY;

        gyro_angles.pitch += gyro_pitch_rate * dt;
        gyro_angles.roll += gyro_roll_rate * dt;

        kalman_update(&kf_pitch, angles.pitch, gyro_pitch_rate, dt);
        kalman_update(&kf_roll, angles.roll, gyro_roll_rate, dt);

        pid_update(&pitch_pid, desired_pitch, kalman_get_angle(&kf_pitch), gyro_pitch_rate);
        pid_update(&roll_pid, desired_roll, kalman_get_angle(&kf_roll), gyro_roll_rate);

        cmd_length = GetCmd(cmd, 10);
        if (cmd_length > 0 && cmd_length < 10) {
            // update throttle values
            led_toggle(ORANGE);

            throttle_value = (uint8_t) atoi(cmd);

            if (throttle_value > MAX_THROTTLE_VALUE)
                throttle_value = MAX_THROTTLE_VALUE;
            else if (throttle_value < MIN_THROTTLE_VALUE)
                throttle_value = MIN_THROTTLE_VALUE;

            if (throttle_value == 0) {
                m1 = m2 = m3 = m4 = 0;
                pid_reset(&pitch_pid);
                pid_reset(&roll_pid);
            } else {
                // calculate throttle
                m1 = motors_correct_throttle(throttle_value);
                m2 = motors_correct_throttle(throttle_value);
                m3 = motors_correct_throttle(throttle_value);
                m4 = motors_correct_throttle(throttle_value);
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
        TickType_t tick_diff;
        sensor_value_t sensor_value;
        angles_t angles;
        angles_t gyro_angles;
        kalman_filter_t kf_pitch;
        kalman_filter_t kf_roll;
        pid_data_t pitch_pid;
        pid_data_t roll_pid;
        pid_data_t yaw_pid;
        char cmd[10];
        int cmd_length;
        int32_t throttle_value = 0;
        int32_t m1, m2, m3, m4;
        float dt;
        int control_div = 0;
        int output_divider = 10;

        while (!g_wifiConnected) {};

        last_wake_time = xTaskGetTickCount();
        tick_diff = xTaskGetTickCount();

        mpu6050_update_readings(&sensor_value);

        mpu6050_calculate_accel_angles(&sensor_value, &angles);

        kalman_reset(&kf_pitch, angles.pitch);
        kalman_reset(&kf_roll, angles.roll);

        // these are parameters that require tuning per hardware platform
        float q_angle = 0.0001f; // decrease this if estimate is slow
        float q_bias = 0.2f; // increate this is estimate is drifting
        float r_measure = 0.003f; // decrease this if estimate is slow

        // set kalman filter variance values
        kalman_set_variances(&kf_pitch, q_angle, q_bias, r_measure);
        kalman_set_variances(&kf_roll, q_angle, q_bias, r_measure);

        // PID updates
        float k_p = 0.5f; // 2.0
        float k_i = 0.0f; // 0.05
        float k_d = 0.21f; //

        float y_k_p = 0.05f;
        float y_k_i = 0.000001f;
        float y_k_d = 1.0f;

        double desired_pitch = 0.0f;
        double desired_roll = 0.0f;
        double desired_yaw = 0.0f;

        pid_init(&pitch_pid, k_p, k_i, k_d);
        pid_init(&roll_pid, k_p, k_i, k_d);
        pid_init(&yaw_pid, y_k_p, y_k_i, y_k_d);

    	while(1)
    	{
            tick_diff = last_wake_time;
            vTaskDelayUntil(&last_wake_time, 1/portTICK_PERIOD_MS); // 1kHz
            tick_diff = last_wake_time - tick_diff;

            // get time elapsed
            dt = (float) tick_diff * portTICK_PERIOD_MS / 1000.0f;

            mpu6050_update_readings(&sensor_value);

            mpu6050_calculate_accel_angles(&sensor_value, &angles);

            double gyro_pitch_rate = (double) sensor_value.gx / GYRO_SENSITIVITY;
            double gyro_roll_rate = (double) sensor_value.gy / GYRO_SENSITIVITY;
            double gyro_yaw_rate = (double) sensor_value.gz / GYRO_SENSITIVITY;

            gyro_angles.yaw += gyro_yaw_rate * dt;

            //kalman_update(&kf_pitch, angles.pitch, gyro_pitch_rate, dt);
            //kalman_update(&kf_roll, angles.roll, gyro_roll_rate, dt);

            throttle_value = (int32_t) g_udpCmdRecvStruct._y_Left / 2;

            int32_t x_right = g_udpCmdRecvStruct._x_Right;
            int32_t y_right = g_udpCmdRecvStruct._y_Right;

            int32_t roll_ctrl = g_udpCmdRecvStruct._x_Right / 10;
            int32_t pitch_ctrl = g_udpCmdRecvStruct._y_Right / 10;

            pid_update(&pitch_pid, desired_pitch, angles.pitch, gyro_pitch_rate);
            pid_update(&roll_pid, desired_roll, angles.roll, gyro_roll_rate);
            pid_update(&yaw_pid, desired_yaw, gyro_angles.yaw, gyro_yaw_rate);

            led_toggle(ORANGE);

            if (throttle_value > MAX_THROTTLE_VALUE)
                throttle_value = MAX_THROTTLE_VALUE;
            else if (throttle_value < MIN_THROTTLE_VALUE)
                throttle_value = MIN_THROTTLE_VALUE;

            if (throttle_value == 0) {
                m1 = m2 = m3 = m4 = 0;
                pid_reset(&pitch_pid);
                pid_reset(&roll_pid);
                pid_reset(&yaw_pid);
            } else {
                // calculate throttle
                m1 = motors_correct_throttle(throttle_value + pid_get_value(&roll_pid) - pid_get_value(&yaw_pid) + 5 - roll_ctrl);
                m2 = motors_correct_throttle(throttle_value - pid_get_value(&roll_pid) - pid_get_value(&yaw_pid) + roll_ctrl);
                m3 = motors_correct_throttle(throttle_value - pid_get_value(&pitch_pid) + pid_get_value(&yaw_pid) - pitch_ctrl);
                m4 = motors_correct_throttle(throttle_value + pid_get_value(&pitch_pid) + pid_get_value(&yaw_pid) + pitch_ctrl);
            }

            motors_set_m1(m1);
            motors_set_m2(m2);
            motors_set_m3(m3);
            motors_set_m4(m4);
        }
    }
