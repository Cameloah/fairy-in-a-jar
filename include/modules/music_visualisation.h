#include <FastLED.h>
#include "led_config.h"
#include "Arduino.h"

//pin microphone input is connected to
#define ANALOG_READ                                     1

#define BASE_LED_NUMBER_MIN                             5





/// /brief initializer for music visualisation
///
void music_vis_init();

/// /brief updater for music visualisation
///
void music_vis_update(CRGBSet& leds);