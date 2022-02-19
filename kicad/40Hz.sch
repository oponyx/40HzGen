EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	8450 2100 8750 2100
Wire Wire Line
	8450 2200 8750 2200
Wire Wire Line
	8450 2300 8750 2300
Wire Wire Line
	8450 2400 8750 2400
Wire Wire Line
	8450 2500 8750 2500
Wire Wire Line
	8450 2600 8750 2600
Wire Wire Line
	8450 2700 8750 2700
Wire Wire Line
	8450 2800 8750 2800
Wire Wire Line
	8450 2900 8750 2900
Wire Wire Line
	8450 3000 8750 3000
Wire Wire Line
	8450 3100 8750 3100
Wire Wire Line
	7250 3200 6950 3200
Wire Wire Line
	7250 3100 6950 3100
Wire Wire Line
	7250 3000 6950 3000
Wire Wire Line
	7250 2900 6950 2900
Wire Wire Line
	7250 2800 6950 2800
Wire Wire Line
	7250 2700 6950 2700
Wire Wire Line
	7250 2500 6950 2500
Wire Wire Line
	7250 2300 6950 2300
Wire Wire Line
	7250 2100 6950 2100
Wire Wire Line
	1500 1900 1200 1900
Wire Wire Line
	1500 2200 1200 2200
Wire Wire Line
	1500 2300 1200 2300
Wire Wire Line
	2300 1800 2600 1800
Wire Wire Line
	2300 1900 2600 1900
Wire Wire Line
	2300 2000 2600 2000
Wire Wire Line
	2300 2100 2600 2100
Wire Wire Line
	2300 2200 2600 2200
Wire Wire Line
	2300 2300 2600 2300
Wire Wire Line
	2300 2400 2600 2400
Wire Wire Line
	2300 2500 2600 2500
Wire Wire Line
	2300 2600 2600 2600
Wire Wire Line
	7850 3400 7850 3550
Wire Wire Line
	1900 3100 1900 3250
$Comp
L MCU_Module:WeMos_D1_mini U2
U 1 1 61EF466A
P 1900 2300
F 0 "U2" H 1900 3693 50  0000 C CNN
F 1 "WeMos_D1_mini" H 2019 3497 50  0000 C CNN
F 2 "Module:WEMOS_D1_mini_light" H 1900 1150 50  0001 C CNN
F 3 "https://wiki.wemos.cc/products:d1:d1_mini#documentation" H 50  1150 50  0001 C CNN
	1    1900 2300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 61F05C36
P 1900 3250
F 0 "#PWR0101" H 1900 3000 50  0001 C CNN
F 1 "GND" H 1905 3077 50  0000 C CNN
F 2 "" H 1900 3250 50  0001 C CNN
F 3 "" H 1900 3250 50  0001 C CNN
	1    1900 3250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 61F064A0
P 7850 3550
F 0 "#PWR0102" H 7850 3300 50  0001 C CNN
F 1 "GND" H 7855 3377 50  0000 C CNN
F 2 "" H 7850 3550 50  0001 C CNN
F 3 "" H 7850 3550 50  0001 C CNN
	1    7850 3550
	1    0    0    -1  
