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
    String html = "<!DOCTYPE html>";
    html += "<html lang='pt-br'>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>Servidor ESP32</title>";
    html += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css'>";
    html += "</head>";
    html += "<body>";
    html += "<div class='container'>";
    html += "<h2>Mensagem:</h2>";
    html += "<form action='/input' method='get'>";
    html += "<div class='form-group'>";
    html += "<input type='text' class='form-control' name='data'>";
    html += "</div>";
    html += "<button type='submit' class='btn btn-primary'>Enviar</button>";
    html += "</form>";
    html += "</div>";
    html += "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js'></script>";
    html += "<script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.16.0/umd/popper.min.js'></script>";
    html += "<script src='https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js'></script>";
    html += "</body>";
    html += "</html>";
    server.send(200, "text/html", html);
  });

  server.on("/input", HTTP_GET, []() {
    if (server.hasArg("data")) {
      String data = server.arg("data");
      Serial.println(data);
      server.send(200, "text/plain", "Dados recebidos com sucesso!");
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
