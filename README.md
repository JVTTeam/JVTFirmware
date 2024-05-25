# JVT Virtual Touch Firmware

## Overview

This firmware is designed to control an electro-tactile feedback system, enabling different levels of stimulation across various zones. It processes serial input to configure stimulation parameters and controls output accordingly.

## Serial Communication

The firmware receives commands via the serial port. The expected format of the serial messages is as follows:

- **Zone Strengths:**
  - `I[number]`: Set the strength of the "Thumb Distal" zone.
  - `J[number]`: Set the strength of the "Thumb Proximal" zone.
  - `K[number]`: Set the strength of the "Index Distal" zone.
  - `L[number]`: Set the strength of the "Index Middle" zone.
  - `M[number]`: Set the strength of the "Index Proximal" zone.
  - `N[number]`: Set the strength of the "Middle Distal" zone.
  - `O[number]`: Set the strength of the "Middle Middle" zone.
  - `P[number]`: Set the strength of the "Middle Proximal" zone.
  - `Q[number]`: Set the strength of the "Ring Distal" zone.
  - `R[number]`: Set the strength of the "Ring Middle" zone.
  - `S[number]`: Set the strength of the "Ring Proximal" zone.
  - `T[number]`: Set the strength of the "Pinky Distal" zone.
  - `U[number]`: Set the strength of the "Pinky Middle" zone.
  - `V[number]`: Set the strength of the "Pinky Proximal" zone.
  - `W[number]`: Set the strength of the "Palm" zone.
  - **Compatibility Interface:**
    - `A[number]`: Sets strengths for zones `I` and `J`.
    - `B[number]`: Sets strengths for zones `K`, `L`, and `M`.
    - `C[number]`: Sets strengths for zones `N`, `O`, and `P`.
    - `D[number]`: Sets strengths for zones `Q`, `R`, and `S`.
    - `E[number]`: Sets strengths for zones `T`, `U`, and `V`.

- **Frequency and Pulse Width:**
  - `X[number]`: Set the frequency of the stimulation cycle.
  - `Y[number]`: Set the pulse width of the stimulation.

Each command must be followed by a number indicating the desired value, e.g., `I500` sets the strength for the "Thumb Distal" zone to 500.

## Functional Description

The firmware initializes the necessary pins for communication and control. It continuously listens for serial input to update the strengths and configuration of the stimulation zones.


### Setup

The `setup()` function initializes the control pins and the serial communication.

**Pin Definitions:**
  - `DIN`, `CLK`, `LE`, `CLR`: Control pin connections to HV2705 analog switch.
  - `A0`: Analog output for controlling stimulation strength.

### Loop

The `loop()` function performs the following tasks:

1. **Serial Input Handling:**
   - Reads and parses the serial input.

2. **Stimulation Cycle:**
   - Ensures stimulation occurs at the specified frequency.
   - Configures the switches and sets the analog output for each zone based on the current strengths and pulse width.

## Usage

1. **Connect the device to a computer via a serial interface.**
2. **Send commands in the specified format to configure the stimulation parameters.**
3. **The device will process the commands and adjust the stimulation accordingly.**

Ensure the strengths are within the range of 0 to 1023, the frequency is between 1 and 1000Hz, and the pulse width is between 0 and 800 microseconds.