#include <ti/drivers/SPI.h>
#include <ti/drivers/GPIO.h>

#include <stdint.h>

#define ADC_SINGELCH 1 << 7
#define ADC_CH0 ADC_SINGELCH | 0 << 4
#define ADC_CH1 ADC_SINGELCH | 1 << 4
#define ADC_CH2 ADC_SINGELCH | 2 << 4
#define ADC_CH3 ADC_SINGELCH | 3 << 4
#define ADC_CH4 ADC_SINGELCH | 4 << 4
#define ADC_CH5 ADC_SINGELCH | 5 << 4
#define ADC_CH6 ADC_SINGELCH | 6 << 4
#define ADC_CH7 ADC_SINGELCH | 7 << 4
#define ADC_CH01 0 << 4
#define ADC_CH10 1 << 4
#define ADC_CH23 2 << 4
#define ADC_CH32 3 << 4
#define ADC_CH45 4 << 4
#define ADC_CH54 5 << 4
#define ADC_CH67 6 << 4
#define ADC_CH76 7 << 4

typedef struct SPI_Trans_t {
  SPI_Transaction trans;
  uint_least8_t interface;
  SPI_Params params;
  uint_least8_t cs;
} SPI_Trans_t;
SPI_Handle *SPI_Handaler;

SPI_Trans_t SPI_ADC;

void SPI_Init();
uint16_t ADC_read(uint8_t ch);
int SPI_trans(SPI_Trans_t *trans);
