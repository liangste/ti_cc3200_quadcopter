#include "config.h"

void UdpCmd_read_delimited_from(void *_buffer, struct UdpCmd *_UdpCmd) {
    char * cbuf = (char*) _buffer;
    uint32_t tmp_val;

    // cbuf[0] is a placeholder
    tmp_val = 0;
    tmp_val |= cbuf[1];
    tmp_val |= cbuf[2] << 8;
    tmp_val |= cbuf[3] << 16;
    tmp_val |= cbuf[4] << 24;
    _UdpCmd->_x_Left = tmp_val;

    // cbuf[5] is a placeholder
    tmp_val = 0;
    tmp_val |= cbuf[6];
    tmp_val |= cbuf[7] << 8;
    tmp_val |= cbuf[8] << 16;
    tmp_val |= cbuf[9] << 24;
    _UdpCmd->_x_Right = tmp_val;

    // cbuf[10] is a placeholder
    tmp_val = 0;
    tmp_val |= cbuf[11];
    tmp_val |= cbuf[12] << 8;
    tmp_val |= cbuf[13] << 16;
    tmp_val |= cbuf[14] << 24;
    _UdpCmd->_y_Left = tmp_val;

    // cbuf[15] is a placeholder
    tmp_val = 0;
    tmp_val |= cbuf[16];
    tmp_val |= cbuf[17] << 8;
    tmp_val |= cbuf[18] << 16;
    tmp_val |= cbuf[19] << 24;
    _UdpCmd->_y_Right = tmp_val;

    // cbuf[20] is a placeholder
    tmp_val = 0;
    tmp_val |= cbuf[21];
    tmp_val |= cbuf[22] << 8;
    tmp_val |= cbuf[23] << 16;
    tmp_val |= cbuf[24] << 24;
    _UdpCmd->_timestamp = tmp_val;
}
