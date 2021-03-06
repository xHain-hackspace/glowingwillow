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

#define TRUNK_STRIP_COUNT 2
#define TRUNK_PIXEL_COUNT 100

#define BRANCH_STRIP_COUNT 6
#define BRANCH_PIXEL_COUNT 90

// Rainbow Effect settings
#define SATURATION 255
#define VALUE 255
#define DELAY 10

// sparke
#define SPARKLE_COUNT 20

// colors
#define RED CHSV(0, SATURATION, VALUE)
#define YELLOW CHSV(42, SATURATION, VALUE)
#define GREEN CHSV(85, SATURATION, VALUE)
#define BLUE CHSV(171, SATURATION, VALUE)
#define PINK CHSV(213, SATURATION, VALUE)
#define BLACK CHSV(0, 0, 0)
#define WHITE CRGB(255, 255, 255)

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
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_6>(branch_leds[5], BRANCH_PIXEL_COUNT);
}

void loop() {
  //trunk_rainbow_wipe();
  //branch_rainbow_wipe();
  //branch_helicopter_single_color(GREEN,100);
  //branch_helicopter_rainbow(40,1,10);//params: nr of rotations total, delay per branch step, color increase per branch step
  //tree_rainbow();
  //tree_fade_color(random(255), 255, 0);
  //branchh_helicopter_single_color(171, 150);
  //tree_wave(180, 30, 130, 8, 25);
  
  tree_rgb_sines(25,10.1,11.2,12.2,0.035); //params: wavelengths, phase shift speed r/g/b, "Breathe" frequency
  branch_sparkle(5, 250, 500);
  FastLED.show();

}

// -- Transform functions ------------------------------------------------
// Used to select individual leds on the tree, translates the hardware wiring
// to the actual positions on the tree

void set_trunk_led(int trunk, int led, CRGB color) {
  if(trunk < 2) {
    trunk_leds[trunk][led] = color;
  } else {
    trunk_leds[trunk-2][TRUNK_PIXEL_COUNT-led-1] = color;
  }
}

void set_branch_led(int branch, int led, CRGB color) {
  if(branch < 0) {
    set_branch_led(branch + BRANCH_STRIP_COUNT, led, color);
  } else if(branch < BRANCH_STRIP_COUNT) {
    branch_leds[branch][led] = color;
  } else {
    set_branch_led(branch - BRANCH_STRIP_COUNT, led, color);
  }
}

// -- sine calculation ------------------------------------------------

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

// -- TRUNK ------------------------------------------------

void trunk_color_wipe(CRGB color) {
  for(int i=0; i<TRUNK_PIXEL_COUNT/2; i++) {
    for(int j=0; j<4; j++) {
      set_trunk_led(j, i, color);
    }
    FastLED.show();
    FastLED.delay(DELAY);
  }
}

void trunk_single_color(CRGB color) {
  for(int i=0; i<TRUNK_STRIP_COUNT; i++) {
    for (int j=0; j<TRUNK_PIXEL_COUNT; j++) {
      trunk_leds[i][j] = color;
    }
  }
}

void trunk_rainbow_wipe() {
  for(int i=0; i<TRUNK_PIXEL_COUNT/2; i++) {
    for (int j=0; j<4; j++) {
      set_trunk_led(j, i, CHSV(i*5, SATURATION, VALUE));
    }
    FastLED.show();
    FastLED.delay(DELAY);
  }
}

// -- BRANCH ------------------------------------------------

void branch_color_wipe(CRGB color) {
  for(int i=0; i<BRANCH_PIXEL_COUNT; i++) {
    for(int j=0; j<BRANCH_STRIP_COUNT; j++) {
      set_trunk_led(j, i, color);
    }
    FastLED.show();
    FastLED.delay(DELAY);
  }
}

void branch_single_color(CRGB color) {
  for(int i=0; i<BRANCH_STRIP_COUNT; i++) {
    for (int j=0; j<BRANCH_PIXEL_COUNT; j++) {
      branch_leds[i][j] = color;
    }
  }
}

//light one branch at a time, options; color, delay per branch
void branch_helicopter_single_color(int hue, int delay) {
  for(int i=0; i<BRANCH_STRIP_COUNT; i++) {
    // branch_single_color(BLACK);
    for (int j=0; j<BRANCH_PIXEL_COUNT; j++) {
      set_branch_led(i, j, CHSV(hue, SATURATION, 255));
      set_branch_led(i-1, j, CHSV(hue, SATURATION, 150));
      set_branch_led(i-2, j, CHSV(hue, SATURATION, 50));
      set_branch_led(i-3, j, BLACK);
    }
    FastLED.show();
    FastLED.delay(delay);
  }
}

//light one branch in helicopter fashion, changing color for every branch, params: nr of rotations total, delay per branch step, color increase per branch step
// void branch_helicopter_rainbow(int rotations, int branch_delay, int color_speed){
//   for (int current_branch=0;current_branch<rotations;current_branch++){
//     branch_helicopter_single_color(CHSV((rotations*BRANCH_STRIP_COUNT+current_branch)*color_speed, SATURATION, VALUE),branch_delay);
//   }
// }

void branch_rainbow_wipe() {
  for(int i=0; i<BRANCH_PIXEL_COUNT; i++) {
    for (int j=0; j<BRANCH_STRIP_COUNT; j++) {
      set_branch_led(j, i, CHSV(i*5, SATURATION, VALUE));
    }
    FastLED.show();
    FastLED.delay(DELAY);
  }
}

// sparkle
struct Sparkle {
  int start_time;
  int led;
  int branch;
  int value;
};

Sparkle sparkles[SPARKLE_COUNT];

