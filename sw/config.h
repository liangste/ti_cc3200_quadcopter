#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_apps_rcm.h"
#include "hw_common_reg.h"
#include "hw_gpio.h"

#include "simplelink.h"

#include "utils.h"
#include "uart.h"
#include "rom_map.h"
#include "rom.h"
#include "pin.h"
#include "interrupt.h"
#include "i2c.h"
#include "gpio.h"
#include "timer.h"
#include "prcm.h"

#include "common.h"
#include "uart_if.h"
#include "timer_if.h"
#include "i2c_if.h"

#include "osi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "FreeRTOSConfig.h"

#include "pinmux.h"
#include "led.h"
#include "sensors.h"
#include "wifi.h"
#include "pid.h"
#include "packet.h"
#include "stabilizer.h"
#include "motors.h"
#include "ahrs.h"

#define UART_PRINT              Report
#define PI                      3.14159265
#define STABILIZER_FREQ         500.0f
#define FUSION_FREQ             500.0f      // sample frequency in Hz
#define FUSION_DT               (float)(1.0 / (FUSION_FREQ / 1.0)) // 250hz


uint32_t stabilizer_ticks;
uint32_t wifi_task_ticks;
uint32_t wifi_recv_ticks;
uint32_t wifi_indicator_ticks;
uint32_t sensor_indicator_ticks;
uint32_t comm_indicator_ticks;

extern int init_status;

#endif
