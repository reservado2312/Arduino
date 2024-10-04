#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>  // Inclua essa linha para usar HTTPClient

// Configurações da rede Wi-Fi
const char* ssid = "Rede Casa";
const char* password = "gerlianexavier23";

// Endereço MAC do computador para Wake-on-LAN
byte macAddress[] = { 0x08, 0x97, 0x98, 0x62, 0x32, 0x48 }; // Exemplo de MAC address
// Porta para Wake-on-LAN
const int wolPort = 9;

WiFiUDP udp;
ESP8266WebServer server(80);

// Pinos dos botões
const int buttonPin = 0;  // GPIO0 para Wake-on-LAN
const int buttonPin2 = 2; // GPIO2 para desligar
String buttonState = "Desligado";  // Estado inicial do botão

void setup() {
  // Inicializar o Serial Monitor
  Serial.begin(115200);

  // Configurar os pinos dos botões como entrada
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);

  // Conectar ao Wi-Fi
  Serial.print("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");

  // Exibir o endereço IP local
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Iniciar o servidor e definir as rotas da página
  server.on("/", handleRoot);
  server.on("/buttonState", handleButtonState); // Rota para AJAX
  server.on("/sendWOL", sendWOL);               // Rota para enviar WOL
  server.on("/shutdownPC", shutdownPC);         // Rota para desligar o PC
  server.begin();
  Serial.println("Servidor iniciado.");
}

void loop() {
  // Verificar se o botão físico para Wake-on-LAN foi pressionado
  if (digitalRead(buttonPin) == LOW) {
    buttonState = "Ligado";
    sendWOLPacket();
  } else {
    buttonState = "Desligado";
  }

  // Verificar se o botão físico para desligar foi pressionado
  if (digitalRead(buttonPin2) == LOW) {
    shutdownPC();  // Desligar o computador
  }

  // Atender às solicitações da página web
  server.handleClient();
}

// Função que lida com a página web principal
void handleRoot() {
  // Página HTML usando UTF-8 e Bootstrap com AJAX
  String html = "<!DOCTYPE html><html lang='pt-BR'>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css' rel='stylesheet'>";
  html += "<title>Controle do Computador</title>";
  html += "<script>";
  html += "function getButtonState() {";
  html += "  var xhttp = new XMLHttpRequest();";
  html += "  xhttp.onreadystatechange = function() {";
  html += "    if (this.readyState == 4 && this.status == 200) {";
  html += "      document.getElementById('buttonState').innerHTML = this.responseText;";
  html += "    }";
  html += "  };";
  html += "  xhttp.open('GET', '/buttonState', true);";
  html += "  xhttp.send();";
  html += "}";
  html += "setInterval(getButtonState, 1000);"; // Atualiza a cada 1 segundo
  html += "</script>";
  html += "</head>";
  html += "<body>";
  html += "<div class='container'>";
  html += "<h1 class='text-center mt-5'>Controle do Computador</h1>";
  html += "<div class='alert alert-info text-center' role='alert'>";
  html += "Estado atual: <strong id='buttonState'>Desconhecido</strong>";
  html += "</div>";
  html += "<button class='btn btn-primary' onclick='sendWOLRequest()'>Ligar Computador</button>";
  html += "<button class='btn btn-danger' onclick='shutdownPCRequest()'>Desligar Computador</button>";
  html += "</div>";
  html += "<script>";
  html += "function sendWOLRequest() {";
  html += "  var xhttp = new XMLHttpRequest();";
  html += "  xhttp.open('GET', '/sendWOL', true);";
  html += "  xhttp.send();";
  html += "  alert('Pacote WOL enviado!');";
  html += "}";
  html += "function shutdownPCRequest() {";
  html += "  var xhttp = new XMLHttpRequest();";
  html += "  xhttp.open('GET', '/shutdownPC', true);";
  html += "  xhttp.send();";
  html += "  alert('Desligando o computador!');";
  html += "}";
  html += "</script>";
  html += "</body></html>";

  // Enviar o conteúdo da página ao cliente
  server.send(200, "text/html", html);
}

// Função que lida com a rota AJAX para obter o estado do botão
void handleButtonState() {
  server.send(200, "text/plain", buttonState);
}

// Função que lida com a rota de envio do WOL
void sendWOL() {
  sendWOLPacket();
  server.send(200, "text/plain", "Pacote WOL enviado");
}

// Função que lida com a rota de desligamento do PC
void shutdownPC() {
  // Aqui você deve enviar a requisição ao servidor no PC para desligá-lo
  // Exemplo de uma requisição HTTP para o servidor Flask no PC
  HTTPClient http;
  http.begin("http://192.168.1.29:5000/shutdown");  // Substitua pelo IP do seu computador
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println(payload);
  }
  http.end();

  server.send(200, "text/plain", "Comando de desligamento enviado");
}

// Função para enviar o pacote WOL
void sendWOLPacket() {
  byte packet[102];  // O pacote mágico tem 102 bytes
  
  // Preenche os primeiros 6 bytes com 0xFF
  for (int i = 0; i < 6; i++) {
    packet[i] = 0xFF;
  }
  
  // Preenche os próximos 16 blocos de 6 bytes com o endereço MAC do computador
  for (int i = 1; i <= 16; i++) {
    for (int j = 0; j < 6; j++) {
      packet[i * 6 + j] = macAddress[j];
    }
  }

  // Calcula o endereço de broadcast e envia o pacote WOL
  IPAddress broadcastIP = getBroadcastAddress();
  udp.beginPacket(broadcastIP, wolPort);  // Enviar para o endereço de broadcast
  udp.write(packet, sizeof(packet));
  udp.endPacket();

  Serial.print("Pacote WOL enviado para o endereço de broadcast: ");
  Serial.println(broadcastIP);
}

// Função para calcular o endereço de broadcast
IPAddress getBroadcastAddress() {
  IPAddress ip = WiFi.localIP();
  IPAddress subnet = WiFi.subnetMask();
  IPAddress broadcast;

  for (int i = 0; i < 4; i++) {
    broadcast[i] = ip[i] | ~subnet[i];
  }
  
  return broadcast;
}
