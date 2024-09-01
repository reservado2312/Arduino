#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Substitua pelas credenciais da sua rede Wi-Fi
const char* ssid = "Rede Casa";
const char* password = "gerlianexavier23";

#define DHTPIN 2     // Pino digital conectado ao sensor DHT

// Descomente o tipo de sensor em uso:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22   // DHT 22 (AM2302)
//#define DHTTYPE    DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// Temperatura e umidade atuais, atualizadas no loop()
float t = 0.0;
float h = 0.0;

// Cria objeto AsyncWebServer na porta 80
AsyncWebServer server(80);

unsigned long previousMillis = 0;    // armazena a última vez que o DHT foi atualizado

// Atualiza as leituras do DHT a cada 10 segundos
const long interval = 10000;  

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP8266 DHT Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">%</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";

// Substitui o placeholder pelos valores do DHT
String processor(const String& var){
  if(var == "TEMPERATURE"){
    return String(t);
  }
  else if(var == "HUMIDITY"){
    return String(h);
  }
  return String();
}

void setup(){
  // Porta serial para fins de depuração
  Serial.begin(115200);
  dht.begin();
  
  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }

  // Exibe o endereço IP e o endereço MAC
  Serial.println("Conectado à rede Wi-Fi");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  
  Serial.print("Endereço MAC: ");
  Serial.println(WiFi.macAddress());

  // Rota para a página web root
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });

  // Inicia o servidor
  server.begin();
}
 
void loop(){  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // salva a última vez que os valores do DHT foram atualizados
    previousMillis = currentMillis;
    
    // Lê a temperatura em Celsius (padrão)
    float newT = dht.readTemperature();
    if (isnan(newT)) {
      Serial.println("Falha ao ler o sensor DHT!");
    }
    else {
      t = newT;
      Serial.println(t);
    }
    
    // Lê a umidade
    float newH = dht.readHumidity();
    if (isnan(newH)) {
      Serial.println("Falha ao ler o sensor DHT!");
    }
    else {
      h = newH;
      Serial.println(h);
    }
  }
}
