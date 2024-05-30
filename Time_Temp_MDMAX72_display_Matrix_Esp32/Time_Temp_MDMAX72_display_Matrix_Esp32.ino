#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <DHT.h>
#include <DHT_U.h>

// Definição de pinos do ESP32
#define MAX_DEVICES 4 // Número de módulos MAX7219 em cascata
#define DATA_PIN 23   // Pino DIN
#define CLK_PIN 18    // Pino CLK
#define CS_PIN 5      // Pino CS

#define DHTPIN 4      // Pino de dados do sensor DHT11
#define DHTTYPE DHT11 // Define o tipo de sensor DHT

// Inicializa o objeto MD_Parola
MD_Parola myDisplay = MD_Parola(MD_MAX72XX::FC16_HW, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "Rede Casa";
const char *password = "gerlianexavier23";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -3 * 3600, 60000);

// Variável para armazenar o tempo da última atualização
unsigned long previousMillis = 0;
const long interval = 1000; // Intervalo para atualizar a hora (1 minuto)

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  timeClient.begin();
  dht.begin();

  // Inicializa o display
  myDisplay.begin();
  myDisplay.setIntensity(10); // Ajusta a intensidade (brilho) dos LEDs (0-15)
  myDisplay.displayClear();

  // Configura o texto inicial
  myDisplay.displayText("Online", PA_CENTER, 100, 1000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void loop() {
  unsigned long currentMillis = millis();

  // Atualiza a hora a cada minuto
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    timeClient.update();
  }

  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *timeinfo;
  timeinfo = localtime((const time_t *)&epochTime);

  // Formata a hora e os minutos em uma string
  char timeStr[6]; // HH:MM
  snprintf(timeStr, sizeof(timeStr), "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);

  // Lê a temperatura e a umidade do sensor DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Verifica se a leitura falhou
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Formata a temperatura e a umidade em uma string
  char tempHumStr[16];
  snprintf(tempHumStr, sizeof(tempHumStr), " %.1fC %.1f%% ", temperature, humidity);

  // Atualiza o display
  if (myDisplay.displayAnimate()) {
    myDisplay.displayReset();
    // Alterna entre a hora e a leitura do sensor
    static bool showTime = true;
    if (showTime) {
      myDisplay.displayText(timeStr, PA_CENTER, 80, 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    } else {
      myDisplay.displayText(tempHumStr, PA_CENTER, 80, 500, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    }
    showTime = !showTime;
  }
}
