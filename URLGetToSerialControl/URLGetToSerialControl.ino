#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

const char* ssid = "Rede Casa";
const char* password = "gerlianexavier23";

WebServer server(80);

bool botao1Estado = false;
bool botao2Estado = false;
bool botao3Estado = false;

#define LED 2
#define RELE1 4
#define RELE2 5 

void getBotaoWeb()
{
   //Verifica se existe uma Requisição do tipo GET com o termo /toggle
  server.on("/toggle", HTTP_GET, []() {
    if (server.hasArg("botao")) {
      int botao = server.arg("botao").toInt();
      if (botao == 1) 
      {
        botao1Estado = !botao1Estado;
        digitalWrite(LED, botao1Estado ? HIGH : LOW);
        server.send(200, "text/plain", botao1Estado ? "1" : "0");
      } 
      else if (botao == 2) 
      {
        botao2Estado = !botao2Estado;
        digitalWrite(RELE1, botao2Estado ? HIGH : LOW);
        server.send(200, "text/plain", botao2Estado ? "1" : "0");
      } 
      else if (botao == 3) 
      {
        botao3Estado = !botao3Estado;
        digitalWrite(RELE2, botao3Estado ? HIGH : LOW);
        server.send(200, "text/plain", botao3Estado ? "1" : "0");
      }
      else {
        server.send(400, "text/plain", "Botão inválido");
      }
    } else {
      server.send(400, "text/plain", "Parâmetro 'botao' não encontrado");
    }
  });
}

void getMenssagem()
{
    //Verifica se existe uma Requisição do tipo GET com o termo /toggle
    server.on("/input", HTTP_GET, []() {
    if (server.hasArg("data")) {
      String data = server.arg("data");
      Serial2.println("");
      Serial2.print("MSG ");
      Serial2.print(data);      
      //server.send(200, "text/plain", "Dados recebidos com sucesso!");
      server.sendHeader("Location", "/");     
      server.send(302, "text/plain", "");
      
    } else {
      server.send(400, "text/plain", "Erro: Nenhum dado recebido");
    }
  });

}

void setup() {
  Serial2.begin(9600);
  Serial.begin(9600);
  delay(1000);
  pinMode(LED, OUTPUT);
  pinMode(RELE1, OUTPUT);
  pinMode(RELE2, OUTPUT);
  digitalWrite(RELE2, botao3Estado);
  // Conectando-se à rede Wi-Fi
  Serial.println();
  Serial.print("Conectando-se a");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial2.println("MSG Conectado");
  // Imprimindo o endereço IP
  Serial.println("");
  Serial2.print("MSG IP: ");
  Serial2.print(WiFi.localIP());
  Serial2.println("");
  Serial.print("IP: ");
  Serial.print(WiFi.localIP());
  Serial.println("");
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
    html += "<h1>Controle de dispositivos Pelo ESP32</h1>";   
    html += "<h3>Dispositivos</h3>";
    html += "<button id='botao1' class='btn btn-primary' onclick='toggleBotao(1)'>LED";
    html += botao1Estado ? " ON" : " OFF";
    html += "</button>";
    html += "<button id='botao2' class='btn btn-primary' onclick='toggleBotao(2)'>Rele 1";
    html += botao2Estado ? " ON" : " OFF";
    html += "</button>";
    html += "<button id='botao3' class='btn btn-primary' onclick='toggleBotao(3)'>Rele 2";
    html += botao1Estado ? " ON" : " OFF";
    html += "</button>";
    html += "</div>";
    html += "<div class='container'>";
    html += "<h3>Mensagem:</h3>";
    html += "<form action='/input' method='get'>";
    html += "<div class='form-group'>";
    html += "<input type='text' class='form-control' name='data'>";
    html += "</div>";
    html += "<button type='submit' class='btn btn-primary'>Enviar</button>";
    html += "</form>";
    html += "</div>";
    html += "<script>";
    html += "function toggleBotao(botao) {";
    html += "var xhr = new XMLHttpRequest();";
    html += "xhr.onreadystatechange = function() {";
    html += "if (xhr.readyState == XMLHttpRequest.DONE) {";
    html += "if (xhr.status == 200) {";
    html += "if (botao == 1) {";
    html += "document.getElementById('botao1').innerText = 'LED ' + (xhr.responseText.trim() == '1' ? ' ON' : ' OFF');";
    html += "} else if (botao == 2) {";
    html += "document.getElementById('botao2').innerText = 'Rele 1 ' + (xhr.responseText.trim() == '1' ? ' ON' : ' OFF');";  
    html += "} else if (botao == 3) {";
    html += "document.getElementById('botao3').innerText = 'Rele 2 ' + (xhr.responseText.trim() == '1' ? ' ON' : ' OFF');";
    html += "}";
    html += "}";
    html += "}";
    html += "};";
    html += "xhr.open('GET', '/toggle?botao=' + botao, true);";
    html += "xhr.send();";
    html += "}";
    html += "</script>";
    html += "</body>";
    html += "</html>";
    server.send(200, "text/html", html);
  });

 
  getBotaoWeb();
  getMenssagem();
  
  server.begin();
  Serial.println("MSG Servidor iniciado");
}

void debugEstadoBotoes()
{
  Serial.println(botao1Estado);
  Serial.println(botao2Estado);
  Serial.println(botao3Estado);
  digitalWrite(RELE2, LOW);
  delay(1000);
  digitalWrite(RELE2, HIGH);
  delay(1000);
}

void loop() {
  server.handleClient();
  //debugEstadoBotoes();
}
