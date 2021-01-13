#include "global.h"
#include "communicatie.h"

#define SPI_PACKET_LENGTH 4


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

// communicatie spi Taak
void comm_spi(){
  // Initialiseer SPI
  ISP_init();
  SPI_Params spiParams;
  SPI_Params_init(&spiParams);
  spiParams.frameFormat = SPI_POL0_PHA0; // mode0
  spiParams.bitRate = 1E6; // 1 MHz
  SPI_Handle masterSpi = SPI_open(CONFIG_SPI_MASTER, &spiParams);
  if (masterSpi == NULL) {
      // Display_printf(display, 0, 0, "Error initializing master SPI\n");
      while (1);
  } else {
      // Display_printf(display, 0, 0, "Master SPI initialized\n");
  }

  // create transaction
  SPI_Transaction spiTransaction;
  spiTransaction.count = SPI_PACKET_LENGTH;
  spiTransaction.txBuf = (void *) TransmitBuffer;
  spiTransaction.rxBuf = (void *) ReciveBuffer;

  // start the loop
  TransmitBuffer[0] = 0;
  while(1){
    // zero the buffers
    memset((void *) TransmitBuffer+1, 0, SPI_PACKET_LENGTH-1);
    memset((void *) ReciveBuffer, 0, SPI_PACKET_LENGTH);

    // set data in the transmit buffer
    ReciveBuffer[0] = TransmitBuffer[0];
    switch(TransmitBuffer[0]){
      case SPIPARM_vermogen:
        TransmitBuffer[1] = mppt_getVermogen();
        break;
      case SPIPARM_snelheid:
        TransmitBuffer[1] = comm_snelheid;
        break;
      case SPIPARM_tempratuur: 
        TransmitBuffer[1] = noodstop_getTemptatuur();
        break;
    }

    // do the transaction
    //TODO: set the CS
    SPI_transaction(masterSpi, &spiTransaction);

  	// read the data out the recive buffer
    switch (ReciveBuffer[0]){
      case SPIPARM_maxVermogen:
        uint8_t maxVermogen = ReciveBuffer[2];
        noodstop_setMaxVermogen(maxVermogen);
        break;
      case SPIPARM_maxSnelheid:
        uint16_t maxSnelheid = ReciveBuffer[2] + ReciveBuffer[3]*0x100; // convert littelendien
        noodstop_setMaxSnelheid(maxSnelheid);
        break;
      case SPIPARM_maxTempratuur:
        uint8_t maxTemptratuur = &ReciveBuffer[2];
        noodstop_setMaxTemptratuur(maxTemptratuur);
        break;
      case SPIPARM_setpoint:
        uint8_t setpoint = &ReciveBuffer[2];
        mppt_setpoint(setpoint);
        break;
    }

    // increment and loop parameter
    TransmitBuffer[0]++;
    if(TransmitBuffer[0] == SPIPARM_count)
      TransmitBuffer[0] = 0;

    usleep(5000);
}
