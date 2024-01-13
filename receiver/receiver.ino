#define FASTLED_ALLOW_INTERRUPTS 0

#include <Arduino.h>
#include <FastLED.h>
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"

#define LED_TYPE WS2812
#define LED_COUNT 35
#define LED_PIN 5
#define BRIGHTNESS 16

const uint8_t BUFSIZE = 4;
const uint8_t LED_STATUS_DIODE = 17;

#define c1 0xCC00CC
#define c2 0xFF5500
#define c3 0xAA5588
#define c4 0xCC0000
#define c5 0x1111FF


const uint32_t clrs[] = { c1, c2, c3, c4, c5 };

static void setup_leds(void);
static void setup_rf(void);
static void leds_off(void);
static void handle_incoming_data();

CRGB leds[LED_COUNT];

uint8_t buf[BUFSIZE];

void setup() {
    setup_rf();
    setup_leds();
}

void loop() {
    if (Mirf.dataReady()) {
        Mirf.getData(buf);
        leds_off();
        handle_incoming_data();
    }
}


void handle_incoming_data() {
    static const uint8_t LED_RIGHT_BEGIN = 0;
    static const uint8_t LED_RIGHT_END = 16;
    static const uint8_t LED_LEFT_END = 35;
    static const uint8_t LED_LEFT_BEGIN = 18;

    for (uint8_t i = 0; i < (BUFSIZE - 1); i++) {
        uint8_t minutes = buf[i];

        if (minutes > 0) {
            for (int8_t j = LED_RIGHT_END; j > (LED_RIGHT_END - minutes); j--) {
                leds[j].setColorCode(clrs[i]);
                FastLED.delay(16);
            }

            FastLED.delay(16);
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

void leds_off(void) {
    for (uint8_t i = LED_COUNT - 1; i > 0; i--) {
        if (i != LED_STATUS_DIODE) {
            leds[i] = 0;
            FastLED.delay(6);
        }
    }
}

void setup_leds(void) {
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.setDither(1);
    FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);
    FastLED.clearData();
    leds[LED_STATUS_DIODE].setColorCode(0x1111FF);
    FastLED.show();
}
