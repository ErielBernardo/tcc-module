#include <HTTPClient.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <Arduino_JSON.h>

// Module id
const int mod_id = 0;

// GPIO where the DS18B20 is connected to
const int oneWireBus = 23;
// GPIO where the LDR is connected to
const int ldr = 22;
// GPIO where the rele is connected to
const int rele =  21;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

// Wifi credentials
const char* ssid = "Bunker";
const char* password = "dosestudos";

//Your Domain name with URL path or IP address with path
String serverName = "https://fastapi-tcc.herokuapp.com/";
//const char*  serverName = "https://fastapi-tcc.herokuapp.com/";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 30 seconds (30000)
unsigned long timerDelay = 3000;