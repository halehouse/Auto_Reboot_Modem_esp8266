#include <ESP8266WiFi.h>

//#include <arduino_secrets.h>
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
//char ssid[] = SECRET_SSID;        // your network SSID (name)
//char password[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

const char* ssid = "Halehouse WiFi";          // Your ssid
const char* password = "t4yl0rm1l43ll4";  // Your Password
const char* testHostname = "www.google.com";
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
      res = WiFi.hostByName(testHostname, HostIP); //dns - test if able to resolve ip for testHostname
      if ((res != 1) || (HostIP == IPAddress(255, 255, 255, 255))) {
        CurrentState = FAILURE_STATE;
      } else {
        CurrentState = SUCCESS_STATE;
      }
      break;

    case FAILURE_STATE:
      ++i;
      if (i > 4){
        reset_device();
        delay(RESET_DELAY);
        CurrentState = TESTING_STATE;
        break;
      } else {
        delay(PROBE_DELAY);
        CurrentState = TESTING_STATE;
      }
      break;      
      

    case SUCCESS_STATE:
      delay(PROBE_DELAY);
      CurrentState = TESTING_STATE;
      break;
  }
}