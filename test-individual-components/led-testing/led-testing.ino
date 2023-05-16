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

  // generate the led_idx
  for (int i = 0; i < 50; ++i) {
    led_idx[i] = 2 + i;
  }
}
// long WHITE_SQUARES = 0xFFFFFF;
// long BLACK_SQUARES = 0x191970;
long WHITE_SQUARES = 0xDC4400;
long BLACK_SQUARES = WHITE_SQUARES;


void loop() {
  for (int i = 4; i < NUM_LEDS; i += 6) {
    leds[i] = CRGB(WHITE_SQUARES);
    leds[i - 1] = CRGB(WHITE_SQUARES);
    leds[i - 2] = CRGB(WHITE_SQUARES);
  }
  for (int i = 7; i < NUM_LEDS; i += 6) {
    leds[i] = CRGB(BLACK_SQUARES);
    leds[i - 1] = CRGB(BLACK_SQUARES);
    leds[i - 2] = CRGB(BLACK_SQUARES);
  }
  FastLED.show();
  delay(500);
}