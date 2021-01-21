#include <pthread.h>
#include <stdint.h>

#include "debug.h"

#include "global.h"
#include "communicatie.h"
#include "MPPT.h"
#include "spi.h"

struct {
  uint8_t maxTempratuur;
  uint8_t maxVermogen;
  uint16_t maxSnelheid;
} nood_maxwaardes;
pthread_t adcThread;
uint8_t temp;

void noodstop_setMaxVermogen(uint8_t value){
  nood_maxwaardes.maxVermogen = value;
}
void noodstop_setMaxSnelheid(uint16_t value){
  nood_maxwaardes.maxSnelheid = value;
}
void noodstop_setMaxTemptratuur(uint8_t value){
  nood_maxwaardes.maxTempratuur = value;
}

void noodstop_activeerNoodstop(){
  // zet noodstop GPIO als uitgang en laag
  GPIO_setConfig(CONF_GPIO_NOODSTOP, GPIO_CFG_OUT_LOW); //TODO: check config
}

void noodstop_snelhied(uint16_t snelhied){
  // controleer snelheid
  if(snelhied > nood_maxwaardes.maxSnelheid){
    // PANIC!!
    mppt_setSetpointOverride(nood_maxwaardes.maxVermogen);
    
    // Wacht 0.5 seconde
    usleep(500E3);

    // activeer noostop
    Status = OVERSPEED;
    noodstop_activeerNoodstop();
  }
}

uint8_t noodstop_getTempratuur(){
  return temp;
}

uint8_t noodstop_readTemp(uint8_t ch){
  uint16_t v = ADC_read(&SPI_ADC);

  //TODO: add real fomula

  return v >> 2;
}

void noodstop_tempratuurHandle(){
  uint8_t tempB;
  do{
    // wacht 0.1 seconde
    usleep(100E3);

    // read tempretures
    temp = noodstop_readTemp(ADC_CH0);
    tempB = noodstop_readTemp(ADC_CH1);
    if(temp < tempB)
      temp = tempB;

    // controleer temperatuur
  }while(temp < nood_maxwaardes.maxTempratuur);

  // vermogen override = 0W
  mppt_setSetpointOverride(0);

  // activeer noostop
  Status = OVERHEAD;
  noodstop_activeerNoodstop();
}

/** noodstopISR()
 * ISR for the external emergency stop from the button or other devides
 * The ISR is created in the syscfg.
 */
void noodstopISR(){
  // Wacht 0.5 seconde
  usleep(500E3);

  // activeer noostop
  Status = EXT_NOODSTOP;
  noodstop_activeerNoodstop();
}

void noodstop_init(){
  // GPIO init
  GPIO_setConfig(CONF_GPIO_NOODSTOP, GPIO_CFG_IN_PU); //TODO: check config

  memset(&nood_maxwaardes, 0xff, sizeof(nood_maxwaardes)); // set all max value to invalid ones

  // controleer max waardes
  while(1){
    if(
         nood_maxwaardes.maxSnelheid <= 60000
      && nood_maxwaardes.maxTempratuur >= 60 && nood_maxwaardes.maxTempratuur <= 120
      && nood_maxwaardes.maxVermogen <= 250
    ){
      break; // stop the loop values are valid
    }
    usleep(100E3);
  }

  // Update status
  if(Status == INIT){
    Status = NOODSTOP_READY;
  }else if(Status == MPPT_READY){
    Status = ALL_READY;
  }else{
    ERROR("invalid Status");
  }
}

void noodstop_start(){
  // start the treath voor het uitlezen van de temptatuur
  adcThread = createSimplePTread(2, &noodstop_tempratuurHandle);
}

void noodstop_deinit(){
  // stop threads
  pthread_exit(adcThread);
}
