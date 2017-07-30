#ifndef FSM_H_
#define FSM_H_

#include "battery.h"

typedef void(*functionPointer)();
typedef functionPointer(*State)();

State idle(void);
State start(void);
State measure(Battery *const battery);
State estimate_soc(Battery *const battery);
State compute_resistance(void);
State compute_capacity(void);
State balancing(void);
State send_data(Battery *const battery);
State shutdown(void);
#endif /* FSM_H_ */