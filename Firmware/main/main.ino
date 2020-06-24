/*
  Copyright (c) 2019 Andy England
  The Infinite Pyramid
*/

#include "BLE.h"
#include <FastLED.h>

/**Bluetooth**/
BLE BT;

#define DATA_PIN 14

#define NUM_PATTERNS 10
#define NUM_GRADIENTS 10

#define NUM_SIDES 6
#define NUM_PER_SIDE 39 //Number of LEDs per side
#define NUM_LEDS NUM_PER_SIDE * NUM_SIDES

double lastTimeStamp = 0;

volatile uint8_t colorIndex = 0;//used to pick position in the gradient
uint8_t pattern = 0;//used to pick which pattern we use
uint8_t gradient  = 0;//used to pick which gradient we use
uint8_t frameDelay = 0;//used to slow down an animation
CRGB leds[NUM_LEDS];//declare our LED object

double old_time = 0;

void setup(void) {
  Serial.begin(115200);
  Serial.println("Serial Begin");
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
  //initializeBattery();
  BT.begin("The Infinite Pyramid");
  old_time = millis();
}

bool state = 0;

void loop(void) {
  //add a new case for each different pattern
  switch (pattern)
  {
    case 0:
      centerAudioBuffer();
      //sparkleSolid();
      break;
    case 1:
      simpleAudio();
      //slowerSparkle();
      break;
    case 2:
      hueSparkle();
      break;
    case 3:
      hueSlowerSparkle();
      break;
    case 4:
      sparkle();
      break;
    case 5:
      solid();
      break;
    case 6:
      solidSlow();
      break;
    case 7:
      trail();
      break;
    case 8:
      solidTrail();
      break;
    case 9:
      slowTrail();
      break;
    case 10:
      centerOut();
      break;
    case 11:
      audioBuffer();
      break;
    default:
      pattern = 0;
      gradient++;
  }
}
