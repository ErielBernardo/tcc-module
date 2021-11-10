#include <Global_Vars.h>

String translateEncryptionType(wifi_auth_mode_t encryptionType) {
  switch (encryptionType) {
    case (WIFI_AUTH_OPEN):
      return "Open";
    case (WIFI_AUTH_WEP):
      return "WEP";
    case (WIFI_AUTH_WPA_PSK):
      return "WPA_PSK";
    case (WIFI_AUTH_WPA2_PSK):
      return "WPA2_PSK";
    case (WIFI_AUTH_WPA_WPA2_PSK):
      return "WPA_WPA2_PSK";
    case (WIFI_AUTH_WPA2_ENTERPRISE):
      return "WPA2_ENTERPRISE";
  }
}

float getTemp() {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
  // delay(1000);
  return temperatureC;
}

void scanNetworks() {

  int numberOfNetworks = WiFi.scanNetworks();

  Serial.print("Number of networks found: ");
  Serial.println(numberOfNetworks);

  for (int i = 0; i < numberOfNetworks; i++) {

    Serial.print("Network name: ");
    Serial.println(WiFi.SSID(i));

    Serial.print("Signal strength: ");
    Serial.println(WiFi.RSSI(i));

    Serial.print("MAC address: ");
    Serial.println(WiFi.BSSIDstr(i));

    Serial.print("Encryption type: ");
    String encryptionTypeDescription = translateEncryptionType(WiFi.encryptionType(i));
    Serial.println(encryptionTypeDescription);
    Serial.println("-----------------------");

  }
}

void connectToNetwork() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Establishing connection to WiFi..");
  }

  Serial.println("Connected to network");

}

void insert_temp(float temp) {

  HTTPClient http;

  String serverPath = serverName + "Insert_TEMP/?temp=" + String(temp) + "&mod_id=" + String(mod_id); // Endpoint + data for input in db
  Serial.print("serverPath = ");
  Serial.println(serverPath);

  // Your Domain name with URL path or IP address with path
  http.begin(serverPath);

  // Specify content-type header
  http.addHeader("Accept", "*/*");
  //http.addHeader("accept", "application/json");
  //http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  //http.addHeader("Accept-Encoding", "gzip, deflate, br");
  //http.addHeader("Content-Type", "application/json");

  // Data to send with HTTP POST
  //  String httpRequestData = "temp=" + String(temp) + "&mod_id=" + String(mod_id);
  //  Serial.print("httpRequestData = ");
  //  Serial.println(httpRequestData);

  // Send HTTP POST request
  int httpResponseCode = http.POST("");

  Serial.print("HTTP POST RESP: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}

void get_mod_temps(int mod_id) {
  HTTPClient http;

  String serverPath = serverName;
  serverPath = serverPath + "read_mod/" + String(mod_id);
  Serial.print("serverPath = ");
  Serial.println(serverPath);

  // Your Domain name with URL path or IP address with path
  http.begin(serverPath.c_str());
  Serial.println("HTTP BEGIN");

  // Send HTTP GET request
  int httpResponseCode = http.GET();
  Serial.print("HTTP GET RESP: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);

  // Start the DS18B20 sensor
  sensors.begin();

  // wifi settings
  scanNetworks();
  connectToNetwork();
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
}

void loop() {
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {

    // Get temperature from sensor
    float temp = getTemp();
    Serial.println(temp);

    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WIFI CONNECTED");
      //get_mod_temps(0);
      //insert_temp(temp);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
