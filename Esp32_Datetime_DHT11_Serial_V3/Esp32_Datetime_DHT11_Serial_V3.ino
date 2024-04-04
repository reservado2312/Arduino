/* 
 * Autor: Jonathan Rafael(Reservado2312)
 * Date:  03/03/2024
 * 
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <time.h>
#include <DHT.h>
#include <Arduino.h>

#define DHTPIN 14     // Pino ao qual o sensor DHT11 está conectado
#define DHTTYPE DHT11 // Tipo do sensor DHT
#define LED_PIN 2 // Pino do Primeiro LED

/*
 * Connections: TOUCH BUTTONS
 * 
 * TTP 223        Arduino
 * 5v         =>  5v       
 * I\O(Sinal) =>  DigitalPort
 * GND        =>  GND
 * 
*/
const int BUTTON_1 = 18; // Pino do botão 1
const int BUTTON_2 = 19; // Pino do botão 2

const int RELE_1 = 25;   // Pino do RELE 1
const int RELE_2 = 26;   // Pino do RELE 2

// Variáveis de estado

bool RELE_1_STATE = false;   // Estado inicial do RELE 1
bool RELE_2_STATE = false;   // Estado inicial do RELE 2

bool L_BUTTON_1_STATE = HIGH; // Estado anterior do botão 1
bool L_BUTTON_2_STATE = HIGH; // Estado anterior do botão 2

bool BUTTON_1_STATE;        // Estado atual do botão 1
bool BUTTON_2_STATE;        // Estado atual do botão 2




DHT dht(DHTPIN, DHTTYPE);

long unsigned int ax_send_time = 0;
long unsigned int ax_send_temp = 0;

const char* ssid = "Rede Casa";
const char* password = "gerlianexavier23";

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  dht.begin();
  pinMode(LED_PIN, OUTPUT); // Configura o Pino do LED como Saída
  pinMode(RELE_1, OUTPUT); // Configura o Pino do Rele 1 como Saída
  pinMode(RELE_2, OUTPUT); // Configura o Pino do Rele 2 como Saída
  pinMode(BUTTON_1, INPUT); // Configura o Pino do botão 1 Rele como Entrada 
  pinMode(BUTTON_2, INPUT); // Configura o Pino do botão 2 Rele como Entrada 



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
  //OBS: CRIAR UMA FUNÇÃO PARA RETORNAR O IP DO DISPOSITIVO PELA SERIAL CASO SOLICITADO


  // Inicializar e sincronizar o tempo
  configTime(-3 * 3600, 0, "pool.ntp.org"); // Ajuste para o fuso horário de Brasília (GMT-3)
  Serial.println("Sincronizando o tempo...");
  while (!time(nullptr)) {
    delay(1000);
    Serial.println("Aguardando sincronização do tempo...");
  }
  Serial.println("Tempo sincronizado");
}


void ChamadaComando()
{
    String comando = Serial2.readStringUntil(' '); // Lê o comando enviado pela porta serial
    String acao; //Recebe acao do comando que Liga ou Desliga algo
    //Exemplo led: LED ON ou LED OFF
    //Exemplo rele: RELE1 ON ou RELE1 OFF
    //Exemplo msg: MSG Sua Menssagem!!
    int pin;

    // Debug: Imprime o comando recebido
    Serial.print("Comando recebido: ");
    Serial.println(comando);

    if (comando == "LED" || comando == "RELE1" || comando == "RELE2") {
      acao = Serial2.readStringUntil('\n'); // Lê a ação (ON ou OFF)
      
      // Debug: Imprime a ação recebida
      Serial.print("Ação recebida: ");
      Serial.println(acao);

      // Determina qual LED deve ser controlado
      if (comando == "LED") {
        pin = LED_PIN;
      } else if (comando == "RELE1") {
        pin = RELE_1;
      } else if (comando == "RELE2") {
        pin = RELE_2;
      }
      if (acao == "ON")
      {
        digitalWrite(pin, HIGH); // Liga o LED
      }
      else if(acao == "OFF")
      {
        digitalWrite(pin, LOW); // Desliga o LED
      }
      else
      {
        //Debug Caso não execute nada ou a ação não exista!
        Serial.println("Erro ao receber ação: ");
        Serial.println(acao);
        Serial.println("No Pino: ");
        Serial.println(pin);
      }      
    }
    else if (comando == "MSG") 
    {
      String mensagem = Serial.readStringUntil('\n'); // Lê a mensagem

      // Imprime a mensagem
      Serial.print("Mensagem: ");
      Serial.print(mensagem);
    }
    else
    {
      //Debug que Trata comando inexistente
      Serial.println("Comando: ");
      Serial.println(comando);
      Serial.println("Inexistente!");
    }
}

