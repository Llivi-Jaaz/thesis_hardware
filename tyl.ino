#include <SPI.h>
#include <LoRa.h>

const int fltswtch = 8;
const int relay_valve = 7;
const int relay_actuator = 6;

String uniqueID = "dev_1";
int floatState;
void(* resetFunc) (void) = 0; //declare reset function at address 0

void setup() {
  Serial.begin(9600);
  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while(1);
  }

  LoRa.setSyncWord(0xF3);
  LoRa.setTxPower(20);

  pinMode(relay_valve, OUTPUT);
  pinMode(relay_actuator, OUTPUT);
  pinMode(fltswtch, INPUT_PULLUP);
}

void loop() {
  floatState = digitalRead(fltswtch);
  printID();

  if (floatState == HIGH) {
    LoRa.print('1');
    LoRa.print('>');
    LoRa.endPacket();
    delay(1500);
    startProcess();
    delay(1000);
    endProcess();

  } else if (floatState == LOW) {
    LoRa.print('0');
    LoRa.print('>');
    LoRa.endPacket();
  }

  delay(1000);
}

void printID() {
  LoRa.beginPacket();
  LoRa.print('<');
  LoRa.print(uniqueID);
  LoRa.print(',');
}

void startProcess() {
  digitalWrite(relay_valve, HIGH);
  digitalWrite(relay_actuator, HIGH);
  delay(15000);
  digitalWrite(relay_valve, LOW);
  digitalWrite(relay_actuator, LOW);
  delay(15000);
  digitalWrite(relay_actuator, HIGH);
  delay(15000);
  digitalWrite(relay_actuator, LOW);
}

void endProcess(){
  digitalWrite(relay_actuator, LOW);
  digitalWrite(relay_valve, LOW);
  resetFunc();
}
