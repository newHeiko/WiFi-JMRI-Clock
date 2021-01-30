EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 5
Title "Wireless FREDI (and clock interface)"
Date "2017-11-19"
Rev "0.2"
Comp "Heiko Rosemann (heiko.rosemann@web.de) CC-BY-SA"
Comment1 ""
Comment2 "Use with 5V VCC only to provide reasonable termination"
Comment3 "Provides VCC power to Railsync"
Comment4 "Loconet Interface"
$EndDescr
$Comp
L wfred_loconet_clock-rescue:RJ12 J301
U 1 1 5920DD7A
P 7350 2600
F 0 "J301" H 7550 3100 60  0000 C CNN
F 1 "Molex 95501-6669" H 7200 3100 60  0000 C CNN
F 2 "myFootprints:Molex_95501-6669" H 7350 2600 60  0001 C CNN
F 3 "" H 7350 2600 60  0001 C CNN
F 4 "538-95501-6669" H 7350 2600 60  0001 C CNN "Mouser"
	1    7350 2600
	0    1    1    0   
$EndComp
$Comp
L wfred_loconet_clock-rescue:GND #PWR023
U 1 1 5920DF18
P 6800 3300
F 0 "#PWR023" H 6800 3300 30  0001 C CNN
F 1 "GND" H 6800 3230 30  0001 C CNN
F 2 "" H 6800 3300 60  0001 C CNN
F 3 "" H 6800 3300 60  0001 C CNN
	1    6800 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 2500 6800 2500
Wire Wire Line
	6800 2500 6800 2800
Wire Wire Line
	6900 2800 6800 2800
Connection ~ 6800 2800
Wire Wire Line
	7650 3100 7650 3200
Wire Wire Line
	6800 3200 7650 3200
Connection ~ 6800 3200
Wire Wire Line
	7750 3200 7750 3100
Connection ~ 7650 3200
Wire Wire Line
	6700 2400 6900 2400
Wire Wire Line
	6700 2900 6900 2900
Wire Wire Line
	6200 2900 6300 2900
Wire Wire Line
	6200 2200 6200 2400
Connection ~ 6200 2400
$Comp
L my_devices:BAR43 D303
U 1 1 5920E30D
P 6500 2900
F 0 "D303" H 6500 3000 40  0000 C CNN
F 1 "BAR43" H 6500 2800 40  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 6500 2900 60  0001 C CNN
F 3 "" H 6500 2900 60  0000 C CNN
F 4 "BAR 43 SMD" H 6500 2900 60  0001 C CNN "Reichelt"
	1    6500 2900
	1    0    0    -1  
$EndComp
$Comp
L my_devices:BAR43 D302
U 1 1 5920E364
P 6500 2400
F 0 "D302" H 6500 2500 40  0000 C CNN
F 1 "BAR43" H 6500 2300 40  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 6500 2400 60  0001 C CNN
F 3 "" H 6500 2400 60  0000 C CNN
F 4 "BAR 43 SMD" H 6500 2400 60  0001 C CNN "Reichelt"
	1    6500 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 2600 5900 2600
Wire Wire Line
	6900 2700 6700 2700
Wire Wire Line
	6700 2700 6700 2600
Connection ~ 6700 2600
$Comp
L wfred_loconet_clock-rescue:R R307
U 1 1 5920E41D
P 5900 4150
F 0 "R307" V 5980 4150 40  0000 C CNN
F 1 "12R" V 5907 4151 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 5830 4150 30  0001 C CNN
F 3 "" H 5900 4150 30  0000 C CNN
F 4 "603-RC0805JR-0712RL" V 5900 4150 60  0001 C CNN "Mouser"
	1    5900 4150
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:R R308
U 1 1 5920E524
P 5900 4850
F 0 "R308" V 5980 4850 40  0000 C CNN
F 1 "12R" V 5907 4851 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 5830 4850 30  0001 C CNN
F 3 "" H 5900 4850 30  0000 C CNN
F 4 "603-RC0805JR-0712RL" V 5900 4850 60  0001 C CNN "Mouser"
	1    5900 4850
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:C C302
U 1 1 5920E582
P 6200 4800
F 0 "C302" H 6200 4900 40  0000 L CNN
F 1 "47p" H 6206 4715 40  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6238 4650 30  0001 C CNN
F 3 "" H 6200 4800 60  0000 C CNN
F 4 "NPO-G0805 47P" H 6200 4800 60  0001 C CNN "Reichelt"
	1    6200 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 4400 5900 4500
