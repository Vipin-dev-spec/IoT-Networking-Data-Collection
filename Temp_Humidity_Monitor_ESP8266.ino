#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "DHT.h"

// Define the type of the temperature sensor (DHT11 in this case)
#define DHTTYPE DHT11
const int DHTPin = 2; // Pin D1 = GPIO5 used for the DHT11 sensor
DHT dht(DHTPin, DHTTYPE); // Initialize DHT sensor

#define ON_Board_LED 2 // On Board LED indicator pin

const char* ssid = "WIFI_SSID_OR_NAME"; // Your WiFi network name (SSID)
const char* password = "WIFIPASSWORD"; // Your WiFi network password

// Host and port for HTTPS connection
const char* host = "script.google.com";
const int httpsPort = 443;

WiFiClientSecure client; // Create a secure WiFi client object
String GS_ID = "GOOGLE_SPREADSHEET_SCRIPT_ID"; // Google spreadsheet script ID

void setup() {
  Serial.begin(9600); // Start serial communication
  delay(500);
  dht.begin(); // Start reading DHT11 sensor
  delay(500);
  WiFi.begin(ssid, password); // Connect to WiFi network
  Serial.println("");
  pinMode(ON_Board_LED, OUTPUT); // Set On Board LED as output
  digitalWrite(ON_Board_LED, HIGH); // Turn off On Board LED

  // Wait for WiFi connection
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
  // Read temperature and humidity from DHT11 sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if sensor readings are valid
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor !");
    delay(500);
    return;
  }

  String Temp = "Temperature : " + String(t) + " °C";
  String Humi = "Humidity : " + String(h) + " %";
  Serial.println(Temp);
  Serial.println(Humi);

  sendData(t, h); // Call sendData function to send data to Google Sheets
}

// Function to send data to Google Sheets
void sendData(float tem, int hum) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);

  // Connect to Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  String string_temperature = String(tem);
  String string_humidity = String(hum, DEC);
  String url = "/macros/s/" + GS_ID + "/exec?temperature=" + string_temperature + "&humidity=" +
               string_humidity;
  Serial.print("requesting URL: ");
  Serial.println(url);

  // Send HTTP GET request to Google Sheets
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");

  // Check if data was sent successfully
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
