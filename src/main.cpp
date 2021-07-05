#include <Arduino.h>
#include <FastLED.h>
#include "modules/music_visualisation.h"
#include "modules/twinkle.h"
#include "modules/ocean.h"
#include "modules/fire.h"
#include "led_config.h"

#ifdef ESP32
    #include "wifilight.h"  // the wifi light feature requires a ESP32 development board
#endif

//_______loop timer______________________________________________________________________________
  int timer_size = 5000;
  int loop_timer = 0;
  float loop_time = 0;
  double timer_start_debug = 0;


//create the FastLED array containing led colors
CRGBArray<LED_NUM> led_arr;

#define PIN_SWITCH                            4

#define EFFECT_DURATION_SEC                   300
#define EFFECT_BLEND_IN_SEC                   20

uint8_t module_current_index;
CRGBArray<LED_NUM> led_arr_current;
CRGBArray<LED_NUM> led_arr_next;

#define EFFECT_MODULE_NUM                     3

void (*module_update[EFFECT_MODULE_NUM])(CRGBSet&) = {
        twinkle_update,
        ocean_update,
        fire_update
};


void setup() {
    //initialize serial communication
    Serial.begin(9600);

    //peripherals
    pinMode(PIN_SWITCH, PULLDOWN);

    //start up FastLED class
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(led_arr, LED_NUM).setCorrection(TypicalLEDStrip);
    //clear all leds
    led_arr = CRGB::Black;

#ifdef ESP32
    // init wifi module
    wifilight_init(led_arr);
#endif

    //initialize music visualisation
    music_vis_init();
    twinkle_init();

    //execute led colors
    FastLED.show();
}

uint8_t blend_opacity = 255;

void loop() {

#ifdef ESP32
    // run wifi update routine
    wifilight_update();

    if(lights[0].lightState == false) {
        //clear all leds
        led_arr = CRGB::Black;
        FastLED.show();
        return;
    }
#endif

    switch(digitalRead(PIN_SWITCH)) {
        case HIGH:
            music_vis_update(led_arr);
            break;

        case LOW:
            EVERY_N_MILLISECONDS(20) {
                EVERY_N_SECONDS(EFFECT_DURATION_SEC) {
                    module_current_index = addmod8( module_current_index, 1, EFFECT_MODULE_NUM);
                    blend_opacity = 0;
                }

                module_update[module_current_index] (led_arr_current);
                module_update[addmod8(module_current_index, 1, EFFECT_MODULE_NUM)] (led_arr_next);
                blend(led_arr_current, led_arr_next, led_arr, LED_NUM, blend_opacity);
            }
            if(blend_opacity <= 254) {
                EVERY_N_MILLISECONDS(EFFECT_BLEND_IN_SEC * 1000 / 255) {
                    blend_opacity++;
                }
            }
            break;

        default:
            return;
    }

    //execute led colors
    FastLED.show();

    //_____loop timer______________________________________________________________________________

    loop_timer++;

    if(loop_timer == timer_size){
        loop_time = millis() - timer_start_debug;
        loop_time = loop_time / timer_size;
        Serial.println(loop_time);
        loop_timer = 0;
        loop_time = 0;
        timer_start_debug = millis();
    }

}