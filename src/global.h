#ifndef GLOBALS_H
#define GLOBALS_H

#include "ti_drivers_config.h"

enum {
  SLEEP,
  INIT,
  MPPT_READY,
  NOODSTOP_READY,
  ALL_READY,
  WORKING,
  OVERHEAD,
  OVERLOAD,
  OVERSPEED,
  EXT_NOODSTOP
} Status;

#endif