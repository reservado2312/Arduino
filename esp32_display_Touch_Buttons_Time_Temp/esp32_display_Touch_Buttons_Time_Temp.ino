#include <XPT2046_Bitbang.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <DHT.h>
#include <WiFi.h>
#include <time.h>

// Definições de pinos e parâmetros
#define MOSI_PIN 32
#define MISO_PIN 39
#define CLK_PIN  25
#define CS_PIN   33
#define DHTPIN 27       // Pino ao qual o DHT11 está conectado
#define DHTTYPE DHT11   // Tipo de sensor DHT
#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 50
#define BUTTON1_X 10
#define BUTTON1_Y 155
#define BUTTON2_X 130
#define BUTTON2_Y 155
#define LedRed 16
#define LedGrenn 4
#define LedBlue 17
#define TFT_GREY 0x5AEB

// Configurações de Wi-Fi e NTP
const char* ssid = "Rede Casa";
const char* password = "gerlianexavier23";
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800; // Offset para o horário de Brasília (-3h em segundos)
const int   daylightOffset_sec = 0; // Sem horário de verão no Brasil atualmente

XPT2046_Bitbang touchscreen(MOSI_PIN, MISO_PIN, CLK_PIN, CS_PIN);
TFT_eSPI tft = TFT_eSPI();
DHT dht(DHTPIN, DHTTYPE);

uint32_t targetTime = 0; // para o próximo timeout de 1 segundo

void setup() {
  Serial.begin(115200);
  
  // Inicializa LEDs
  pinMode(LedRed, OUTPUT);
  pinMode(LedGrenn, OUTPUT);
  pinMode(LedBlue, OUTPUT);
  digitalWrite(LedRed, LOW);
  digitalWrite(LedGrenn, LOW);
  digitalWrite(LedBlue, LOW);

  // Inicializa o touchscreen e o display TFT
  touchscreen.begin();
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.invertDisplay(true);

  // Inicializa o sensor DHT11
  dht.begin();

  // Conecta à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Inicializa e sincroniza o NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  targetTime = millis() + 1000;

  // Desenha os botões na tela
  tft.fillRect(BUTTON1_X, BUTTON1_Y, BUTTON_WIDTH, BUTTON_HEIGHT, TFT_RED);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Power OFF Pc", BUTTON1_X + 15, BUTTON1_Y + 20);

  tft.fillRect(BUTTON2_X, BUTTON2_Y, BUTTON_WIDTH, BUTTON_HEIGHT, TFT_BLUE);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Power ON Pc", BUTTON2_X + 15, BUTTON2_Y + 20);

  // Exibe o texto personalizado no topo da tela
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Control HUB Online", 8, 10);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
}

void loop() {
  // Atualização de relógio, data, temperatura e umidade
  if (targetTime < millis()) {
    targetTime = millis() + 1000;

    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      uint8_t hh = timeinfo.tm_hour;
      uint8_t mm = timeinfo.tm_min;
      uint8_t ss = timeinfo.tm_sec;

      // Atualiza a hora digital
      int xpos = 20;
      int ypos = 75; 
      int ysecs = ypos + 24;

      if (hh < 10) xpos += tft.drawChar('0', xpos, ypos, 6);
      xpos += tft.drawNumber(hh, xpos, ypos, 6);
      xpos += tft.drawChar(':', xpos, ypos - 8, 6);
      if (mm < 10) xpos += tft.drawChar('0', xpos, ypos, 6);
      xpos += tft.drawNumber(mm, xpos, ypos, 6);
      xpos += tft.drawChar(':', xpos, ypos - 8, 6);
      if (ss < 10) xpos += tft.drawChar('0', xpos, ypos, 6);
      tft.drawNumber(ss, xpos, ypos, 6);

      // Exibe a data
      char dateStr[12];
      snprintf(dateStr, sizeof(dateStr), "%02d/%02d/%04d", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
      tft.setTextSize(2);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.drawCentreString(dateStr, 125, 35, 2);


      // Lê a temperatura e umidade do sensor DHT11
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();

      // Exibe a temperatura e umidade no display
      char tempHumStr[30];
      snprintf(tempHumStr, sizeof(tempHumStr), "T: %.1f C H: %.1f %%", temperature, humidity);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.drawString(tempHumStr, 4, 125);
      tft.setTextSize(1);
    }
  }

  // Verifica toques na tela para os botões
  TouchPoint touch = touchscreen.getTouch();

  if (touch.zRaw != 0) {
    // Retrato 1
    //int16_t x = map(touch.x, 23, 290, 0, tft.width());
    //int16_t y = map(touch.y, 20, 217, 0, tft.height());

    //portatil
    int16_t x = map(touch.y, 23, 290, 0, tft.width());
    int16_t y = map(touch.x, 20, 217, 0, tft.height());

    if (x > BUTTON1_X && x < BUTTON1_X + BUTTON_WIDTH &&
        y > BUTTON1_Y && y < BUTTON1_Y + BUTTON_HEIGHT) {
      Serial.println("Botao 1 pressionado");
      digitalWrite(LedRed, HIGH);
      digitalWrite(LedGrenn, HIGH);
      digitalWrite(LedBlue, HIGH);
    }

    if (x > BUTTON2_X && x < BUTTON2_X + BUTTON_WIDTH &&
        y > BUTTON2_Y && y < BUTTON2_Y + BUTTON_HEIGHT) {
      Serial.println("Botao 2 pressionado");
      digitalWrite(LedRed, LOW);
      digitalWrite(LedGrenn, LOW);
      digitalWrite(LedBlue, LOW);
    }
  }

  delay(100);  // Ajusta o delay conforme necessário
}
