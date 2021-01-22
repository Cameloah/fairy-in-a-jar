#include "modules/music_visualisation.h"
#include "tools/filters.h"



//variable microphone input is stored in
uint16_t sensor_input = 0;


//modified MA filter
uint16_t inputFiltered = 0;
  
//max value envelope
const uint8_t arr_max_size = 20;
uint16_t arr_max[arr_max_size];
uint16_t value_max = 0;

//MA filter
const uint8_t arr_avg_size = 200;
uint16_t arr_avg[arr_avg_size];
double value_avg = 0;


//number of leds proportional to sensor input level
uint8_t led_level = 0;

//number of leds proportional to average sensor input level
uint8_t led_avg_level = 0;

//factor, 8 bit color number is divided by to fade to darkness
float fade_scale = 1.03;


void music_vis_init() {
    //set color increments
}

uint8_t palette_color = 0;

void music_vis_update(CRGBSet& leds) {
    
    //read microphone level
    sensor_input = analogRead(ANALOG_READ);


  EVERY_N_MILLISECONDS(200) {palette_color++;}

  if(palette_color > 240) {
    palette_color = 0;
  }

    //_____filter__________________________________________________________________________________

    //max value envelope
      //reset max value
      value_max = 0;

      for (uint8_t i = 0; i < arr_max_size; i++) 
      {
        //compute max value
        if(value_max < arr_max[i]){
          value_max = arr_max[i];
        }

        //shift all entries forwards
        if(i < arr_max_size - 1){
          arr_max[i] = arr_max[i+1];
        }

        //update last entry with new value
        else{
          arr_max[i] = sensor_input;
        }
      }

    

    //modified MA (moving average) filter
      inputFiltered = envelopeFilter(value_max, 3);

    //MA filter
      value_avg = 0;
      for (int i = 0; i < arr_avg_size; i++)
      {
        //sum up all entries
        value_avg += arr_avg[i];

        //shift all entries forwards
        if(i < arr_avg_size - 1){
          arr_avg[i] = arr_avg[i+1];
        }

        //update last entry with new value
        else{
          arr_avg[i] = sensor_input;
        }
      }

      //compute average sensor value
      value_avg /= arr_avg_size;
    


    Serial.print("\t");
    Serial.print(value_max);
    Serial.print("\t");
    Serial.print(value_avg);
    Serial.print("\t");
    Serial.print(envelopeFilter(sensor_input, 5));
    Serial.print("\t");
    Serial.println(inputFiltered);
  

    //map sensor value to number of leds
    led_level = map(inputFiltered, 0, 600, 0, LED_NUM);
    led_avg_level = map(1.3*value_avg, 0, 600, 0, LED_NUM) + BASE_LED_NUMBER_MIN;
    
    
    for (int i = 0; i < LED_NUM; i++) {
    //The leds we want to show
      if (i < led_level && i > led_avg_level)
        leds[i] = blend(leds[i], ColorFromPalette( PartyColors_p, palette_color, 254, LINEARBLEND), 100);

      else if (i <= led_avg_level)
        leds[i] = blend(leds[i], CRGB::Purple, 40);

      //All the other LEDs fade to eventual total darkness
      else 
        leds[i] = CRGB(leds[i].r/fade_scale, leds[i].g/fade_scale, leds[i].b/fade_scale);
    }
}