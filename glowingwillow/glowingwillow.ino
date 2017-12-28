#include <FastLED.h>

// v3
// strip config
#define TRUNK_PIN_1 26
#define TRUNK_PIN_2 25
#define BRANCH_PIN_1 15 // Branch pointing to assembly   (Branch 2)
#define BRANCH_PIN_2 27 // Branch pointing to led drum 1 (Branch ?)
#define BRANCH_PIN_3 4  // Branch pointing to led drum 2 (Branch 3)
#define BRANCH_PIN_4 13 // Branch pointing to laser wall (Branch 8)
#define BRANCH_PIN_5 14 // Branch pointing to tents 1    (Branch 7)
#define BRANCH_PIN_6 12 // Branch pointing to tents 2    (Branch 12)

// Todo richtigen PIN Anschluss finden

#define TRUNK_STRIP_COUNT 2
#define TRUNK_PIXEL_COUNT 100

#define BRANCH_STRIP_COUNT 6
#define BRANCH_PIXEL_COUNT 90

// Rainbow Effect settings
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
  //trunk_rainbow_wipe();
  //branch_rainbow_wipe();
  //branch_helicopter_single_color(GREEN,100);
  //branch_helicopter_rainbow(40,1,10);//params: nr of rotations total, delay per branch step, color increase per branch step
  //FastLED.delay(5000);
  //trunk_single_color(BLACK);
  //branch_single_color(BLACK);
  test_sine();
  
  
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

////helper functions for sine calculation////

//returns amplitude modulation term (0...1) based on system time and frequency parameter [1/s]
float amplitude_modulation(float freq_amp_mod){
  if (freq_amp_mod == 0) return 1;
  else return (0.5*(1.0+sinf(freq_amp_mod*2.0*3.1415*float(millis())/1000.0)));
}

//returns wave propgation term (0...1) based on system time and pixel position
//parameter: pixel position, phase offset at pos 0 [px], phase shift with time [px/s], wavelength (size of one wave) [px]
float wave_propagation(float pixel_pos,float phase_offset, float phase_shift_speed, float wavelength){
  return 0.5*(1.0+sinf(2.0*3.1415*(pixel_pos-phase_offset-phase_shift_speed*float(millis())/1000.0)/wavelength));
}

//test function for sine effects
void test_sine() {
  //set parameters
  float amplitude = 1; //0...1 -> max brightness
  float freq_amp_mod = 0.1; //amplitude/brightness modulation [1/s]
  float wavelength = 12.5;//width of full sine wave along the strip [px]
  float phase_offset = wavelength/4.0;//wavelength*3.0/4.0; //offset of sine at start [px]
  float phase_shift_speed = 10;//how fast sine moves along the strip [px/s]
 
  //set trunk
  phase_offset = wavelength/4.0;
  for(uint8_t curr_pixel=0; curr_pixel<TRUNK_PIXEL_COUNT/2; curr_pixel++) {
    for (uint8_t j=0; j<4; j++) {

      set_trunk_led(j, curr_pixel, CHSV(8, 255, 0+int(255.0*amplitude*amplitude_modulation(freq_amp_mod)*wave_propagation(curr_pixel, phase_offset, phase_shift_speed, wavelength))));
    }          
  }
 //set branches
 phase_offset = wavelength/4.0+2;//offset correction for leds in holder
 for(uint8_t i=0; i<BRANCH_STRIP_COUNT; i++) {
  for (uint8_t j=0; j<BRANCH_PIXEL_COUNT; j++) {
    branch_leds[i][j] = CHSV(8, 255, 0+int(255.0*amplitude*amplitude_modulation(freq_amp_mod)*wave_propagation(j, phase_offset, phase_shift_speed, wavelength)));
  }
 }
 //show
 FastLED.show(); 
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

//light one branch at a time, options; color, delay per branch
void branch_helicopter_single_color(CRGB color, int heli_delay) {
  for(uint8_t i=0; i<BRANCH_STRIP_COUNT; i++) {
    branch_single_color(BLACK);
    for (uint8_t j=0; j<BRANCH_PIXEL_COUNT; j++) {
      branch_leds[i][j] = color;
    }
    FastLED.show();
    FastLED.delay(heli_delay);
  }  
}

//light one branch in helicopter fashion, changing color for every branch, params: nr of rotations total, delay per branch step, color increase per branch step
void branch_helicopter_rainbow(int rotations, int branch_delay, int color_speed){
  for (int current_branch=0;current_branch<rotations;current_branch++){
    branch_helicopter_single_color(CHSV((rotations*BRANCH_STRIP_COUNT+current_branch)*color_speed, SATURATION, VALUE),branch_delay);
  }
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

