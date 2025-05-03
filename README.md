


## Overview

This project demonstrates a digital stopwatch simulated using the PIC18F452 microcontroller in Proteus. The stopwatch displays time in the format `HH:MM:SS:FF` (hours, minutes, seconds, and fractions of a second) on a 16x2 LCD display.

The simulation includes:
- Start, Stop, and Reset push-buttons
- 16x2 LCD display (XHD162A)
- Potentiometer for contrast control
- Timer logic implemented in C using MPLAB X with the XC8 compiler

## Features

- LCD interfacing in 4-bit mode
- Accurate timer simulation using delays
- Displays elapsed time in real-time
- Button-based control: Start, Stop, and Reset

## Components Used

- **Microcontroller**: PIC18F452
- **LCD Display**: 16x2 (XHD162A)
- **Push Buttons**: Start, Stop, Reset
- **Resistor**: 4kΩ for pull-up
- **Potentiometer**: 10kΩ for LCD contrast
- **Power Supply and Ground Rails**

## Getting Started

### Requirements

- [Proteus Design Suite](https://www.labcenter.com/)
- [MPLAB X IDE](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)
- [MPLAB XC8 Compiler](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers)

### Running the Simulation

1. Clone the repository.
2. Open the `.pdsprj` file in Proteus.
3. Build the code in MPLAB X and generate the `.hex` file.
4. Load the `.hex` file into the microcontroller in Proteus.
5. Click **Run** in Proteus to start the simulation.

## Code Overview

The stopwatch uses 4-bit LCD communication and timer logic based on software delays. The code is modular with the following structure:
- `LCD_Init()`, `LCD_Command()`, `LCD_Char()` for LCD operations
- `start()` and `stop()` functions to handle time counting and button interrupts
- `main()` initializes the peripherals and handles button input

### Time Format

`[HR2][HR1]:[MN2][MN1]:[SEC2][SEC1]:[FR2][FR1]`

