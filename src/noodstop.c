#include <ti/drivers/GPIO.h>
#include <ti/drivers/ADC.h>
#include <ti/drivers/SPI.h>
#include <pthread.h>

#include "global.h"
#include "communicatie.h"
#include "MPPT.h"

struct {
  uint8_t maxTempratuur;
  uint8_t maxVermogen;
  uint16_t maxSnelheid;
} nood_maxwaardes;

extern pthread_t createSimplePTread(int prio, void * fn);

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

  // set status
}

void noodstop_snelhied(uint16_t snelhied){
  // controleer snelheid
  if(snelhied > nood_maxwaardes.maxSnelheid){
    // PANIC!!
    mppt_vermogenOverride(nood_maxwaardes.maxVermogen);
    
    // Wacht 0.5 seconde
    usleep(500E3);

    // activeer noostop
    Status = OVERSPEED;
    noodstop_activeerNoodstop();
  }
}

void noodstop_tempratuurHandle(){
  uint8_t tempratuur;
  do{
    // wacht 0.1 seconde
    usleep(100E3);

    // lees ADC uit
    //TODO: do it!

    // bereken temperatuur
    //?? Yeti wat is die formule?
    tempratuur = ~0;
    
    // controleer temperatuur
  }while(tempratuur < nood_maxwaardes.maxTempratuur);

  // vermogen override = 0W
  mppt_vermogenOverride(0);

  // activeer noostop
  Status = OVERHEAD;
  noodstop_activeerNoodstop();
}

void noodstop_noodstopISR(){
  // Wacht 0.5 seconde
  usleep(500000);

  // activeer noostop
  Status = EXT_NOODSTOP;
  noodstop_activeerNoodstop();
}

void noodstop_init(){
  // ADC init
  //TODO: do it!

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

  // wacht tot status == werken
  while(Status != WORKING){
    usleep(1E3);
  }

  // start the treath voor het uitlezen van de temptatuur
  pthread_t adcThread = createSimplePTread(1, &noodstop_tempratuurHandle);

  // Maak interrupt voor noodstop signaal
  //TODO: do it!

  // wait until it stops
  while(Status = WORKING){
    usleep(5E3);
  }

  // stop threadhs
  Task_delete(adcThread);
  //TODO: deinit ADC
}
