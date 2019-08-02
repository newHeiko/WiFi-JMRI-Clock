EESchema Schematic File Version 2
LIBS:wfred_loconet_clock-rescue
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
LIBS:my_devices
LIBS:ESP8266
LIBS:wfred_loconet_clock-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
Title "Wireless FREDI (and clock interface)"
Date "2017-11-19"
Rev "0.2"
Comp "Heiko Rosemann (heiko.rosemann@web.de) CC-BY-SA"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "WiFi controller and directly related peripheral stuff"
$EndDescr
$Comp
L ESP-12E U201
U 1 1 5A11AA62
P 4700 2600
F 0 "U201" H 4700 2500 50  0000 C CNN
F 1 "ESP-12E" H 4700 2700 50  0000 C CNN
F 2 "ESP8266:ESP-12E_SMD" H 4700 2600 50  0001 C CNN
F 3 "" H 4700 2600 50  0001 C CNN
	1    4700 2600
	1    0    0    -1  
$EndComp
NoConn ~ 4450 3500
NoConn ~ 4550 3500
NoConn ~ 4650 3500
NoConn ~ 4750 3500
NoConn ~ 4850 3500
NoConn ~ 4950 3500
$Comp
L VCC #PWR011
U 1 1 5A11E004
P 2800 1500
F 0 "#PWR011" H 2800 1600 30  0001 C CNN
F 1 "VCC" H 2800 1600 30  0000 C CNN
F 2 "" H 2800 1500 60  0001 C CNN
F 3 "" H 2800 1500 60  0001 C CNN
	1    2800 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 1500 2800 3000
Wire Wire Line
	2800 3000 3800 3000
Wire Wire Line
	3800 2500 2800 2500
Connection ~ 2800 2500
$Comp
L GND #PWR012
U 1 1 5A11E30D
P 7100 3700
F 0 "#PWR012" H 7100 3700 30  0001 C CNN
F 1 "GND" H 7100 3630 30  0001 C CNN
F 2 "" H 7100 3700 60  0001 C CNN
F 3 "" H 7100 3700 60  0001 C CNN
	1    7100 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 2500 7100 3700
Wire Wire Line
	5600 3000 7150 3000
Connection ~ 7100 3000
$Comp
L VCC #PWR013
U 1 1 5A11E4AA
P 6700 1550
F 0 "#PWR013" H 6700 1650 30  0001 C CNN
F 1 "VCC" H 6700 1650 30  0000 C CNN
F 2 "" H 6700 1550 60  0001 C CNN
F 3 "" H 6700 1550 60  0001 C CNN
	1    6700 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 2300 7150 2300
Wire Wire Line
	5600 2400 7150 2400
$Comp
L CONN_3 K201
U 1 1 5A11EE74
P 7500 2400
F 0 "K201" V 7450 2400 50  0000 C CNN
F 1 "UART_ESP" V 7550 2400 40  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 7500 2400 60  0001 C CNN
F 3 "" H 7500 2400 60  0001 C CNN
F 4 "RND 205-00631" V 7500 2400 60  0001 C CNN "Reichelt"
	1    7500 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 2500 7100 2500
$Comp
L CONN_2 P201
U 1 1 5A11F720
P 7500 2900
F 0 "P201" V 7450 2900 40  0000 C CNN
F 1 "ESP_BOOTLOAD" V 7550 2900 40  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 7500 2900 60  0001 C CNN
F 3 "" H 7500 2900 60  0001 C CNN
F 4 "RND 205-00631" V 7500 2900 60  0001 C CNN "Reichelt"
	1    7500 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 2700 7700 2700
$Comp
L R R201
U 1 1 5A11FF91
P 6500 1950
F 0 "R201" V 6580 1950 40  0000 C CNN
F 1 "10k" V 6507 1951 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 6430 1950 30  0001 C CNN
F 3 "" H 6500 1950 30  0000 C CNN
F 4 "603-RC0805JR-0710KL" V 6500 1950 60  0001 C CNN "Mouser"
	1    6500 1950
	-1   0    0    1   
