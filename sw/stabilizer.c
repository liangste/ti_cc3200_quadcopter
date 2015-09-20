#include "config.h"

#define PI                      3.14159265
#define PID_KP                  0.7
#define PID_KI                  0.0
#define PID_KD                  8.0
#define LPF_ALPHA               ((1)/(1+1/SENSORS_UPDATE_FREQ))
#define HPF_ALPHA               0.5
#define CPMT_FILTER_ALPHA       0.8
#define YAW_COMPENSATION        8.0

void StabilizerTask(void *pvParameters) {
    uint32_t lastWakeTime, dtick;
    uint32_t divider;
    struct float3x accl3xF = {0};
    struct float3x gyro3xF = {0};
    float roll, pitch, yaw;

    dtick = lastWakeTime = xTaskGetTickCount();
    divider = 0;

    while(1)
    {
        stabilizer_ticks++;
        // 500Hz
        vTaskDelayUntil(&lastWakeTime, FREQ2TIX(SAMPLEFREQ));
        dtick = lastWakeTime - dtick;
        if (dtick != 2)
            UART_PRINT("ticks elapsed %d\r\n", dtick);
        dtick = lastWakeTime;
        sensors_update(&accl3xF, &gyro3xF);
#ifdef SHOW_SENSOR_READINGS
        UART_PRINT("accl_x %f, accl_y %f, accl_z %f, gyro_x %f, gyro_y %f, gyro_z %f\r\n",
            accl3xF.x, accl3xF.y, accl3xF.z,
            gyro3xF.x, gyro3xF.y, gyro3xF.z
        );
#endif

        // 250Hz
        if (++divider >= 2) {
            ahrs_update(gyro3xF.x, gyro3xF.y, gyro3xF.z, accl3xF.x, accl3xF.y, accl3xF.z);
            ahrs_get_rpy(&roll, &pitch, &yaw);

#ifdef SHOW_RPY
            UART_PRINT("roll %f, pitch %f, yaw %f\r\n", roll, pitch, yaw);
#endif

            divider = 0;
        }

#ifdef SHOW_TICKS
        UART_PRINT("stabilizer %d ", stabilizer_ticks);
        UART_PRINT(" wifi task %d ", wifi_task_ticks);
        UART_PRINT(" wifi recv %d ", wifi_recv_ticks);
        UART_PRINT(" wifi indicator %d", wifi_indicator_ticks);
        UART_PRINT(" sensor indicator %d", sensor_indicator_ticks);
        UART_PRINT(" comm indicator %d\r\n", comm_indicator_ticks);
#endif

        /*
        if(MAP_UARTCharGetNonBlocking(CONSOLE) == 'w')
            UART_PRINT("%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%u,%u,%u,%u,%d,%d,%d\r\n",
                gyro_pitch, gyro_roll,
                accl_pitch, accl_roll,
                k_pitch, k_roll,
                mTagetPWMs.motor_1, mTagetPWMs.motor_2,
                mTagetPWMs.motor_3, mTagetPWMs.motor_4,
                accl_x, accl_y, accl_z);
        */
    }
}