$EndComp
Text Label 7000 2100 0    50   ~ 0
RST
Text Label 1250 1900 0    50   ~ 0
RST
Text Label 7000 2500 0    50   ~ 0
ADC
Text Label 2350 1800 0    50   ~ 0
ADC
Text Label 8500 2200 0    50   ~ 0
TXD
Text Label 1250 2300 0    50   ~ 0
TXD
Text Label 1250 2200 0    50   ~ 0
RXD
Text Label 8500 2400 0    50   ~ 0
RXD
Text Label 8500 3100 0    50   ~ 0
IO16
Text Label 2350 1900 0    50   ~ 0
IO16
Text Label 8500 2900 0    50   ~ 0
IO14
Text Label 2350 2400 0    50   ~ 0
IO14
Text Label 8500 2700 0    50   ~ 0
IO12
Text Label 2350 2500 0    50   ~ 0
IO12
Text Label 8500 2600 0    50   ~ 0
SCL
Text Label 2350 2000 0    50   ~ 0
SCL
Text Label 2350 2100 0    50   ~ 0
SDA
Text Label 8500 2500 0    50   ~ 0
SDA
Text Label 8500 2300 0    50   ~ 0
IO2
Text Label 2350 2300 0    50   ~ 0
IO2
Text Label 2350 2200 0    50   ~ 0
IO0
Text Label 8500 2100 0    50   ~ 0
IO0
Text Label 8500 2800 0    50   ~ 0
IO13
Text Label 2350 2600 0    50   ~ 0
IO13
Text Label 8500 3000 0    50   ~ 0
IO15
$Comp
L Transistor_FET:IRF540N Q1
U 1 1 61F11E81
P 4700 5750
F 0 "Q1" H 4904 5796 50  0000 L CNN
F 1 "IRF540N" H 4904 5705 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 4950 5675 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf540n.pdf" H 4700 5750 50  0001 L CNN
	1    4700 5750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 61F16928
P 4350 6000
F 0 "R3" H 4420 6046 50  0000 L CNN
F 1 "10k" H 4420 5955 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 4280 6000 50  0001 C CNN
F 3 "~" H 4350 6000 50  0001 C CNN
	1    4350 6000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 61F171EF
P 4300 5250
F 0 "R2" H 4370 5296 50  0000 L CNN
F 1 "220" H 4370 5205 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 4230 5250 50  0001 C CNN
F 3 "~" H 4300 5250 50  0001 C CNN
	1    4300 5250
	1    0    0    -1  
$EndComp
$Comp
L Isolator:PC817 U3
U 1 1 61F1F909
P 3950 5650
F 0 "U3" H 3950 5975 50  0000 C CNN
F 1 "PC817" H 3950 5884 50  0000 C CNN
F 2 "Package_DIP:DIP-4_W7.62mm" H 3750 5450 50  0001 L CIN
F 3 "http://www.soselectronic.cz/a_info/resource/d/pc817.pdf" H 3950 5650 50  0001 L CNN
	1    3950 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 5750 3600 5750
Wire Wire Line
	3600 5750 3600 5850
$Comp
L power:GND #PWR0103
U 1 1 61F2263F
P 3600 5850
F 0 "#PWR0103" H 3600 5600 50  0001 C CNN
F 1 "GND" H 3605 5677 50  0000 C CNN
F 2 "" H 3600 5850 50  0001 C CNN
F 3 "" H 3600 5850 50  0001 C CNN
	1    3600 5850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 61F229AF
P 3450 5550
F 0 "R1" V 3243 5550 50  0000 C CNN
F 1 "470" V 3334 5550 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3335 5550 50  0001 C CNN
F 3 "~" H 3450 5550 50  0001 C CNN
	1    3450 5550
	0    1    1    0   
$EndComp
Wire Wire Line
	3650 5550 3600 5550
Wire Wire Line
	4250 5550 4300 5550
Wire Wire Line
	4300 5400 4300 5550
Wire Wire Line
	4300 5100 4300 5050
Wire Wire Line
	4300 5050 4800 5050
Wire Wire Line
	4250 5750 4350 5750
Wire Wire Line
	4350 5850 4350 5750
Connection ~ 4350 5750
Wire Wire Line
	4350 5750 4500 5750
Wire Wire Line
	4350 6150 4350 6250
Wire Wire Line
	4350 6250 4800 6250
Wire Wire Line
	4800 6250 4800 5950
