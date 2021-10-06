#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h

const char* ssid = SECRET_SSID;          // Your ssid
const char* password = SECRET_PASS;  // Your Password
const char* testHostname = "google.com";
int pingResult;
IPAddress HostIP;
unsigned int localPort = 80;
const int RELAY_Pin = 0;  //Relay Pin
int res;

#define MINUTES (60L * 1000)
#define SECONDS 1000
const unsigned long PROBE_DELAY = 10 * SECONDS;
const unsigned long RESET_DELAY = 5 * MINUTES;
const unsigned long RESET_PULSE = 10 * SECONDS;
int Nreset_events = 0;
int i = 0;

enum {
  TESTING_STATE = 0,
  FAILURE_STATE = 1,
  SUCCESS_STATE = 2
};

int CurrentState = TESTING_STATE;

void setup() {
  pinMode(RELAY_Pin, OUTPUT);

  Serial.begin(115200);
  delay(10);
  Serial.println(__FILE__);
  delay(10);

  // Connecting to a WiFi network
  Serial.print(String("Connecting to ") + ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi connected");

  Serial.println("Delay to give modem time to startup.");
  delay(RESET_DELAY);
}

void reset_device() {
  // keep track of number of resets
  Nreset_events++;
  Serial.println(String("\nDisconnected... resetting - ") + String(Nreset_events));
  digitalWrite(RELAY_Pin, HIGH);
  delay(RESET_PULSE);
  digitalWrite(RELAY_Pin, LOW);
}

void loop() {
  switch (CurrentState) {

    case TESTING_STATE: 
      Serial.println("Begining Test");
      res = Ping.ping(testHostname);
      if (!res) {  
        Serial.println("failed test");
        CurrentState = FAILURE_STATE;
      } else {
        CurrentState = SUCCESS_STATE;
      }
      break;

    case FAILURE_STATE:
      i = (i+5); //off setting the cool down percentage
      Serial.print("Number of failed tries: ");
      Serial.println(i);
      if (i > 20){
        Serial.println("Begining Reset");
        reset_device();
        delay(RESET_DELAY);
        CurrentState = TESTING_STATE;
        i = 0;
        break;
      } else {
        delay(PROBE_DELAY);
        CurrentState = TESTING_STATE;
      }
      break;      
      
    case SUCCESS_STATE:
      Serial.println("Successful Test");  
      Serial.println();    
      delay(PROBE_DELAY);
      if (i > 0) { //cool down; This should help with high packet loss.
        --i; 
      }       
      Serial.println(i);
      CurrentState = TESTING_STATE;
      break;
  }
}
