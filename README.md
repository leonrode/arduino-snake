# arduino-snake
A snake implementation on an Arduino Mega 2560 and an 8x8 LED matrix with a Dual-Axis Joystick, featuring my [BetterJoystick](https://github.com/lionrocker/better-joystick) library.

## Introduction

This sketch utilizes an 8x8 LED Matrix and a dual-axis joystick to control and display the snake game. It uses my custom-written [BetterJoystick](https://github.com/lionrocker/better-joystick) library to interface the dual-axis joystick.

## Usage

You will need an Arduino board, a dual-axis joystick, and an 8x8 LED matrix.

Connect the 8x8 LED Matrix with pins `DIN = 7`, `CLK = 10`, and `CS = 11`. Additionally, connect the joystick to with pins `X = A1`, `Y = A2`, and `switch = 3`. Then, compile and upload the `.ino` file.

## License

This software is published under the [MIT License](https://opensource.org/licenses/MIT). License text can be found in the LICENSE file.

