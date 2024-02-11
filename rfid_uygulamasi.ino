#include <SPI.h>
#include <RFID.h>

RFID rfid(10, 9);
int pc = 7;  // pc pinini 7. pine bağladık
bool cardPresent = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
  pinMode(pc, OUTPUT);  // pc pinini çıkış olarak ayarladık
}

void loop() {
  cardPresent = rfid.isCard();

  if (cardPresent) {
    if (rfid.readCardSerial()) {
      Serial.print("Kart ID: ");
      for (int i = 0; i < 5; i++) {
        Serial.print(rfid.serNum[i], HEX);
      }
      Serial.println();

      if (checkAllowedCard()) {
        digitalWrite(pc, HIGH); // Kart algılama
        delay(1000);
      } else {
        Serial.println("Kart izni yok");
      }

      rfid.halt();
      digitalWrite(pc, LOW);
    }
  }
}

bool checkAllowedCard() {
  byte allowedCardID[5] = {0xE2, 0x9E, 0x90, 0x89, 0x65};

  for (int i = 0; i < 5; i++) {
    if (rfid.serNum[i] != allowedCardID[i]) {
      return false; 
    }
  }
  return true; 
}
