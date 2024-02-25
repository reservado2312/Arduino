#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char *ssid = "Rede Casa";
const char *password = "gerlianexavier23";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -3 * 3600, 60000);

void setup() {
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  timeClient.begin();
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *timeinfo;
  timeinfo = localtime((const time_t *)&epochTime);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 5);
  display.println("Online");
  display.setTextSize(0);
  display.setCursor(80, 5);
  //localização exata para a hora no canto superior na direita
  display.print(timeinfo->tm_hour);
  display.print(':');
  display.print(timeinfo->tm_min);
  display.print(':');
  display.println(timeinfo->tm_sec);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("TEMP: 25 Cº");
  display.println("HRA: 55 %");

  display.display();
  delay(1000);
}
