#include <stdint.h>
#include <pthread.h>
#include <ti/sysbios/BIOS.h>

#include "global.h"
#include "communicatie.h"
#include "noodstop.h"
#include "MPPT.h"

#define ERROR(msg) return; //TODO: create error handeler

//TODO: maybe detach pthread?
//TODO: add stacksize option
pthread_t createSimplePTread(int prio, void * fn){
  struct sched_param priParam;
  priParam.sched_priority = prio;

  pthread_attr_t pAttrs;
  pthread_attr_init(&pAttrs);
  pthread_attr_setschedparam(&pAttrs, &priParam);

  pthread_t thread;
  if( pthread_create(&thread, &pAttrs, fn, NULL) != 0 ){
    ERROR("could not create pthread")
  }

  return thread;
}

void startInit(){
  createSimplePTread(1, &comm_spi);
  createSimplePTread(1, &noodstop_init);
  createSimplePTread(1, &mppt_init);
  //TODO: add systeembeheer
}

int main(void)
{
  Board_init(); // initilaze board
  
  //TODO: add logic

  BIOS_start(); // start the BIOS
}
