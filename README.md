# mcuromread
Simple Arduino sketches to read the contents of the on-chip ROM in old microcontrollers.

Originally created for dumping the ROMs of the MCUs found in the Sony HIL-C1/HIL-1000 Hi-Vision Laserdisc players, but can be used to dump any standard ROM or MCU, which can be set to "PROM (read) mode".

How to use:

1. Load the sketch to the Arduino Mega.
2. Power off the Arduino (disconnect USB cable).
3. Connect the Arduino Mega to the MCU according to the instructions below.
4. Double check the connections.
5. Connect the Arduino to a PC.
6. Use a terminal program to save the serial output into a file (or copy/paste from the Serial Monitor)
e.g. use minicom:  
```minicom -s -C mcuromdump.log```  
Here, specify the serial port dev to use under 'Serial port setup' (/dev/ttyACMx), press Esc or choose Exit. Then press reset button on the Arduino Mega. After the dumping process is finished quit minicom (Ctrl+a then q).
7. Convert the text based log to a binary file, e.g.  
```grep : mcuromdump.log | cut -f2 -d: | xxd -r -p > mcuromdump.bin```

# Wiring for Hitachi H8/325 HD6473258P10 (SDIP64 package)

```
  HD6473258 pin - Arduino Mega pin
 ----------------------------------
 A00: 56 (P1-0) - D49
 A01: 55 (P1-1) - D48
 A02: 54 (P1-2) - D47
 A03: 53 (P1-3) - D46
 A04: 52 (P1-4) - D45
 A05: 51 (P1-5) - D44
 A06: 50 (P1-6) - D43
 A07: 49 (P1-7) - D42

 A08: 47 (P2-0) - D37
 A09: 13 (NMI_) - D36
 A10: 45 (P2-2) - D35
 A11: 44 (P2-3) - D34
 A12: 43 (P2-4) - D33
 A13: 42 (P2-5) - D32
 A14: 41 (P2-6) - D31

 D00: 17 (P4-0) - D22
 D01: 18 (P4-1) - D23
 D02: 19 (P4-2) - D24
 D03: 20 (P4-3) - D25
 D04: 21 (P4-4) - D26
 D05: 22 (P4-5) - D27
 D06: 23 (P4-6) - D28
 D07: 24 (P4-7) - D29

 CE_: 40 (P2-7) - GND
 OE_: 46 (P2-1) - GND

 Connect +5V to HD6473258 pins: 31 (P7-0), 32 (P7-1), 14 (Vcc), 39 (Vcc)
 Connect GND to HD6473258 pins: 12 (MD0), 11 (MD1), 15 (STBY_), 16 (Vss), 48 (Vss)
```

See page 205 of "Hitachi Single-Chip Microcomputer H8/325 ... Series Hardware Manual" for more info.

# Wiring for Mitsubishi M37451E8SP (SDIP64 package)

```
      M37451 pin - Arduino Mega pin
 ----------------------------------
 A00:  56 (P0-0) - D49
 A01:  55 (P0-1) - D48
 A02:  54 (P0-2) - D47
 A03:  53 (P0-3) - D46
 A04:  52 (P0-4) - D45
 A05:  51 (P0-5) - D44
 A06:  50 (P0-6) - D43
 A07:  49 (P0-7) - D42

 A08:  48 (P1-0) - D37
 A09:  47 (P1-1) - D36
 A10:  46 (P1-2) - D35
 A11:  45 (P1-3) - D34
 A12:  44 (P1-4) - D33
 A13:  43 (P1-5) - D32
 A14:  16 (P5-0) - D31
 D00:  40 (P2-0) - D22
 D01:  39 (P2-1) - D23
 D02:  38 (P2-2) - D24
 D03:  37 (P2-3) - D25
 D04:  36 (P2-4) - D26
 D05:  35 (P2-5) - D27
 D06:  34 (P2-6) - D28
 D07:  33 (P2-7) - D29

 Xin:  29 (CLK)  - D41 (not necessary)

 Connect +5V to M37451 pins: 12 (P5-4), 13 (P5-3), 64 (Vcc), 42 (P1-6), 41 (P1-7), 27 (Vpp)
 Connect GND to M37451 pins: 1-11, 14 (CE_ / P5-2) 15 (OE_ / P5-1) , 15 (17-24, 28 (RST_), 32, 57, 58, 59, 62, 63

 Note: Data sheet mentions that even in PROM mode an oscillator or an external clock should be connected.  
 Therefore a clock signal is generated onto D41, but actually the ROM contents could be read without it.

```

See page 2-48 of "Mitsubishi Microcomputers 7451 Group" data sheet for more info.
