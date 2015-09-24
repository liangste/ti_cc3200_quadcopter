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
Sheet 2 4
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
L DEA202450BT U5
U 1 1 55238D72
P 4850 3000
F 0 "U5" H 4850 2750 60  0000 C CNN
F 1 "DEA202450BT-1294C1-H" H 4850 3200 60  0000 C CNN
F 2 "cc3200_quad_footprints:DEA202450BT" H 4850 3000 60  0001 C CNN
F 3 "" H 4850 3000 60  0000 C CNN
	1    4850 3000
	1    0    0    -1  
$EndComp
$Comp
L INDUCTOR_SMALL L4
U 1 1 55238DCD
P 5650 2900
F 0 "L4" H 5650 3000 50  0000 C CNN
F 1 "3.6nH" H 5650 2850 50  0000 C CNN
F 2 "Capacitors_SMD:C_0402" H 5650 2900 60  0001 C CNN
F 3 "" H 5650 2900 60  0000 C CNN
	1    5650 2900
	1    0    0    -1  
$EndComp
$Comp
L C C24
U 1 1 55238E02
P 6050 3100
F 0 "C24" H 6050 3200 40  0000 L CNN
F 1 "1.0pF" H 6056 3015 40  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 6088 2950 30  0001 C CNN
F 3 "" H 6050 3100 60  0000 C CNN
	1    6050 3100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR069
U 1 1 55238E50
P 6050 3350
F 0 "#PWR069" H 6050 3350 30  0001 C CNN
F 1 "GND" H 6050 3280 30  0001 C CNN
F 2 "" H 6050 3350 60  0000 C CNN
F 3 "" H 6050 3350 60  0000 C CNN
	1    6050 3350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR070
U 1 1 55239017
P 4850 3450
F 0 "#PWR070" H 4850 3450 30  0001 C CNN
F 1 "GND" H 4850 3380 30  0001 C CNN
F 2 "" H 4850 3450 60  0000 C CNN
F 3 "" H 4850 3450 60  0000 C CNN
	1    4850 3450
	1    0    0    -1  
$EndComp
Text HLabel 4300 2900 0    60   Input ~ 0
RF
$Comp
L AH316M245001 U6
U 1 1 5522DDA0
P 6550 2900
F 0 "U6" H 6500 3000 60  0000 C CNN
F 1 "AH316M245001" H 6550 2800 60  0000 C CNN
F 2 "cc3200_quad_footprints:AH316M" H 6550 2900 60  0001 C CNN
F 3 "" H 6550 2900 60  0000 C CNN
	1    6550 2900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR071
U 1 1 5522DDFB
P 7000 2950
F 0 "#PWR071" H 7000 2950 30  0001 C CNN
F 1 "GND" H 7000 2880 30  0001 C CNN
F 2 "" H 7000 2950 60  0000 C CNN
F 3 "" H 7000 2950 60  0000 C CNN
	1    7000 2950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR072
U 1 1 5528506E
P 6150 2800
F 0 "#PWR072" H 6150 2800 30  0001 C CNN
F 1 "GND" H 6150 2730 30  0001 C CNN
F 2 "" H 6150 2800 60  0000 C CNN
F 3 "" H 6150 2800 60  0000 C CNN
	1    6150 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 2900 5400 2900
Wire Wire Line
	5900 2900 6100 2900
Wire Wire Line
	6050 3350 6050 3300
Wire Wire Line
	4750 3400 4950 3400
Wire Wire Line
	4850 3450 4850 3400
Connection ~ 4850 3400
Wire Wire Line
	4300 2900 4450 2900
Connection ~ 6050 2900
Wire Wire Line
	7000 2900 7000 2950
Wire Wire Line
	6150 2800 6150 2750
Wire Wire Line
	6150 2750 6050 2750
Wire Wire Line
	6050 2750 6050 2900
$EndSCHEMATC
