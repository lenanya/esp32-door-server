#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h> 

//#define USE_SERIAL

#define TRIG 4
#define ECHO 5
#define LED 17
#define DHTPIN 16

DHT dht11(DHTPIN, DHT11);

const char* ssid = "SSIDHERE";
const char* password = "WIFIPASSWORD";
String servername = "http://yourpcip:port/distance.php";
String servername_temp = "http://yourpcip:port/temperature.php";

float duration_us, distance_cm;

void setup() {
    WiFi.begin(ssid, password);
    #ifdef USE_SERIAL
    Serial.begin (115200);
    Serial.println("Connecting to WiFi");
    #endif
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        #ifdef USE_SERIAL
        Serial.print(".");
        #endif 
    }
    #ifdef USE_SERIAL
    Serial.println("");
    Serial.println("Connected to WiFi");
    #endif
    dht11.begin();
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(LED, OUTPUT);
}

void loop() {
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    duration_us = pulseIn(ECHO, HIGH);
    distance_cm = 0.017 * duration_us;
    String query = servername + "?distance=" + String(distance_cm);

    if (distance_cm > 70) {
        digitalWrite(LED, HIGH);
    } else {
        digitalWrite(LED, LOW);
    }

    float tempC = dht11.readTemperature();

    String query_temp = servername_temp + "?temp=" + String(tempC);

    #ifdef USE_SERIAL
    Serial.print("Temperature: ");
    Serial.println(tempC);
    #endif

    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
       
        http.begin(query.c_str());

        int httpResponseCode = http.GET();

        if (httpResponseCode>0) {
            #ifdef USE_SERIAL
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            String payload = http.getString();
            Serial.println(payload);
            #endif
        }
        else {
            #ifdef USE_SERIAL
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
            #endif
        }

        http.end();

        HTTPClient http_temp;
       
        http_temp.begin(query_temp.c_str());

        int http_tempResponseCode = http_temp.GET();

        if (httpResponseCode>0) {
            #ifdef USE_SERIAL
            Serial.print("HTTP Response code: ");
            Serial.println(http_tempResponseCode);
            String payload_temp = http_temp.getString();
            Serial.println(payload_temp);
            #endif
        }
        else {
            #ifdef USE_SERIAL
            Serial.print("Error code: ");
            Serial.println(http_tempResponseCode);
            #endif
        }

        http_temp.end();
    } else {
        #ifdef USE_SERIAL
        Serial.println("WiFi disconnected :(");
        #endif
    }
    delay(1000);
}
