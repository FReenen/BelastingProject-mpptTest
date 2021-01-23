#include <stdint.h>
#include <ti/drivers/PWM.h>

#include "debug.h"
#include "global.h"
#include "MPPT.h"
#include "spi.h"

#define PWM_MIN 0.001
#define PWM_MAX 0.9

uint8_t mppt_setpoint = 0xff, mppt_setpointOverrite, mppt_vermogen;
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
    ERROR("faild to open PWM");
    return;
  }

  mppt_setpointP = &mppt_setpoint;
  mppt_start();

  // whit until it recevs a valid setpoint
  while(mppt_setpoint > 250)
    usleep(1e3);

  // Update status
  if(Status == INIT){
    Status = MPPT_READY;
  }else if(Status == NOODSTOP_READY){
    Status = ALL_READY;
  }else{
    ERROR("invalid Status");
  }
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

  //TODO: addjust stap size so that big stap is a bit less than 5 wattand small stap is less than .5 watt
  double duty = 0, bigStap = 0.05, smallStap = 0.002;
  enum mppt_state {
    bigStaps,
    smallStaps
  } state;
  int16_t vermogenDiff, lastVermogen;
  while(1){
    mppt_setPWM(duty);
    if(state == bigStaps){ // big stap state - finding roughly the setpoint a bit faster or find the peek power point
      while(1){
        // set stap
        if(mppt_vermogen - *(mppt_setpointP) > 0){
          duty += bigStap;
        }else{
          duty -= bigStap;
        }
        mppt_setPWM(duty);

        // wait for change to happen
        //TODO: finetune delay
        usleep(10E3);

        // mesure power
        mppt_meetVermogen();

        // check the dirrerance width the setpoint
        vermogenDiff = mppt_vermogen - *(mppt_setpointP);
        if(vermogenDiff < 0) // get absolute value
          vermogenDiff  = -vermogenDiff;

        //TODO: don't let the peek power point detecten trigger if it oversoots or needs to ajust down.
        if(vermogenDiff < 50 || mppt_vermogen < lastVermogen){
          state = smallStaps;
          break;
        }
      }
    }else{ // small stap state - aparanly it's close to the target. now get closer to it.
      while(1){
        // set stap
        if(mppt_vermogen - *(mppt_setpointP) > 0){
          duty += smallStap;
        }else{
          duty -= smallStap;
        }
        mppt_setPWM(duty);

        // wait for change to happen
        //TODO: finetune delay
        usleep(10E3);

        // mesure power
        mppt_meetVermogen();

        // check the dirrerance width the setpoint
        vermogenDiff = mppt_vermogen - *(mppt_setpointP);
        if(vermogenDiff < 0) // get absolute value
          vermogenDiff  = -vermogenDiff;

        if(vermogenDiff > 50){
          state = bigStaps;
          break;
        }

        if(vermogenDiff > 5){
          break;
        }

        // we reached the setpoint within 0.5 watts
        usleep(100E3);
        mppt_meetVermogen();
      }
    }
  }
}

void mppt_deinit(){
  mppt_setpoint = 0xff;
  mppt_setPWM(0);
  PWM_stop(mppt_pwm);
}
