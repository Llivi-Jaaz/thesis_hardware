const int fltswtch = 8;
const int relay_valve = 7;
const int relay_actuator = 6;

bool isProcessRunning = false;
unsigned long startTime;

void setup() {
  pinMode(relay_valve, OUTPUT);
  pinMode(relay_actuator, OUTPUT);
  pinMode(fltswtch, INPUT_PULLUP);
}

void loop() {
  int floatState = digitalRead(fltswtch);

  if (floatState == HIGH && !isProcessRunning) {
    startProcess();
  }

  if (isProcessRunning) {
    unsigned long currentTime = millis(); //returns the number of milliseconds since the Arduino board began running
    if (currentTime - startTime >= 5000) {
      digitalWrite(relay_valve, LOW);
    }
    if (currentTime - startTime >= 30000) {
      digitalWrite(relay_actuator, LOW);
      isProcessRunning = false;
    }
  } else {
    digitalWrite(relay_valve, LOW);
    digitalWrite(relay_actuator, LOW);
  }
}

void startProcess() {
  isProcessRunning = true;
  startTime = millis();
  digitalWrite(relay_valve, HIGH);
  digitalWrite(relay_actuator, HIGH);
}
