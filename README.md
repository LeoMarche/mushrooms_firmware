# mushrooms_firmware

This repo contains the code needed for our POC project - controlled mushrooms growing environnment.
The code is in two parts :
- .ino for arduino code
- .py for raspberry pi code

## Setup

In order to get the projet working, you need to :
1. Wire all the components together according to the wiring_diagram.png
2. Install LCD and THsensor libraries in the Arduino IDE.
3. Install pyserial and Flask library on the raspberry pi : `sudo apt install python3-pip && python3 -m pip install flask pyserial`
4. Upload the .ino code to the arduino using Arduino IDE software and a computer
5. Run raspbian on the raspberry, replace the `PORT` variable in the mushroom_controller.py file by the accurate value for your setup, and finally run the `python3 serv/mushroom_controller.py`.

## Arduino Code

The Arduino Code contains four major parts :
- functions to retrieve data from the sensors
- functions to calculate the command using PI blocks
- functions to apply command to the actuators
- a routine to handle serial communication with the raspberry pi

## Raspberry pi code

The raspberry pi code contains a library of functions that can be used to communicate with the arduino, parametrize it and retrieve metrics. It also contains a little example script that runs all the functions.