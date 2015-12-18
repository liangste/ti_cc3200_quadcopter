#include "config.h"

#define OSI_STACK_SIZE          524    // stack size in number of *words*
#define mainDELAY_LOOP_COUNT    100000

int init_status;

extern void(* const g_pfnVectors[])(void);

static void board_init(void);

void vAssertCalled(const char *pcFile, unsigned long ulLine) {
		LOOP_FOREVER();
}

void vApplicationIdleHook(void) {
}

void vApplicationMallocFailedHook(void) {
		UART_PRINT("[Error] FreeRTOS malloc failed\r\n");
		LOOP_FOREVER();
}

void vApplicationStackOverflowHook(OsiTaskHandle *pxTask,
				signed char *pcTaskName) {
		UART_PRINT("[Error] FreeRTOS stack overflow\r\n");
		LOOP_FOREVER();
}

static void board_init(void) {
		MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);

		MAP_IntMasterEnable();
		MAP_IntEnable(FAULT_SYSTICK);

		PRCMCC3200MCUInit();
}

// ORANGE Blinking LED
void WifiIndicatorTask(void *pvParameters) {
		while(1)
		{
				wifi_indicator_ticks++;
				if (g_wifiInitialized)
				{
						if (g_wifiConnected)
								led_set(ORANGE);
						else
								led_toggle(ORANGE);
				}
				vTaskDelay(100);
		}
}

// GREEN Blinking LED
void SensorIndicatorTask(void *pvParameters) {
		while(1)
		{
				/*
				sensor_indicator_ticks++;
				if (g_sensorInitialized)
				{
						if (g_sensorConnected)
								led_toggle(GREEN);
						g_sensorConnected = 0;
				}
				*/
				vTaskDelay(1000);
		}
}

// BLUE Blinking LED
void CommIndicatorTask(void *pvParameters) {
		while(1)
		{
				comm_indicator_ticks++;
				if (g_commConnected) {
						led_toggle(BLUE);
						g_commConnected = 0;
				}
				else
						led_clear(BLUE);
				vTaskDelay(40);
		}
}

int main()
{
		board_init();
		pinmux_config();
		InitTerm();

		UART_PRINT("========================================\r\n");
		UART_PRINT("   Initializing CC3200 Quad Firmware\r\n");
		UART_PRINT("========================================\r\n");

		init_status = 0;
		led_init();
		//sensors_init(&init_status);
		motors_init();
		//pid_init();

		/*
		if (init_status) {
				UART_PRINT("[X] Init failed\r\n");
				LOOP_FOREVER();
		}

		wifiDataLock = xSemaphoreCreateMutex();
		wifiDataAvailable = false;

		VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);

		xTaskCreate( WifiIndicatorTask, NULL,
						OSI_STACK_SIZE, (void *) NULL, 5, NULL );
		xTaskCreate( SensorIndicatorTask, NULL,
						OSI_STACK_SIZE, (void *) NULL, 5, NULL );
		xTaskCreate( CommIndicatorTask, NULL,
						OSI_STACK_SIZE, (void *) NULL, 5, NULL );
		xTaskCreate( WifiConnectTask, NULL,
						OSI_STACK_SIZE, (void*) NULL, 5, NULL );
		xTaskCreate( StabilizerTask, NULL,
						OSI_STACK_SIZE, (void *) NULL, 5, NULL );

		UART_PRINT("Starting FreeRTOS Scheduling\r\n");
		vTaskStartScheduler();
		*/

		while(1) {};
		return 0;
}
