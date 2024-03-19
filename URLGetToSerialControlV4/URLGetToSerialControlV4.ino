#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include <DHT.h>
#define DHTPIN 14     // Pino ao qual o sensor DHT11 está conectado
#define DHTTYPE DHT11 // Tipo do sensor DHT

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Rede Casa";
const char* password = "gerlianexavier23";

WebServer server(80);

bool botao1Estado = false;
bool botao2Estado = false;
bool botao3Estado = false;

#define LED 2
#define RELE1 4
#define RELE2 5 

void getTemperature() {
    float temp = dht.readTemperature();
    if (isnan(temp)) {
        server.send(500, "text/plain", "Erro ao ler temperatura");
        return;
    }
    String tempString = String(temp);
    server.send(200, "text/plain", tempString);
}

void getHumidity() {
    float humidi = dht.readHumidity();
    if (isnan(humidi)) {
        server.send(500, "text/plain", "Erro ao ler Umidade");
        return;
    }
    String humidiString = String(humidi);
    server.send(200, "text/plain", humidiString);
}


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
      Serial.println("");
      Serial.print("MSG ");
      Serial.print(data);   
      //server.send(200, "text/plain", "Dados recebidos com sucesso!");
      server.sendHeader("Location", "/");     
      server.send(302, "text/plain", "");
      
    } else {
      server.send(400, "text/plain", "Erro: Nenhum dado recebido");
    }
  });

}

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(1000);
  pinMode(LED, OUTPUT);
  pinMode(RELE1, OUTPUT);
  pinMode(RELE2, OUTPUT);

  // Conectando-se à rede Wi-Fi
  Serial.println();
  Serial.print("Conectando-se a");
  Serial.print(ssid);
  Serial.println("");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);    
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("MSG Conectado");
  // Imprimindo o endereço IP
  Serial.println("");
  Serial.print("IP: ");
  Serial.print(WiFi.localIP());
  Serial.println("");
  // Iniciando o servidor web
  server.on("/", HTTP_GET, []() {
    String html = "<!DOCTYPE html>";
    html += "<html lang='pt-br' data-bs-theme='dark'>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>Servidor ESP32</title>";
    html += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css'>";
    html += "</head>";
    html += "<body>";
    html += "<div class='container' data-bs-theme='blue'>";
    html += "<h1>Controle de dispositivos Pelo ESP32</h1>";   
    html += "<h3>Dispositivos Conectados ao ESP32</h3>";
    html += "<h5 id='temp'></h5>";
    html += "<h5 id='humidi'></h5>";
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
    html += "<br>";
    html += "<h3>Dispositivos Externos</h3>";
    html += "<label for='iframeSrc'>Ip do Dispositivo Externo: </label>";
    html += "<br>";
    html += "<input type='text' id='iframeSrc' placeholder='http://192.168.1.22'>";
    html += "<button onclick='changeIframeSrc()'>Mudar IP</button>";
    html += "<div class='embed-responsive embed-responsive-1by1'>";
    html += "<iframe id='myIframe' class='embed-responsive-item' src='http://192.168.1.35:81/stream'></iframe>";
    html += "</div>";

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

    html += "function getTemperature() {";
    html += "var xhr = new XMLHttpRequest();";
    html += "xhr.onreadystatechange = function() {";
    html += "if (xhr.readyState == XMLHttpRequest.DONE) {";
    html += "if (xhr.status == 200) {";
    html += "document.getElementById('temp').innerText = 'Temperatura: ' + xhr.responseText + ' °C';";
    html += "}";
    html += "}";
    html += "};";
    html += "xhr.open('GET', '/temperature', true);";
    html += "xhr.send();";
    html += "}";
    html += "setInterval(getTemperature, 5000);"; // Atualiza a temperatura a cada 5 segundos

    html += "function getHumidity() {";
    html += "var xhr = new XMLHttpRequest();";
    html += "xhr.onreadystatechange = function() {";
    html += "if (xhr.readyState == XMLHttpRequest.DONE) {";
    html += "if (xhr.status == 200) {";
    html += "document.getElementById('humidi').innerText = 'Umidade: ' + xhr.responseText + ' %';";
    html += "}";
    html += "}";
    html += "};";
    html += "xhr.open('GET', '/humidity', true);";
    html += "xhr.send();";
    html += "}";
    html += "setInterval(getHumidity, 5000);"; // Atualiza a Umidade a cada 5 segundos


    html += " function changeIframeSrc() {";
    html += " var newSrc = document.getElementById('iframeSrc').value;";
    html += " document.getElementById('myIframe').src = newSrc;";
    html += " }";

    html += "</script>";
    html += "</body>";
    html += "</html>";
    server.send(200, "text/html", html);
  });

  server.on("/temperature", HTTP_GET, []() {
      getTemperature();
  });

  server.on("/humidity", HTTP_GET, []() {
      getHumidity();
  });


  getBotaoWeb();
  getMenssagem();
  
  server.begin();
  Serial.println("MSG Servidor  ESP32 iniciado");
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
