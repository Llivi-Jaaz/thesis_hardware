#include <SPI.h>
#include <LoRa.h>

const int fltswtch = 8;
const int relay_valve = 7;
const int relay_actuator = 6;

String uniqueID = "dev_1";

bool isProcessRunning = false;
int floatState;
int relayState;
unsigned long startTime;
unsigned long lastExecutionTime_LoRa = 0;
unsigned long lastExecutionTime_Process = 0;

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
  unsigned long currentTime = millis();

  if (currentTime - lastExecutionTime_LoRa >= 2000) {
    lastExecutionTime_LoRa = currentTime;
    floatState = digitalRead(fltswtch);

    printID();
  
    if (floatState == HIGH && !isProcessRunning) {
      LoRa.print('1');
      LoRa.print('>');
      LoRa.endPacket();
      delay(2500);
      startProcess();
    } else {
      LoRa.print('0');
      LoRa.print('>');
      LoRa.endPacket();
    }
  }
}

void printID() {
  LoRa.beginPacket();
  LoRa.print('<');
  LoRa.print(uniqueID);
  LoRa.print(',');
}

void startProcess() {
  isProcessRunning = true;
  startTime = millis();
  unsigned long valveOffTime = startTime + 5000;
  unsigned long previousTime = millis();
  bool relayState = HIGH;
  
  while (millis() - startTime < 36000) {
    digitalWrite(relay_valve, HIGH);
    digitalWrite(relay_actuator, relayState);
    relayState = !relayState; 

    if (millis() >= valveOffTime) {
      digitalWrite(relay_valve, LOW);
    }
    
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - previousTime;
  
    if (elapsedTime < 12000) {
      delay(12000 - elapsedTime);
    }
    
    previousTime = millis(); 
  }
  
  digitalWrite(relay_actuator, LOW);
  isProcessRunning = false;
}
