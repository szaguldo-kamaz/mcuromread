/*
 Simple ROM reader

 https://github.com/szaguldo-kamaz/

 Wiring for M5M27C256

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

*/

#define LED 13

unsigned  int addr=0x0000;
unsigned char data=0x00;
char outstr[8]={0,0,0,0, 0,0,0,0};

void setup() {

  Serial.begin(115200);
  Serial.println("M5M27C256 ROMdump by Kamm - github.com/szaguldo-kamaz");
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  DDRA=0x00; // D00-D07 (22..29)
  DDRC=0xFF; // A08-A15 (37..30)
  DDRL=0xFF; // A00-A07 (49..42)

  Serial.println("Waiting for 10 sec before starting to read the M5M27C256...");
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
    if (addr==32768){
      Serial.println("Finished at 32767.");
      addr=0xFFFF;
    }
    // spin this forever
    delay(10000);
  }

}
