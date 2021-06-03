#include "modules/fire.h"
#include "led_config.h"

bool gReverseDirection = false;

  // This first palette is the basic 'black body radiation' colors,
  // which run from black to red to bright yellow to white.
CRGBPalette16 gPal = HeatColors_p;
  
  // These are other ways to set up the color palette for the 'fire'.
  // First, a gradient from black to red to yellow to white -- similar to HeatColors_p
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
  
  // Second, this palette is like the heat colors, but blue/aqua instead of red/yellow
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);
  
  // Third, here's a simpler, three-step gradient, from black to red to white
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);

void fire_update(CRGBSet& leds)
{
  FastLED.delay(1000 / FRAMES_PER_SECOND);
  random16_add_entropy( random16());
    // Fourth, the most sophisticated: this one sets up a new palette every
  // time through the loop, based on a hue that changes every time.
  // The palette is a gradient from black, to a dark color based on the hue,
  // to a light color based on the hue, to white.
  //
     
     static uint8_t hue = 0;
     EVERY_N_MILLIS(1000) {
       hue++;
     }

     CRGB darkcolor  = CHSV(hue,255,192); // pure hue, three-quarters brightness
     CRGB lightcolor = CHSV(hue,128,255); // half 'whitened', full brightness
     gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);


// Array of temperature readings at each simulation cell
  static byte heat[ARRAY_LENGTH_HEAT];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < ARRAY_LENGTH_HEAT; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / ARRAY_LENGTH_HEAT) + 2));
    }
  
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= ARRAY_LENGTH_HEAT - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < LED_NUM; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[ j * (ARRAY_LENGTH_HEAT / LED_NUM)], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (LED_NUM-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = blend(leds[pixelnumber], color, BLEND_FACTOR_SMOOTH);

      if(j < FIRE_BASE_LED_NUM)
        leds[j] = blend(leds[j], ColorFromPalette( gPal, 80), 30);
    }
}

