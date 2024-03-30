#include <SPI.h>
#include <LoRa.h>

int fltswtch = 8;
int led = 3;



int counter = 0;
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSyncWord(0xF3);
  LoRa.setTxPower(20);

  pinMode(fltswtch, INPUT_PULLUP);
  pinMode (led, OUTPUT);

  byte uniqueID[8];
  LoRa.getUniqueId(uniqueID);
}


void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);
  // send packet
  LoRa.beginPacket();
  LoRa.print("Data:");
  LoRa.print(counter);

  Serial.print("LoRa Module Unique ID: ");
  for (int i = 0; i < 8; i++) {
    Serial.print(uniqueID[i], HEX);
    Serial.print(" ");
  }
  int floatState = digitalRead(fltswtch);

  if (floatState == LOW) {
    digitalWrite(led, LOW);
    Serial.println("WATER LEVEL: LOW");
    LoRa.print("WATER LEVEL: LOW");
  }
  else {
    digitalWrite(led, HIGH);
    Serial.println("WATER LEVEL: HIGH");
    LoRa.print("WATER LEVEL: HIGH");
  }
 LoRa.endPacket();
  counter++;
  delay(1000);
}