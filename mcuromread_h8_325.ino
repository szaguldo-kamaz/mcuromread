/*
 Simple ROM reader for Hitachi H8/325 HD6473258 microcontrollers

 https://github.com/szaguldo-kamaz/

 Wiring for DP-64S packages (see page 205 of "Hitachi Single-Chip Microcomputer H8/325 ... Series Hardware Manual"):

  HD6473258 pin - Arduino Mega pin
 ---------------------------------
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

*/

#define LED 13

unsigned  int addr=0x0000;
unsigned char data=0x00;
char outstr[8]={0,0,0,0, 0,0,0,0};

void setup() {

  Serial.begin(115200);
  Serial.println("HD6473258 ROMdump by Kamm - github.com/szaguldo-kamaz");
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  DDRA=0x00; // D00-D07 (D22..D29)
  DDRC=0xFF; // A08-A15 (D37..D30)
  DDRL=0xFF; // A00-A07 (D49..D42)

  Serial.println("Giving 10 sec for the HD6473258 to wake up properly...");
  delay(10000);
  Serial.println("Starting.");

}

void loop() {

  if (addr<32768) {
    PORTL=addr & 0xFF;
    PORTC=addr >> 8;
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

  delay(1);

}
