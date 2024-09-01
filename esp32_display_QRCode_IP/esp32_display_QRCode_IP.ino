#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <qrcode_espi.h>


#include <DHT.h>
#define DHTPIN 27     // Pino ao qual o sensor DHT11 está conectado
#define DHTTYPE DHT11 // Tipo do sensor DHT

DHT dht(DHTPIN, DHTTYPE);

TFT_eSPI display = TFT_eSPI();
QRcode_eSPI qrcode (&display);


const char* ssid = "Rede Casa";
const char* password = "gerlianexavier23";

WebServer server(80);

bool botao1Estado = false;
bool botao2Estado = false;
bool botao3Estado = false;

#define LED_R 4
#define LED_G 16
#define LED_B 17 

unsigned long startMillis;  // Variável para armazenar o tempo inicial
const unsigned long interval = 10000;  // Intervalo de 5 segundos (5000 milissegundos)
bool functionExecuted = false;  // Variável para controlar a execução única da função

String IpAddres;


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
        digitalWrite(LED_R, botao1Estado ? HIGH : LOW);
        server.send(200, "text/plain", botao1Estado ? "1" : "0");
      } 
      else if (botao == 2) 
      {
        botao2Estado = !botao2Estado;
        digitalWrite(LED_G, botao2Estado ? HIGH : LOW);
        server.send(200, "text/plain", botao2Estado ? "1" : "0");
      } 
      else if (botao == 3) 
      {
        botao3Estado = !botao3Estado;
        digitalWrite(LED_B, botao3Estado ? HIGH : LOW);
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
      display.setRotation(2);
      display.fillScreen(TFT_BLACK);
      display.drawString(data, 0, 10, 1);
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

  // Inicializa o display
  display.begin();
  display.invertDisplay(true);
  qrcode.init();
   
  // Inicializa o sensor DHT11
  dht.begin();

  startMillis = millis();  // Armazena o tempo inicial


  delay(1000);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

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
  display.setTextColor(TFT_GREEN);
  //display.drawString("Teste", 5, 2, 1);
  IpAddres = "http://"+ WiFi.localIP().toString();
  qrcode.create(IpAddres);
  //display.fillScreen(TFT_BLACK);
  //display.drawString(data, 0, 10, 1);
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
    html += "<h1 class='text-center'>Controle de dispositivos Pelo ESP32</h1>";   
    html += "<h3 class='text-center'>LEDS ESP32 DISPLAY</h3>";
    html += "<div class='btn-group d-flex justify-content-center' role='group' aria-label='Basic example'>";
    html += "<button id='botao1' class='btn btn-primary' onclick='toggleBotao(1)'>LED RED";
    html += botao1Estado ? " OFF" : " ON";
    html += "</button>";
    html += "<button id='botao2' class='btn btn-secondary' onclick='toggleBotao(2)'>RLED GREEN";
    html += botao2Estado ? " OFF" : " ON";
    html += "</button>";
    html += "<button id='botao3' class='btn btn-warning' onclick='toggleBotao(3)'>LED BLUE";
    html += botao1Estado ? " OFF" : " ON";
    html += "</button>";
    html += "</div>";
    html += "</div>";
    html += "<div class='container'>";
    html += "<h3 class='text-center'>MENSSAGEM</h3>";
    html += "<form action='/input' method='get'>";
    html += "<div class='form-group'>";
    html += "<input type='text' class='form-control' name='data'>";
    html += "</div>";
    html += "<button type='submit' class='btn btn-primary'>Enviar</button>";
    html += "</form>";

    html += "<h3 class='text-center' >Senssor DHT Interno</h5>";
    html += "<h5 id='temp' class='text-center'></h5>";
    html += "<h5 id='humidi' class='text-center'></h5>";

    html += "<br>";
    html += "<h3 class='text-center'>Dispositivos Externos</h3>";
    html += "<label for='iframeSrc'>Ip do Dispositivo Externo: </label>";
    html += "<br>";
    html += "<input type='text' id='iframeSrc' placeholder='http://192.168.1.22' class='w-100'>";
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
    html += "document.getElementById('botao1').innerText = 'LED RED ' + (xhr.responseText.trim() == '1' ? ' OFF' : ' ON');";
    html += "} else if (botao == 2) {";
    html += "document.getElementById('botao2').innerText = 'LED GREEN  ' + (xhr.responseText.trim() == '1' ? ' OFF' : ' ON');";  
    html += "} else if (botao == 3) {";
    html += "document.getElementById('botao3').innerText = 'LED BLUE ' + (xhr.responseText.trim() == '1' ? ' OFF' : ' ON');";
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

void loop() {
  server.handleClient();
  unsigned long currentMillis = millis();  // Armazena o tempo atual

    // Verifica se o tempo decorrido é maior ou igual a 5 segundos e se a função ainda não foi executada
  if ((currentMillis - startMillis >= interval) && !functionExecuted) {
    display.setRotation(2);
    display.fillScreen(TFT_BLACK);
    display.drawString("Servidor Iniciado!", 60, 10, 2);
    display.drawString("IP : " + IpAddres, 50, 25, 2);
    functionExecuted = true;  // Atualiza a variável para evitar múltiplas execuções
  }

}
