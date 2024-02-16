#include <WiFi.h>
#include <WiFiClient.h>
#include <time.h>

const char* ssid = "Rede Casa";
const char* password = "gerlianexavier23";

void setup() {
  Serial.begin(9600);

  // Conectando-se à rede Wi-Fi
  //Serial.println();
  //Serial.print("Conectando-se a ");
  //Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.println("Conectado à rede Wi-Fi");
  // Imprimindo o endereço IP
  //Serial.print("Endereço IP: ");
  //Serial.println(WiFi.localIP());


  // Inicializar e sincronizar o tempo
  configTime(-3 * 3600, 0, "pool.ntp.org"); // Ajuste para o fuso horário de Brasília (GMT-3)
  //Serial.println("Sincronizando o tempo...");
  while (!time(nullptr)) {
    delay(1000);
    //Serial.println("Aguardando sincronização do tempo...");
  }
  //Serial.println("Tempo sincronizado");

}

void loop() {
  time_t now = time(nullptr);
  struct tm* timeinfo;
  timeinfo = localtime(&now);

  // Formatando a hora atual
  char buffer[80];
  //strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
  strftime(buffer, 80, "%H:%M:%S", timeinfo);
  
  // Enviar hora pela porta serial
  Serial.println(buffer);

  // Aguardar 1 segundo
  delay(1000);
}
