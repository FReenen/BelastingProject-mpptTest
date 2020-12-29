#ifndef COMMUNICATIE_H
#define COMMUNICATIE_H

#include <mqueue.h>

// enummeration for packet IDs
typedef enum {
  PACKET_UPDATEBELASTING,
  PACKET_NOODSTOP,
  PACKET_INITPARAMS
} packetID_t;

// packet structures
typedef struct {
  packetID_t id;
  bool overheeat;
  bool overload;
  bool overspeed;
} initAandrijving;

typedef struct {
  packetID_t id;
  uint8_t maxVermogen;
  uint16_t maxSnelhied;
  uint8_t maxTemptratuur;
  uint8_t vermogenSetpoint;
} initBelasting;

typedef struct {
  packetID_t id;
  uint8_t maxVermogen;
  uint16_t maxSnelhied;
  uint8_t maxTemptratuur;
} maxWaardesBelasting;

typedef struct {
  packetID_t id;
  uint8_t rendement;
  uint16_t snelheid;
  uint16_t stroom;
  uint16_t spanning;
} dataBelasting;

typedef struct {
  packetID_t id;
  uint16_t snelheidSetpoint;
  uint16_t maxSpanning;
  uint16_t maxStroom;
} initAandrijving;

typedef struct {
  packetID_t id;
  uint8_t rendement;
  uint8_t vermogen;
  uint16_t stroom;
  uint16_t spanning;
} dataAandrijving;

union {
  initAandrijving;
  initBelasting;
  maxWaardesBelasting;
  dataBelasting;
  initAandrijving;
  dataAandrijving;
} I2CPacket_t;

#endif