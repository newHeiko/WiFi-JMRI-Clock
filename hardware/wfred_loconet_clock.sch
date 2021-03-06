EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
Title "Wireless FREDI (and clock interface)"
Date "2017-11-19"
Rev "0.2"
Comp "Heiko Rosemann (heiko.rosemann@web.de) CC-BY-SA"
Comment1 ""
Comment2 "External connections (Battery)"
Comment3 "Power supply"
Comment4 "Master sheet for subsheets"
$EndDescr
Wire Wire Line
	2150 2400 2150 2600
Connection ~ 2150 2600
Wire Wire Line
	2150 3100 2150 3300
Wire Wire Line
	2450 3300 2450 3100
Wire Wire Line
	4300 900  4300 1100
Wire Wire Line
	4300 1700 4300 1900
Wire Wire Line
	3600 2500 3300 2500
Wire Wire Line
	3300 2500 3300 2700
Wire Wire Line
	3600 2800 3500 2800
Wire Wire Line
	3500 2800 3500 3300
Wire Wire Line
	3300 3100 3300 3300
Wire Wire Line
	5200 2000 5200 2200
Wire Wire Line
	5000 2200 5200 2200
Wire Wire Line
	5200 2800 5000 2800
Connection ~ 5200 2200
Wire Wire Line
	6300 2200 6300 2400
Wire Wire Line
	6600 2200 6600 2400
Connection ~ 6300 2200
Wire Wire Line
	6300 2800 6300 3300
Wire Wire Line
	6600 2800 6600 3300
Wire Wire Line
	5500 2200 5500 3100
Connection ~ 5500 2200
$Comp
L wfred_loconet_clock-rescue:C C101
U 1 1 5920B38E
P 2150 2900
F 0 "C101" H 2150 3000 40  0000 L CNN
F 1 "22u/25V" H 2156 2815 40  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 2188 2750 30  0001 C CNN
F 3 "" H 2150 2900 60  0000 C CNN
F 4 "X5R-G0805 22/25" H 2150 2900 60  0001 C CNN "Reichelt"
	1    2150 2900
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:GND #PWR01
U 1 1 5920B390
P 2150 3300
F 0 "#PWR01" H 2150 3300 30  0001 C CNN
F 1 "GND" H 2150 3230 30  0001 C CNN
F 2 "" H 2150 3300 60  0001 C CNN
F 3 "" H 2150 3300 60  0001 C CNN
	1    2150 3300
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:GND #PWR02
U 1 1 5920B391
P 1850 3300
F 0 "#PWR02" H 1850 3300 30  0001 C CNN
F 1 "GND" H 1850 3230 30  0001 C CNN
F 2 "" H 1850 3300 60  0001 C CNN
F 3 "" H 1850 3300 60  0001 C CNN
	1    1850 3300
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:GND #PWR03
U 1 1 5920B392
P 2450 3300
F 0 "#PWR03" H 2450 3300 30  0001 C CNN
F 1 "GND" H 2450 3230 30  0001 C CNN
F 2 "" H 2450 3300 60  0001 C CNN
F 3 "" H 2450 3300 60  0001 C CNN
	1    2450 3300
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:+BATT #PWR04
U 1 1 5920B394
P 2150 2400
F 0 "#PWR04" H 2150 2350 20  0001 C CNN
F 1 "+BATT" H 2150 2500 30  0000 C CNN
F 2 "" H 2150 2400 60  0001 C CNN
F 3 "" H 2150 2400 60  0001 C CNN
	1    2150 2400
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:INDUCTOR L101
U 1 1 5920B395
P 4300 1400
F 0 "L101" V 4250 1400 40  0000 C CNN
F 1 "22u" V 4400 1400 40  0000 C CNN
F 2 "Inductors_SMD:L_Fastron_PISN_Handsoldering" H 4300 1400 60  0001 C CNN
F 3 "" H 4300 1400 60  0000 C CNN
F 4 "L-242418FPS 22µ" V 4300 1400 60  0001 C CNN "Reichelt"
	1    4300 1400
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:+BATT #PWR05
U 1 1 5920B396
P 4300 900
F 0 "#PWR05" H 4300 850 20  0001 C CNN
F 1 "+BATT" H 4300 1000 30  0000 C CNN
F 2 "" H 4300 900 60  0001 C CNN
F 3 "" H 4300 900 60  0001 C CNN
	1    4300 900 
	1    0    0    -1  
$EndComp
NoConn ~ 3600 2200
$Comp
L wfred_loconet_clock-rescue:C C103
U 1 1 5920B397
P 3300 2900
F 0 "C103" H 3300 3000 40  0000 L CNN
F 1 "100n" H 3306 2815 40  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3338 2750 30  0001 C CNN
F 3 "" H 3300 2900 60  0000 C CNN
F 4 "X7R-G0805 100N" H 3300 2900 60  0001 C CNN "Reichelt"
	1    3300 2900
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:GND #PWR06
U 1 1 5920B398
P 3300 3300
F 0 "#PWR06" H 3300 3300 30  0001 C CNN
F 1 "GND" H 3300 3230 30  0001 C CNN
F 2 "" H 3300 3300 60  0001 C CNN
F 3 "" H 3300 3300 60  0001 C CNN
	1    3300 3300
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:GND #PWR07
U 1 1 5920B399
P 3500 3300
F 0 "#PWR07" H 3500 3300 30  0001 C CNN
F 1 "GND" H 3500 3230 30  0001 C CNN
F 2 "" H 3500 3300 60  0001 C CNN
F 3 "" H 3500 3300 60  0001 C CNN
	1    3500 3300
	1    0    0    -1  
