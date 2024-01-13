#include <Arduino.h>
#include <SPI.h>
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"

const uint8_t BUFSIZE = 4;
uint8_t buf[BUFSIZE];

void setup() {


  setup_rf();
  // uint8_t b[4] = { 0x05, 0x03, 0x03, 0x06 };
  // Mirf.send(b);
  // while (Mirf.isSending()) {};
  // Serial.begin(9600);
  Serial.begin(115200);
}


void loop() {
  if (Serial.available() >= 1) {
    delay(500);

    for (uint8_t i = 0; i < BUFSIZE - 1; i++) {
      int data = Serial.read();

      if (data > 0) {
        buf[i] = (uint8_t)data;
      } else {
        buf[i] = (uint8_t)0;
      }
    }

    Mirf.send(buf);
    while (Mirf.isSending()) {}

    while (Serial.available()) {
      Serial.read();
    }
  }
}

void setup_rf() {
  Mirf.cePin = 10;
  Mirf.csnPin = 9;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  Mirf.setRADDR((byte *)"AOIEA");
  Mirf.setTADDR((byte *)"FMTED");
  Mirf.payload = BUFSIZE;
  Mirf.channel = 90;
  Mirf.config();
}
