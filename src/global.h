#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>
#include "ti_drivers_config.h"

typedef enum Status_t {
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
} Status_t;

Status_t Status;

pthread_t createSimplePTread(int prio, void * fn);

#endif
