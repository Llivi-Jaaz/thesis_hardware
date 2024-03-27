int fltswtch = 8;
int led = 3;

void setup() {
  Serial.begin(9600);
  pinMode(fltswtch, INPUT_PULLUP);
  pinMode (led, OUTPUT);
}

void loop() {
  int floatState = digitalRead(fltswtch);

  if (floatState == LOW) {
    digitalWrite(led, LOW);
    Serial.println("WATER LEVEL: LOW");
  }
  else {
    digitalWrite(led, HIGH);
    Serial.println("WATER LEVEL - HIGH");
  }
  delay(1000);
}
