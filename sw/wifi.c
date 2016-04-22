#include "config.h"

/*/////////////////////////////////////////////////////////////////////////////
    Defines
/////////////////////////////////////////////////////////////////////////////*/

#define RECV_BUF_SIZE           (MAX_UdpCmd_SIZE*100)
#define OSI_STACK_SIZE          524

/*/////////////////////////////////////////////////////////////////////////////
    Variables
/////////////////////////////////////////////////////////////////////////////*/

unsigned char  g_ulStatus = 0;
unsigned long  g_ulStaIp = 0;
unsigned long  g_ulPingPacketsRecv = 0;
unsigned long  g_uiGatewayIP = 0;
char *pcSsidName = "cc3200_quadcopter";
int g_iSockID;
int iAddrSize;

SlSockAddrIn_t  g_localAddr;
int g_portNum = 5000;

TaskHandle_t xRecvTaskHandle = NULL;

typedef enum {
    // Choosing this number to avoid overlap w/ host-driver's error codes
    LAN_CONNECTION_FAILED = -0x7D0,
    CLIENT_CONNECTION_FAILED = LAN_CONNECTION_FAILED - 1,
    DEVICE_NOT_IN_STATION_MODE = CLIENT_CONNECTION_FAILED - 1,
    STATUS_CODE_MAX = -0xBB8
} e_AppStatusCodes;

extern bool g_wifiConnected;
extern bool g_wifiDataAvailable;
extern struct UdpCmd g_udpCmdRecvStruct;

char g_RecvBuf[RECV_BUF_SIZE];

/*/////////////////////////////////////////////////////////////////////////////
    Function Prototypes
/////////////////////////////////////////////////////////////////////////////*/

static long ConfigureSimpleLinkToDefaultState();
static void InitializeAppVariables();
static int ConfigureAPMode();
void vUDPRecvTask(void *);

/*/////////////////////////////////////////////////////////////////////////////
    Wireless Co-processor Event Handlers
/////////////////////////////////////////////////////////////////////////////*/

void SimpleLinkWlanEventHandler(SlWlanEvent_t *pSlWlanEvent)
{
    switch(pSlWlanEvent->Event)
    {
        case SL_WLAN_CONNECT_EVENT:
        {
            UART_PRINT("[WLAN EVENT] SL_WLAN_CONNECT_EVENT\n\r");
            SET_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
        }
        break;

        case SL_WLAN_DISCONNECT_EVENT:
        {
            UART_PRINT("[WLAN EVENT] SL_WLAN_DISCONNECT_EVENT\n\r");
            slWlanConnectAsyncResponse_t*  pEventData = NULL;

            CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
            CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);

            pEventData = &pSlWlanEvent->EventData.STAandP2PModeDisconnected;

            if(SL_USER_INITIATED_DISCONNECTION == pEventData->reason_code)
            {
                UART_PRINT("Device disconnected from the AP on application's "
                            "request \n\r");
            }
            else
            {
                UART_PRINT("Device disconnected from the AP on an ERROR..!! \n\r");
            }

        }
        break;

        case SL_WLAN_STA_CONNECTED_EVENT:
        {
            UART_PRINT("[WLAN EVENT] SL_WLAN_STA_CONNECTED_EVENT\n\r");
            SET_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
        }
        break;

        case SL_WLAN_STA_DISCONNECTED_EVENT:
        {
            // Disconnect from Android phone triggers this event
            g_wifiConnected = 0;
            UART_PRINT("[WLAN EVENT] SL_WLAN_STA_DISCONNECTED_EVENT\n\r");
            CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
            CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);
            vTaskDelete(xRecvTaskHandle);
        }
        break;

        default:
        {
            UART_PRINT("[WLAN EVENT] Unexpected event \n\r");
        }
        break;
    }
}

