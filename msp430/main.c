#include <msp430.h>
#include <stdint.h>

#define READBUFFER_LEN 10
#define WRITEBUFFER_LEN 10

// this buffers store the the receved and send data
unsigned char ReadBuffer[READBUFFER_LEN];
unsigned char WriteBuffer[WRITEBUFFER_LEN];

// the pointers are used to remenber the location it was on the buffer
unsigned char *ReadBuffer_p = &ReadBuffer[0];
unsigned char *WriteBuffer_p = &WriteBuffer[0];

// if it finisched receving/sending a packet this wil become true
bool NewPacket = false;

enum {SLEEP, WRITE, READ} State;
typedef enum {} PacketID_t; //TODO: add packet ids

void Setup(void);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD; // Stop WDT

  P1SEL |= BIT6 + BIT7;     // Assign I2C pins to USCI_B0
  P1SEL2|= BIT6 + BIT7;     // Assign I2C pins to USCI_B0

  Setup();
  State = SLEEP;

  while(1){
    while(!NewPacket){
      //TODO: add sleep mode
    }
    
    __disable_interrupt(); // don't want the interupt to destoy you beautiful packet
    NewPacket = false; // recet new packet flag
    uint8_t bytesReceved = ReadBuffer_p - &ReadBuffer[0];
    uint8_t data[READBUFFER_LEN];
    
    // copy data from the buffer used for the communication so it can receve the next packet
    memcpy(&data[0], &ReadBuffer[0], bytesReceved);

    __enable_interrupt(); // data is save

    switch (data[0]){
      case /* packet id */:
        /* read packet */
        break;
    }
  }
}

/*
 * Deze interrupt is beide voor RXIFG en TXIFG.
 * Dus vlaggetjes bekijken om te zien wat er gebeurd
 */
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
  if(IFG2 & UCB0TXIFG && State == WRITE){
    // Read data from I2C
    UCB0TXBUF = *WriteBuffer_p;
    WriteBuffer_p++;
    if(WriteBuffer_p > &WriteBuffer[WRITEBUFFER_LEN]){
      State = SLEEP;
      NewPacket = true;
    }
  }else if(IFG2 & UCB0RXIFG && State == READ){
    // Write data to I2C
    *ReadBuffer_p = UCB0RXBUF;
    ReadBuffer_p++;
    if(ReadBuffer_p > &ReadBuffer[READBUFFER_LEN]){
      State = WRITE;
    }
  }
}

/*
 * Hier krijgen we de start/stop condities
 */
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
  if(UCB0STAT & UCSTTIFG){
    // start signaal ontvangen
    State = READ;
    // reset pointers
    WriteBuffer_p = &WriteBuffer[0];
    ReadBuffer_p = &ReadBuffer[0];
  }else if(UCB0STAT & UCSTPIFG){
    // stop signaal, klaar met transactie
    State = SLEEP;
    NewPacket = true;
  }

  // Clear interrupt flags
  UCB0STAT &= ~(UCSTPIFG + UCSTTIFG);
}

void Setup(void){
  __disable_interrupt();
  UCB0CTL1 |= UCSWRST;            // reset
  UCB0CTL0 = UCMODE_3 + UCSYNC;   // I2C Slave, synchronous mode
  UCB0I2COA = 0x48;               // eigen slave adres is  048h
  UCB0CTL1 &= ~UCSWRST;           // reset uit
  UCB0I2CIE |= UCSTPIE + UCSTTIE; // detectie voor start en stop
  IE2 |= UCB0RXIE | UCB0TXIE;     // interrupts aan
  __enable_interrupt();
}
