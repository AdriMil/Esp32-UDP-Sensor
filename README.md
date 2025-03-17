# UDP Sensors

## Introduction
This repository provide the code to create an UDP Temperature and Humidity sensors.
Wifi and UDP parameters (target IP and port) are configurable through a web interface.

## Hardware

### Components list
- Esp32 wroom32
- AHT10 sensor
- 2 buttons
- 2 10k Ohms resistors

### Wiring 
![Wiring](/docs/Wiring.png)

### Comments
A [pull up wiring](https://learn.sparkfun.com/tutorials/pull-up-resistors/all) is used with resistors :

- _When the button is pressed, the input pin is pulled low. The value of resistor controls how much current you want to flow from VCC, through the button, and then to ground._
- _When the button is not pressed, the input pin is pulled high. The value of the pull-up resistor controls the voltage on the input pin._ 

## Software

### Functional diagram
![Functional Diagram](/docs/FunctionalDiagram.png)

