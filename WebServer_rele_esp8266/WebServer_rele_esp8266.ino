#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Rede Casa";
const char* password = "gerlianexavier23";

ESP8266WebServer server(80);


int RELE1 = 0;
int RELE2 = 2;

bool botao1Estado = false;
bool botao2Estado = false;


void getBotaoWeb()
{
   //Verifica se existe uma Requisição do tipo GET com o termo /toggle
  server.on("/toggle", HTTP_GET, []() {
    if (server.hasArg("botao")) {
      int botao = server.arg("botao").toInt();
      if (botao == 1) 
      {
        botao1Estado = !botao1Estado;
        digitalWrite(RELE1, botao1Estado ? HIGH : LOW);
        server.send(200, "text/plain", botao1Estado ? "1" : "0");
      } 
      else if (botao == 2) 
      {
        botao2Estado = !botao2Estado;
        digitalWrite(RELE2, botao2Estado ? HIGH : LOW);
        server.send(200, "text/plain", botao2Estado ? "1" : "0");
      }
      else {
        server.send(400, "text/plain", "Botão inválido");
      }
    } else {
      server.send(400, "text/plain", "Parâmetro 'botao' não encontrado");
    }
  });
}


void setup() {
 // Serial port for debugging purposes
  Serial.begin(115200);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }

  // Print ESP8266 Local IP Address
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  pinMode(RELE1, OUTPUT);
  pinMode(RELE2, OUTPUT);

  // Iniciando o servidor web
  server.on("/", HTTP_GET, []() 
  {
      String html = "<!DOCTYPE html>";
      html += "<html lang='pt-br' data-bs-theme='dark'>";
      html += "<head>";
      html += "<meta charset='UTF-8'>";
      html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
      html += "<title>Servidor ESP8266</title>";
      html += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css'>";
      html += "</head>";
      html += "<body>";
      html += "<div class='container bg-dark'>";
      html += "<h1>Controle de Reles Pelo ESP8266</h1>";   
      //html buttons
      html += "<button id='botao1' class='btn btn-primary' onclick='toggleBotao(1)'>Rele 1";
      html += botao1Estado ? " ON" : " OFF";
      html += "</button>";
      
      html += "<button id='botao2' class='btn btn-primary' onclick='toggleBotao(2)'>Rele 2";
      html += botao2Estado ? " ON" : " OFF";
      html += "</button>";

      html += "</div>";
      html += "<script>";

      html += "function toggleBotao(botao) {";
      html += "var xhr = new XMLHttpRequest();";
      html += "xhr.onreadystatechange = function() {";
      html += "if (xhr.readyState == XMLHttpRequest.DONE) {";
      html += "if (xhr.status == 200) {";
      html += "if (botao == 1) {";
      html += "document.getElementById('botao1').innerText = 'Rele 1 ' + (xhr.responseText.trim() == '1' ? ' ON' : ' OFF');";
      
      html += "} else if (botao == 2) {";
      html += "document.getElementById('botao2').innerText = 'Rele 2 ' + (xhr.responseText.trim() == '1' ? ' ON' : ' OFF');";  
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
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
