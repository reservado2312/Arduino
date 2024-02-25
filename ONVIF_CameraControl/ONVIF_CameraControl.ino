#include <WiFi.h>
#include <onvif.h>
#include  <ArduinoHttpClient.h>

const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";

OnvifDevice onvif;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  onvif.begin("ENDERECO_IP_CAMERA", "USUARIO_CAMERA", "SENHA_CAMERA");
}

void loop() {
  // Exemplo de movimentação da câmera para cima
  onvif.ptzMove(Onvif::Direction::UP, 0.5); // Movimento contínuo em 50% da velocidade máxima

  delay(5000); // Espera 5 segundos

  // Exemplo de parada da movimentação
  onvif.ptzStop();

  delay(5000); // Espera 5 segundos

  // Exemplo de movimentação da câmera para a direita
  onvif.ptzMove(Onvif::Direction::RIGHT, 0.5); // Movimento contínuo em 50% da velocidade máxima

  delay(5000); // Espera 5 segundos

  // Exemplo de parada da movimentação
  onvif.ptzStop();

  delay(5000); // Espera 5 segundos

  // Você pode adicionar mais comandos de movimentação conforme necessário
}
