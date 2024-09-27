#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Servo.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WiFiClient client;
HTTPClient http;

Servo Saservo, Sbservo, Scservo, Sdservo, Seservo;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  Saservo.attach(8);   // 엄지손가락 서보 모터 연결
  Sbservo.attach(9);   // 새끼손가락 서보 모터 연결
  Scservo.attach(10);  // 약지손가락 서보 모터 연결
  Sdservo.attach(11);  // 중지손가락 서보 모터 연결
  Seservo.attach(12);  // 검지손가락 서보 모터 연결
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    String serverPath = "http://yourserver.com/api/servo";

    String sensorData = "";
    for (int i = 0; i <= 5; i++) {
      int sensorValue = analogRead(i);
      sensorData += "analog" + String(i) + "=" + String(sensorValue) + "&";
    }
    
    http.begin(client, serverPath);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpResponseCode = http.POST(sensorData);
    
    if (httpResponseCode > 0) {
      String payload = http.getString();
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      Saservo.write(doc["f1"]);
      Sbservo.write(doc["f5"]);
      Scservo.write(doc["f4"]);
      Sdservo.write(doc["f3"]);
      Seservo.write(doc["f2"]);
       delay(500);         // 0.5초 
    } else {
      Serial.print("HTTP Error: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
  delay(1000);  // 데이터 송수신 주기
}
