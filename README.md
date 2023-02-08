# MIDI Controller Firmware

This is the firmware for a MIDI controller that uses analog-to-digital conversion and digital inputs. It is compatible with the MIDIUSB library and utilizes the MCP3008 ADC chip for reading the analog inputs.

## Key Features

- 8 potentiometers and 8 buttons connected to the ADC and digital input, respectively
- MIDI control change messages sent for button presses and potentiometer changes
- Analog inputs mapped to MIDI values 0-127

## Libraries

- MIDIUSB.h
- MCP_ADC.h

## Pin Definitions

- ADC chip select: pin 10
- Potentiometer inputs: pins 0-7 on the ADC
- Button inputs: pins 2-9 with pull-up resistors

## Usage

Upload this firmware to an Arduino board, connect the necessary components (ADC, buttons, potentiometers), and connect the board to your computer or MIDI device using a USB cable. The firmware will continuously read the inputs and send MIDI control change messages as necessary.
