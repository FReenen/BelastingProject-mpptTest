#include <ti/drivers/PWM.h>

#include "global.h"
#include "MPPT.h"

#define US_TO_COUNT(n) n*1000

void mppt_vermogenOverride(uint8_t vermogen){
  return;
}

void mppt_init(){
  PWM_init();

  PWM_Params params;
  PWM_Params_init(&params);
  params.dutyUnits = PWM_PERIOD_COUNTS;
  params.dutyValue = US_TO_COUNT(10);
  params.periodUnits = PWM_PERIOD_US;
  params.periodValue = 0;
  PWM_Handle pwm = PWM_open(CONFIG_PWM_0, &params);
  if (pwm == NULL) {
    while (1);
  }
}