void ComandoReles()
{
  // Lê o estado atual doS botões
  BUTTON_1_STATE = digitalRead(BUTTON_1);
  BUTTON_2_STATE = digitalRead(BUTTON_2);
  

  // Verifica se o botão 1 foi pressionado
  if (BUTTON_1_STATE == LOW && L_BUTTON_1_STATE == HIGH) {
    // Inverte o estado do RELE 1
    RELE_1_STATE = !RELE_1_STATE;
    // Atualiza o estado do RELE 1 caso ledState seja verdadeio ou falso usando o
    // operador condicional ternário (variavel/função que retorne true ou false ? HIGH : FALSE)  para determinar o valor a ser escrito no pino do LED
    digitalWrite(RELE_1, RELE_1_STATE ? HIGH : LOW);

    //ledState1 ? Funcao1Butao1() : Funcao2Butao1();
  }
  // Salva o estado atual do botão 2 para a próxima iteração
  L_BUTTON_1_STATE = BUTTON_1_STATE;

  // Verifica se o botão 2 foi pressionado
  if (BUTTON_2_STATE == LOW && L_BUTTON_2_STATE == HIGH) {
    // Inverte o estado do RELE 2
    RELE_2_STATE = !RELE_2_STATE;
    // Atualiza o estado do RELE 2 caso ledState seja verdadeio ou falso usando o
    // operador condicional ternário (variavel/função que retorne true ou false ? HIGH : FALSE)  para determinar o valor a ser escrito no pino do LED
    digitalWrite(RELE_2, RELE_2_STATE ? HIGH : LOW);

    //ledState1 ? Funcao1Butao1() : Funcao2Butao1();
  }
  // Salva o estado atual do botão 2 para a próxima iteração
  L_BUTTON_2_STATE = BUTTON_2_STATE;

}



void loop() {
  //if (Serial2.available() > 0) 
  //{ // Verifica se há dados disponíveis para leitura
  //  ChamadaComando();
  //}


   ComandoReles();

  if (millis() - ax_send_time >= 1000) 
  {    
    time_t now = time(nullptr);
    struct tm* timeinfo;
    timeinfo = localtime(&now);
    // Formatando a hora atual
    char buffer[80];
    
    // Enviar hora pela porta serial
    strftime(buffer, 80, "%H:%M:%S", timeinfo);
    Serial2.print("TIME ");
    Serial2.print(buffer);
    Serial2.println();

    // Enviar DATA pela porta serial
    strftime(buffer, 80, "%d/%m/%Y", timeinfo);
    Serial2.print("DATE ");
    Serial2.print(buffer);
    Serial2.println();

    ax_send_time = millis();
  }


  if(millis() - ax_send_temp >= 5000)
  {
    float temp = dht.readTemperature();
    float humidi = dht.readHumidity();

    Serial2.print("TEMP ");
    Serial2.print(temp);
    Serial2.println();

    Serial2.print("RANGE_BAR ");
    Serial2.print(humidi);
    Serial2.println();

    ax_send_temp = millis();
  }
}
