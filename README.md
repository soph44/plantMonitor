# Plant Monitoring System

This project is still WIP.

## About The Project

This project is a HW/SW project.

The HW consists of a set of microcontrollers and a set of sensors and cameras to collect data and monitor the livelihood of a plant (moisture, temperature, humidity, light). The number of microcontrollers per plant is minimized by multiplexing several sets to sensors to a each sensor microcontroller. This gives flexibility to the number of sets (plants) that can be monitored at the same time.

The SW comprises of a front end dashboard display of dynamic sensor data and imaging of each plant. The data and images will also be posted to a database for later correlation analysis.

### Architecture


Electrical Diagram:


Sequence Diagram:


## Technology Stack

* Hardware
    * Microcontrollers:
        * Arduino Uno R3
            * Sensors: DHT11 temperature/humidity, TEMT6000 light, MH Series conductive moisture
            * Display: LCD1602 I2C
            * MUX: SN74HC4051N
        * ESP32-CAM
            * OV7670 camera
            * ST7789V OLED LCD Display
* Software
    * Front End: Flask/Dash
    * Database: PostgreSQL
    * Deployment: Docker

## File Structure


### Local Deployment

TBD instructions on setup

## Example Dashboard

![Example Dashboard](.png)