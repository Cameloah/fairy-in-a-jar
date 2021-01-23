# fairy_in_a_jar

WIP  

This project developed from a larger project where LEDs were controlled by an auxiliary music input using an Arduino Uno and a spectum analyzer shield by SparkFun.
In this project I wanted to minimize the form factor and simplify the components. Regarding the overall appearance I got some inspiration from **Gemi Michelle** https://www.youtube.com/watch?v=jgufxIUwslw. So for the DIY of the fairy jar, check out her video.

## Hardware

MCU: Arduino Nano  
Sound-Sensor: Sound Detector by RoboDyn  
LEDs: WS2812b  
number of LEDs: 18  
current consumption: ~1A DC

## Software

IDE: Visual Studio Code, Platformio extension  
Controlling of the LEDs using the FastLED library by Daniel Garcia

## Electronic Circuit

### RGB LEDs
Some of the cheaper options of the RGB led WS2812b are quite unstable in their data connection which can result in flickering due to missinterpretation of the signal.  
This issue was solved by adding a 100nF capacitor in close proximity to every LED.  
This type of LEDs draws a maximum of 60 mA at full White brightness. Powering the LEDs using the arduino 5V output is therefore not recommended.  

### Microphone Module
Depending on the quality of the LEDs, they also might interfer with the microphone module. Lighting up a number of LEDs depending on the microphone input amplitude therefore can result in unpredictable feedback.  
To solve this soultion, a DC-DC isolator was added to provide an isolated power source for the microphone module. The GND was still commonly grounded with the MCU

### Power Supply
For this build, a simple 5V USB Smartphone charger was used. A 200 uF electrolyte capacitor was added at the power input to increase the supply's stability and decrease higher frequency harmonic noise.

## How to port this Code to Your Project





