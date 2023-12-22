# HUD-V2
Upgraded HUD for the DAQ system, using the ESP32-S3 Development board from Seeed Studio. The goal of this project is to simplify the understanding of data in real time. This should allow the driver to have a better understanding of the car as they drive it, as well as their performance, and other metrics.

## Hardware
Utilizes the ESP32-S3 using the TAMC Termod S3 board. Utilizes ST7701 driver and 16-bit RGB to control screen. Datasheet can be found [here](https://files.seeedstudio.com/products/ZX4D30NE01S-UR-4827.pdf)

## Library
We use the Arduino GFX library together with LVGL to control the screens display.
