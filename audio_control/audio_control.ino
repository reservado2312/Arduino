#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10  // Pino SS do módulo RFID
#define RST_PIN 9  // Pino RST do módulo RFID

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("Aproxime um cartão RFID válido...");
}

void loop() {
  // Procurar por cartões RFID
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Cartão detectado, tentar autenticar
    if (validCard()) {
      // Cartão válido, imprimir na Serial
      Serial.println("Cartão RFID válido. ID:");
      printCardUID();
    } else {
      Serial.println("Cartão RFID inválido.");
      printCardUID();
    }
    delay(1000);  // Aguardar um pouco antes de procurar outro cartão
  }
}

bool validCard() {
  // Implemente a lógica para verificar se o cartão é válido aqui
  // Exemplo: verificando se o UID do cartão está na lista de cartões válidos
  // Substitua a lógica abaixo de acordo com a sua aplicação

  // Exemplo de UID de cartão válido (substitua com o UID do seu cartão)
  byte validUID[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x0AC8AB1A};
//0A C8 AB 1A
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] != validUID[i]) {
      return false;  // O UID não corresponde ao cartão válido
    }
  }

  return true;  // UID corresponde ao cartão válido
}

void printCardUID() {
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
