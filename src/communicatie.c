#include <ti/drivers/SPI.h>

#include "debug.h"

#include "global.h"
#include "communicatie.h"
#include "spi.h"

#define SPI_PACKET_LENGTH 4

SPI_Trans_t SPI_MSPTrans;
unsigned char ReciveBuffer[SPI_PACKET_LENGTH];
unsigned char TransmitBuffer[SPI_PACKET_LENGTH];

uint16_t comm_snelheid;

typedef enum {
  SPIPARM_maxVermogen,
  SPIPARM_maxSnelheid,
  SPIPARM_maxTempratuur,
  SPIPARM_setpoint,
  SPIPARM_vermogen,
  SPIPARM_snelheid,
  SPIPARM_tempratuur,
  SPIPARM_count
} SPI_Register_t;

#define SPI_INTERVAL (1/100) / SPIPARM_count // update all params 100 time a second

// communicatie spi Taak
void comm_init(){
  // create SPI transaction
  SPI_Params_init(&SPI_MSPTrans.params);
  SPI_MSPTrans.interface = CONFIG_SPI_MASTER;
  SPI_MSPTrans.cs = CONFIG_GPIO_CSMSP;
  SPI_ADC.params.frameFormat = SPI_POL0_PHA0; // mode 0
  SPI_MSPTrans.params.bitRate = 1E6; // 1 MHz
  SPI_MSPTrans.trans.count = SPI_PACKET_LENGTH;
  SPI_MSPTrans.trans.txBuf = &TransmitBuffer[0];
  SPI_MSPTrans.trans.rxBuf = &ReciveBuffer[0];

  // start the loop
  TransmitBuffer[0] = 0;
  while(1){
    // zero the buffers
    memset(&TransmitBuffer[0] + 1, 0, SPI_PACKET_LENGTH-1);
    // memset(&ReciveBuffer[0], 0, SPI_PACKET_LENGTH);

    // set data in the transmit buffer
    switch(TransmitBuffer[0]){
      case SPIPARM_vermogen:
        TransmitBuffer[1] = mppt_getVermogen();
        break;
      case SPIPARM_snelheid:
        TransmitBuffer[1] = comm_snelheid;
        break;
      case SPIPARM_tempratuur: 
        TransmitBuffer[1] = noodstop_getTempratuur();
        break;
    }

    // do the transaction
    SPI_trans(&SPI_MSPTrans);

  	// read the data out the receive buffer
    switch (ReciveBuffer[0]){
      case SPIPARM_maxVermogen:
        noodstop_setMaxVermogen(ReciveBuffer[2]);
        break;
      case SPIPARM_maxSnelheid:
        noodstop_setMaxSnelheid(ReciveBuffer[2] + ReciveBuffer[3]*0x100); // littel endian
        break;
      case SPIPARM_maxTempratuur:
        noodstop_setMaxTemptratuur(ReciveBuffer[2]);
        break;
      case SPIPARM_setpoint:
        mppt_setSetpoint(ReciveBuffer[2]);
        break;
    }

    // increment and loop parameter
    TransmitBuffer[0]++;
    if(TransmitBuffer[0] == SPIPARM_count)
      TransmitBuffer[0] = 0;

    usleep(SPI_INTERVAL);
  }
}
