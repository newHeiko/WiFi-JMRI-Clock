EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 5
Title "Wireless FREDI (and clock interface)"
Date "2017-11-19"
Rev "0.2"
Comp "Heiko Rosemann (heiko.rosemann@web.de) CC-BY-SA"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "Clock interface"
$EndDescr
Text GLabel 3900 2700 0    60   Input ~ 0
CLOCK1
Text GLabel 3900 3500 0    60   Input ~ 0
CLOCK2
$Comp
L wfred_loconet_clock-rescue:R R501
U 1 1 5A1291DD
P 4450 2700
F 0 "R501" V 4530 2700 40  0000 C CNN
F 1 "220" V 4457 2701 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 4380 2700 30  0001 C CNN
F 3 "" H 4450 2700 30  0000 C CNN
F 4 "603-RC0805JR-07220RL" V 4450 2700 60  0001 C CNN "Mouser"
	1    4450 2700
	0    1    1    0   
$EndComp
$Comp
L wfred_loconet_clock-rescue:R R502
U 1 1 5A12924F
P 4450 3500
F 0 "R502" V 4530 3500 40  0000 C CNN
F 1 "220" V 4457 3501 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 4380 3500 30  0001 C CNN
F 3 "" H 4450 3500 30  0000 C CNN
F 4 "603-RC0805JR-07220RL" V 4450 3500 60  0001 C CNN "Mouser"
	1    4450 3500
	0    1    1    0   
$EndComp
$Comp
L wfred_loconet_clock-rescue:LED D501
U 1 1 5A12928F
P 5000 3100
F 0 "D501" H 5000 3200 50  0000 C CNN
F 1 "red" H 5000 3000 50  0000 C CNN
F 2 "myFootprints:LED_D3.0mm" H 5000 3100 60  0001 C CNN
F 3 "" H 5000 3100 60  0000 C CNN
F 4 "LED 3MM RT" H 5000 3100 60  0001 C CNN "Reichelt"
	1    5000 3100
	0    1    1    0   
$EndComp
$Comp
L wfred_loconet_clock-rescue:LED D502
U 1 1 5A129325
P 5300 3100
F 0 "D502" H 5300 3200 50  0000 C CNN
F 1 "red" H 5300 3000 50  0000 C CNN
F 2 "myFootprints:LED_D3.0mm" H 5300 3100 60  0001 C CNN
F 3 "" H 5300 3100 60  0000 C CNN
F 4 "LED 3MM RT" H 5300 3100 60  0001 C CNN "Reichelt"
	1    5300 3100
	0    -1   -1   0   
$EndComp
$Comp
L wfred_loconet_clock-rescue:CONN_2 P501
U 1 1 5A12938D
P 6150 3100
F 0 "P501" V 6100 3100 40  0000 C CNN
F 1 "CLOCK" V 6200 3100 40  0000 C CNN
F 2 "myFootprints:AKL183-02" H 6150 3100 60  0001 C CNN
F 3 "" H 6150 3100 60  0001 C CNN
F 4 "AKL182-02 + AKL169-02" V 6150 3100 60  0001 C CNN "Reichelt"
	1    6150 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 2700 4200 2700
Wire Wire Line
	4700 2700 5000 2700
Wire Wire Line
	5600 2700 5600 3000
Wire Wire Line
	5600 3000 5800 3000
Wire Wire Line
	5300 2900 5300 2700
Connection ~ 5300 2700
Wire Wire Line
	5000 2900 5000 2700
Connection ~ 5000 2700
Wire Wire Line
	3900 3500 4200 3500
Wire Wire Line
	4700 3500 5000 3500
Wire Wire Line
	5600 3500 5600 3200
Wire Wire Line
	5600 3200 5800 3200
Wire Wire Line
	5300 3300 5300 3500
Connection ~ 5300 3500
Wire Wire Line
	5000 3300 5000 3500
