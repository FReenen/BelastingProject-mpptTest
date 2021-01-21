#include <stdint.h>
#include <ti/drivers/PWM.h>

#include "global.h"
#include "MPPT.h"
#include "spi.h"

#define PWM_MIN 0.001
#define PWM_MAX 0.9

uint8_t mppt_setpoint, mppt_setpointOverrite, mppt_vermogen;
uint8_t *mppt_setpointP;

PWM_Handle mppt_pwm;

void mppt_setSetpointOverride(uint8_t vermogen){
  mppt_setpointOverrite = vermogen;
  mppt_setpointP = &mppt_setpointOverrite;
}
void mppt_setSetpoint(uint8_t vermogen){
  mppt_setpoint = vermogen;
}
uint8_t mppt_getVermogen(){
  return mppt_vermogen;
}

void mppt_meetVermogen(){
  uint16_t spanning = ADC_read(ADC_CH2);
  uint16_t curent = ADC_read(ADC_CH3);
  //TODO: add real formula for spanning and curent
  mppt_vermogen = (spanning * curent) / 100;
}

void mppt_init(){
  PWM_init();

  PWM_Params params;
  PWM_Params_init(&params);
  params.dutyUnits = PWM_DUTY_FRACTION;
  params.dutyValue = 0;
  params.periodUnits = PWM_PERIOD_US;
  params.periodValue = 10; // 100 kHz
  mppt_pwm = PWM_open(CONFIG_PWM_0, &params);
  if (mppt_pwm == NULL) {
    while (1);
  }

  mppt_setpointP = &mppt_setpoint;
  mppt_start();
}

void mppt_setPWM(double d){
  if(d < PWM_MIN){
    PWM_setDuty(mppt_pwm, 0);
    return;
  }
  if(d > PWM_MAX){
    d = PWM_MAX;
  }
  PWM_setDuty(mppt_pwm, (uint32_t) ((double) PWM_DUTY_FRACTION_MAX * d));
}

void mppt_start(){
  PWM_start(mppt_pwm);

  //TODO: implement the mppt algaritme
  double duty = 0, step = 0.01;
  while(1){ //(Status == WORKING){
    mppt_setPWM(duty);
    duty += step;
    if(duty > 1 || duty < 0){
      step = -step;
      duty += step;
    }
    usleep(100);
  }
}