Connection ~ 4800 6250
$Comp
L Diode:BYV79-200 D1
U 1 1 61F3A6D4
P 4800 5250
F 0 "D1" V 4754 5330 50  0000 L CNN
F 1 "BYV79-200" V 4845 5330 50  0000 L CNN
F 2 "Diode_THT:D_DO-15_P5.08mm_Vertical_KathodeUp" H 4800 5075 50  0001 C CNN
F 3 "http://pdf.datasheetcatalog.com/datasheet/philips/BYV79-100.pdf" H 4800 5250 50  0001 C CNN
	1    4800 5250
	0    1    1    0   
$EndComp
Connection ~ 4800 5050
Wire Wire Line
	4800 5050 5400 5050
Wire Wire Line
	5400 5050 5400 5250
Text Label 5500 5450 0    50   ~ 0
+IN
Text Label 5500 5550 0    50   ~ 0
-IN
Text Label 5500 5250 0    50   ~ 0
+OUT
Text Label 5500 5350 0    50   ~ 0
-OUT
Wire Wire Line
	4800 6250 5400 6250
Wire Wire Line
	4800 5050 4800 5100
Wire Wire Line
	4800 5400 4800 5500
Wire Wire Line
	5750 5350 4900 5350
Wire Wire Line
	4900 5350 4900 5500
Wire Wire Line
	4900 5500 4800 5500
Connection ~ 4800 5500
Wire Wire Line
	4800 5500 4800 5550
$Comp
L Converter_ACDC:HLK-PM01 PS1
U 1 1 61F3A82E
P 9450 5250
F 0 "PS1" H 9450 5575 50  0000 C CNN
F 1 "HLK-PM01" H 9450 5484 50  0000 C CNN
F 2 "Converter_ACDC:Converter_ACDC_HiLink_HLK-PMxx" H 9450 4950 50  0001 C CNN
F 3 "http://www.hlktech.net/product_detail.php?ProId=54" H 9850 4900 50  0001 C CNN
	1    9450 5250
	1    0    0    -1  
$EndComp
Text Label 9900 5150 0    50   ~ 0
+5V
Text Label 9900 5350 0    50   ~ 0
GND
$Comp
L Connector:Conn_01x04_Male J3
U 1 1 61F698E3
P 7350 5100
F 0 "J3" H 7458 5381 50  0000 C CNN
F 1 "SSD1306" H 7458 5290 50  0000 C CNN
F 2 "Connector_JST:JST_EH_B4B-EH-A_1x04_P2.50mm_Vertical" H 7350 5100 50  0001 C CNN
F 3 "~" H 7350 5100 50  0001 C CNN
	1    7350 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 5000 7850 5000
Wire Wire Line
	7550 5100 7850 5100
Wire Wire Line
	7550 5200 7850 5200
Wire Wire Line
	7550 5300 7850 5300
Text Label 7650 5000 0    50   ~ 0
GND
Text Label 7650 5100 0    50   ~ 0
+5V
Text Label 7650 5200 0    50   ~ 0
SCL
Text Label 7650 5300 0    50   ~ 0
SDA
$Comp
L Switch:SW_Push SW1
U 1 1 61FC425F
P 2200 4950
F 0 "SW1" H 2200 5235 50  0000 C CNN
F 1 "OK" H 2200 5144 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H5mm" H 2200 5150 50  0001 C CNN
F 3 "~" H 2200 5150 50  0001 C CNN
	1    2200 4950
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW2
U 1 1 61FC478C
P 2200 5400
F 0 "SW2" H 2200 5685 50  0000 C CNN
F 1 "UP" H 2200 5594 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H5mm" H 2200 5600 50  0001 C CNN
F 3 "~" H 2200 5600 50  0001 C CNN
	1    2200 5400
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW3
U 1 1 61FC4FCE
P 2200 5900
F 0 "SW3" H 2200 6185 50  0000 C CNN
F 1 "DOWN" H 2200 6094 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H5mm" H 2200 6100 50  0001 C CNN
F 3 "~" H 2200 6100 50  0001 C CNN
	1    2200 5900
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW4
U 1 1 61FC575F
P 2200 6350
F 0 "SW4" H 2200 6635 50  0000 C CNN
F 1 "CANC" H 2200 6544 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H5mm" H 2200 6550 50  0001 C CNN
F 3 "~" H 2200 6550 50  0001 C CNN
	1    2200 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 4950 2600 4950
