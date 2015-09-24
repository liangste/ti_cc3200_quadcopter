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
Sheet 5 6
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 5350 4050 3    60   Input ~ 0
scl
Text HLabel 5450 4050 3    60   Input ~ 0
sda
$Comp
L FXAS21002 U8
U 1 1 55542198
P 5250 3150
F 0 "U8" H 5800 3900 60  0000 C CNN
F 1 "FXAS21002" H 5800 4000 60  0000 C CNN
F 2 "cc3200_quad_footprints:FXAS21002C" H 5800 3900 60  0001 C CNN
F 3 "" H 5800 3900 60  0000 C CNN
	1    5250 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR077
U 1 1 555421BE
P 5700 2050
F 0 "#PWR077" H 5700 2050 30  0001 C CNN
F 1 "GND" H 5700 1980 30  0001 C CNN
F 2 "" H 5700 2050 60  0000 C CNN
F 3 "" H 5700 2050 60  0000 C CNN
	1    5700 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 2250 5950 2250
Connection ~ 5150 2250
Connection ~ 5250 2250
Connection ~ 5350 2250
Wire Wire Line
	5450 2250 5450 2000
Wire Wire Line
	5450 2000 5700 2000
Wire Wire Line
	5700 2000 5700 2050
Wire Wire Line
	4500 2850 4500 2250
Connection ~ 5050 2250
Wire Wire Line
	5950 2250 5950 2850
Connection ~ 5450 2250
$Comp
L GND #PWR078
U 1 1 5554220C
P 4500 4150
F 0 "#PWR078" H 4500 4150 30  0001 C CNN
F 1 "GND" H 4500 4080 30  0001 C CNN
F 2 "" H 4500 4150 60  0000 C CNN
F 3 "" H 4500 4150 60  0000 C CNN
	1    4500 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 3250 4500 4150
Connection ~ 4500 3350
Wire Wire Line
	4500 4050 5250 4050
Connection ~ 5150 4050
Connection ~ 4500 3450
Connection ~ 5050 4050
Connection ~ 4500 4050
$Comp
L C C28
U 1 1 55542472
P 6350 2850
F 0 "C28" H 6350 2950 40  0000 L CNN
F 1 "0.1uF" H 6356 2765 40  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 6388 2700 30  0001 C CNN
F 3 "" H 6350 2850 60  0000 C CNN
	1    6350 2850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR079
U 1 1 555424D5
P 6350 3050
F 0 "#PWR079" H 6350 3050 30  0001 C CNN
F 1 "GND" H 6350 2980 30  0001 C CNN
F 2 "" H 6350 3050 60  0000 C CNN
F 3 "" H 6350 3050 60  0000 C CNN
	1    6350 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 2950 6100 2950
Wire Wire Line
	6100 2650 6100 3050
Wire Wire Line
	6100 2650 6350 2650
Wire Wire Line
	6100 3050 5950 3050
Connection ~ 6100 2950
$Comp
L +3.3V #PWR080
U 1 1 55542514
P 6350 2650
F 0 "#PWR080" H 6350 2610 30  0001 C CNN
F 1 "+3.3V" H 6350 2760 30  0000 C CNN
F 2 "" H 6350 2650 60  0000 C CNN
F 3 "" H 6350 2650 60  0000 C CNN
	1    6350 2650
	1    0    0    -1  
$EndComp
$Comp
L C C31
U 1 1 555426F4
P 6650 3150
F 0 "C31" H 6650 3250 40  0000 L CNN
F 1 "0.1uF" H 6656 3065 40  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 6688 3000 30  0001 C CNN
F 3 "" H 6650 3150 60  0000 C CNN
	1    6650 3150
	0    1    1    0   
$EndComp
$Comp
L GND #PWR081
U 1 1 55542786
P 6850 3200
F 0 "#PWR081" H 6850 3200 30  0001 C CNN
F 1 "GND" H 6850 3130 30  0001 C CNN
F 2 "" H 6850 3200 60  0000 C CNN
F 3 "" H 6850 3200 60  0000 C CNN
	1    6850 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 3150 6450 3150
Wire Wire Line
	6850 3150 6850 3200
$Comp
L GND #PWR082
U 1 1 555427DF
P 6000 3550
F 0 "#PWR082" H 6000 3550 30  0001 C CNN
F 1 "GND" H 6000 3480 30  0001 C CNN
F 2 "" H 6000 3550 60  0000 C CNN
F 3 "" H 6000 3550 60  0000 C CNN
	1    6000 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 3450 6000 3450
Wire Wire Line
	6000 3450 6000 3550
$Comp
L C C29
U 1 1 5554281A
P 6350 3600
F 0 "C29" H 6350 3700 40  0000 L CNN
F 1 "0.1uF" H 6356 3515 40  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 6388 3450 30  0001 C CNN
F 3 "" H 6350 3600 60  0000 C CNN
	1    6350 3600
	1    0    0    -1  
$EndComp
$Comp
L C C30
U 1 1 55542848
P 6600 3600
F 0 "C30" H 6600 3700 40  0000 L CNN
F 1 "1.0uF" H 6606 3515 40  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 6638 3450 30  0001 C CNN
F 3 "" H 6600 3600 60  0000 C CNN
	1    6600 3600
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR083
U 1 1 5554289B
P 6700 3400
F 0 "#PWR083" H 6700 3360 30  0001 C CNN
F 1 "+3.3V" H 6700 3510 30  0000 C CNN
F 2 "" H 6700 3400 60  0000 C CNN
F 3 "" H 6700 3400 60  0000 C CNN
	1    6700 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 3250 6250 3250
Wire Wire Line
	6250 3250 6250 3400
Wire Wire Line
	6250 3400 6700 3400
Connection ~ 6350 3400
Connection ~ 6600 3400
Wire Wire Line
	5950 3350 6150 3350
Wire Wire Line
	6150 3350 6150 3800
Wire Wire Line
	6150 3800 6600 3800
Connection ~ 6350 3800
$Comp
L GND #PWR084
U 1 1 5554297B
P 6350 3850
F 0 "#PWR084" H 6350 3850 30  0001 C CNN
F 1 "GND" H 6350 3780 30  0001 C CNN
F 2 "" H 6350 3850 60  0000 C CNN
F 3 "" H 6350 3850 60  0000 C CNN
	1    6350 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 3850 6350 3800
Text Notes 5950 4050 0    60   ~ 0
sets slave address to 0x20
$Comp
L +3.3V #PWR085
U 1 1 55542D73
P 4350 3150
F 0 "#PWR085" H 4350 3110 30  0001 C CNN
F 1 "+3.3V" H 4350 3260 30  0000 C CNN
F 2 "" H 4350 3150 60  0000 C CNN
F 3 "" H 4350 3150 60  0000 C CNN
	1    4350 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 3150 4500 3150
Text HLabel 4100 2950 0    60   Input ~ 0
INT1
Wire Wire Line
	4100 2950 4450 2950
Wire Wire Line
	4450 2950 4450 3000
Wire Wire Line
	4450 3000 4500 3000
Wire Wire Line
	4500 3000 4500 3050
Text HLabel 4100 2850 0    60   Input ~ 0
INT2
Wire Wire Line
	4100 2850 4450 2850
Wire Wire Line
	4450 2850 4450 2900
Wire Wire Line
	4450 2900 4500 2900
Wire Wire Line
	4500 2900 4500 2950
$EndSCHEMATC
