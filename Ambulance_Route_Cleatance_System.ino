#include <WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <TinyGPS.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

TinyGPS gps;

WiFiClient client;

const int sw1 = 18;
const int sw2 = 19;

float flat, flon;
unsigned long age;

const char* ssid = "ambulance";
const char* password = "ambulance";

const char* webserver = "gnrtechno.in";

const char* server = "api.thingspeak.com";
const String apiKey = "MEEYFDITOJF6M9H6";

char phone_no1[] = "+917075649477";
char phone_no2[] = "+919505264456";

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  pinMode(sw1, INPUT);
  pinMode(sw2, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcdline1("Ambulance Route ");
  lcdline2("Clearance System");
  delay(2000);
  lcdline1("     USING     ");
  lcdline2("Raspberry Pico W");
  delay(1000);
  lcdline1("Connecting with");
  lcdline2(ssid);
  delay(100);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  lcdline1("Wi-Fi Connected");
  delay(1000);
}

void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  lcdline1("Reading...");
  delay(300);

  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial2.available())
    {
      char c = Serial2.read();
      if (gps.encode(c))
        newData = true;
    }
  }

  if (newData)
  {
    lcd.clear();
    gps.f_get_position(&flat, &flon, &age);
    lcd.print("LAT:");
    lcd.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    lcd.setCursor(0, 1);
    lcd.print("LON:");
    lcd.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    delay(700);

    if (client.connect(webserver, 80))
    {
      client.print("GET http://gnrtechno.in/iotprojects/GNR003/write.php?a1=");
      client.print(String(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6));
      client.print("&a2=");
      client.print(String(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6));
      client.println();
      lcd.clear();    lcd.print("- Data Sent 1 -");   delay(500);
      thingspeakSend(String(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6), String(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6));
      delay(500);
    }
  }

  if (digitalRead(sw1) == LOW)
  {
    lcdline1("Route 1 Selected");
    delay(1000);
    sendSMS(phone_no1, "http://gnrtechno.in/iotprojects/GNR003");
    delay(500);
  }

  if (digitalRead(sw2) == LOW)
  {
    lcdline1("Route 2 Selected");
    delay(1000);
    sendSMS(phone_no2, "http://gnrtechno.in/iotprojects/GNR003");
    delay(500);
  }

  client.stop();
  delay(700);
}

void lcdline1(String message1) {
  lcd.clear();
  lcd.print(message1);
}

void lcdline2(String message2) {
  lcd.setCursor(0, 1);
  lcd.print(message2);
}

void thingspeakSend(String data1, String data2)
{
  lcd.clear();  lcd.print("Uploading... ");
  delay(500);
  if (client.connect(server, 80))
  {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += data1;
    postStr += "&field2=";
    postStr += data2;
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.println(" -- Data Sent -- ");
    lcd.setCursor(0, 1);  lcd.print("- Data Sent 2 -");
    delay(500);
  }
  else
  {
    Serial.println("-- Data Error --");
    lcd.setCursor(0, 1);  lcd.print("-- Data Error --");
    delay(500);
  }
  client.stop();
  delay(500);
}

void sendSMS(char *number, char *msg) {
  delay(500);
  lcd.clear();  lcd.print("Message Sending");   lcd.setCursor(0, 1);  lcd.print(number);
  delay(1000);
  Serial1.print("AT\r");
  delay(400);
  Serial1.print("AT+CMGF=1\r");
  delay(400);
  Serial1.print("AT+CMGS=\"");
  Serial1.print(number);
  Serial1.println("\"\r\n");
  delay(500);
  Serial1.println(msg);
  delay(500);
  Serial1.write(byte(26));
  delay(2000);
  lcd.clear();  lcd.print("- Message Sent -");
  delay(500);
}