void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent)
{
    switch(pNetAppEvent->Event)
    {
        case SL_NETAPP_IPV4_IPACQUIRED_EVENT:
        case SL_NETAPP_IPV6_IPACQUIRED_EVENT:
        {
            SET_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);
        }
        break;

        case SL_NETAPP_IP_LEASED_EVENT:
        {
            SET_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);

            g_ulStaIp = (pNetAppEvent)->EventData.ipLeased.ip_address;

            UART_PRINT("[NETAPP EVENT] IP Leased to Client: IP=%d.%d.%d.%d\r\n",
                        SL_IPV4_BYTE(g_ulStaIp,3), SL_IPV4_BYTE(g_ulStaIp,2),
                        SL_IPV4_BYTE(g_ulStaIp,1), SL_IPV4_BYTE(g_ulStaIp,0));
            g_wifiConnected = 1;
        }
        break;

        case SL_NETAPP_IP_RELEASED_EVENT:
        {
            CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);
            UART_PRINT("[NETAPP EVENT] IP Released for Client: IP=%d.%d.%d.%d\r\n",
                        SL_IPV4_BYTE(g_ulStaIp,3), SL_IPV4_BYTE(g_ulStaIp,2),
                        SL_IPV4_BYTE(g_ulStaIp,1), SL_IPV4_BYTE(g_ulStaIp,0));
        }
        break;

        default:
        {
            UART_PRINT("[NETAPP EVENT] Unexpected event [0x%x] \n\r",
                       pNetAppEvent->Event);
        }
        break;
    }
}

void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent,
                                  SlHttpServerResponse_t *pHttpResponse)
{
    // Unused
}

void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent)
{
    UART_PRINT("[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n",
               pDevEvent->EventData.deviceEvent.status,
               pDevEvent->EventData.deviceEvent.sender);
}

void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
    switch( pSock->Event )
    {
        case SL_SOCKET_TX_FAILED_EVENT:
            switch( pSock->socketAsyncEvent.SockTxFailData.status)
            {
                case SL_ECLOSE:
                    UART_PRINT("[SOCK ERROR] - close socket (%d) operation "
                        "failed to transmit all queued packets\n\n",
                        pSock->socketAsyncEvent.SockTxFailData.sd);
                    break;
                default:
                    UART_PRINT("[SOCK ERROR] - TX FAILED  :  socket %d , reason "
                        "(%d) \n\n",
                        pSock->socketAsyncEvent.SockTxFailData.sd,
                        pSock->socketAsyncEvent.SockTxFailData.status);
          break;
            }
            break;

        default:
            UART_PRINT("[SOCK EVENT] - Unexpected Event [%x0x]\n\n",
                pSock->Event);
          break;
    }
}

/*/////////////////////////////////////////////////////////////////////////////
    Function Definitions
/////////////////////////////////////////////////////////////////////////////*/

static void InitializeAppVariables() {
    g_ulStatus = 0;
    g_ulStaIp = 0;
    g_ulPingPacketsRecv = 0;
    g_uiGatewayIP = 0;

    g_localAddr.sin_family = SL_AF_INET;
    g_localAddr.sin_port = sl_Htons((unsigned short)g_portNum);
    g_localAddr.sin_addr.s_addr = 0;

    g_wifiConnected = 0;
    g_wifiInitialized = 0;
}

static int ConfigureAPMode(void)
{
    long   lRetVal = -1;

    lRetVal = sl_WlanSetMode(ROLE_AP);
    ASSERT_ON_ERROR(lRetVal);

    lRetVal = sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_SSID,
        strlen(pcSsidName),
        (unsigned char*)pcSsidName);
    ASSERT_ON_ERROR(lRetVal);

    UART_PRINT("Device is configured in AP mode\n\r");

    /* Restart Network processor */
    lRetVal = sl_Stop(SL_STOP_TIMEOUT);

    // reset status bits
    CLR_STATUS_BIT_ALL(g_ulStatus);

    return sl_Start(NULL,NULL,NULL);
}

