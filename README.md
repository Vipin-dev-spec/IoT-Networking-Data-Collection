# IoT Data Monitoring and Logging

This repository contains code and resources for an IoT data monitoring and logging system. The system is designed to collect sensor data from various components and log it into Google Sheets for analysis and visualization.

## Components Used

- **ESP8266 Microcontroller (ESP-12E):** Handles WiFi connectivity and data transmission.
- **DHT Sensor (DHT11/DHT22):** Measures temperature and humidity.
- **Ultrasonic Sensor (HC-SR04):** Provides distance measurement functionality.
- **Breadboard and Jumper Wires:** Used for prototyping and connecting components.
- **Batteries:** Power source for portable operation.

## Code Files

1. **ESP8266 Code (`Temp_Humidity_Monitor_ESP8266.ino`) and (`ESP8266_Ultrasonic_Distance_Sensor.ino`):**
   - Handles WiFi connection setup.
   - Reads data from DHT and ultrasonic sensors.
   - Sends sensor data securely to Google Sheets.

2. **Google Apps Script (`ESP8266_Google_Sheets_Integration.gs`):**
   - Receives HTTP requests with sensor data.
   - Updates Google Sheets with received data.

## Setup Instructions

1. **ESP8266 Setup:**
   - Install the necessary libraries for ESP8266WiFi and WiFiClientSecure.
   - Configure the code with your WiFi credentials, Google Sheets script ID, and sensor pins.
   - Upload the code to your ESP8266 microcontroller.

2. **Google Sheets Setup:**
   - Create a Google Sheets document to store the sensor data.
   - Deploy the `doGet` function in Google Apps Script as a web app and note down the URL.

3. **Integration:**
   - Update the ESP8266 code with the Google Apps Script URL.
   - Ensure the hardware components are connected correctly.

## Usage

1. Power on the IoT system.
2. The ESP8266 will connect to your WiFi network and start sending sensor data to Google Sheets.
3. Access the Google Sheets document to view and analyze the collected data.

## Contributors

- [@Venkatesh4049](https://github.com/Venkatesh4049)
- [@Knighwastaken](https://github.com/Knighwastaken)
## Google Sheets Data

The sensor data collected by the ESP8266 microcontroller is logged into a Google Sheets document for easy visualization and analysis. You can access the live data and view historical records using the following link:

[Google Sheets Data](https://docs.google.com/spreadsheets/d/1bMwGJG1JDO8-xLX91U8VLeVC9zSmNcJ0W1tW4ydkbBQ/edit#gid=0)

