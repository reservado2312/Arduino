/*
   Curso de Arduino e AVR WR Kits Channel
   
   Aula 88 - RFID - Introdução
   
    
   Autor: Eng. Wagner Rambo  Data: Outubro de 2016
   
   www.wrkits.com.br | facebook.com/wrkits | youtube.com/user/canalwrkits
   
*/

// --- Bibliotecas Auxiliares ---
#include <SPI.h>
#include <MFRC522.h>
#include <Keyboard.h>

// --- Mapeamento de Hardware ---
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Cria instância com MFRC522
 

// --- Variáveis Globais --- 
char st[20];
char yourPwd[] = "2312";

// --- Configurações Iniciais ---
void setup() 
{
  Serial.begin(9600);   // Inicia comunicação Serial em 9600 baud rate
  SPI.begin();          // Inicia comunicação SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Keyboard.begin();
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
   
  
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
  
  if (conteudo.substring(1) == "67 BB 78 34") //UID 1 - Chaveiro
  {
    Serial.println("Desbloqueando");
    Serial.println();
    //yourPwd
    Keyboard.press(KEY_RETURN);
    delay(200);
    Keyboard.releaseAll();
    Keyboard.print(yourPwd);
    delay(3000);
     
  }
 
  if (conteudo.substring(1) == "0A C8 AB 1A") //UID 2 - Cartao
  {
    Serial.println("Bloqueando");
    Serial.println();
    Keyboard.press(KEY_LEFT_GUI); // Tecla Win (Windows)
    Keyboard.press('l'); // Tecla 'l'
    delay(100);
    Keyboard.releaseAll();
    delay(3000);
    
    
  }
  
  
} //end loop
 
 
