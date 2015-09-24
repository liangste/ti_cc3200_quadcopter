EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:cc3200_quad
LIBS:dips-s
LIBS:cc3200_quad-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 6
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L +3.3V #PWR069
U 1 1 552823C3
P 4950 3350
F 0 "#PWR069" H 4950 3310 30  0001 C CNN
F 1 "+3.3V" H 4950 3460 30  0000 C CNN
F 2 "" H 4950 3350 60  0000 C CNN
F 3 "" H 4950 3350 60  0000 C CNN
	1    4950 3350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR070
U 1 1 552823D7
P 4950 3900
F 0 "#PWR070" H 4950 3900 30  0001 C CNN
F 1 "GND" H 4950 3830 30  0001 C CNN
F 2 "" H 4950 3900 60  0000 C CNN
F 3 "" H 4950 3900 60  0000 C CNN
	1    4950 3900
	1    0    0    -1  
$EndComp
$Comp
L C C25
U 1 1 5528260E
P 4950 3600
F 0 "C25" H 4950 3700 40  0000 L CNN
F 1 "0.1uF" H 4956 3515 40  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 4988 3450 30  0001 C CNN
F 3 "" H 4950 3600 60  0000 C CNN
	1    4950 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 3350 4950 3400
Wire Wire Line
	4950 3800 4950 3900
Wire Wire Line
	4950 3400 5150 3400
Wire Wire Line
	5150 3400 5150 3450
Wire Wire Line
	5150 3450 5200 3450
Wire Wire Line
	4950 3800 5150 3800
Wire Wire Line
	5150 3800 5150 3650
Wire Wire Line
	5150 3650 5200 3650
Text HLabel 6250 3450 2    60   Input ~ 0
SDA_IN
Text HLabel 6250 3650 2    60   Input ~ 0
SCL_IN
Wire Wire Line
	6100 3450 6250 3450
Wire Wire Line
	6100 3650 6250 3650
$Comp
L MS5637-02BA03 U6
U 1 1 55277D01
P 5650 3550
F 0 "U6" H 5600 3750 60  0000 C CNN
F 1 "MS5637-02BA03" H 5650 3350 60  0000 C CNN
F 2 "cc3200_quad_footprints:MS5637-02BA03" H 5650 3550 60  0000 C CNN
F 3 "" H 5650 3550 60  0000 C CNN
	1    5650 3550
	1    0    0    -1  
$EndComp
$EndSCHEMATC