Wire Wire Line
	2600 6350 2400 6350
Wire Wire Line
	2400 5900 2600 5900
Connection ~ 2600 5900
Wire Wire Line
	2400 5400 2600 5400
$Comp
L power:GND #PWR0104
U 1 1 61FFAD2A
P 1800 6800
F 0 "#PWR0104" H 1800 6550 50  0001 C CNN
F 1 "GND" H 1805 6627 50  0000 C CNN
F 2 "" H 1800 6800 50  0001 C CNN
F 3 "" H 1800 6800 50  0001 C CNN
	1    1800 6800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 6203662C
P 1050 6550
F 0 "R4" H 1120 6596 50  0000 L CNN
F 1 "10k" H 1120 6505 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 980 6550 50  0001 C CNN
F 3 "~" H 1050 6550 50  0001 C CNN
	1    1050 6550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 6203F468
P 1300 6550
F 0 "R5" H 1370 6596 50  0000 L CNN
F 1 "10k" H 1370 6505 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 1230 6550 50  0001 C CNN
F 3 "~" H 1300 6550 50  0001 C CNN
	1    1300 6550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 62041FA2
P 1550 6550
F 0 "R6" H 1620 6596 50  0000 L CNN
F 1 "10k" H 1620 6505 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 1480 6550 50  0001 C CNN
F 3 "~" H 1550 6550 50  0001 C CNN
	1    1550 6550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 62044C7D
P 1800 6550
F 0 "R7" H 1870 6596 50  0000 L CNN
F 1 "10k" H 1870 6505 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 1730 6550 50  0001 C CNN
F 3 "~" H 1800 6550 50  0001 C CNN
	1    1800 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1050 6700 1050 6750
Wire Wire Line
	1050 6750 1300 6750
Wire Wire Line
	1800 6750 1800 6700
Wire Wire Line
	1550 6700 1550 6750
Connection ~ 1550 6750
Wire Wire Line
	1550 6750 1800 6750
Wire Wire Line
	1300 6700 1300 6750
Connection ~ 1300 6750
Wire Wire Line
	1800 6400 1800 6350
Wire Wire Line
	1800 6350 2000 6350
Wire Wire Line
	2000 5900 1550 5900
Wire Wire Line
	2000 4950 1050 4950
Text Label 1800 4950 0    50   ~ 0
IO14
Text Label 1800 5900 0    50   ~ 0
IO12
Text Label 1800 6350 0    50   ~ 0
IO13
$Comp
L Device:C C2
U 1 1 62067DD3
P 6200 1700
F 0 "C2" H 6250 1800 50  0000 L CNN
F 1 "100n" H 6250 1600 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 6238 1550 50  0001 C CNN
F 3 "~" H 6200 1700 50  0001 C CNN
	1    6200 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C3
U 1 1 6206851F
P 6550 1700
F 0 "C3" H 6668 1746 50  0000 L CNN
F 1 "10uF" H 6668 1655 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 6588 1550 50  0001 C CNN
F 3 "~" H 6550 1700 50  0001 C CNN
	1    6550 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 620698FC
P 9650 2450
F 0 "R8" V 9443 2450 50  0000 C CNN
F 1 "10k" V 9534 2450 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 9535 2450 50  0001 C CNN
F 3 "~" H 9650 2450 50  0001 C CNN
	1    9650 2450
	0    1    1    0   