$EndComp
$Comp
L R R202
U 1 1 5A11FFE4
P 6500 3350
F 0 "R202" V 6580 3350 40  0000 C CNN
F 1 "10k" V 6507 3351 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 6430 3350 30  0001 C CNN
F 3 "" H 6500 3350 30  0000 C CNN
F 4 "603-RC0805JR-0710KL" V 6500 3350 60  0001 C CNN "Mouser"
	1    6500 3350
	-1   0    0    1   
$EndComp
$Comp
L R R203
U 1 1 5A120047
P 6700 1950
F 0 "R203" V 6780 1950 40  0000 C CNN
F 1 "10k" V 6707 1951 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 6630 1950 30  0001 C CNN
F 3 "" H 6700 1950 30  0000 C CNN
F 4 "603-RC0805JR-0710KL" V 6700 1950 60  0001 C CNN "Mouser"
	1    6700 1950
	-1   0    0    1   
$EndComp
Wire Wire Line
	6700 2800 5600 2800
Wire Wire Line
	6500 2700 6500 2200
Connection ~ 6500 2700
Wire Wire Line
	6700 2200 6700 3200
Wire Wire Line
	6500 1700 6500 1600
Wire Wire Line
	6700 1550 6700 1700
Connection ~ 6700 1600
Wire Wire Line
	6500 2900 6500 3100
$Comp
L GND #PWR014
U 1 1 5A120E87
P 6500 3700
F 0 "#PWR014" H 6500 3700 30  0001 C CNN
F 1 "GND" H 6500 3630 30  0001 C CNN
F 2 "" H 6500 3700 60  0001 C CNN
F 3 "" H 6500 3700 60  0001 C CNN
	1    6500 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 3600 6500 3700
Wire Wire Line
	6500 1600 6700 1600
$Comp
L C C201
U 1 1 5A127BCA
P 1200 2400
F 0 "C201" H 1200 2500 40  0000 L CNN
F 1 "100n" H 1206 2315 40  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 1238 2250 30  0001 C CNN
F 3 "" H 1200 2400 60  0000 C CNN
F 4 "X7R-G0805 100N" H 1200 2400 60  0001 C CNN "Reichelt"
	1    1200 2400
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR015
U 1 1 5A127C0E
P 1200 1800
F 0 "#PWR015" H 1200 1900 30  0001 C CNN
F 1 "VCC" H 1200 1900 30  0000 C CNN
F 2 "" H 1200 1800 60  0001 C CNN
F 3 "" H 1200 1800 60  0001 C CNN
	1    1200 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 1800 1200 2200
$Comp
L GND #PWR016
U 1 1 5A127CDB
P 1200 2900
F 0 "#PWR016" H 1200 2900 30  0001 C CNN
F 1 "GND" H 1200 2830 30  0001 C CNN
F 2 "" H 1200 2900 60  0001 C CNN
F 3 "" H 1200 2900 60  0001 C CNN
	1    1200 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 2600 1200 2900
$Comp
L C C202
U 1 1 5A199174
P 1500 2400
F 0 "C202" H 1500 2500 40  0000 L CNN
F 1 "22u" H 1506 2315 40  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 1538 2250 30  0001 C CNN
F 3 "" H 1500 2400 60  0000 C CNN
F 4 "X5R-G0805 22/25" H 1500 2400 60  0001 C CNN "Reichelt"
	1    1500 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 2000 1800 2000
Wire Wire Line
	1500 2000 1500 2200
Connection ~ 1200 2000
Wire Wire Line
	1500 2600 1500 2800
Wire Wire Line
	1200 2800 1800 2800
Connection ~ 1200 2800
NoConn ~ 3800 2300
Text GLabel 7700 3200 2    60   Output ~ 0
STATUS_LED
Wire Wire Line
	3250 2700 3800 2700