Wire Wire Line
	5900 4500 6200 4500
Wire Wire Line
	6200 4500 6200 4600
Connection ~ 5900 4500
$Comp
L wfred_loconet_clock-rescue:GND #PWR024
U 1 1 5920E62F
P 6200 5100
F 0 "#PWR024" H 6200 5100 30  0001 C CNN
F 1 "GND" H 6200 5030 30  0001 C CNN
F 2 "" H 6200 5100 60  0001 C CNN
F 3 "" H 6200 5100 60  0001 C CNN
	1    6200 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 5000 6200 5100
$Comp
L wfred_loconet_clock-rescue:BC817-40 Q303
U 1 1 5920E776
P 5800 5700
F 0 "Q303" H 5800 5551 40  0000 R CNN
F 1 "BC817-40" H 5800 5850 40  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 5600 5802 29  0001 C CNN
F 3 "" H 5800 5700 60  0000 C CNN
F 4 "BC 817-40 SMD" H 5800 5700 60  0001 C CNN "Reichelt"
	1    5800 5700
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:GND #PWR025
U 1 1 5920E813
P 5900 6300
F 0 "#PWR025" H 5900 6300 30  0001 C CNN
F 1 "GND" H 5900 6230 30  0001 C CNN
F 2 "" H 5900 6300 60  0001 C CNN
F 3 "" H 5900 6300 60  0001 C CNN
	1    5900 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 6300 5900 5900
Wire Wire Line
	5900 5100 5900 5200
$Comp
L wfred_loconet_clock-rescue:R R306
U 1 1 5920E9B7
P 5550 5200
F 0 "R306" V 5630 5200 40  0000 C CNN
F 1 "82k" V 5557 5201 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 5480 5200 30  0001 C CNN
F 3 "" H 5550 5200 30  0000 C CNN
F 4 "603-RC0805JR-0782KL" V 5550 5200 60  0001 C CNN "Mouser"
	1    5550 5200
	0    1    1    0   
$EndComp
$Comp
L wfred_loconet_clock-rescue:R R304
U 1 1 5920EA5E
P 4850 5200
F 0 "R304" V 4930 5200 40  0000 C CNN
F 1 "47k" V 4857 5201 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 4780 5200 30  0001 C CNN
F 3 "" H 4850 5200 30  0000 C CNN
F 4 "603-RC0805JR-0747KL" V 4850 5200 60  0001 C CNN "Mouser"
	1    4850 5200
	0    1    1    0   
$EndComp
$Comp
L wfred_loconet_clock-rescue:R R305
U 1 1 5920EAAC
P 5150 5700
F 0 "R305" V 5230 5700 40  0000 C CNN
F 1 "2k2" V 5157 5701 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 5080 5700 30  0001 C CNN
F 3 "" H 5150 5700 30  0000 C CNN
F 4 "603-RC0805JR-072K2L" V 5150 5700 60  0001 C CNN "Mouser"
	1    5150 5700
	0    1    1    0   
$EndComp
$Comp
L wfred_loconet_clock-rescue:R R302
U 1 1 5920EB49
P 4200 5550
F 0 "R302" V 4280 5550 40  0000 C CNN
F 1 "47k" V 4207 5551 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 4130 5550 30  0001 C CNN
F 3 "" H 4200 5550 30  0000 C CNN
F 4 "603-RC0805JR-0747KL" V 4200 5550 60  0001 C CNN "Mouser"
	1    4200 5550
	-1   0    0    1   
$EndComp
$Comp
L wfred_loconet_clock-rescue:R R310
U 1 1 5920EBE7
P 7650 5650
F 0 "R310" V 7730 5650 40  0000 C CNN
F 1 "47k" V 7657 5651 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 7580 5650 30  0001 C CNN
F 3 "" H 7650 5650 30  0000 C CNN
F 4 "603-RC0805JR-0747KL" V 7650 5650 60  0001 C CNN "Mouser"
	1    7650 5650
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:R R309
U 1 1 5920ECD0
P 7650 4850
F 0 "R309" V 7730 4850 40  0000 C CNN
F 1 "100k" V 7657 4851 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 7580 4850 30  0001 C CNN
F 3 "" H 7650 4850 30  0000 C CNN
F 4 "603-RC0805JR-07100KL" V 7650 4850 60  0001 C CNN "Mouser"
	1    7650 4850
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:C C303
U 1 1 5920ED58
P 7950 5650
F 0 "C303" H 7950 5750 40  0000 L CNN
F 1 "100n" H 7956 5565 40  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7988 5500 30  0001 C CNN
F 3 "" H 7950 5650 60  0000 C CNN
F 4 "X7R-G0805 100N" H 7950 5650 60  0001 C CNN "Reichelt"
	1    7950 5650
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:C C301
U 1 1 5920EDE7
P 5150 6000
F 0 "C301" H 5150 6100 40  0000 L CNN
F 1 "1n" H 5156 5915 40  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5188 5850 30  0001 C CNN
F 3 "" H 5150 6000 60  0000 C CNN
F 4 "NPO-G0805 1,0N" H 5150 6000 60  0001 C CNN "Reichelt"
	1    5150 6000
	0    1    1    0   
