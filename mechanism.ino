  const int fltswtch = 8;
  const int relay_valve = 7;
  const int relay_actuator = 6;

  void setup()   
  {   
  pinMode(relay_valve,OUTPUT);
   pinMode(relay_actuator,OUTPUT);
  pinMode(fltswtch, INPUT_PULLUP);
  }   
  void loop()    
  {   
  int floatState = digitalRead(fltswtch);

  if (floatState == 1) {
  delay(2000);
  digitalWrite(relay_valve,HIGH);
  digitalWrite(relay_actuator,HIGH);
  delay(5000);
  digitalWrite(relay_valve, LOW);
  delay(25000);
  digitalWrite(relay_actuator, LOW);
  } 
  else {
    digitalWrite(relay_valve, LOW);
    digitalWrite(relay_actuator, LOW);
  }
  } 
