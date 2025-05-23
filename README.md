# UDP Sensors

## Introduction

<p align="justify">
This repository provides the code to create an UDP Temperature and Humidity sensors.
Wifi and UDP parameters (ssid, password, target IP, port and measurments frequency) are configurable through a web interface.

Refer to [this documentation](docs/AccessPoint/README.md) to get more information about network configuration. 
</p>

## Motivation

<p align="justify">
The aim is to create a cheap sensor that can send T° and H% data via UDP within a local network. Then, by using a domotic server, I will be able to read these values and use them through a domotic interface. So there is no stored data, no permanent web interface and no API.
The domotic server can see when a UDP packet is sent on a given port.
</p>

<p align="center">
<img src="docs/Screenshots/NetworkDiagram.png" width="300"/>
</p>
<p align="center">
  <b>Network diagram</b>
</p>

### Constraints
<p align="justify">
To save energy, I have decided not to create a web server that is always on. It is not necessary to measure T° or H% every 30 seconds.
So I decided to use deepsleep between each measurement, with a configurable frequency.
When the sensor wakes up, it sends sensor readings and goes back to sleep.

If the sensors wake up while there is a wifi failure, it will start a power saving loop while re-establishing the wifi connection. See [functional diagram](#functional-diagram) for more details.
</p>

## Hardware

### Components list
- Esp32 wroom32
- AHT10 sensor
- 2x buttons
- 2x 10k Ohms resistors

### Wiring 
<p align="center">
<img src="docs/Screenshots/Wiring.png" width=450/>
</p>
<p align="center">
  <b>Wiring diagram</b>
</p>

#### Comments

A [pull up wiring](https://learn.sparkfun.com/tutorials/pull-up-resistors/all) is used with resistors :

- _When the button is pressed, the input pin is pulled low. The value of resistor controls how much current you want to flow from VCC, through the button, and then to ground._
- _When the button is not pressed, the input pin is pulled high. The value of the pull-up resistor controls the voltage on the input pin._ 


### Box modelisation

A box has been designed (Fusion360) based on ESP32 size: 
<p align="center">
<img src="docs/Screenshots/Animation.gif" width="350px"/>
</p>
<p align="center">
  <b>Box design</b>
</p>

### Final result
<p align="center">
<img src="docs/Screenshots/FinalBox.png" width=350/>
</p>
<p align="center">
  <b>Final box</b>
</p>

## Software

### Functional diagram

<p align="center">
<img src="docs/Screenshots/FunctionalDiagram.png"/>
</p>
<p align="center">
  <b>Functional diagram</b>
</p>

Refer to [this documentation](docs/AccessPoint/README.md) to get more information on the Access Point mode. 

### Final result

 Through my domotic server I have the following visualisation for temperatures over the time:
 <p align="center">
<img src="docs/Screenshots/Visualisation.png" width=550/>
</p>
<p align="center">
  <b>Temperatures visualisation</b>
</p>

## What's next ? 


- Hardware
  - Power using 3V battery 

- Software
  - Easy update using OTA
  - Auto-detect software update

- Deployment
  - Automaticaly generate binaries files using GitHub action