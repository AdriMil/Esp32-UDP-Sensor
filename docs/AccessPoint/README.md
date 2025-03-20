# Access Point mode

<p align="justify">

Refer to the [functional diagram](../../README.md#functional-diagram) to know in which condition you can run the Access Point mode.

:warning: If there is no activity on the Access Point web pages for **10 minutes**, the ESP32 will enter sleep mode. The only way to return to Access Point mode is to press the Reset button see [functional diagram](../../README.md#functional-diagram). Each time you access a page, the 10 minutes timer will be reset.
</p>

## Credentials

When the ESP32 start up in Access Point mode, you should connect to its Wi-Fi network.

Access point values are defined in the [default.cpp file](../../lib/default/default.cpp).

SSID: ESP32 \
Password: 123456789

## Interfaces

<p align="justify">

Once connected to the access point, you must access the server user interface available on the `192.168.4.1` IP address.
</p>

## Index pages

This page will allows you to `Configure Wifi and UDP` or to get `More information`.

<p align="center">
<img src="../Screenshots//Index.png" width=350/>
</p>
<p align="center">
  <b>Home page</b>
</p>

## Wifi and UDP Configuration

This page will allows you to `Configure Wifi and UDP`. \
Default values are defined in the [default.cpp file](../../lib/default/default.cpp)

Requested information are: 

<div align="center">


|   Parameters                  |   Default value   | Constraintes                      | Comments                                          |
|:-----------------------------:|:----------------: |:-------------------               |:--------------------------------------------------|
|     Device ID                 |     404           | Min: 1 <br>Max: 50                |    Added in UDP message to identify the device    |
|     Wifi SSID                 |                   |                                   | Your Wifi SSID                                    |
|     Wifi password             |                   |                                   | Your Wifi password                                |
|     UDP port                  |     5000          | Min: 5000 <br>Max: 65535          | Port use to send UDP message                      |
|     UDP Ip Target             |     192.168.1.255 | IP format only <br>www.xxx.yyy.zzz| Target Ip on your local network                   |
|     UDP Message frequency     |     1200          | Min: 60 <br>Max: 18000            | T° and H% transmission frequency in seconds       |
</div>

<br>
<br>
<p align="center">
<img src="../Screenshots/Configuration.png" height=400 width=250/>
</p>
<p align="center">
  <b>Wifi and UDP configuration page</b>
</p>

<p align="justify">

When the _Send_ button is pressed, the ESP32 will reboot and try to connect to your wireless network according to your settings. If it cannot connect to the wireless network, it will restart the access point mode. See [functional diagram](../../README.md#functional-diagram).
</p>
