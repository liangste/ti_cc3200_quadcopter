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
Sheet 3 4
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
L BATTERY BT1
U 1 1 55257CF7
P 4300 2600
F 0 "BT1" H 4300 2800 50  0000 C CNN
F 1 "JST_CONNECTOR" H 4300 2410 50  0000 C CNN
F 2 "cc3200_quad_footprints:JST_2PIN_2mm" H 4300 2600 60  0001 C CNN
F 3 "" H 4300 2600 60  0000 C CNN
	1    4300 2600
	0    1    1    0   
$EndComp
$Comp
L GND #PWR073
U 1 1 55257D33
P 4450 3000
F 0 "#PWR073" H 4450 3000 30  0001 C CNN
F 1 "GND" H 4450 2930 30  0001 C CNN
F 2 "" H 4450 3000 60  0000 C CNN
F 3 "" H 4450 3000 60  0000 C CNN
	1    4450 3000
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR074
U 1 1 55258B05
P 6850 2200
F 0 "#PWR074" H 6850 2160 30  0001 C CNN
F 1 "+3.3V" H 6850 2310 30  0000 C CNN
F 2 "" H 6850 2200 60  0000 C CNN
F 3 "" H 6850 2200 60  0000 C CNN
	1    6850 2200
	1    0    0    -1  
$EndComp
$Comp
L CP2 C25
U 1 1 55259DDF
P 4600 2500
F 0 "C25" H 4600 2600 40  0000 L CNN
F 1 "100uF" H 4606 2415 40  0000 L CNN
F 2 "Capacitors_Tantalum_SMD:TantalC_SizeA_EIA-3216_HandSoldering" H 4638 2350 30  0001 C CNN
F 3 "" H 4600 2500 60  0000 C CNN
	1    4600 2500
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR075
U 1 1 552604FE
P 4300 2200
F 0 "#PWR075" H 4300 2150 20  0001 C CNN
F 1 "+BATT" H 4300 2300 30  0000 C CNN
F 2 "" H 4300 2200 60  0000 C CNN
F 3 "" H 4300 2200 60  0000 C CNN
	1    4300 2200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR076
U 1 1 55294ACA
P 6850 2850
F 0 "#PWR076" H 6850 2850 30  0001 C CNN
F 1 "GND" H 6850 2780 30  0001 C CNN
F 2 "" H 6850 2850 60  0000 C CNN
F 3 "" H 6850 2850 60  0000 C CNN
	1    6850 2850
	1    0    0    -1  
$EndComp
$Comp
L MCP1726 U7
U 1 1 55C912E0
P 5700 2450
F 0 "U7" H 5700 2200 60  0000 C CNN
F 1 "MCP1726" H 5700 2700 60  0000 C CNN
F 2 "SMD_Packages:SOIC-8-N" H 5700 2450 60  0001 C CNN
F 3 "" H 5700 2450 60  0000 C CNN
	1    5700 2450
	1    0    0    -1  
$EndComp
$Comp
L CP2 C26
U 1 1 55259D90
P 6850 2500
F 0 "C26" H 6850 2600 40  0000 L CNN
F 1 "100uF" H 6856 2415 40  0000 L CNN
F 2 "Capacitors_Tantalum_SMD:TantalC_SizeA_EIA-3216_HandSoldering" H 6888 2350 30  0001 C CNN
F 3 "" H 6850 2500 60  0000 C CNN
	1    6850 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 2200 4300 2300
Wire Wire Line
	4300 2300 5250 2300
Wire Wire Line
	4300 2900 4600 2900
Wire Wire Line
	4600 2900 4600 2700
Wire Wire Line
	4450 2900 4450 3000
Connection ~ 4450 2900
Connection ~ 4600 2300
Wire Wire Line
	5250 2400 4950 2400
Wire Wire Line
	4950 2300 4950 2500
Connection ~ 4950 2300
Wire Wire Line
	6850 2300 6850 2200
Wire Wire Line
	6150 2400 6300 2400
Wire Wire Line
	4950 2500 5250 2500
Connection ~ 4950 2400
Wire Wire Line
	6150 2300 6850 2300
Wire Wire Line
	6300 2400 6300 2300
Connection ~ 6300 2300
$Comp
L GND #PWR077
U 1 1 55C92789
P 5200 2650
F 0 "#PWR077" H 5200 2650 30  0001 C CNN
F 1 "GND" H 5200 2580 30  0001 C CNN
F 2 "" H 5200 2650 60  0000 C CNN
F 3 "" H 5200 2650 60  0000 C CNN
	1    5200 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 2650 5200 2600
Wire Wire Line
	5200 2600 5250 2600
Wire Wire Line
	6850 2700 6850 2850
NoConn ~ 6150 2500
NoConn ~ 6150 2600
$EndSCHEMATC
