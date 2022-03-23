# Data Capture for Arduino Nicla Sense ME

This provides a [PlatformIO](https://platformio.org/) project to flash the Arduino Nicla Sense ME with firmware suitable for data capture with the [SensiML Data Capture Lab](https://sensiml.com/products/data-capture-lab/).

This code is meant as a reference code for setting up data capture with any Arduino Device. This firmware collects data from the BHI260AP smart sensor.

Documentation for using this firmware is provided at [SensiML's documentation site](https://sensiml.com/documentation/firmware/arduino-nicla-sense-me/arduino-nicla-sense-me.html). The process for Nano33 and Nicla Sense ME are identical, aside from environment names.

## Requirements

- PlatformIO IDE (or plugin for your favorite IDE, command line interface)
- Arduino Nicla Sense ME
- SensiML Data Capture Lab (Windows)

## Known Issues

- BLE is limited to 100Hz or lower for IMU due to throughput issues.
- The current PlatformIO codebase has a bug in .platformio\packages\framework-arduino-mbed\libraries\Nicla_System\src\Nicla_System.cpp that causes crashes.  There is a fixed version of this module in the Extras folder.
