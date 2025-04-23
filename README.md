# MiRuś

<p><b>MiRuś</b> is an Arduino UNO based mobile robot project designed only for entertainment purposes only (so far). It's outer shell 3D printed design is based on Mobile Industrial Robots' <i>MiR200</i>, however it is scaled down by over a half MiR's size. It is not designed to carry any load neither to perform any tasks or missions.</p>

<p align="center">
  <img src="image-url-here" alt="image" width="400"/>
</p>

---

## Table of Contents

- [Features](#features)
- [Hardware](#hardware)
- [Libraries](#libraries)
- [Program logic](#program-logic)
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)
- [Media](#media)

---

## Features

### Wandering

<p>As it was mentioned before, project's sole purpose is show-off entertainment thus It's movement capabiities are narrow. Arduino code of MiRus only allows him to drive in a straight line mindlessly until detection of an obstacle in certain range.</p>

### Colision prevention

<p>When an obstacle is detected, the robot turns by a random angle to have a clear path, or stays motionless when obstacle was detected too close.</p>

### PID control

<p>To make sure the robot drives in a straight line the robot drives by simplified version of PID regulator</p>

### LED state indication

<p>Depending on the action robot is performing, LED strip lights up colors according to actual MiR200 robot.</p>

---

## Hardware

- Arduino UNO Rev 3
- HC-SR04 (40cm range)
- DFROBOT 6V DC motors with encoders
- Cytron MDD3A dual motor driver
- WS2812B addressable LED strip (~90cm)
- 5V step down module

---

## Libraries

- EncoderClass.h - contains EncoderClass with efficient data acquisition from two incremental encoders
- Adafruit_NeoPixel.h
- CytronMotorDriver.h
- avr/power.h

---

## Installation

<p>Instructions to follow when first using the code</p>

- Copy raw file
- Paste into Arduino IDE

---

## Usage

- Upload the code through Arduino IDE
- Assemble the robot
- Turn on the robot
- Monitor serial port information if needed

---

## License

This project is under [MIT License](LICENSE)

---

## Contact

- [LinkedIn](https://www.linkedin.com/in/micha%C5%82-w%C3%B3jcik-562213266/)

---
