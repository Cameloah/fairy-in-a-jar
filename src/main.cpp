#include <Arduino.h>
#include <FastLED.h>
#include "modules/music_visualisation.h"
#include "modules/twinkle.h"
#include "modules/ocean.h"
#include "tools/blend.h"
#include "led_config.h"


//_______loop timer______________________________________________________________________________
  int timer_size = 5000;
  int loop_timer = 0;
  float loop_time = 0;
  int timer_start = 0;

  
//create the FastLED array containing led colors
CRGBArray<LED_NUM> led_arr;
//CRGB leds[LED_NUM];

#define PIN_SWITCH                            3

#define EFFECT_DURATION_SEC                   10
#define EFFECT_BLEND_IN_SEC                   5

uint8_t module_current_index;
#define EFFECT_MODULE_NUM                     2
void (*module_update[EFFECT_MODULE_NUM])(CRGBSet&) = {
        ocean_update,
        twinkle_update
};


void setup() {
  //initialize serial communication
  Serial.begin(250000);

  //periferals
  pinMode(PIN_SWITCH, INPUT);

  //initialize music visualisation
  music_vis_init();
  //initialize effect modules
  twinkle_init();


  //start up FastLED object
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(led_arr, LED_NUM).setCorrection(TypicalLEDStrip);

  //clear all leds
    for (int i = 0; i < LED_NUM; i++)
    {
      led_arr[i] = CRGB(0, 0, 0);
    }

  //execute led colors
  FastLED.show(); 
}



void loop() {

  switch(digitalRead(PIN_SWITCH)) {
    case HIGH:
      //music_vis_update(led_arr);
      ocean_update(led_arr);
      break;

    case LOW:
      //twinkle_update(led_arr);

      EVERY_N_SECONDS(EFFECT_DURATION_SEC) {
        module_current_index = addmod8( module_current_index, 1, EFFECT_MODULE_NUM);
      }
      
      module_update[module_current_index] (led_arr);
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