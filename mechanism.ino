#include <SPI.h>
#include <LoRa.h>

const int fltswtch = 8;
const int relay_valve = 7;
const int relay_actuator = 6;

String uniqueID = "dev_ID";

bool isProcessRunning = false;
unsigned long startTime;

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

  pinMode(relay_valve, OUTPUT);
  pinMode(relay_actuator, OUTPUT);
  pinMode(fltswtch, INPUT_PULLUP);
}

void loop() {
  LoRa.beginPacket();

  Serial.print("ID: ");
  Serial.print(uniqueID);
  LoRa.print('<');
  LoRa.print(uniqueID);
  LoRa.print(',');
  
  Serial.print("Status: ");

  int floatState = digitalRead(fltswtch);

  if (floatState == HIGH && !isProcessRunning) {
    startProcess();
    Serial.print(true);
    LoRa.print(true);
    LoRa.print(',');
  } else if (floatState == LOW) {
    Serial.print(false);
    LoRa.print(false);
    LoRa.print(',');
  }

  LoRa.print('>');
  LoRa.endPacket();
  delay(2000);
}

void startProcess() {
  isProcessRunning = true;
  startTime = millis();
  unsigned long valveOffTime = startTime + 5000;
  bool relayState = HIGH;
  
  while (millis() - startTime < 36000) {
    digitalWrite(relay_valve, HIGH);
    digitalWrite(relay_actuator, relayState);
    relayState = !relayState; 

    if (millis() >= valveOffTime) {
      digitalWrite(relay_valve, LOW);
    }
    
    delay(12000); 
  }
  
  digitalWrite(relay_actuator, LOW);
  isProcessRunning = false;
}
