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



//initialize color struct
color_increments_t color_peak_1;
color_increments_t color_base_1;

//factor, 8 bit color number is divided by to fade to darkness
float fade_scale = 1.03;




void music_vis_init() {
    //set color increments
    color_peak_1.r = PEAK_INC_R;
    color_peak_1.g = PEAK_INC_G;
    color_peak_1.b = PEAK_INC_B;

    color_base_1.r = BASE_INC_R;
    color_base_1.g = BASE_INC_G;
    color_base_1.b = BASE_INC_B;
}


void music_vis_update(CRGBSet& leds) {
    
    //read microphone level
    sensor_input = analogRead(ANALOG_READ);


    //send microphone value to PC
    //Serial.print(sensor_input);

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
    led_avg_level = map(value_avg, 0, 600, 0, LED_NUM) + 4;
    
    for (int i = 0; i < LED_NUM; i++) {
    //The leds we want to show
      if (i < led_level && i > led_avg_level) {
        if (leds[i].r + color_peak_1.r > PEAK_MAX_R)
          leds[i].r = PEAK_MAX_R;
        else if (leds[i].r + color_peak_1.r < 0)
          leds[i].r = 0;
        else
          leds[i].r = leds[i].r + color_peak_1.r;
          
        if (leds[i].g + color_peak_1.g > PEAK_MAX_G)
          leds[i].g = PEAK_MAX_G;
        else if (leds[i].g + color_peak_1.g < 0)
          leds[i].g = 0;
        else 
          leds[i].g = leds[i].g + color_peak_1.g;

        if (leds[i].b + color_peak_1.b > PEAK_MAX_B)
          leds[i].b = PEAK_MAX_B;
        else if (leds[i].b + color_peak_1.b < 0)
          leds[i].b = 0;
        else 
          leds[i].b = leds[i].b + color_peak_1.b;  
      }

      else if (i <= led_avg_level) {
        if (leds[i].r + color_base_1.r > BASE_MAX_R)
          leds[i].r = BASE_MAX_R;
        else if (leds[i].r + color_base_1.r < 0)
          leds[i].r = 0;
        else
          leds[i].r = leds[i].r + color_base_1.r;
          
        if (leds[i].g + color_base_1.g > BASE_MAX_G)
          leds[i].g = BASE_MAX_G;
        else if (leds[i].g + color_base_1.g < 0)
          leds[i].g = 0;
        else 
          leds[i].g = leds[i].g + color_base_1.g;

        if (leds[i].b + color_base_1.b > BASE_MAX_B)
          leds[i].b = BASE_MAX_B;
        else if (leds[i].b + color_base_1.b < 0)
          leds[i].b = 0;
        else 
          leds[i].b = leds[i].b + color_base_1.b;
      }


      //All the other LEDs fade to eventual total darkness
      else {
        leds[i] = CRGB(leds[i].r/fade_scale, leds[i].g/fade_scale, leds[i].b/fade_scale);
      }
    }
}