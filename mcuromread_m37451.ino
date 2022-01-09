/*
 Simple ROM reader for Mitsubishi M37451 microcontrollers

 https://github.com/szaguldo-kamaz/

 Wiring for 64P4B packages (see page 2-48 of "Mitsubishi Microcomputers 7451 Group" datasheet):
 (tested with M37451E8SP)

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

 Note: Data sheet mentions that even in PROM mode an oscillator or an external clock should be connected, therefore a clock signal is generated onto D41, but actually the ROM contents could be read without it.

*/

#define LED 13
#define CLK 41

unsigned  int addr=0x0000;
unsigned char data=0x00;
char outstr[8]={0,0,0,0, 0,0,0,0};

void setup() {

  Serial.begin(115200);
  Serial.println("M37451 ROMdump by Kamm - github.com/szaguldo-kamaz");
  pinMode(LED, OUTPUT);
  pinMode(CLK, OUTPUT);
  digitalWrite(LED, HIGH);
  digitalWrite(CLK, HIGH);

  DDRA=0x00; // D00-D07 (22..29)
  DDRC=0xFF; // A08-A15 (37..30)
  DDRL=0xFF; // A00-A07 (49..42)

  Serial.println("Giving 10 sec for the M37451 to wake up properly...");
  delay(10000);
  Serial.println("Starting.");

}

void loop() {

  if (addr<32768) {
    digitalWrite(CLK, HIGH);
    PORTL=addr & 0xFF;
    PORTC=addr >> 8;
    delay(1);
    digitalWrite(CLK, LOW);
    delay(1);
    data=PINA;
    snprintf(outstr,8,"%04x:%02x",addr,data);
    Serial.println(outstr);
    addr++;
  } else {
    if (addr==32768) {
      Serial.println("Finished at 0x7fff.");
      addr=0xFFFF;
    }
    // spin this forever
    delay(10000);
  }

}
