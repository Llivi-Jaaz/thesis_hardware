#include <SPI.h>
#include <LoRa.h>

int fltswtch = 8;
int led = 3;
int counter = 0;
// int uniqueID = 12345678;
String uniqueID = 'dev_1';
float longitude = 192.4567;
float latitude = 107.111;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSyncWord(0xF3);
  LoRa.setTxPower(20);

  pinMode(fltswtch, INPUT_PULLUP);
  pinMode (led, OUTPUT);
}


void loop() {
  // Sending packets
  Serial.print("\nPacket: ");
  Serial.println(counter);

  // Printing packets (LoRa)
  LoRa.beginPacket();
  // LoRa.print("Packet: ");
  // LoRa.print(counter);

  // Printing LoRa ID (LoRa)
  Serial.print("ID: ");
  Serial.print(uniqueID);
  // LoRa.print("ID: ");
  LoRa.print('<');
  LoRa.print(uniqueID);
  LoRa.print(',');

  // Float Switch status (LoRa)
  Serial.print("Status: ");
  //LoRa.print("Status: ");
  
  int floatState = digitalRead(fltswtch);

  if (floatState == false) {
  digitalWrite(led, LOW);
  Serial.print(false);
  LoRa.print(false);
  LoRa.print(',');
  }
  else {
  digitalWrite(led, HIGH);
  Serial.print(true);
  LoRa.print(true);
  LoRa.print(',');
  }
  LoRa.print(longitude);
  LoRa.print(',');
  LoRa.print(latitude);
  LoRa.print('>');
  LoRa.endPacket();
  counter++;
  delay(2000);
}