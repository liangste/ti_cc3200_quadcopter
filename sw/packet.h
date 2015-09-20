#ifndef __PACKET_H__
#define __PACKET_H__

#define ACTUAL_UdpCmd_Size 25
#define MAX_UdpCmd_SIZE 32

struct UdpCmd {
    signed long _x_Left;
    signed long _x_Right;
    signed long _y_Left;
    signed long _y_Right;
    signed long _timestamp;
};

void UdpCmd_read_delimited_from(void *_buffer, struct UdpCmd *_UdpCmd);

#endif