static long ConfigureSimpleLinkToDefaultState()
{
    static SlVersionFull   ver;
    static _WlanRxFilterOperationCommandBuff_t  RxFilterIdMask;

    unsigned char ucVal = 1;
    unsigned char ucConfigOpt = 0;
    unsigned char ucConfigLen = 0;
    unsigned char ucPower = 0;

    long lRetVal = -1;
    long lMode = -1;

    lMode = sl_Start(0, 0, 0);
    ASSERT_ON_ERROR(lMode);

    // If the device is not in station-mode, try configuring it in station-mode
    if (ROLE_STA != lMode)
    {
        if (ROLE_AP == lMode)
        {
            // If the device is in AP mode, we need to wait for this event
            // before doing anything
            while(!IS_IP_ACQUIRED(g_ulStatus))
            {
#ifndef SL_PLATFORM_MULTI_THREADED
              _SlNonOsMainLoopTask();
#endif
            }
        }

        // Switch to STA role and restart
        lRetVal = sl_WlanSetMode(ROLE_STA);
        ASSERT_ON_ERROR(lRetVal);

        lRetVal = sl_Stop(0xFF);
        ASSERT_ON_ERROR(lRetVal);

        UART_PRINT("start in Station Mode\r\n");
        lRetVal = sl_Start(0, 0, 0);
        ASSERT_ON_ERROR(lRetVal);

        // Check if the device is in station again
        if (ROLE_STA != lRetVal)
        {
            // We don't want to proceed if the device is not coming up in STA-mode
            return DEVICE_NOT_IN_STATION_MODE;
        }
    }

    // Get the device's version-information
    ucConfigOpt = SL_DEVICE_GENERAL_VERSION;
    ucConfigLen = sizeof(ver);
    lRetVal = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &ucConfigOpt,
                                &ucConfigLen, (unsigned char *)(&ver));
    ASSERT_ON_ERROR(lRetVal);

    UART_PRINT("Host Driver Version: %s\n\r",SL_DRIVER_VERSION);
    UART_PRINT("Build Version %d.%d.%d.%d.31.%d.%d.%d.%d.%d.%d.%d.%d\n\r",
    ver.NwpVersion[0],ver.NwpVersion[1],ver.NwpVersion[2],ver.NwpVersion[3],
    ver.ChipFwAndPhyVersion.FwVersion[0],ver.ChipFwAndPhyVersion.FwVersion[1],
    ver.ChipFwAndPhyVersion.FwVersion[2],ver.ChipFwAndPhyVersion.FwVersion[3],
    ver.ChipFwAndPhyVersion.PhyVersion[0],ver.ChipFwAndPhyVersion.PhyVersion[1],
    ver.ChipFwAndPhyVersion.PhyVersion[2],ver.ChipFwAndPhyVersion.PhyVersion[3]);

    // Set connection policy to Auto + SmartConfig
    //      (Device's default connection policy)
    lRetVal = sl_WlanPolicySet(SL_POLICY_CONNECTION,
                                SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);
    ASSERT_ON_ERROR(lRetVal);

    // Remove all profiles
    lRetVal = sl_WlanProfileDel(0xFF);
    ASSERT_ON_ERROR(lRetVal);

    //
    // Device in station-mode. Disconnect previous connection if any
    // The function returns 0 if 'Disconnected done', negative number if already
    // disconnected Wait for 'disconnection' event if 0 is returned, Ignore
    // other return-codes
    //
    lRetVal = sl_WlanDisconnect();
    if(0 == lRetVal)
    {
        // Wait
        while(IS_CONNECTED(g_ulStatus))
        {
#ifndef SL_PLATFORM_MULTI_THREADED
              _SlNonOsMainLoopTask();
#endif
        }
    }

    // Enable DHCP client
    lRetVal = sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE,1,1,&ucVal);
    ASSERT_ON_ERROR(lRetVal);

    // Disable scan
    ucConfigOpt = SL_SCAN_POLICY(0);
    lRetVal = sl_WlanPolicySet(SL_POLICY_SCAN , ucConfigOpt, NULL, 0);
    ASSERT_ON_ERROR(lRetVal);

    // Set Tx power level for station mode
    // Number between 0-15, as dB offset from max power - 0 will set max power
    ucPower = 0;
    lRetVal = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID,
            WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, 1, (unsigned char *)&ucPower);
    ASSERT_ON_ERROR(lRetVal);

    // Set PM policy to normal
    lRetVal = sl_WlanPolicySet(SL_POLICY_PM , SL_NORMAL_POLICY, NULL, 0);
    ASSERT_ON_ERROR(lRetVal);

    // Unregister mDNS services
    lRetVal = sl_NetAppMDNSUnRegisterService(0, 0);
    ASSERT_ON_ERROR(lRetVal);

    // Remove  all 64 filters (8*8)
    memset(RxFilterIdMask.FilterIdMask, 0xFF, 8);
    lRetVal = sl_WlanRxFilterSet(SL_REMOVE_RX_FILTER, (_u8 *)&RxFilterIdMask,
                       sizeof(_WlanRxFilterOperationCommandBuff_t));
    ASSERT_ON_ERROR(lRetVal);

    lRetVal = sl_Stop(SL_STOP_TIMEOUT);
    ASSERT_ON_ERROR(lRetVal);

    InitializeAppVariables();

    return lRetVal; // Success
}