$EndComp
$Comp
L Device:R R9
U 1 1 6206D8BA
P 9650 2750
F 0 "R9" V 9443 2750 50  0000 C CNN
F 1 "10k" V 9534 2750 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 9535 2750 50  0001 C CNN
F 3 "~" H 9650 2750 50  0001 C CNN
	1    9650 2750
	0    1    1    0   
$EndComp
$Comp
L Device:R R10
U 1 1 6207165C
P 9650 3050
F 0 "R10" V 9443 3050 50  0000 C CNN
F 1 "10k" V 9534 3050 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 9535 3050 50  0001 C CNN
F 3 "~" H 9650 3050 50  0001 C CNN
	1    9650 3050
	0    1    1    0   
$EndComp
$Comp
L Device:R R11
U 1 1 62075437
P 9650 3350
F 0 "R11" V 9443 3350 50  0000 C CNN
F 1 "10k" V 9534 3350 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 9535 3350 50  0001 C CNN
F 3 "~" H 9650 3350 50  0001 C CNN
	1    9650 3350
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR0105
U 1 1 62078DE2
P 2600 4750
F 0 "#PWR0105" H 2600 4600 50  0001 C CNN
F 1 "+3.3V" H 2615 4923 50  0000 C CNN
F 2 "" H 2600 4750 50  0001 C CNN
F 3 "" H 2600 4750 50  0001 C CNN
	1    2600 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 4750 2600 4950
$Comp
L power:+3.3V #PWR0106
U 1 1 620807CC
P 7850 1800
F 0 "#PWR0106" H 7850 1650 50  0001 C CNN
F 1 "+3.3V" H 7865 1973 50  0000 C CNN
F 2 "" H 7850 1800 50  0001 C CNN
F 3 "" H 7850 1800 50  0001 C CNN
	1    7850 1800
	1    0    0    -1  
$EndComp
$Comp
L RF_Module:ESP-12F U1
U 1 1 61EF2869
P 7850 2700
F 0 "U1" H 7850 4075 50  0000 C CNN
F 1 "ESP-12F" H 7850 3866 50  0000 C CNN
F 2 "RF_Module:ESP-12E" H 7850 2700 50  0001 C CNN
F 3 "http://wiki.ai-thinker.com/_media/esp8266/esp8266_series_modules_user_manual_v1.1.pdf" H 7500 2800 50  0001 C CNN
	1    7850 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 1800 7850 1900
$Comp
L power:+3.3V #PWR0107
U 1 1 6208C6F5
P 2000 1400
F 0 "#PWR0107" H 2000 1250 50  0001 C CNN
F 1 "+3.3V" H 2015 1573 50  0000 C CNN
F 2 "" H 2000 1400 50  0001 C CNN
F 3 "" H 2000 1400 50  0001 C CNN
	1    2000 1400
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0108
U 1 1 6209E392
P 1800 1400
F 0 "#PWR0108" H 1800 1250 50  0001 C CNN
F 1 "+5V" H 1815 1573 50  0000 C CNN
F 2 "" H 1800 1400 50  0001 C CNN
F 3 "" H 1800 1400 50  0001 C CNN
	1    1800 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 1400 1800 1500
Wire Wire Line
	2000 1400 2000 1500
$Comp
L Device:C C4
U 1 1 620A58F0
P 10000 3550
F 0 "C4" H 10115 3596 50  0000 L CNN
F 1 "100n" H 10115 3505 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 10038 3400 50  0001 C CNN
F 3 "~" H 10000 3550 50  0001 C CNN
	1    10000 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 1550 6200 1450
$Comp
L Device:C C1
U 1 1 620B6712
P 5300 1700
F 0 "C1" H 5415 1746 50  0000 L CNN
F 1 "100n" H 5415 1655 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 5338 1550 50  0001 C CNN
F 3 "~" H 5300 1700 50  0001 C CNN
	1    5300 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 3350 9300 3350
Wire Wire Line
	9300 3350 9300 3050
