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
union {
  struct {
    packetID_t id = PACKET_NOODSTOP;
    bool overheeat;
    bool overload;
    bool overspeed;
  } initAandrijving;

  struct {
    packetID_t id = PACKET_INITPARAMS;
    uint8_t maxVermogen;
    uint16_t maxSnelhied;
    uint8_t maxTemptratuur;
    uint8_t vermogenSetpoint;
  } initBelasting;

  struct {
    packetID_t id = 0x03;
    uint8_t maxVermogen;
    uint16_t maxSnelhied;
    uint8_t maxTemptratuur;
  } maxWaardesBelasting;

  struct {
    packetID_t id = 0x04;
    uint8_t rendement;
    uint16_t snelheid;
    uint16_t stroom;
    uint16_t spanning;
  };

  struct {
    packetID_t id = 0x05;
    uint16_t snelheidSetpoint;
    uint16_t maxSpanning;
    uint16_t maxStroom;
  } initAandrijving;

  struct {
    packetID_t id = 0x06;
    uint8_t rendement;
    uint8_t vermogen;
    uint16_t stroom;
    uint16_t spanning;
  };
} I2CPacket_t;

typedef struct {
  uint8_t maxTemperatuur;
  uint8_t maxVermogen;
  uint16_t maxSnelheid;
} maxwaardes_t;

mqd_t MaxWaardes_mq;

extern void mppt_setpoint(uint8_t vermogen);

#endif