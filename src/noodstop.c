#include <mqueue.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/ADC.h>

#include "global.h"
#include "communicatie.h"
#include "MPPT.h"

struct maxwaardes_t nood_maxwaardes;

extern pthread_t createSimplePTread(int prio, void * fn);

void nood_activeerNoodstop(){
  //BLOCK: zet noodstop GPIO als uitgang
  //TODO: do it!

  //BLOCK: noodstop GPIO = laag
  //TODO: do it!
}

void snelhied(uint16_t snelhied){
  //TODO: whoosh.

  //BLOCK: controleer snelheid
  if(snelhied > nood_maxwaardes.maxSnelheid){
    //BLOCK: vermogen overwrite = max vermogen
    mppt_vermogenOverride(nood_maxwaardes.maxVermogen);
    
    //BLOCK: Wacht 0.5 seconde
    usleep(500000);

    //BLOCK: activeer noostop
    nood_activeerNoodstop();
  }
}

void nood_tempratuurHandle(){
  do{
    //BLOCK: wacht 0.1 seconde
    usleep(100000);

    //BLOCK: lees ADC uit
    //TODO: do it!

    //BLOCK: bereken temperatuur
    //?? Yeti wat is die formule?
    
    //BLOCK: controleer temperatuur
  }while(tempratuur < nood_maxwaardes.maxTemptratuur);

  //BLOCK: vermogen override = 0W
  mppt_vermogenOverride(0);

  //BLOCK: activeer noostop
  nood_activeerNoodstop();
}

void nood_noodstopISR(){
  //BLOCK: Wacht 0.5 seconde
  usleep(500000);

  //BLOCK: activeer noostop
  nood_activeerNoodstop();
}

void nood_init(){
  //BLOCK: ADC init
  //TODO: do it!

  //BLOCK: GPIO init
  //TODO: do it!

  //BLOCK: wacht op Max waardes
  mq_receive(MaxWaardes_mq, (char*)&nood_maxwaardes, sizeof(nood_maxwaardes), NULL);

  //BLOCK: Update status
  if(Status == INIT){
    Status = NOODSTOP_READY;
  }else if(Status == MPPT_READY){
    Status = ALL_READY;
  }else{
    ERROR("invalid Status");
  }

  //BLOCK: wacht tot status == werken
  while(Status != WORKING){
    usleep(1000);
  }

  //BLOCK: maak tread voor ADC
  createSimplePTread(1, &nood_tempratuurHandle);

  //BLOCK: Maak interrupt voor noodstop signaal
  //TODO: do it!
}
