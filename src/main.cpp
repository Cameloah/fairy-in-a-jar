#include <Arduino.h>
#include <FastLED.h>
#include "modules/music_visualisation.h"
#include "modules/twinkle.h"
#include "modules/ocean.h"
#include "led_config.h"


//_______loop timer______________________________________________________________________________
  int timer_size = 5000;
  int loop_timer = 0;
  float loop_time = 0;
  int timer_start = 0;

  
//create the FastLED array containing led colors
CRGBArray<LED_NUM> leds;
//CRGB leds[LED_NUM];

#define PIN_SWITCH                            3




void setup() {
  //initialize serial communication
  Serial.begin(250000);

  //periferals
  pinMode(PIN_SWITCH, INPUT);

  //initialize music visualisation
  music_vis_init();
  //initialize twinkle effect
  twinkle_init();
  //initialize ocean effect
  ocean_init((CRGB**) &leds.leds);

  //start up FastLED object
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_NUM).setCorrection(TypicalLEDStrip);

  //clear all leds
    for (int i = 0; i < LED_NUM; i++)
    {
      leds[i] = CRGB(0, 0, 0);
    }

  //execute led colors
  FastLED.show(); 
}



void loop() {

  switch(digitalRead(PIN_SWITCH)) {
    case HIGH:
      music_vis_update(leds);
      break;

    case LOW:
      twinkle_update(leds);
      break;

    default:
      return;
  }
  

  //execute led colors
  FastLED.show();
    
  //_____loop timer______________________________________________________________________________

    /*loop_timer++;

    if(loop_timer == timer_size){
      loop_time = millis() - timer_start;
      loop_time = loop_time / timer_size;
      Serial.println(loop_time);
      loop_timer = 0;
      loop_time = 0;
      timer_start = millis();
    }*/
  
}