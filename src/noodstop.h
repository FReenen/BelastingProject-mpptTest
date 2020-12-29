#ifndef NOODSTOP_H
#define NOODSTOP_H

#include <stdint.h>

void noodstop_setMaxVermogen(uint8_t value);
void noodstop_setMaxSnelheid(uint16_t value);
void noodstop_setMaxTemptratuur(uint8_t value);

void noodstop_snelhied(uint16_t snelhied);
void noodstop_init();

#endif