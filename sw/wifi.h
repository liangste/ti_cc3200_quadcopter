#ifndef __WIFI_H__
#define __WIFI_H__

#include <stdbool.h>
#include "semphr.h"

void wifi_init(void);

unsigned char g_wifiInitialized;    // initialization of Wifi sub system
bool g_wifiConnected;

SemaphoreHandle_t wifiDataLock;

bool g_wifiDataAvailable;
struct UdpCmd g_udpCmdRecvStruct;

void WifiConnectTask(void *);

#endif
