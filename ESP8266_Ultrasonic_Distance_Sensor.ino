#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const int trigPin = 12;
const int echoPin = 14;
#define SOUND_VELOCITY 0.0346
#define CM_TO_M 0.01
#define ON_Board_LED 2 // On Board LED indicator pin

const char* ssid = "WIFI_SSID_OR_NAME"; // Your WiFi network name (SSID)
const char* password = "WIFIPASSWORD"; // Your WiFi network password

long duration;
float distanceCm;
float distanceMeters;

// Host and port for HTTPS connection
const char* host = "script.google.com";
const int httpsPort = 443;

WiFiClientSecure client; // Create a WiFiClientSecure object
String GS_ID = "GOOGLE_SPREADSHEET_ID"; // Google spreadsheet script ID

void setup() {
  Serial.begin(9600);
  delay(500);

  pinMode(trigPin, OUTPUT); // Set trigPin as Output
  pinMode(echoPin, INPUT); // Set echoPin as Input
  delay(500);

  WiFi.begin(ssid, password); // Connect to WiFi network
  Serial.println("");
  pinMode(ON_Board_LED, OUTPUT); // Set On Board LED as output
  digitalWrite(ON_Board_LED, HIGH); // Turn off On Board LED

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
  }
  digitalWrite(ON_Board_LED, HIGH); // Turn off On Board LED when connected
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  client.setInsecure(); // Set client to operate in insecure mode
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH); // Measure sound wave travel time
  if (isnan(duration) || isnan(duration)) {
    Serial.println("Failed to read from distance sensor !");
    delay(500);
    return;
  }

  distanceCm = duration * SOUND_VELOCITY / 2; // Calculate distance in centimeters
  distanceMeters = distanceCm * CM_TO_M; // Convert to meters

  String DCM = "Distance CM : " + String(distanceCm) + " cm";
  String DM = "Distance Meter : " + String(distanceMeters) + "m";
  Serial.println(DCM);
  Serial.println(DM);

  sendData(distanceCm, distanceMeters); // Call sendData function to send data to Google Sheets
}

void sendData(float discm, float dism) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpsPort)) { // Connect to Google Sheets
    Serial.println("connection failed");
    return;
  }

  String string_distancecm = String(discm);
  String string_distancem = String(dism, DEC);
  String url = "/macros/s/" + GS_ID + "/exec?distance_cm=" + string_distancecm + "&distance_m=" +
               string_distancem;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successful!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }

  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
}
