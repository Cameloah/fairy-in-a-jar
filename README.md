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
1. **Get the Files!** It is most convenient to use the same IDE: VSC with the Platformio extension. *More Information:* https://platformio.org/install/ide?install=vscode. Download the project files or clone into this project.

2. **Port to your Arduino Board!** Change the configuration in `/platformio.ini` according to your arduino board.

3. **Set your LED configuration!** Navigate to `/include/led_config.h` and check the pin you're using on the arduino, the number of your LEDs and the type of your LEDs.

4. **Configure the microphone input!** Navigate to `/include/music_visualisation.h` and check the pin the analog signal of the microphone is connected to your arduino.

5. **Flash your Arduino!"** You can now upload the code to your MCU and enjoy the show.

### Ambient Light Mode
When you pull D3 on your MCU low, the programm enters Ambient Light Mode.  
In this mode, the main loop cycles through a list of color animations `module_update`. There are 3 prebuilt animation modules `fire_update,  
twinkle_update,  
ocean_update`  
Here, you can add or remove animations as you like. Just dont forget to change the number of modules in the list `EFFECT_MODULE_NUM`. You can change the duration an animation is active, by changing `EFFECT_DURATION_SEC`. The time it takes to blend from one animation to the next, can be set in `EFFECT_BLEND_IN_SEC`.

### Music Visualisation Mode
Pull D3 high on your MCU to enter Music Visualisation Mode. If everything is set correctly, the microphone can be placed in proximity of a speaker. The LEDs will react to the music.

Good Luck with your own project and have fun!  

Cheers, Cameloah






