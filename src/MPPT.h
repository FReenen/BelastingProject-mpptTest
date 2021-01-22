#ifndef MPPT_H
#define MPPT_H

#include <stdint.h>

void mppt_init();
void mppt_start();
void mppt_deinit();
void mppt_setSetpointOverride(uint8_t vermogen);
void mppt_setSetpoint(uint8_t vermogen);

#endif