$Comp
L power:+3.3V #PWR0112
U 1 1 620CF5BD
P 9300 2300
F 0 "#PWR0112" H 9300 2150 50  0001 C CNN
F 1 "+3.3V" H 9315 2473 50  0000 C CNN
F 2 "" H 9300 2300 50  0001 C CNN
F 3 "" H 9300 2300 50  0001 C CNN
	1    9300 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 2450 9300 2450
Connection ~ 9300 2450
Wire Wire Line
	9300 2450 9300 2300
Wire Wire Line
	9500 2750 9300 2750
Connection ~ 9300 2750
Wire Wire Line
	9300 2750 9300 2450
Wire Wire Line
	9500 3050 9300 3050
Connection ~ 9300 3050
Wire Wire Line
	9300 3050 9300 2750
Wire Wire Line
	9800 3350 10000 3350
Wire Wire Line
	10000 3400 10000 3350
Connection ~ 10000 3350
Wire Wire Line
	10000 3700 10000 3800
$Comp
L power:GND #PWR0113
U 1 1 620EBE92
P 10000 3800
F 0 "#PWR0113" H 10000 3550 50  0001 C CNN
F 1 "GND" H 10005 3627 50  0000 C CNN
F 2 "" H 10000 3800 50  0001 C CNN
F 3 "" H 10000 3800 50  0001 C CNN
	1    10000 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9800 3050 10300 3050
Wire Wire Line
	9800 2750 10300 2750
Wire Wire Line
	9800 2450 10300 2450
Wire Wire Line
	10000 3350 10300 3350
Wire Wire Line
	2300 2700 2600 2700
Text Label 2350 2700 0    50   ~ 0
IO15
Wire Wire Line
	3300 5550 3050 5550
Text Label 3050 5550 0    50   ~ 0
IO15
Text Label 10100 3350 0    50   ~ 0
RST
Text Label 10100 3050 0    50   ~ 0
IO0
Text Label 7000 2300 0    50   ~ 0
EN
Text Label 10100 2750 0    50   ~ 0
EN
Connection ~ 2600 4950
Wire Wire Line
	2600 4950 2600 5400
Connection ~ 2600 5400
Wire Wire Line
	2600 5400 2600 5900
Wire Wire Line
	2600 5900 2600 6350
Wire Wire Line
	1550 5900 1550 6400
Wire Wire Line
	1050 4950 1050 6400
Wire Wire Line
	2000 5400 1300 5400
Wire Wire Line
	1300 5400 1300 6400
Text Label 1800 5400 0    50   ~ 0
IO16
Wire Wire Line
	1300 6750 1550 6750
Wire Wire Line
	1800 6750 1800 6800
Connection ~ 1800 6750
Text Label 10100 2450 0    50   ~ 0
IO2
$Comp
L Connector:Screw_Terminal_01x06 J1
U 1 1 6223608A
P 5950 5450
F 0 "J1" H 6030 5442 50  0000 L CNN
F 1 "Power" H 6030 5351 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-3-6-5.08_1x06_P5.08mm_Horizontal" H 5950 5450 50  0001 C CNN
F 3 "~" H 5950 5450 50  0001 C CNN
	1    5950 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 4850 5450 4850
Wire Wire Line
	5750 4950 5450 4950
Text Label 5500 4850 0    50   ~ 0
ACIN1
Text Label 5500 4950 0    50   ~ 0
ACIN2
Text Label 8550 5350 0    50   ~ 0
ACIN1
$Comp
L Device:Fuse_Small F1
U 1 1 62287233
P 8900 5150
F 0 "F1" H 8900 5335 50  0000 C CNN
F 1 "200mA" H 8900 5244 50  0000 C CNN
F 2 "Fuse:Fuse_1206_3216Metric" H 8900 5150 50  0001 C CNN
F 3 "~" H 8900 5150 50  0001 C CNN
	1    8900 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 5150 9000 5150
Wire Wire Line
	8550 5350 9050 5350
