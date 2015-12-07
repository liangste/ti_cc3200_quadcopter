# Hardware Overview

## Introduction

The hardware is designed using [KiCAD](http://kicad-pcb.org/) and manufactured with [OSI Park](https://oshpark.com/) PCB manufacturing facility. Build version for the KiCAD used is 2014-12-04 bZR 5312.

[CC3200](http://www.ti.com/product/cc3200) is a Wifi-based microcontroller made by Texas Instruments. For this project, CC3200 is used as the processor as well as communication interface. The chip runs at 80MHz and has an embedded Wifi co-processor that I can link with and control using my smartphone. I used this chip because 1) it's not too expensive, 2) its suitable for a 4-layer design, which is the current maximum amount of layers for OSH Park and 3) because it has a Wifi co-processor in the same die.

## Hardware Breakdown

### Wifi processor

VQFN-64 package in a 9mm x 9mm package. Contains GPIO, UART, I2C, SPI, PWM, and Parallel Camera interface.

### Power

Main power source for the quad is a 3.7V Li-Po battery. The voltaged is stepped-down to 3.3V using Low-DropOut (LDO) regulators.

### Motor Control

4 MOSFETs are used to control 4 DC motors. GPIO_9, 10, 11, and 28 provide the PWM signals from CC3200.

### Acceleromter + Gyro

MPU6050 is an Gyroscope + Accelerometer 2 in 1 sensor with I2C and SPI interfaces. One nice thing about the MPU6050 is the Low-Pass Filter which can be used to filter out unwanted Acceleromter high-frequency noise caused by motor vibrations.

### Indicator LEDs

3 LEDs indicate Wifi-connectivity status, Sensor status and transmission indications. 1 Red LED is used to indicate power status.

## Build and Test

Once schematics and layouts of the board is done. The design is sent to OSH Park for processing. Turnaround time was usually 2 weeks for 2 layers boards and 3-4 weeks for 4 layer boards. Once the board arrives, I picked them up and soldered with components that I ordered separately from Digikey. I constructed a ControLeo2-based reflow oven from [Whizoo](http://www.whizoo.com/) and used it to solder SMD components on the top of the board. The bottom is left for larger components like MOSFETS, NAND memory and LDO regulators that I can solder by hand.

I used the following chart as a guide for testing each hardware components. The rows go from easiest to hardest to verify.

| Component | How to test |
| :--------- | :-----------: |
|CC3200 Power|Is the Red Power LED on? Probe at 3.3V|
|JTAG| Use CCS to connect to CC3200 using TCK and TMS lines |
|Flash Programming| Use Tx and Rx lines to flash binaries to NAND using UniFlash |
|UART|Print out characters and receive characters from the terminal. If Flash Programming work, this should also work|
|LEDs|Blink LEDs in a sequence|
|PMW|Make sure that varying PWM duty cycle changes motor speed|
|I2C|Do a read-write-read test to scrach-pad register on MPU6050. Also test by reading its signature register|
|RF|Start a SimpleLink TCP Server and perform a packet transmission test with smartphone|