$EndComp
$Comp
L my_devices:BAR43 D301
U 1 1 5920EE82
P 5200 4900
F 0 "D301" H 5200 5000 40  0000 C CNN
F 1 "BAR43" H 5200 4800 40  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 5200 4900 60  0001 C CNN
F 3 "" H 5200 4900 60  0000 C CNN
F 4 "BAR 43 SMD" H 5200 4900 60  0001 C CNN "Reichelt"
	1    5200 4900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5800 5200 5900 5200
Connection ~ 5900 5200
Wire Wire Line
	5100 5200 5200 5200
Wire Wire Line
	5200 5100 5200 5200
Connection ~ 5200 5200
Wire Wire Line
	3900 5200 4200 5200
Wire Wire Line
	4200 5200 4200 5300
Wire Wire Line
	5400 5700 5500 5700
Wire Wire Line
	5350 6000 5500 6000
Wire Wire Line
	5500 6000 5500 5700
Connection ~ 5500 5700
Wire Wire Line
	4700 5700 4800 5700
Wire Wire Line
	4950 6000 4800 6000
Wire Wire Line
	4800 6000 4800 5700
Connection ~ 4800 5700
Text GLabel 4700 5700 0    60   Input ~ 0
LN_TX
$Comp
L wfred_loconet_clock-rescue:GND #PWR026
U 1 1 5920F1D7
P 4200 6300
F 0 "#PWR026" H 4200 6300 30  0001 C CNN
F 1 "GND" H 4200 6230 30  0001 C CNN
F 2 "" H 4200 6300 60  0001 C CNN
F 3 "" H 4200 6300 60  0001 C CNN
	1    4200 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 5800 4200 6300
$Comp
L power:VCC #PWR027
U 1 1 5920F266
P 5200 4500
F 0 "#PWR027" H 5200 4600 30  0001 C CNN
F 1 "VCC" H 5200 4600 30  0000 C CNN
F 2 "" H 5200 4500 60  0001 C CNN
F 3 "" H 5200 4500 60  0001 C CNN
	1    5200 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 4500 5200 4700
Connection ~ 4200 5200
Text GLabel 3900 5200 0    60   Output ~ 0
LN_RX0
$Comp
L wfred_loconet_clock-rescue:GND #PWR028
U 1 1 5920F3BF
P 7650 6300
F 0 "#PWR028" H 7650 6300 30  0001 C CNN
F 1 "GND" H 7650 6230 30  0001 C CNN
F 2 "" H 7650 6300 60  0001 C CNN
F 3 "" H 7650 6300 60  0001 C CNN
	1    7650 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 5900 7650 6000
Wire Wire Line
	7950 5850 7950 6000
Wire Wire Line
	7950 6000 7650 6000
Connection ~ 7650 6000
Wire Wire Line
	7650 5100 7650 5200
Wire Wire Line
	7950 5200 7950 5450
Wire Wire Line
	7150 5200 7650 5200
Connection ~ 7650 5200
$Comp
L power:VCC #PWR029
U 1 1 5920F511
P 7650 4500
F 0 "#PWR029" H 7650 4600 30  0001 C CNN
F 1 "VCC" H 7650 4600 30  0000 C CNN
F 2 "" H 7650 4500 60  0001 C CNN
F 3 "" H 7650 4500 60  0001 C CNN
	1    7650 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 4500 7650 4600
