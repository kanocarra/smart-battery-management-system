#ifndef __COMMS_H
#define __COMMS_H

#include "battery.h"
#include "bms.h"

void send_packet(Battery const* battery);
void send_status(StatusA* status_regA, StatusB* status_regB);

#endif /*__ COMMS_H */