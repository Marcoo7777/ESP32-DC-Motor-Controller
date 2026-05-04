# ESP32-DC-Motor-Controller

Simple embedded project using ESP32.

## Features
- Control DC motor direction using a button
- PWM speed control (LEDC)
- Smooth acceleration and deceleration

## How it works
- Button input is read using GPIO with pull-up
- On button press:
  - motor direction toggles
- While button is held:
  - motor speed increases or decreases
- PWM signal (LEDC) controls motor speed
- GPIO pin controls motor direction (PHASE)