Wire Wire Line
	8800 5150 8550 5150
Text Label 8550 5150 0    50   ~ 0
ACIN2
$Comp
L Connector:Conn_01x02_Male J2
U 1 1 622AC1D8
P 10400 5200
F 0 "J2" H 10372 5082 50  0000 R CNN
F 1 "5V" H 10372 5173 50  0000 R CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-3-2-5.08_1x02_P5.08mm_Horizontal" H 10400 5200 50  0001 C CNN
F 3 "~" H 10400 5200 50  0001 C CNN
	1    10400 5200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10200 5150 10200 5200
Wire Wire Line
	9850 5150 10200 5150
Wire Wire Line
	10200 5300 10200 5350
Wire Wire Line
	9850 5350 10200 5350
$Comp
L Regulator_Linear:AMS1117-3.3 U4
U 1 1 622B7F9B
P 5750 1450
F 0 "U4" H 5750 1692 50  0000 C CNN
F 1 "AMS1117-3.3" H 5750 1601 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 5750 1650 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 5850 1200 50  0001 C CNN
	1    5750 1450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0109
U 1 1 622B9A74
P 5750 2100
F 0 "#PWR0109" H 5750 1850 50  0001 C CNN
F 1 "GND" H 5755 1927 50  0000 C CNN
F 2 "" H 5750 2100 50  0001 C CNN
F 3 "" H 5750 2100 50  0001 C CNN
	1    5750 2100
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0110
U 1 1 622E8A4B
P 5300 1400
F 0 "#PWR0110" H 5300 1250 50  0001 C CNN
F 1 "+5V" H 5315 1573 50  0000 C CNN
F 2 "" H 5300 1400 50  0001 C CNN
F 3 "" H 5300 1400 50  0001 C CNN
	1    5300 1400
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0111
U 1 1 622E92F1
P 6200 1400
F 0 "#PWR0111" H 6200 1250 50  0001 C CNN
F 1 "+3.3V" H 6215 1573 50  0000 C CNN
F 2 "" H 6200 1400 50  0001 C CNN
F 3 "" H 6200 1400 50  0001 C CNN
	1    6200 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 1450 6200 1450
Wire Wire Line
	6200 1450 6200 1400
Wire Wire Line
	5450 1450 5300 1450
Wire Wire Line
	5300 1450 5300 1400
Connection ~ 6200 1450
Wire Notes Line
	10850 900  10850 4150
Wire Notes Line
	4850 900  4850 4150
Wire Notes Line
	4850 900  10850 900 
Wire Notes Line
	4850 4150 10850 4150
Wire Wire Line
	5300 2000 5750 2000
Wire Wire Line
	6550 2000 6550 1850
Wire Wire Line
	6200 1850 6200 2000
Connection ~ 6200 2000
Wire Wire Line
	6200 2000 6550 2000
Wire Wire Line
	5750 1750 5750 2000
Connection ~ 5750 2000
Wire Wire Line
	5750 2000 6200 2000
Wire Wire Line
	5750 2100 5750 2000
Wire Wire Line
	6550 1550 6550 1450
Wire Wire Line
	6550 1450 6200 1450
Wire Wire Line
	5300 1550 5300 1450
Connection ~ 5300 1450
Wire Wire Line
	5300 1850 5300 2000
Wire Wire Line
	5400 5550 5400 6250
Wire Wire Line
	5400 5250 5400 5450
Wire Wire Line
	5400 5250 5750 5250
Connection ~ 5400 5250
Wire Wire Line
	5400 5450 5750 5450
Wire Wire Line
	5400 5550 5750 5550
Wire Wire Line
	5750 5650 5500 5650
Wire Wire Line
	5750 5750 5500 5750
Text Label 5550 5650 0    50   ~ 0
ACIN1
Text Label 5550 5750 0    50   ~ 0
ACIN2
$EndSCHEMATC
