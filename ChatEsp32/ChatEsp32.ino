#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>

const char* ssid = "Rede Casa";
const char* password = "gerlianexavier23";

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81); // WebSocket porta 81

void setup() {
  Serial.begin(115200);
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
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Iniciando o servidor web
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", "<!DOCTYPE html>\
    <html lang='pt-br'>\
    <head>\
    <meta charset='UTF-8'>\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
    <title>ESP32 Chat</title>\
    <link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css'>\
    </head>\
    <body>\
    <div class='container'>\
    <h2>ESP32 Chat</h2>\
    <div id='chatbox'></div>\
    <input type='text' id='message' class='form-control' placeholder='Digite sua mensagem'>\
    <button onclick='sendMessage()' class='btn btn-primary mt-2'>Enviar</button>\
    </div>\
    <script src='https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js'></script>\
    <script>\
    var socket = new WebSocket('ws://' + window.location.hostname + ':81/');\
    socket.onmessage = function(event) {\
      $('#chatbox').append('<p>' + event.data + '</p>');\
    };\
    function sendMessage() {\
      var message = $('#message').val();\
      socket.send(message);\
      $('#message').val('');\
    }\
    </script>\
    </body>\
    </html>");
  });

  webSocket.begin();
  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  webSocket.loop();
}
