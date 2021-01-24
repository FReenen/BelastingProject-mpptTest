#include <Wire.h>
#include <SPI.h>
#include <stdint.h>

#define I2C Wire

#define startInit_PIN 9
#define startSys_PIN  8
#define ready_PIN     7
#define noodstop_PIN  6

#define CSK_PIN   13
#define MISO_PIN  12
#define MOSI_PIN  11
#define CSMSP_PIN 10

typedef enum {
  SPIPARM_maxVermogen,
  SPIPARM_maxSnelheid,
  SPIPARM_maxTemperatuur,
  SPIPARM_setpoint,
  SPIPARM_vermogen,
  SPIPARM_snelheid,
  SPIPARM_temperatuur,
  SPIPARM_count
} SPI_Register_t;
uint8_t SPI_RxBuffer[4];
uint8_t * SPI_TxBuffer;
uint8_t SPI_Params[SPIPARM_count][2];
uint8_t pos;

void setSetpoint(uint8_t v){
  SPI_Params[SPIPARM_setpoint][0] = v;
}
void setMaxVermogen(uint8_t v){
  SPI_Params[SPIPARM_maxVermogen][0] = v;
}
void setMaxTemperatuur(uint8_t v){
  SPI_Params[SPIPARM_maxTemperatuur][0] = v;
}
void setMaxSnelheid(uint16_t v){
  SPI_Params[SPIPARM_maxSnelheid][0] = v & 0x0FF;
  SPI_Params[SPIPARM_maxSnelheid][1] = v > 8;
}

void setup(){
  Serial.begin(115200);
  Serial.println("The Fantastic Five - Belasting regelaar - MPPT Test");
  Serial.println("Init Arduino");

  // init GPIO
  pinMode(noodstop_PIN, INPUT);
  digitalWrite(startInit_PIN, LOW);
  pinMode(startInit_PIN, OUTPUT);
  digitalWrite(startSys_PIN, LOW);
  pinMode(startSys_PIN, OUTPUT);

  // init I2C
  I2C.begin();

  // init SPI
  pinMode(MISO_PIN, OUTPUT);
  SPCR |= _BV(SPE); // set SPI in slave mode
  SPI.attachInterrupt();

  Serial.println("Init CC3220S");

  // set all SPI params to 0
  memset(&SPI_Params[0][0], 0x00, sizeof(SPI_Params));

  // set max limits
  setMaxVermogen(250);    // deci watts; 0 to 250
  setMaxTemperatuur(120); // deg c; 60 to 120
  setMaxSnelheid(60000);  // RPM; 0 to 60000

  // set inital setpoint
  setSetpoint(50); // deci watt; 0 to 250

  // trigger the initalisation
  digitalWrite(startInit_PIN, HIGH);

  // sync paramaters until the CC3220S is ready
  uint16_t timer = millis();
  while(!digitalRead(ready_PIN)){
    spiRun();
  }

  Serial.println("Start CC3220S");

  // start the system
  digitalWrite(startSys_PIN, 1);
}

ISR(SPI_STC_vect){
  SPI_RxBuffer[pos] = SPDR;
  if(pos == 0)
    SPDR = *(SPI_TxBuffer);
  else
    SPDR = *(SPI_TxBuffer + 1);
  pos--;
}

void spiRun(){
  while(pos == 3)
    continue;
  
  SPI_TxBuffer = &SPI_Params[SPI_RxBuffer[3]][0];

  while(!digitalRead(CSMSP_PIN))
    continue;
  
  pos = 3;
}

void loop(){
  spiRun();

  switch(SPI_RxBuffer[3]){
    case SPIPARM_vermogen:
      Serial.print("vermogen: ");
      Serial.print(SPI_RxBuffer[1], DEC);
      Serial.println(" deci watt");
      if(SPI_RxBuffer[1] > 3 == SPI_Params[SPIPARM_setpoint][0] > 3){
        Serial.println("reached setpoint :)");
        setSetpoint((SPI_Params[SPIPARM_setpoint][0] == 50) ? 100 : 50);
      }
      break;
    case SPIPARM_snelheid:
      Serial.print("snelheid: ");
      Serial.print(SPI_RxBuffer[1] + SPI_RxBuffer[0]*0x100, DEC);
      Serial.println(" RPM");
      break;
    case SPIPARM_temperatuur:
      Serial.print("temperatuur: ");
      Serial.print(SPI_RxBuffer[1], DEC);
      Serial.println(" dec c");
      break;
  }
}
