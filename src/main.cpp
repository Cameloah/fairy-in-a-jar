#include <Arduino.h>
#include <FastLED.h>

//_______LEDs____________________________________________________________________________________
  
  //pin LED strip is connected to
  #define LED_PIN        5

  //number of LEDs connected
  #define LED_NUM        18

  
  //create the FastLED array containing led colors
  CRGB leds[LED_NUM];

//_______Microphone______________________________________________________________________________

  //pin microphone sinput is connected to
  #define ANALOG_READ     1

  //variable microphone input is stored in
  int sensor_input = 0;

//_______filter__________________________________________________________________________________

  //modified MA filter
  int inputFiltered = 0;
  int inputFilteredPrev = 0;
  float multiplicator_prev = 1;
  
  //max value envelope
  const int arr_max_size = 20;
  int arr_max[arr_max_size];
  int value_max = 0;

  //MA filter
  const int arr_avg_size = 200;
  int arr_avg[arr_avg_size];
  float value_avg = 0;

//_______loop timer______________________________________________________________________________
  int timer_size = 5000;
  int loop_timer = 0;
  float loop_time = 0;
  int timer_start = 0;

//_______music visualization_____________________________________________________________________

  //number of leds proportional to sensor input level
  int led_level = 0;

  //number of leds proportional to average sensor input level
  int led_avg_level = 0;

  //Stuct containing information on how much to increment or decrement each color every cycle
  struct color {
    int r;
    int g;
    int b;
  };

  //factor, 8 bit color number is divided by to fade to darkness
  float fade_scale = 1.03;

  //initialize color struct
  struct color color_peak_1; 
  struct color color_base_1;





void setup() {
  //initialize serial communication
  Serial.begin(250000);
  
  //_____LEDs____________________________________________________________________________________
    //set color increments
    color_peak_1.r = -1;
    color_peak_1.g = 4;
    color_peak_1.b = 20;

    color_base_1.r = 10;
    color_base_1.g = 4;
    color_base_1.b = -4;


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

  //_____Microphone______________________________________________________________________________

    //read analog value
    sensor_input = analogRead(ANALOG_READ);

    //If 0 or smaller, discard immediately
    if (sensor_input < 0){
    return;}

    //send microphone value to PC
    Serial.print(sensor_input);

  //_____filter__________________________________________________________________________________

    //max value envelope
      //reset max value
      value_max = 0;

      for (int i = 0; i < arr_max_size; i++) 
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
      inputFiltered = (value_max + (inputFilteredPrev * multiplicator_prev)) / (multiplicator_prev+1);
      inputFilteredPrev = inputFiltered;

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
    Serial.println(inputFiltered);
  

  //_____Music Programm__________________________________________________________________________

    //map sensor value to number of leds
    led_level = map(inputFiltered, 0, 600, 0, LED_NUM);
    led_avg_level = map(value_avg, 0, 600, 0, LED_NUM) + 4;
    
    for (int i = 0; i < LED_NUM; i++) 
    //The leds we want to show
      if (i < led_level && i > led_avg_level) {
        if (leds[i].r + color_peak_1.r > 255)
          leds[i].r = 255;
        else if (leds[i].r + color_peak_1.r < 0)
          leds[i].r = 0;
        else
          leds[i].r = leds[i].r + color_peak_1.r;
          
        if (leds[i].g + color_peak_1.g > 255)
          leds[i].g = 255;
        else if (leds[i].g + color_peak_1.g < 0)
          leds[i].g = 0;
        else 
          leds[i].g = leds[i].g + color_peak_1.g;

        if (leds[i].b + color_peak_1.b > 255)
          leds[i].b = 255;
        else if (leds[i].b + color_peak_1.b < 0)
          leds[i].b = 0;
        else 
          leds[i].b = leds[i].b + color_peak_1.b;  
      }

      else if (i <= led_avg_level) {
        if (leds[i].r + color_base_1.r > 255)
          leds[i].r = 255;
        else if (leds[i].r + color_base_1.r < 0)
          leds[i].r = 0;
        else
          leds[i].r = leds[i].r + color_base_1.r;
          
        if (leds[i].g + color_base_1.g > 180)
          leds[i].g = 180;
        else if (leds[i].g + color_base_1.g < 0)
          leds[i].g = 0;
        else 
          leds[i].g = leds[i].g + color_base_1.g;

        if (leds[i].b + color_base_1.b > 100)
          leds[i].b = 100;
        else if (leds[i].b + color_base_1.b < 0)
          leds[i].b = 0;
        else 
          leds[i].b = leds[i].b + color_base_1.b;
      }


      //All the other LEDs fade to eventual total darkness
      else {
        leds[i] = CRGB(leds[i].r/fade_scale, leds[i].g/fade_scale, leds[i].b/fade_scale);
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