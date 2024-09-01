#include <TFT_eSPI.h> // Biblioteca específica para o display TFT
#include <SPI.h>
#include <DHT.h>      // Biblioteca para o sensor DHT
#include <WiFi.h>     // Biblioteca para conectar à rede Wi-Fi
#include <time.h>     // Biblioteca para gerenciar o tempo

#define TFT_GREY 0x5AEB
#define DHTPIN 27       // Defina o pino ao qual o DHT11 está conectado
#define DHTTYPE DHT11   // Defina o tipo de sensor DHT

TFT_eSPI tft = TFT_eSPI();       // Invoca a biblioteca do display TFT
DHT dht(DHTPIN, DHTTYPE);        // Inicializa o sensor DHT11

uint32_t targetTime = 0;                    // para o próximo timeout de 1 segundo

const char* ssid     = "Rede Casa";          // Insira o SSID da sua rede Wi-Fi
const char* password = "gerlianexavier23";         // Insira a senha da sua rede Wi-Fi

// Defina o servidor NTP e o fuso horário
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800;         // Offset para o horário de Brasília (-3h em segundos)
const int   daylightOffset_sec = 0;         // Sem horário de verão no Brasil atualmente

void setup(void) {
  tft.init();
  dht.begin();  // Inicializa o sensor DHT11
  tft.setRotation(2);
  tft.fillScreen(TFT_WHITE);

  // Exibe o texto personalizado no topo da tela
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.drawString("My Digital Clock", 30, 10); // Personalize sua mensagem
  tft.setTextColor(TFT_RED, TFT_WHITE);
  tft.setTextSize(1);

  // Conecta à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Inicializa e sincroniza o NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  targetTime = millis() + 1000;
}

void loop() {
  if (targetTime < millis()) {
    // Configura a próxima atualização para 1 segundo depois
    targetTime = millis() + 1000;

    // Obtém o tempo atual
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      return;
    }

    // Ajusta os valores do tempo
    uint8_t hh = timeinfo.tm_hour;
    uint8_t mm = timeinfo.tm_min;
    uint8_t ss = timeinfo.tm_sec;

    // Atualiza a hora digital
    int xpos = 30;
    int ypos = 75; // Canto superior esquerdo do texto do relógio, cerca de metade da tela
    int ysecs = ypos + 24;

    // Desenha as horas e minutos
    if (hh < 10) xpos += tft.drawChar('0', xpos, ypos, 6); // Adiciona o zero à esquerda para horas no relógio de 24h
    xpos += tft.drawNumber(hh, xpos, ypos, 6);             // Desenha as horas
    xpos += tft.drawChar(':', xpos, ypos - 8, 6);
    if (mm < 10) xpos += tft.drawChar('0', xpos, ypos, 6); // Adiciona o zero à esquerda nos minutos
    xpos += tft.drawNumber(mm, xpos, ypos, 6);             // Desenha os minutos
    xpos += tft.drawChar(':', xpos, ypos - 8, 6);

    // Desenha os segundos
    if (ss < 10) xpos += tft.drawChar('0', xpos, ypos, 6); // Adiciona zero à esquerda
    tft.drawNumber(ss, xpos, ypos, 6);                     // Desenha os segundos

    // Exibe a data abaixo do relógio
    char dateStr[12];
    snprintf(dateStr, sizeof(dateStr), "%02d/%02d/%04d", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
    tft.setTextSize(2);
    tft.setTextColor(TFT_GREEN, TFT_WHITE); // Cor do texto da data
    tft.drawCentreString(dateStr, 155, 35, 2); // Desenha a data no centro da tela abaixo do horário

    // Lê a temperatura e umidade do sensor DHT11
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Exibe a temperatura e umidade no display
    char tempHumStr[30];
    snprintf(tempHumStr, sizeof(tempHumStr), "T: %.1f C H: %.1f %%", temperature, humidity);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.drawString(tempHumStr, 30, 120); // Personalize sua mensagem
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.setTextSize(1);
  }
}