Text GLabel 7150 5200 0    60   Output ~ 0
LN_RX1
$Comp
L wfred_loconet_clock-rescue:BC817-40 Q302
U 1 1 5920FF12
P 4600 2200
F 0 "Q302" H 4600 2051 40  0000 R CNN
F 1 "BC817-40" H 4600 2350 40  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 4400 2302 29  0001 C CNN
F 3 "" H 4600 2200 60  0000 C CNN
F 4 "BC 817-40 SMD" H 4600 2200 60  0001 C CNN "Reichelt"
	1    4600 2200
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:BC817-40 Q301
U 1 1 59210062
P 4300 2600
F 0 "Q301" H 4300 2451 40  0000 R CNN
F 1 "BC817-40" H 4300 2750 40  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 4100 2702 29  0001 C CNN
F 3 "" H 4300 2600 60  0000 C CNN
F 4 "BC 817-40 SMD" H 4300 2600 60  0001 C CNN "Reichelt"
	1    4300 2600
	-1   0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:R R301
U 1 1 59210145
P 4200 1850
F 0 "R301" V 4280 1850 40  0000 C CNN
F 1 "4k7" V 4207 1851 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 4130 1850 30  0001 C CNN
F 3 "" H 4200 1850 30  0000 C CNN
F 4 "603-RC0805JR-074K7L" V 4200 1850 60  0001 C CNN "Mouser"
	1    4200 1850
	-1   0    0    1   
$EndComp
$Comp
L wfred_loconet_clock-rescue:R R303
U 1 1 5921022B
P 4700 2950
F 0 "R303" V 4780 2950 40  0000 C CNN
F 1 "47R" V 4707 2951 40  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 4630 2950 30  0001 C CNN
F 3 "" H 4700 2950 30  0000 C CNN
F 4 "603-RC0805JR-0747RL" V 4700 2950 60  0001 C CNN "Mouser"
	1    4700 2950
	-1   0    0    1   
$EndComp
$Comp
L wfred_loconet_clock-rescue:CONN_3 K301
U 1 1 59210360
P 3750 1300
F 0 "K301" V 3700 1300 50  0000 C CNN
F 1 "TERMINATION" V 3800 1300 40  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 3750 1300 60  0001 C CNN
F 3 "" H 3750 1300 60  0001 C CNN
F 4 "RND 205-00631" V 3750 1300 60  0001 C CNN "Reichelt"
	1    3750 1300
	-1   0    0    1   
$EndComp
Wire Wire Line
	4100 1400 4200 1400
Wire Wire Line
	4200 1400 4200 1600
Wire Wire Line
	4200 1300 4100 1300
Wire Wire Line
	4700 1400 4700 2000
Connection ~ 4200 1400
Wire Wire Line
	4200 2100 4200 2200
Wire Wire Line
	4400 2200 4200 2200
Connection ~ 4200 2200
Wire Wire Line
	4700 2400 4700 2600
Wire Wire Line
	4500 2600 4700 2600
Connection ~ 4700 2600
Wire Wire Line
	4200 2800 4200 3300
Wire Wire Line
	4200 3300 4700 3300
Wire Wire Line
	4700 3300 4700 3200
Wire Wire Line
	5400 3300 5400 2600
Connection ~ 4700 3300
Text GLabel 5900 2700 3    60   BiDi ~ 0
LN_RXTX
Text GLabel 5900 3800 1    60   BiDi ~ 0
LN_RXTX
Wire Wire Line
	5900 2600 5900 2700
Connection ~ 5900 2600
Wire Wire Line
	5900 3800 5900 3900
Wire Wire Line
	4100 1200 4200 1200
Wire Wire Line
	4200 1000 4200 1200
$Comp
L my_devices:TC1240 U301
U 1 1 5A1AB828
P 2300 2400
F 0 "U301" H 2300 2400 60  0000 C CNN
F 1 "TC1240" H 2300 2300 60  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-6_Handsoldering" H 2300 2400 60  0001 C CNN
F 3 "" H 2300 2400 60  0001 C CNN
F 4 "579-TC1240ECHTR" H 2300 2400 60  0001 C CNN "Mouser"
	1    2300 2400
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:C C304
U 1 1 5A1ABA44
P 1300 2550
F 0 "C304" H 1300 2650 40  0000 L CNN
F 1 "22u" H 1306 2465 40  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 1338 2400 30  0001 C CNN
F 3 "" H 1300 2550 60  0000 C CNN
F 4 "X5R-G0805 22/25" H 1300 2550 60  0001 C CNN "Reichelt"
	1    1300 2550
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:C C305
U 1 1 5A1ABAB1
P 3200 2400
F 0 "C305" H 3200 2500 40  0000 L CNN
F 1 "22u" H 3206 2315 40  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3238 2250 30  0001 C CNN
F 3 "" H 3200 2400 60  0000 C CNN
F 4 "X5R-G0805 22/25" H 3200 2400 60  0001 C CNN "Reichelt"
	1    3200 2400
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:GND #PWR030
U 1 1 5A1ABBB5
P 2300 3200
F 0 "#PWR030" H 2300 3200 30  0001 C CNN
F 1 "GND" H 2300 3130 30  0001 C CNN
F 2 "" H 2300 3200 60  0001 C CNN
F 3 "" H 2300 3200 60  0001 C CNN
	1    2300 3200
	1    0    0    -1  
