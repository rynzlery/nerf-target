/*
Declare in another file (which name is the same as
directory to be compiled first) these parameters :

#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""

const char* ssid     = "";
const char* password = "";
const String host    = "";
*/

const int analogSensor = A0;
const int temperaturePin = 0;
const int humidityPin = 5;
const int threshold = 20;
int sensorReading = 0;

void setup () {
 
  Serial.begin(9600);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
 
void loop() {
 
  if (WiFi.status() == WL_CONNECTED) {
    
    sensorReading = analogRead(analogSensor);
    if (sensorReading >= threshold) {
      // set value
      Firebase.pushFloat("number", sensorReading);
      // handle error
      if (Firebase.failed()) {
          Serial.print("setting /number failed:");
          Serial.println(Firebase.error());  
          return;
      }
      
      HTTPClient http;
      String url = host + "?power=" + String(sensorReading, DEC);
      Serial.println(url);
      http.begin(url);
      int httpCode = http.GET();
      
      if (httpCode > 0) {
        String payload = http.getString();
        Serial.println(payload);
      }else {
        Serial.println("FAILED TO GET DATA");
      }
      http.end();
      delay(3000);
    }
  }
}
