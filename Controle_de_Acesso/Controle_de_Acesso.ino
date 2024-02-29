/*
 * Controle de acesso 
 * Usando Modulo RFiD RF522, Rele 2 Canais e Modulo MP3 JQ 6500 16Mb
   Autor: Eng. Wagner Rambo  Data: Outubro de 2016
   Reescrito: Jonathan Rafel  Data: 28 de Fevereiro de 2024  
*/

// --- Bibliotecas Auxiliares ---
#include <SPI.h>
#include <MFRC522.h>

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <JQ6500_Serial.h>

#define RELE1 3
#define RELE2 4



// Create the mp3 module object, 
//   Arduino Pin 8 is connected to TX of the JQ6500
//   Arduino Pin 6 is connected to one end of a  1k resistor, 
//     the other end of the 1k resistor is connected to RX of the JQ6500
//   If your Arduino is 3v3 powered, you can omit the 1k series resistor
SoftwareSerial mySerial(8, 6);
JQ6500_Serial mp3(mySerial);

// --- Mapeamento de Hardware ---
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Cria instância com MFRC522
 

// --- Variáveis Globais --- 
char st[20];
int TGA = 2500; //Tempo que a carga do rele ficara ativa


// --- Configurações Iniciais ---
void setup() 
{
  Serial.begin(9600);   // Inicia comunicação Serial em 9600 baud rate
  SPI.begin();          // Inicia comunicação SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  mySerial.begin(9600);
  pinMode(RELE1 ,OUTPUT);
  pinMode(RELE2 ,OUTPUT);
  mp3.reset();
  mp3.setVolume(20);
  mp3.setLoopMode(MP3_LOOP_NONE);  
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  if(mp3.getStatus() != MP3_STATUS_PLAYING)
  {
    mp3.playFileByIndexNumber(1);  
  }
  /*
   Sequência dos audios de retorno
   1 - Iniciando Sistema
   2 - Acesso Autprizado
   3 - Acesso Negado

   consegui esses audios através do site
   https://ttsmaker.com/br
   ou também qaulquer site ou ptograma que gere 
   audios através de textos , podendo ser até 
   sua própia voz
  
  */
  
} //end setup



// --- Loop Infinito ---
void loop() 
{
  // Verifica novos cartões
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Seleciona um dos cartões
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  // Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  
  if (conteudo.substring(1) == "67 BB 78 34") //UID 1 - Tag 1
  {
    Serial.println("Tag 1");
    Serial.println();
    if(mp3.getStatus() != MP3_STATUS_PLAYING)
    {
      mp3.playFileByIndexNumber(2);  
      digitalWrite(RELE2, HIGH);
      delay(TGA);
      digitalWrite(RELE2, LOW);       
    }     

  }else if (conteudo.substring(1) == "0A C8 AB 1A") //UID 2 - Tag 2
  {
    Serial.println("Tag 2");
    Serial.println();
    if(mp3.getStatus() != MP3_STATUS_PLAYING)
    {
      mp3.playFileByIndexNumber(2);  
      digitalWrite(RELE1, HIGH);
      delay(TGA);
      digitalWrite(RELE1, LOW);
    }

  }else
  {
    //Qualquer outra Teg NFC não cadstrada
    if(mp3.getStatus() != MP3_STATUS_PLAYING)
    {
      mp3.playFileByIndexNumber(3);  
    }
  }
  
  
} //end loop
 