Connection ~ 5000 3500
$Comp
L wfred_loconet_clock-rescue:SWITCH_INV SW501
U 1 1 5A15342E
P 4500 4100
F 0 "SW501" H 4300 4250 50  0000 C CNN
F 1 "SWITCH_INV" H 4350 3950 50  0000 C CNN
F 2 "myFootprints:OS102011MS2Q" H 4500 4100 60  0001 C CNN
F 3 "" H 4500 4100 60  0000 C CNN
F 4 "611-OS102011MS2QN1" H 4500 4100 60  0001 C CNN "Mouser"
	1    4500 4100
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:SWITCH_INV SW502
U 1 1 5A15348B
P 4500 4600
F 0 "SW502" H 4300 4750 50  0000 C CNN
F 1 "SWITCH_INV" H 4350 4450 50  0000 C CNN
F 2 "myFootprints:OS102011MS2Q" H 4500 4600 60  0001 C CNN
F 3 "" H 4500 4600 60  0000 C CNN
F 4 "611-OS102011MS2QN1" H 4500 4600 60  0001 C CNN "Mouser"
	1    4500 4600
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:SWITCH_INV SW503
U 1 1 5A153526
P 4500 5100
F 0 "SW503" H 4300 5250 50  0000 C CNN
F 1 "SWITCH_INV" H 4350 4950 50  0000 C CNN
F 2 "myFootprints:OS102011MS2Q" H 4500 5100 60  0001 C CNN
F 3 "" H 4500 5100 60  0000 C CNN
F 4 "611-OS102011MS2QN1" H 4500 5100 60  0001 C CNN "Mouser"
	1    4500 5100
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:SWITCH_INV SW504
U 1 1 5A1535A1
P 4500 5600
F 0 "SW504" H 4300 5750 50  0000 C CNN
F 1 "SWITCH_INV" H 4350 5450 50  0000 C CNN
F 2 "myFootprints:OS102011MS2Q" H 4500 5600 60  0001 C CNN
F 3 "" H 4500 5600 60  0000 C CNN
F 4 "611-OS102011MS2QN1" H 4500 5600 60  0001 C CNN "Mouser"
	1    4500 5600
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:GND #PWR045
U 1 1 5A15360C
P 3800 6100
F 0 "#PWR045" H 3800 6100 30  0001 C CNN
F 1 "GND" H 3800 6030 30  0001 C CNN
F 2 "" H 3800 6100 60  0001 C CNN
F 3 "" H 3800 6100 60  0001 C CNN
	1    3800 6100
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:GND #PWR046
U 1 1 5A15363B
P 5200 6100
F 0 "#PWR046" H 5200 6100 30  0001 C CNN
F 1 "GND" H 5200 6030 30  0001 C CNN
F 2 "" H 5200 6100 60  0001 C CNN
F 3 "" H 5200 6100 60  0001 C CNN
	1    5200 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 6100 5200 5500
Wire Wire Line
	5200 4000 5000 4000
Wire Wire Line
	5000 4500 5200 4500
Connection ~ 5200 4500
Wire Wire Line
	5000 5000 5200 5000
Connection ~ 5200 5000
Wire Wire Line
	5000 5500 5200 5500
Connection ~ 5200 5500
Wire Wire Line
	3800 4100 3800 4600
Wire Wire Line
	3800 5600 4000 5600
Wire Wire Line
	3800 5100 4000 5100
Connection ~ 3800 5600
Wire Wire Line
	3800 4600 4000 4600
Connection ~ 3800 5100
Wire Wire Line
	3800 4100 4000 4100
Connection ~ 3800 4600
Text GLabel 5400 4200 2    60   Output ~ 0
LOCO1
Text GLabel 5400 4700 2    60   Output ~ 0
LOCO2
Text GLabel 5400 5200 2    60   Output ~ 0
LOCO3
Text GLabel 5350 5700 2    60   Output ~ 0
LOCO4
Wire Wire Line
	5400 4700 5000 4700
Wire Wire Line
	5000 4200 5400 4200
Wire Wire Line
	5400 5200 5000 5200
Wire Wire Line
	5000 5700 5350 5700
Wire Wire Line
	5300 2700 5600 2700
Wire Wire Line
	5000 2700 5300 2700
Wire Wire Line
	5300 3500 5600 3500
Wire Wire Line
	5000 3500 5300 3500
Wire Wire Line
	5200 4500 5200 4000
Wire Wire Line
	5200 5000 5200 4500
Wire Wire Line
	5200 5500 5200 5000
Wire Wire Line
	3800 5600 3800 6100
Wire Wire Line
	3800 5100 3800 5600
Wire Wire Line
	3800 4600 3800 5100
$EndSCHEMATC
