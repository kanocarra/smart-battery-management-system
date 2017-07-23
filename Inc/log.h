//SD library includes
#include "defines.h"
#include "tm_stm32_delay.h"
#include "tm_stm32_fatfs.h"
#include "tm_stm32_gpio.h"
#include "tm_stm32_spi.h"
#include "bms.h"

void open_log_file(void);

void log_data(Battery *const battery);
