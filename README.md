# Pixy2 Traffic Light Warning Sound System

## Overview
This is an Arduino project that uses a Pixy2 camera to detect the color of traffic lights and outputs appropriate warning sounds according to state changes.

## Project Name Meaning
**BEEP (Blind pedestrian Environmental Early Protection)**
- **Common Meaning**: The familiar "beep" sound from cars, warnings, etc.
- **Project Meaning**: It signifies a system that provides early protection for the pedestrian environment of visually impaired individuals, perfectly aligning with the familiar image of a warning sound.

## Hardware Requirements
- Arduino Uno/Nano
- Pixy2 Camera
- 2 Buzzers
  - Main Buzzer (for red light and blinking): Digital Pin 2
  - Auxiliary Buzzer (for green light only): Digital Pin 3

## Functions
1.  **Traffic Light State Detection**
    - **Green Light**: Signatures 1, 2, 3
    - **Red Light**: Signatures 4, 5, 6

2.  **State Transition Alerts**
    - **Red → Green**: 1-second warning sound
    - **Green → Red**: 1-second warning sound
    - **Green → Blinking**: Rapid repeating sound (80ms × 8 times)
    - **Blinking → Red**: 1-second warning sound

3.  **Blinking State Detection**
    - The system immediately recognizes a blinking state if the green light turns off and on even once.
    - Detects traffic light blinking in real-time for immediate state transition.

## Key Features
- Serial Monitoring (115200 baud)
- Emoji indicators for state transitions
- A rapid repeating sound alerts to an emergency situation when the green light blinks.