void wifi_init(void) {
    long lRetVal = -1;
    UART_PRINT("Initializing Wifi\r\n");

    InitializeAppVariables();

    UART_PRINT("Configuring to Default State\r\n");
    lRetVal = ConfigureSimpleLinkToDefaultState();
    if(lRetVal < 0)
    {
        if (DEVICE_NOT_IN_STATION_MODE == lRetVal)
            UART_PRINT("Failed to configure the device in its default state\n\r");

        LOOP_FOREVER();
    }
    UART_PRINT("Device is configured in default state \n\r");

    //
    // Asumption is that the device is configured in station mode already
    // and it is in its default state
    //
    lRetVal = sl_Start(NULL,NULL,NULL);

    if (lRetVal < 0)
    {
        UART_PRINT("Failed to start the device \n\r");
        LOOP_FOREVER();
    }

    UART_PRINT("Device started as STATION \n\r");

    //
    // Configure the networking mode and ssid name(for AP mode)
    //
    if(lRetVal != ROLE_AP)
    {
        if(ConfigureAPMode() != ROLE_AP)
        {
            UART_PRINT("Unable to set AP mode, exiting Application...\n\r");
            sl_Stop(SL_STOP_TIMEOUT);
            LOOP_FOREVER();
        }
    }

    UART_PRINT("[!] Wifi initialized\r\n");
    g_wifiInitialized = 1;
}

void vUDPRecvTask(void *pvParameters) {
    //UART_PRINT("created Wifi Receiver task\r\n");
    size_t      recvSize;
    SlSockAddrIn_t  sAddr;

    g_iSockID = sl_Socket(SL_AF_INET,SL_SOCK_DGRAM, 0);
    sl_Bind(g_iSockID, (SlSockAddr_t *)&g_localAddr, iAddrSize);

    while(g_wifiConnected)
    {
        wifi_recv_ticks++;
        recvSize = sl_RecvFrom(g_iSockID, g_RecvBuf, RECV_BUF_SIZE, 0,
                 ( SlSockAddr_t *)&sAddr, (SlSocklen_t*)&iAddrSize );
            // now handle the buffer
        if (recvSize) {
            UdpCmd_read_delimited_from(g_RecvBuf, &g_udpCmdRecvStruct);
            g_wifiDataAvailable = true;
        }
        vTaskDelay(1);
    }
    sl_Close(g_iSockID);
}

void WifiConnectTask(void *pvParameters) {
    unsigned char ucDHCP;
    long lRetVal = -1;
    iAddrSize = sizeof(SlSockAddrIn_t);

    wifi_init();

    while(1)
    {
        wifi_task_ticks++;
        while(!IS_IP_ACQUIRED(g_ulStatus)) {}

        unsigned char len = sizeof(SlNetCfgIpV4Args_t);
        SlNetCfgIpV4Args_t ipV4 = {0};

        // get network configuration
        lRetVal = sl_NetCfgGet(SL_IPV4_AP_P2P_GO_GET_INFO,&ucDHCP,&len,
                                (unsigned char *)&ipV4);
        if (lRetVal < 0)
        {
            UART_PRINT("Failed to get network configuration \n\r");
            LOOP_FOREVER();
        }

        led_set(GREEN);
        UART_PRINT("Connect a client to Device\n\r");
        while(!IS_IP_LEASED(g_ulStatus)) {}
        UART_PRINT("Client is connected to Device\n\r");
        g_wifiConnected = true;

        wifi_recv_ticks = 0;
        // create a task here to handle connectoin
        xTaskCreate( vUDPRecvTask, NULL,
                OSI_STACK_SIZE, (void *) "UDPRecvTask", 5, &xRecvTaskHandle);
        while(g_wifiConnected){
            vTaskDelay(1);
        }
    }
}
