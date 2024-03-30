#include <SPI.h>
#include <LoRa.h>

int fltswtch = 8;
int led = 3;


byte uniqueID[8];
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

  byte uniqueID[] = {0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78, 0x90};

}


void loop() {
  // Sending packets
  Serial.print("Packet: ");
  Serial.println(counter);
  LoRa.beginPacket();
  LoRa.print("Data:");
  LoRa.print(counter);

  // Printing LoRa ID
  Serial.print("ID: ");
  for (int i = 0; i < 8; i++) {
  Serial.print(uniqueID[i], HEX);
  Serial.print(" ");
  Serial.println(" ");
  }

  // Float Switch status
  int floatState = digitalRead(fltswtch);

  if (floatState == LOW) {
  digitalWrite(led, LOW);
  Serial.println("LOW");
  LoRa.print("LOW");
  }
  else {
  digitalWrite(led, HIGH);
  Serial.println("HIGH");
  LoRa.print("HIGH");
  }
 LoRa.endPacket();
  counter++;
  delay(1000);
}