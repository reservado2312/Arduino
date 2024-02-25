#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define DHTPIN 5

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char *ssid = "Rede Casa";
const char *password = "gerlianexavier23";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -3 * 3600, 60000);


// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

float temp = 0, humid = 0;

DHT_Unified dht(DHTPIN, DHTTYPE);


void testfillroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    // The INVERSE color is used so round-rects alternate white/black
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, SSD1306_INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void introDot(void) {  

  for(int16_t i=0; i<display.width()-10; i+=1) {   
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(i, i+10);  
    display.print("OwO");
    //localização exata para a hora no canto superior na direita
    display.display();
    delay(10);
    display.clearDisplay();
  }

  delay(2000);
}

void setup() 
{
 Serial.begin(9600);
 delay(10);
 dht.begin();
 WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    delay(1000); 
  }
  Serial.println("Connected to WiFi");

  timeClient.begin();
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  delay(1000);
  display.clearDisplay();
  introDot();
  display.clearDisplay();
}



void loop() 
{  
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  temp = event.temperature;
  humid = event.relative_humidity;
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *timeinfo;
  timeinfo = localtime((const time_t *)&epochTime);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(50, 5);
  display.println("Online");
  display.display();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(30, 20);  
  //localização exata para a hora no canto superior na direita
  display.print(timeinfo->tm_hour);
  display.print(':');
  display.print(timeinfo->tm_min);
//  display.print(':');
//  display.println(timeinfo->tm_sec);
//  display.display();
  display.setCursor(30, 40);  
  display.print(temp);
  display.print(" C ");
  display.display();
  delay(5000);
  display.clearDisplay();
}
