#ifndef __WIFI_H__
#define __WIFI_H__

#include <stdbool.h>
#include "semphr.h"

void wifi_init(void);

unsigned char g_wifiInitialized;    // initialization of Wifi sub system
unsigned char g_wifiConnected;

unsigned char g_commConnected;

SemaphoreHandle_t wifiDataLock;
bool wifiDataAvailable;

void WifiConnectTask(void *);

#endif
