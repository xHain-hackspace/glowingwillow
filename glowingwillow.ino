#include <FastLED.h>

// strip config
#define TRUNK_PIN_1 12
#define TRUNK_PIN_2 27
#define BRANCH_PIN_1 25
#define BRANCH_PIN_2 26
#define BRANCH_PIN_3 4
#define BRANCH_PIN_4 21
#define BRANCH_PIN_5 13
#define BRANCH_PIN_6 27

#define TRUNK_STRIP_COUNT 2
#define TRUNK_PIXEL_COUNT 100

#define BRANCH_STRIP_COUNT 6
#define BRANCH_PIXEL_COUNT 90

// Effect settings
#define SATURATION 255
#define VALUE 255
#define DELAY 10

// colors
#define RED CHSV(0, SATURATION, VALUE)
#define YELLOW CHSV(42, SATURATION, VALUE)
#define GREEN CHSV(85, SATURATION, VALUE)
#define BLUE CHSV(171, SATURATION, VALUE)
#define PINK CHSV(213, SATURATION, VALUE)
#define BLACK CHSV(0, 0, 0)

CRGB trunk_leds[TRUNK_STRIP_COUNT][TRUNK_PIXEL_COUNT];
CRGB branch_leds[BRANCH_STRIP_COUNT][BRANCH_PIXEL_COUNT];

void setup() {
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_1>(trunk_leds[0], TRUNK_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_2>(trunk_leds[1], TRUNK_PIXEL_COUNT);

  // FastLED.addLeds<NEOPIXEL, BRANCH_PIN_1>(branch_leds[0], TRUNK_PIXEL_COUNT);
  // FastLED.addLeds<NEOPIXEL, BRANCH_PIN_2>(branch_leds[1], TRUNK_PIXEL_COUNT);
  // FastLED.addLeds<NEOPIXEL, BRANCH_PIN_3>(branch_leds[2], TRUNK_PIXEL_COUNT);
  // FastLED.addLeds<NEOPIXEL, BRANCH_PIN_4>(branch_leds[3], TRUNK_PIXEL_COUNT);
  // FastLED.addLeds<NEOPIXEL, BRANCH_PIN_5>(branch_leds[4], TRUNK_PIXEL_COUNT);
  // FastLED.addLeds<NEOPIXEL, BRANCH_PIN_6>(branch_leds[5], TRUNK_PIXEL_COUNT);
}

void loop() {
  trunk_rainbow_wipe();
  delay(250);
  trunk_single_color(BLACK);
  // trunk_single_color(GREEN);
  // delay(2500);
  // trunk_single_color(BLUE);
  // delay(2500);
}

// -- Transform functions ------------------------------------------------
// Used to select individual leds on the tree, translates the hardware wiring
// to the actual positions on the tree

void set_trunk_led(uint8_t trunk, uint8_t led, CRGB color) {
  if(trunk < 2) {
    trunk_leds[trunk][led] = color;
  } else {
    trunk_leds[trunk-2][TRUNK_PIXEL_COUNT-led-1] = color;
  }
}

// void set_branch_led(uint8_t branch, uint8_t led, CRGB color)
// void set_leaf_led(uint8_t leaf, uint8_t led, CRGB color)

// -- Effects ------------------------------------------------

void trunk_color_wipe(CRGB color) {
  for(uint8_t i=0; i<50; i++) {
    for(uint8_t j=0; j<4; j++) {
      set_trunk_led(j, i, color);
    }
    FastLED.show();
    delay(DELAY);
  }
}

void trunk_single_color(CRGB color) {
  for(uint8_t i=0; i<TRUNK_STRIP_COUNT; i++) {
    for (uint8_t j=0; j<TRUNK_PIXEL_COUNT; j++) {
      trunk_leds[i][j] = color;
    }
  }
  FastLED.show();
}

void trunk_rainbow_wipe() {
  for(uint8_t i=0; i<50; i++) {
    for (uint8_t j=0; j<4; j++) {
      set_trunk_led(j, i, CHSV(i*5, SATURATION, VALUE));
    }
    FastLED.show();
    delay(DELAY);
  }
}