Text GLabel 3250 2700 0    60   Output ~ 0
CLOCK2
Text GLabel 3350 2900 0    60   Input ~ 0
KEY_CONF
$Comp
L CAPAPOL C203
U 1 1 5C82A7FA
P 1800 2400
F 0 "C203" H 1850 2500 40  0000 L CNN
F 1 "100u" H 1850 2300 40  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 1900 2250 30  0000 C CNN
F 3 "" H 1800 2400 300 0000 C CNN
	1    1800 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 2000 1800 2200
Connection ~ 1500 2000
Wire Wire Line
	1800 2800 1800 2600
Connection ~ 1500 2800
$Comp
L +BATT #PWR017
U 1 1 5C82B48A
P 2600 1500
F 0 "#PWR017" H 2600 1450 20  0001 C CNN
F 1 "+BATT" H 2600 1600 30  0000 C CNN
F 2 "" H 2600 1500 60  0001 C CNN
F 3 "" H 2600 1500 60  0001 C CNN
	1    2600 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 2400 3800 2400
Text GLabel 3250 2800 0    60   Output ~ 0
CLOCK1
Wire Wire Line
	3350 2900 3800 2900
Wire Wire Line
	5600 2500 5800 2500
$Comp
L GND #PWR018
U 1 1 5C85530A
P 2600 3700
F 0 "#PWR018" H 2600 3700 30  0001 C CNN
F 1 "GND" H 2600 3630 30  0001 C CNN
F 2 "" H 2600 3700 60  0001 C CNN
F 3 "" H 2600 3700 60  0001 C CNN
	1    2600 3700
	1    0    0    -1  
$EndComp
$Comp
L R R204
U 1 1 5C85534D
P 2600 1950
F 0 "R204" V 2680 1950 40  0000 C CNN
F 1 "100k" V 2607 1951 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 2530 1950 30  0000 C CNN
F 3 "" H 2600 1950 30  0000 C CNN
	1    2600 1950
	1    0    0    -1  
$EndComp
$Comp
L R R205
U 1 1 5C855376
P 2600 3050
F 0 "R205" V 2680 3050 40  0000 C CNN
F 1 "47k" V 2607 3051 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 2530 3050 30  0000 C CNN
F 3 "" H 2600 3050 30  0000 C CNN
	1    2600 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 1500 2600 1700
Wire Wire Line
	2600 2200 2600 2800
Connection ~ 2600 2400
Wire Wire Line
	2600 3300 2600 3700
Wire Wire Line
	3800 2800 3250 2800
Text GLabel 3350 2600 0    60   Input ~ 0
KEY_HOUR
Wire Wire Line
	3350 2600 3800 2600
Text GLabel 7700 2700 2    60   Input ~ 0
KEY_MINUTE
Wire Wire Line
	7150 2800 6800 2800
Wire Wire Line
	6800 2800 6800 2700
Connection ~ 6800 2700
Wire Wire Line
	5800 2600 5600 2600
Wire Wire Line
	6700 3200 7700 3200
Connection ~ 6700 2800
Text GLabel 5800 2500 2    60   Output ~ 0
CLOCK3
Text GLabel 5800 2600 2    60   Output ~ 0
CLOCK4
Wire Wire Line
	6500 2900 5600 2900
$Comp
L R R206
U 1 1 5D44B97B
P 3600 1950
F 0 "R206" V 3680 1950 40  0000 C CNN
F 1 "10k" V 3607 1951 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 3530 1950 30  0001 C CNN
F 3 "" H 3600 1950 30  0000 C CNN
F 4 "603-RC0805JR-0710KL" V 3600 1950 60  0001 C CNN "Mouser"
	1    3600 1950
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR019
U 1 1 5D44BB12
P 3600 1500
F 0 "#PWR019" H 3600 1600 30  0001 C CNN
F 1 "VCC" H 3600 1600 30  0000 C CNN
F 2 "" H 3600 1500 60  0001 C CNN
F 3 "" H 3600 1500 60  0001 C CNN
	1    3600 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 1500 3600 1700
Wire Wire Line
	3600 2200 3600 2600
Connection ~ 3600 2600
$EndSCHEMATC