$EndComp
$Comp
L wfred_loconet_clock-rescue:GND #PWR031
U 1 1 5A1ABBFB
P 3200 3200
F 0 "#PWR031" H 3200 3200 30  0001 C CNN
F 1 "GND" H 3200 3130 30  0001 C CNN
F 2 "" H 3200 3200 60  0001 C CNN
F 3 "" H 3200 3200 60  0001 C CNN
	1    3200 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 2700 3200 2700
Wire Wire Line
	3200 2600 3200 2700
Connection ~ 3200 2700
Wire Wire Line
	2300 3000 2300 3200
Wire Wire Line
	1300 2750 1300 2850
Wire Wire Line
	1300 2850 1500 2850
Wire Wire Line
	1500 2850 1500 2700
Wire Wire Line
	1500 2700 1600 2700
Wire Wire Line
	1300 2350 1300 2250
Wire Wire Line
	1300 2250 1500 2250
Wire Wire Line
	1500 2250 1500 2400
Wire Wire Line
	1500 2400 1600 2400
$Comp
L power:VCC #PWR032
U 1 1 5A1ABEC8
P 1400 1950
F 0 "#PWR032" H 1400 2050 30  0001 C CNN
F 1 "VCC" H 1400 2050 30  0000 C CNN
F 2 "" H 1400 1950 60  0001 C CNN
F 3 "" H 1400 1950 60  0001 C CNN
	1    1400 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 1950 1400 2100
Wire Wire Line
	1400 2100 1600 2100
$Comp
L power:+5V #PWR033
U 1 1 5A1AC01A
P 3200 1950
F 0 "#PWR033" H 3200 2040 20  0001 C CNN
F 1 "+5V" H 3200 2040 30  0000 C CNN
F 2 "" H 3200 1950 60  0001 C CNN
F 3 "" H 3200 1950 60  0001 C CNN
	1    3200 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 1950 3200 2100
Wire Wire Line
	3000 2100 3200 2100
Connection ~ 3200 2100
$Comp
L power:+5V #PWR034
U 1 1 5A1AC27E
P 4200 1000
F 0 "#PWR034" H 4200 1090 20  0001 C CNN
F 1 "+5V" H 4200 1090 30  0000 C CNN
F 2 "" H 4200 1000 60  0001 C CNN
F 3 "" H 4200 1000 60  0001 C CNN
	1    4200 1000
	1    0    0    -1  
$EndComp
Connection ~ 4200 1200
$Comp
L power:+5V #PWR035
U 1 1 5A1AC467
P 6200 2200
F 0 "#PWR035" H 6200 2290 20  0001 C CNN
F 1 "+5V" H 6200 2290 30  0000 C CNN
F 2 "" H 6200 2200 60  0001 C CNN
F 3 "" H 6200 2200 60  0001 C CNN
	1    6200 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 2400 6300 2400
Wire Wire Line
	6800 2800 6800 3200
Wire Wire Line
	6800 3200 6800 3300
Wire Wire Line
	7650 3200 7750 3200
Wire Wire Line
	6200 2400 6200 2900
Wire Wire Line
	6700 2600 6900 2600
Wire Wire Line
	5900 4500 5900 4600
Wire Wire Line
	5900 5200 5900 5500
Wire Wire Line
	5200 5200 5300 5200
Wire Wire Line
	5500 5700 5600 5700
Wire Wire Line
	4800 5700 4900 5700
Wire Wire Line
	4200 5200 4600 5200
Wire Wire Line
	7650 6000 7650 6300
Wire Wire Line
	7650 5200 7650 5400
Wire Wire Line
	7650 5200 7950 5200
Wire Wire Line
	4200 1400 4700 1400
Wire Wire Line
	4200 2200 4200 2400
Wire Wire Line
	4700 2600 4700 2700
Wire Wire Line
	4700 3300 5400 3300
Wire Wire Line
	5900 2600 6700 2600
Wire Wire Line
	3200 2700 3200 3200
Wire Wire Line
	3200 2100 3200 2200
Wire Wire Line
	4200 1200 4200 1300
$EndSCHEMATC
