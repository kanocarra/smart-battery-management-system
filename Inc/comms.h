#ifndef __COMMS_H
#define __COMMS_H

#include "usart.h"
#include "battery.h"


void send_packet(Battery const* battery);

#endif /*__ COMMS_H */