$EndComp
NoConn ~ 5000 2500
$Comp
L wfred_loconet_clock-rescue:VCC #PWR08
U 1 1 5920B39A
P 5200 2000
F 0 "#PWR08" H 5200 2100 30  0001 C CNN
F 1 "VCC" H 5200 2100 30  0000 C CNN
F 2 "" H 5200 2000 60  0001 C CNN
F 3 "" H 5200 2000 60  0001 C CNN
	1    5200 2000
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:GND #PWR09
U 1 1 5920B39D
P 6300 3300
F 0 "#PWR09" H 6300 3300 30  0001 C CNN
F 1 "GND" H 6300 3230 30  0001 C CNN
F 2 "" H 6300 3300 60  0001 C CNN
F 3 "" H 6300 3300 60  0001 C CNN
	1    6300 3300
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:GND #PWR010
U 1 1 5920B39E
P 6600 3300
F 0 "#PWR010" H 6600 3300 30  0001 C CNN
F 1 "GND" H 6600 3230 30  0001 C CNN
F 2 "" H 6600 3300 60  0001 C CNN
F 3 "" H 6600 3300 60  0001 C CNN
	1    6600 3300
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:C C102
U 1 1 59210DD0
P 2450 2900
F 0 "C102" H 2450 3000 40  0000 L CNN
F 1 "22u/25V" H 2456 2815 40  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 2488 2750 30  0001 C CNN
F 3 "" H 2450 2900 60  0000 C CNN
F 4 "X5R-G0805 22/25" H 2450 2900 60  0001 C CNN "Reichelt"
	1    2450 2900
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:C C104
U 1 1 59210E13
P 6300 2600
F 0 "C104" H 6300 2700 40  0000 L CNN
F 1 "22u/25V" H 6306 2515 40  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6338 2450 30  0001 C CNN
F 3 "" H 6300 2600 60  0000 C CNN
F 4 "X5R-G0805 22/25" H 6300 2600 60  0001 C CNN "Reichelt"
	1    6300 2600
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:C C105
U 1 1 59210E8B
P 6600 2600
F 0 "C105" H 6600 2700 40  0000 L CNN
F 1 "22u/25V" H 6606 2515 40  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6638 2450 30  0001 C CNN
F 3 "" H 6600 2600 60  0000 C CNN
F 4 "X5R-G0805 22/25" H 6600 2600 60  0001 C CNN "Reichelt"
	1    6600 2600
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:L6920D U101
U 1 1 5920B393
P 4300 2500
F 0 "U101" H 4300 2500 60  0000 C CNN
F 1 "L6920D" H 4300 2400 60  0000 C CNN
F 2 "Housings_SSOP:TSSOP-8_4.4x3mm_Pitch0.65mm" H 4300 2500 60  0001 C CNN
F 3 "" H 4300 2500 60  0001 C CNN
F 4 "L 6920 D" H 4300 2500 60  0001 C CNN "Reichelt"
F 5 "511-L6920DTR" H 4300 2500 60  0001 C CNN "Mouser"
	1    4300 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 3100 4300 3100
$Sheet
S 6100 4200 1200 900 
U 5A122619
F0 "Wifi connection" 60
F1 "wfred-wifi.sch" 60
$EndSheet
$Sheet
S 7500 4200 1200 900 
U 5A1290E1
F0 "clock-interface" 60
F1 "clockInterface.sch" 60
$EndSheet
Wire Wire Line
	2450 2600 2450 2700
Wire Wire Line
	1850 2800 1850 3300
$Comp
L wfred_loconet_clock-rescue:BATT_HOLDER_2 B101
U 1 1 5C82AFB3
P 1200 4300
F 0 "B101" H 1200 4400 60  0000 C CNN
F 1 "BATT_HOLDER_2" H 1200 4300 60  0000 C CNN
F 2 "myFootprints:NEW_AGE_COMPACT_AA_PCBEdges" H 1200 4300 60  0001 C CNN
F 3 "" H 1200 4300 60  0001 C CNN
	1    1200 4300
	0    1    1    0   
$EndComp
Wire Wire Line
	1500 3100 1500 2800
Wire Wire Line
	1500 2800 1850 2800
Wire Wire Line
	900  2600 2150 2600
Wire Wire Line
	900  2600 900  3100
Wire Wire Line
	2150 2600 2150 2700
Wire Wire Line
	2150 2600 2450 2600
Wire Wire Line
	5200 2200 5200 2800
Wire Wire Line
	5200 2200 5500 2200
Wire Wire Line
	6300 2200 6600 2200
Wire Wire Line
	5500 2200 6300 2200
$EndSCHEMATC
