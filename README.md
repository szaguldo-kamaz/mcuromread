# mcuromread
Simple Arduino sketches to read the contents of the on-chip ROM in old microcontrollers.

Originally created for dumping the ROMs of the MCUs found in the Sony HIL-C1/HIL-1000 Hi-Vision Laserdisc players (for details please see: 
http://www.laserdisc.hu/sony_hils/)
, but can be used to dump any standard ROM or MCU, which can be set to "PROM (read) mode".

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

You should see something similiar (regarding the address + data lines):
![H8/325 + Arduino wire arrangement](https://user-images.githubusercontent.com/86873213/150737227-e2b4ea5f-36aa-46e3-aa9e-097feb525ac4.jpg)

So, what to do with the HD6473258's ROM dump?

The vector table is at \[0x0000-0x002F\], the first 16-bit vector is the reset vector.
The reset vector probably points to 0x0100, so that's where the actual code should begin.
With the help of an H8/300 disassembler, like [dah8300](http://www.karola.fi/rak/sw/dah8300/), you can check whether the code makes sense or not
(begins with zeroing the RAM in the MCU, configuring ports, and things like that can be a good sign).

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

You should see something similiar (regarding the address + data lines, A14 (yellow wire) is left unconnected in this photo, should go to the "bottom" side):

![M37451 + Arduino Wire arrangement](https://user-images.githubusercontent.com/86873213/150737193-9e4f27c1-2391-47fd-9592-24674373ac56.jpg)

So, what to do with the M37451E8SP's ROM dump?

The M37451E8SP has 16Kbytes ROM, but when reading from the ROM, the 0x0000-0x3fff (first 16Kbyte
s) region showed only zeroes, then the real data followed from 0x4000 to 0x7fff. So when reading 32Kbytes from a 16Kbyte ROM, the first 16Kbytes can be ignored:

```dd if=dump_m37451e8sp__32kb.bin of=dump_m37451e8sp__16kb.bin count=1 bs=16k skip=1```

When the M37451E8SP is in normal operation, the 16Kb ROM is mapped in the higher region, so in case of 16Kbytes, in the 0xC000-0xFFFF region.
The interrupt vector table is at the end of the ROM in the 0xFFE0-0xFFFF range. And the reset vector should be the last one (0xFFFE), which points where the actual code begins. Depending on the application, you can also look for meaningful ASCII strings in the ROM.
If these seem intact, then you can try [this MELPS740 disassembler](https://github.com/szaguldo-kamaz/m740dasm), which now includes the symbols/vector table for the M37451.

See page 2-48 of "Mitsubishi Microcomputers 7451 Group" data sheet for more info.


# Wiring for Mitsubishi M5M27C256 (DIP28 package)

```
      M5M27C256 pin - Arduino Mega pin
 ----------------------------------
 A00:            10 - D49
 A01:             9 - D48
 A02:             8 - D47
 A03:             7 - D46
 A04:             6 - D45
 A05:             5 - D44
 A06:             4 - D43
 A07:             3 - D42
 A08:            25 - D37
 A09:            24 - D36
 A10:            21 - D35
 A11:            23 - D34
 A12:             2 - D33
 A13:            26 - D32
 A14:            27 - D31
 D00:            11 - D22
 D01:            12 - D23
 D02:            13 - D24
 D03:            15 - D25
 D04:            16 - D26
 D05:            17 - D27
 D06:            18 - D28
 D07:            19 - D29
 
 Connect +5V to M5M27C256 pin: 28 (Vcc/Vdd)
 Connect GND to M5M27C256 pins: 14 GND/Vss, 20 (CE_), 22 (OE_)
```

You should see something similiar:

![romread1000](https://user-images.githubusercontent.com/86873213/160757917-ce96882e-56cc-4425-86f5-20f0d496623e.jpg)

This should also work with other 27xxxx (27C128, 27C512 etc.) ROMs also. I used this to read two  [M27C256 EPROMs](https://datasheet.datasheetarchive.com/originals/distributors/Databooks-2/Book291-104.pdf) from a Pioneer HLD-1000 and from a Toshiba XR-HD1 Hi-Vision LaserDisc player.
