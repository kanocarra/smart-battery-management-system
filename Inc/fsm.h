#ifndef FSM_H_
#define FSM_H_

#include "battery.h"

typedef void(*functionPointer)();
typedef functionPointer(*State)();

State idle(Battery *const battery);
State start(Battery *const battery);
State measure(Battery *const battery);
State estimate_soc(Battery *const battery);
State compute_resistance(Battery *const battery);
State compute_capacity(Battery *const battery);
State balancing(Battery *const battery);
State send_data(Battery *const battery);
State shutdown(Battery *const battery);
#endif /* FSM_H_ */