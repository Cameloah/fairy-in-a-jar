#include <Arduino.h>
#include <FastLED.h>
#include "modules/music_visualisation.h"
#include "led_config.h"


//_______loop timer______________________________________________________________________________
  int timer_size = 5000;
  int loop_timer = 0;
  float loop_time = 0;
  int timer_start = 0;

  
//create the FastLED array containing led colors
CRGBArray<LED_NUM> leds;



void setup() {
  //initialize serial communication
  Serial.begin(250000);

  //initialize music visualisation
  music_vis_init();
  
  //start up FastLED object
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, LED_NUM);

  //clear all leds
    for (int i = 0; i < LED_NUM; i++)
    {
      leds[i] = CRGB(0, 0, 0);
    }

  //execute led colors
  FastLED.show(); 
}



void loop() {

  music_vis_update(leds);

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