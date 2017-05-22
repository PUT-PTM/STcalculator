README

Overview
This project was made on Poznan University of Technology. We decided to make calculator, it is also called calculating machine. It's a small electronic&mechanical device that performs calculations, requiring manual action for each individual opertion.

Description
This project consists of:
STM32F407VG microcontroller,
HD44780 display,
Membrane 4x4 Matrix Array keyboard

The heart of this device is microcontroller STM32F4 Discovery. We use LCD HD44780 to display numbers and everything else. To enter numbers we use a membrane keypad. All these things form the whole and are enclosed in a printed 3D cover.

Tools
Software that we use is CooCox CoIDE Version: 1.7.8

How to run
You should upload the program to the STM32F4 board eg in Coocox.

How to compile
There are no any special things to do. Just copy the project.

Future improvements
There is no any bugs. There is a 0.0001 precision.
You can add brackets for example to calculate something like this(2+2)*2=8

Attributions
To handle with printing in LCD we use graphic library https://github.com/emusan/stm32f4-charlcd
To handle with convert decimal to binary and binary to decimal we use functions convertBinaryToDecimal() and  convertDecimalToBinary() from
https://www.programiz.com/c-programming/examples/binary-decimal-convert

License
You are using this project at your own risk.
Authors are not responsible for damage caused by the project.
MIT

Credits
Natalia Popielarz natalia.popielarz@put.poznan.pl
Piotr Wołyński piotr.wolynski@put.poznan.pl


The project was conducted during the Microprocessor Lab course held by the Institute of Control and Information Engineering, Poznan University of Technology.
Supervisor: Tomasz Mańkowski