void branch_sparkle(int min_value, int max_value, int fade_speed) {

  int now = millis();

  for (int i=0; i<SPARKLE_COUNT; i++) {
    if (sparkles[i].value <= min_value) {
      branch_leds[sparkles[i].branch][sparkles[i].led] = BLACK;

      // new sparkle
      sparkles[i].led = random(BRANCH_PIXEL_COUNT);
      sparkles[i].branch = random(BRANCH_STRIP_COUNT);
      sparkles[i].value = random(max_value);
      sparkles[i].start_time = now;
    } else {
      sparkles[i].value = sparkles[i].value - (now - sparkles[i].start_time) / fade_speed;
    }

    branch_leds[sparkles[i].branch][sparkles[i].led] = CHSV(0, 0, sparkles[i].value);
  }
}

// -- Whole tree ------------------------------------------------

void tree_random(){
    for(int i=0;i<TRUNK_STRIP_COUNT;i++){
    for(int j=0;j<TRUNK_PIXEL_COUNT;j++){
      trunk_leds[i][j]=CHSV(random(255),255,random(2)*255);
    }
  }

 for(int m=0;m<BRANCH_STRIP_COUNT;m++){
  for(int n=0;n<BRANCH_PIXEL_COUNT;n++){
      branch_leds[m][n]=CHSV(random(255),255,random(2)*255);
    }
  }
  FastLED.show();
}

void tree_rainbow() {
  trunk_rainbow_wipe();
  branch_rainbow_wipe();
  FastLED.delay(5000);
  trunk_single_color(BLACK);
  branch_single_color(BLACK);
}

void tree_off() {
  trunk_single_color(BLACK);
  branch_single_color(BLACK);
  FastLED.show();
}

void tree_fade_color(int hue, int saturation, int delay) {
  for(int i=50; i<200; i+=5) {
    trunk_single_color(CHSV(hue, saturation, i));
    branch_single_color(CHSV(hue, saturation, i));
    FastLED.show();
    FastLED.delay(delay);
  }

  for(int i=200; i>50; i-=5) {
    trunk_single_color(CHSV(hue, saturation, i));
    branch_single_color(CHSV(hue, saturation, i));
    FastLED.show();
    FastLED.delay(delay);
  }
}

void tree_wave(int hue, int min_value, int max_value, int phase_shift_speed, int wavelength) {
  int saturation = 255;
  int value = 0;

  for(int i=0; i<4; i++) {
    for(int j=0; j<TRUNK_PIXEL_COUNT/2; j++) {
      value = min_value + wave_propagation(j, 0, phase_shift_speed, wavelength) * (max_value-min_value);
      set_trunk_led(i, j, CHSV(hue, saturation, value));
    }
  }

  for (int i=0; i<BRANCH_STRIP_COUNT; i++) {
    for(int j=0; j<BRANCH_PIXEL_COUNT; j++) {
      value = min_value + wave_propagation(j, -50, phase_shift_speed, wavelength) * (max_value-min_value);
      branch_leds[i][j] = CHSV(hue, saturation, value);
    }
  }
}

//whole tree function for rgb sine effects
//params:
//wavelength:              width of full sine wave along the strip [px]
//phase_shift_speed_r/g/b: how fast sine moves along the strip [px/s]
//freq_amp_mod:            amplitude/brightness modulation [1/s] ("breathe" frequency)
void tree_rgb_sines(float wavelength,float phase_shift_speed_r,float phase_shift_speed_g,float phase_shift_speed_b,float freq_amp_mod) {
  //set constant parameters
  float amplitude = 1; //0...1 -> max brightness
  float phase_offset = 0;//wavelength*3.0/4.0; //offset of sine at start [px]
  
  //float wavelength = 200;//width of full sine wave along the strip [px]
  //float freq_amp_mod = 0; //amplitude/brightness modulation [1/s] ("breathe" frequency)
  //float phase_shift_speed_r = 10;//how fast sine moves along the strip [px/s]
  //float phase_shift_speed_g = 11;//how fast sine moves along the strip [px/s]
  //float phase_shift_speed_b = 12;//how fast sine moves along the strip [px/s]

  //set trunk
  phase_offset = 0;
  for(uint8_t curr_pixel=0; curr_pixel<TRUNK_PIXEL_COUNT/2; curr_pixel++) {
    for (uint8_t j=0; j<4; j++) {//TODO: hardcoded value (number of "logical" strips in trunk)

      set_trunk_led(j, curr_pixel, CRGB(int(255.0*amplitude*amplitude_modulation(freq_amp_mod)*wave_propagation(curr_pixel, phase_offset, phase_shift_speed_r, wavelength)), int(255.0*amplitude*amplitude_modulation(freq_amp_mod)*wave_propagation(curr_pixel, phase_offset, phase_shift_speed_g, wavelength)), int(255.0*amplitude*amplitude_modulation(freq_amp_mod)*wave_propagation(curr_pixel, phase_offset, phase_shift_speed_b, wavelength))));
    }
  }
 //set branches
 phase_offset = -50+2;//offset correction for leds in holder =2, correction for trunk pixels = -50 TODO:hardcoded value: "logical" pixels in trunk
 for(uint8_t i=0; i<BRANCH_STRIP_COUNT; i++) {
  for (uint8_t j=0; j<BRANCH_PIXEL_COUNT; j++) {
    branch_leds[i][j] = CRGB(int(255.0*amplitude*amplitude_modulation(freq_amp_mod)*wave_propagation(j, phase_offset, phase_shift_speed_r, wavelength)), int(255.0*amplitude*amplitude_modulation(freq_amp_mod)*wave_propagation(j, phase_offset, phase_shift_speed_g, wavelength)), int(255.0*amplitude*amplitude_modulation(freq_amp_mod)*wave_propagation(j, phase_offset, phase_shift_speed_b, wavelength)));
  }
 }
}

