#ifndef COMMUNICATIE_H
#define COMMUNICATIE_H

//// enummeration for packet IDs
//typedef enum {
//  PACKET_UPDATEBELASTING,
//  PACKET_NOODSTOP,
//  PACKET_INITPARAMS
//} packetID_t;
//
//// packet structures
//typedef struct packet_0 {
//  packetID_t id;
//  bool overheeat;
//  bool overload;
//  bool overspeed;
//} noodstop;
//
//typedef struct packet_1 {
//  packetID_t id;
//  uint8_t maxVermogen;
//  uint16_t maxSnelhied;
//  uint8_t maxTemptratuur;
//  uint8_t vermogenSetpoint;
//} initBelasting;
//
//typedef struct packet_2 {
//  packetID_t id;
//  uint8_t maxVermogen;
//  uint16_t maxSnelhied;
//  uint8_t maxTemptratuur;
//} maxWaardesBelasting;
//
//typedef struct packet_3 {
//  packetID_t id;
//  uint8_t rendement;
//  uint16_t snelheid;
//  uint16_t stroom;
//  uint16_t spanning;
//} dataBelasting;
//
//typedef struct packet_4 {
//  packetID_t id;
//  uint16_t snelheidSetpoint;
//  uint16_t maxSpanning;
//  uint16_t maxStroom;
//} initAandrijving;
//
//typedef struct packet_5 {
//  packetID_t id;
//  uint8_t rendement;
//  uint8_t vermogen;
//  uint16_t stroom;
//  uint16_t spanning;
//} dataAandrijving;
//
//typedef union I2CPacket_t {
//  initAandrijving initA;
//  initBelasting initB;
//  maxWaardesBelasting maxB;
//  dataAandrijving dataA;
//  dataBelasting. dataB;
//} I2CPacket_t;

void comm_init();

#endif
