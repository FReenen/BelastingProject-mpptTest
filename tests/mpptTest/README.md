# MPPT test

De MPPT test wordt grootendeels handmatig uitgevoerdt. Er wordt een arduino gebruikt om de initalisatie te starten.

## setup

### CC3220S

In debugmode met de orginaile code.

#### breakboints

* mppt.c: mppt_start(): last usleep

### Arduino

De arduino wordt gebuikt voor het simuleeren van de volgende signaalen:

* start initialisatie
* start
* SPI (direct naar de CC3220S ipv met I2C via de MSP)

### dingen die gemeten worden

* vermogen uit de motor
* PWM signaal
