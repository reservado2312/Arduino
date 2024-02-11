#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

const char* ssid = "Rede Casa";
const char* password = "gerlianexavier23";

WebServer server(80);

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Conectando-se à rede Wi-Fi
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectado à rede Wi-Fi");
    // Imprimindo o endereço IP
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Iniciando o servidor web
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", "<h2>Menssagem: </h2><form action='/input' method='get'><input type='text' name='data'><input type='submit' value='Enviar'></form>");
  });

  server.on("/input", HTTP_GET, []() {
    if (server.hasArg("data")) {
      String data = server.arg("data");
      //Serial.print("Dados recebidos: ");
      Serial.println(data);
      //server.sendHeader("Location", "/");
      server.send(200, "text/plain", "Dados recebidos com sucesso!");
      //server.send(302, "text/plain", "");
      
    } else {
      server.send(400, "text/plain", "Erro: Nenhum dado recebido");
    }
  });

  server.begin();
  Serial.println("Servidor iniciado");
}
  
void loop() {
  server.handleClient();
}
