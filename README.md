# S11-C-Driver
Generic C driver for Sensair Sunrise CO2 sensor (S11)

## Introduction
The Sensair Sunrise CO2 sensor is a low power sensor suitable for battery operated equipment.
It has two hardware interfaces, I2C and modbus(via UART). This driver supports the I2C mode exclusively.

All sensor features are supported including:
* Continuous measurement mode
* Single measurement mode
* Sensor calibration
* Sensor configuration
* Sensor info

# Compatability & Integration
This driver is intended for microcontroller use, but can be used by any OS with a C compiler and an I2C peripheral or interface.
The driver takes care of all logic, parsing and state changes. All the user has to provide are callbacks for I2C read and write functions.

## Examples
Get started with this driver by browsing the examples folder:

## Sensor documentation
Sensair provides documentation with specs, functional description etc on https://senseair.com/products/power-counts/sunrise/

## API documentation
This API is documented with Doxygen-compliant comments in the source code.
Doxygen documentation is available in the docs folder:

