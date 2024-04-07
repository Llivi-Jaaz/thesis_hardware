#include <WiFi.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define ssid "LuvFam"
#define password "Qu13sc3nt@38!"
#define FireBaseAPIKey "AIzaSyCMamClOkwHXPfWjdYa64R1AIAQbuuPw1k"
#define FireBaseDatabaseURL "https://dp-gutterguard-default-rtdb.asia-southeast1.firebasedatabase.app/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];

char Id[numChars] = {0};
int IsClogged = 0;
double latitude = 0.0;
double longitude = 0.0;
boolean newData = false;
bool signupOK = false;
unsigned long PrevMillis = 0;
unsigned long ConnectionCheckingMillis = 0;
String RecordedTimeDate;

void setup() {
  Serial.begin(115200);
  CheckConnection();
}

void loop() {
   recvWithStartEndMarkers();
   if (newData == true) {
     strcpy(tempChars, receivedChars);
     if(Firebase.ready() && signupOK) {
       unsigned long CurrentMillis = millis();
       if (CurrentMillis - ConnectionCheckingMillis >= 1000) {
         ConnectionCheckingMillis = CurrentMillis;
         CheckConnection();
       }
       if (CurrentMillis - PrevMillis >= 15000) {
         PrevMillis = CurrentMillis;
         DateTime();
         parseData();
         SendDataToDatabase();
       }
     }
     newData = false;
   }
}

void InitializeConnection() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }
  Serial.println("Connected to WiFi!");

  config.api_key = FireBaseAPIKey;
  config.database_url = FireBaseDatabaseURL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  CheckConnection(); 
  timeClient.begin();
}

void CheckConnection() {
  if(WiFi.status() != WL_CONNECTED) {
    InitializeConnection();
  }
}

void SendDataToDatabase() {
  String UniqueId = String(Id);
  String Clog = String(IsClogged);
  String When = String(RecordedTimeDate);
  String lat = String(latitude);
  String longi = String(longitude);
//UNIQUE ID SENDING
  if (Firebase.RTDB.pushString(&fbdo, "Gutterrrr/Id", UniqueId)) {
    Serial.print("PASSED! ");
    Serial.print("PATH: " + fbdo.dataPath());
    Serial.print("  ||  Id: ");
    Serial.println(Id);
  }
  else {
    Serial.print("FAILED - ");
    Serial.println("REASON: " + fbdo.errorReason());
  }

//CLOG STATUS SENDING
  if (Firebase.RTDB.pushString(&fbdo, "Gutterrrr/IsClogged", Clog)) {
    Serial.print("SUCCESS ");
    Serial.print("PATH: " + fbdo.dataPath());
    Serial.print("  ||  IsClogged: ");
    Serial.println(IsClogged);
  }
  else {
    Serial.print("FAILED - ");
    Serial.println("REASON: " + fbdo.errorReason());
  }

//DATE TIME SENDING
  //   if (Firebase.RTDB.pushString(&fbdo, "GutterLocations/DeviceTrial/DateTime", When)) {
  //   Serial.print("SUCCESS ");
  //   Serial.print("PATH: " + fbdo.dataPath());
  //   Serial.print("  Date: ");
  //   Serial.println(RecordedTimeDate);
  // }
  // else {
  //   Serial.print("FAILED - ");
  //   Serial.println("REASON: " + fbdo.errorReason());
  // }

//LATITUDE SENDING
    if (Firebase.RTDB.pushString(&fbdo, "Gutterrrr/Latitude", latitude)) {
    Serial.print("SUCCESS ");
    Serial.print("PATH: " + fbdo.dataPath());
    Serial.print("  Latitude: ");
    Serial.println(latitude);
  }
  else {
    Serial.print("FAILED - ");
    Serial.println("REASON: " + fbdo.errorReason());
  }

//LATITUDE SENDING
    if (Firebase.RTDB.pushString(&fbdo, "Gutterrrr/Longitude", longitude)) {
    Serial.print("SUCCESS ");
    Serial.print("PATH: " + fbdo.dataPath());
    Serial.print("  Longitude: ");
    Serial.println(longitude);
  }
  else {
    Serial.print("FAILED - ");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false) {
      rc = Serial.read();

      if (recvInProgress == true) {
          if (rc != endMarker) {
              receivedChars[ndx] = rc;
              ndx++;
              if (ndx >= numChars) {
                  ndx = numChars - 1;
              }
          } else {
              receivedChars[ndx] = '\0'; // terminate the string
              recvInProgress = false;
              ndx = 0;
              newData = true;
          }
      } else if (rc == startMarker) {
          recvInProgress = true;
      }
  }
}

void parseData() {
  char * strtokIndx;
  strtokIndx = strtok(tempChars, ",");
  strcpy(Id, strtokIndx);

  strtokIndx = strtok(NULL, ",");
  IsClogged = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  latitude = strtod(strtokIndx, NULL);

  strtokIndx = strtok(NULL, ",");
  longitude = strtod(strtokIndx, NULL);
}

void DateTime() {
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *localTime = localtime((time_t *)&epochTime);
  char formattedTime[32];
  strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d %H:%M:%S", localTime);
  RecordedTimeDate = formattedTime;
}
