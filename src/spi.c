#include "spi.h"

#include "global.h"
#include "debug.h"


uint8_t ADC_TxBuf[4];
uint8_t ADC_RxBuf[4];

void SPI_Init(){
  SPI_init();

  // init ADC
  SPI_Params_init(&SPI_ADC.params);
  SPI_ADC.interface = CONFIG_SPI_MASTER;
  SPI_ADC.cs = CONFIG_GPIO_CSMSP;
  SPI_ADC.params.frameFormat = SPI_POL0_PHA0; // mode 0
  SPI_ADC.params.bitRate = 1E6; // 1 MHz
  SPI_ADC.trans.count = 4;
  SPI_ADC.trans.txBuf = &ADC_TxBuf;
  SPI_ADC.trans.rxBuf = &ADC_RxBuf;

  ADC_TxBuf[0] = 1; // set start bit
}

uint16_t ADC_read(uint8_t ch){
  while(SPI_Handaler != NULL)
    usleep(10);
  ADC_TxBuf[1] = ch; // set channel
  SPI_trans(&SPI_ADC);
  return (ADC_TxBuf[2] & 0b11) * 0x100 | ADC_TxBuf[3];
}

int SPI_trans(SPI_Trans_t *trans){
  SPI_Handle spih;

  // wait until last transation is done
  while(SPI_Handaler != NULL)
    usleep(10);
  SPI_Handaler = &spih;

  // open spi connection with paramaters of this one
  spih = SPI_open(CONFIG_SPI_MASTER, &trans->params);
  if (spih == NULL) {
    ERROR("initialising SPI for communicating with the MSP\n");
    return 1;
  }

  // do the transaction
  GPIO_write(trans->cs, 0);
  SPI_transfer(spih, &trans->trans);
  GPIO_write(trans->cs, 1);

  // close it so its can be used next time
  SPI_close(spih);
  SPI_Handaler = NULL;
  return 0;
}
