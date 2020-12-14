#include <ti/drivers/I2CSlave.h>

#include "communicatie.h"

#define COMM_I2C_ADDRESS 0xB1

extern pthread_t createSimplePTread(int prio, void * fn);

union I2CPacket_t PacketBuffer; 

void dataBijhouden(){
  //BLOCK: wacht op werken
  //??

  while(1){
    //BLOCK: vermogen DUT aflezen
    //??

    //BLOCK: Snelheid aflezen
    //??

    //BLOCK: bereken koppel
    //??

    //BLOCK: Vermogen belasting aflezen
    //??

    //BLOCK: Spanning belasting aflezen
    //??

    //BLOCK: bereken rendement
    //??

    //BLOCK: Temperatuur aflezen
    //??

    //BLOCK: sleep 1ms
    usleep(1000);
  }
}

// communicatie I2C Taak
void comm_i2c(){

  //BLOCK: init start begint
  //??

  //BLOCK: Initialiseer I2C
  I2C_init();
  //TODO: findout corect driver for this
  Slave_Handle      handle;
  I2CSlave_Params   params;
  I2CSlave_Params_init(&params);
  params.transferMode = I2CSLAVE_MODE_BLOCKING;
  params.slaveAddress = COMM_I2C_ADDRESS;
  handle = I2CSlave_open(CONFIG_I2C_0, &params);
  if (!handle) {
    System_printf("I2CSlave did not open");
  }

  //BLOCK: GPIO init
  //??

  //BLOCK: maak thread aan om waardes te updaten
  createSimplePTread(1, &dataBijhouden);

  I2CPacket_t packet;

  while(1){
    //BLOCK: read package
    //TODO: figgerout how it actualy works
    I2C_read(handle, &packet, sizeof(Packet));

    //BLOCK: send reply
    //TODO: figgerout how it actualy works
    I2C_send(handle, &PacketBuffer, sizeof(PacketBuffer));

  	//BLOCK: identificeer package
    switch (packet){
      case PACKET_UPDATEBELASTING:
        //BLOCK: Update setpoint
        mppt_setpoint(packet.vermogenSetpoint);
        break;
      
      case PACKET_NOODSTOP:
        //BLOCK: Update Overheat, Overspeed, overload waardes updaten
        //??
        break;

      case PACKET_INITPARAMS:
        //BLOCK: Verstuur data naar noodstop
        maxwaardes_t p;
        p.maxTemperatuur = packet.maxTemperatuur;
        p.maxVermogen = packet.maxVermogen;
        p.maxSnelheid = packet.maxSnelheid;
        //mq_send(MaxWaardes_mq, (const char*)&p, sizeof(p), 3);

        //BLOCK: Verstuur data naar MPPT
        mppt_setpoint(packet.vermogenSetpoint);
        break;
    }
}
