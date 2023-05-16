#include <FastLED.h>

#define LED_PIN 2
#define NUM_LEDS 50

CRGB leds[NUM_LEDS];

int led_idx[16 * 3];

// int colors[] = { 0x191970, 0xFFFFFF, 0x191970, 0xFFFFFF, 0x191970, 0xFFFFFF };

void setup() {
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(255);
    pinMode(LED_PIN, OUTPUT);

    for (int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = 0xDC4400;
    }
    FastLED.show();
}
// long WHITE_SQUARES = 0xFFFFFF;
// long BLACK_SQUARES = 0x191970;
long WHITE_SQUARES = 0xDC4400;
long BLACK_SQUARES = WHITE_SQUARES;

int bright = 0;

int dx = 1;

void loop() {
    if (bright == 255) {
        dx = -1;
    } else if (bright == 0) {
        dx = 1;
    }
    bright += dx;
    FastLED.setBrightness(bright);
    FastLED.show();
    delay(6);
}