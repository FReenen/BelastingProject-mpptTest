#include <stdint.h>
#include <pthread.h>
#include <ti/sysbios/BIOS.h>
#include "ti_drivers_config.h"

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

int main(void)
{
  Board_init(); // initilaze board

  

  BIOS_start(); // start the BIOS
}
