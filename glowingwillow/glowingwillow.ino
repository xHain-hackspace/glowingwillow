#include <FastLED.h>

// v3
// strip config
#define TRUNK_PIN_1 26
#define TRUNK_PIN_2 25
#define BRANCH_PIN_1 12 // Branch 6
#define BRANCH_PIN_2 27 // Branch 
#define BRANCH_PIN_3 4  // Branch 3
#define BRANCH_PIN_4 13 // Branch 8
#define BRANCH_PIN_5 14 // Branch 7
#define BRANCH_PIN_6 15 // Branch 2

// Todo richtigen PIN Anschluss finden

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
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_1>(branch_leds[0], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_2>(branch_leds[1], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_3>(branch_leds[2], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_4>(branch_leds[3], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_5>(branch_leds[4], BRANCH_PIXEL_COUNT);

  // Todo richtigen PIN Anschluss finden
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_6>(branch_leds[5], BRANCH_PIXEL_COUNT);
}

void loop() {
  trunk_rainbow_wipe();
  branch_rainbow_wipe();
  trunk_single_color(BLACK);
  branch_single_color(BLACK);
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

void set_branch_led(uint8_t branch, uint8_t led, CRGB color) {
    branch_leds[branch][led] = color;
}

// void set_leaf_led(uint8_t leaf, uint8_t led, CRGB color)

// -- Effects ------------------------------------------------

//
// TRUNK
//
void trunk_color_wipe(CRGB color) {
  for(uint8_t i=0; i<TRUNK_PIXEL_COUNT/2; i++) {
    for(uint8_t j=0; j<4; j++) {
      set_trunk_led(j, i, color);
    }
    FastLED.show();
    FastLED.delay(DELAY);
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
  for(uint8_t i=0; i<TRUNK_PIXEL_COUNT/2; i++) {
    for (uint8_t j=0; j<4; j++) {
      set_trunk_led(j, i, CHSV(i*5, SATURATION, VALUE));
    }
    FastLED.show();
    FastLED.delay(DELAY);
  }
}

//
// BRANCH
//
void branch_color_wipe(CRGB color) {
  for(uint8_t i=0; i<BRANCH_PIXEL_COUNT; i++) {
    for(uint8_t j=0; j<BRANCH_STRIP_COUNT; j++) {
      set_trunk_led(j, i, color);
    }
    FastLED.show();
    FastLED.delay(DELAY);
  }
}

void branch_single_color(CRGB color) {
  for(uint8_t i=0; i<BRANCH_STRIP_COUNT; i++) {
    for (uint8_t j=0; j<BRANCH_PIXEL_COUNT; j++) {
      branch_leds[i][j] = color;
    }
  }
  FastLED.show();
}

void branch_rainbow_wipe() {
  for(uint8_t i=0; i<BRANCH_PIXEL_COUNT; i++) {
    for (uint8_t j=0; j<BRANCH_STRIP_COUNT; j++) {
      set_branch_led(j, i, CHSV(i*5, SATURATION, VALUE));
    }
    FastLED.show();
    FastLED.delay(DELAY);
  }
}

