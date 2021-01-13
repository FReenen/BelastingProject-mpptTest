#include <stdint.h>
#include <ti/drivers/PWM.h>

#include "global.h"
#include "MPPT.h"

#define PWM_MIN 0.001
#define PWM_MAX 0.9

uint8_t mppt_setpoint, mppt_setpointOverrite;
uint8_t *mppt_setpointP;

PWM_Handle mppt_pwm;

void mppt_setSetpointOverride(uint8_t vermogen){
  mppt_setpointOverrite = vermogen;
  mppt_setpointP = &mppt_setpointOverrite;
}
void mppt_setSetpoint(uint8_t vermogen){
  mppt_setpoint = vermogen;
}

void mppt_init(){
  PWM_init();

  PWM_Params params;
  PWM_Params_init(&params);
  params.dutyUnits = PWM_DUTY_FRACTION;
  params.dutyValue = 0;
  params.periodUnits = PWM_PERIOD_US;
  params.periodValue = 0;
  mppt_pwm = PWM_open(CONFIG_PWM_0, &params);
  if (mppt_pwm == NULL) {
    while (1);
  }

  mppt_setpointP = &mppt_setpoint;
}


void mppt_setPWM(double d){
  if(d < PWM_MIN){
    PWM_setDuty(mppt_pwm, 0);
    return;
  }
  if(d > PWM_MAX){
    d = 0.9;
  }
  PWM_setDuty(mppt_pwm, (uint32_t) ((double) PWM_DUTY_FRACTION_MAX * d));
}

void mppt_start(){
  PWM_start(mppt_pwm);

  double duty = 0, step = 0.01;
  while(Status == WORKING){
    mppt_setPWM(duty);
    duty += step;
    if(duty > 1){
      step = -step;
      duty += step;
    }
  }
}