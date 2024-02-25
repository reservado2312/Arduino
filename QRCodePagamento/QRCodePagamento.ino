#include <qrcode.h>

#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>
//

MCUFRIEND_kbv tft;

void setup() {
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  tft.begin(ID);

  tft.setRotation(1);
  tft.fillScreen(TFT_WHITE);

  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(6)];
  qrcode_initText(&qrcode, qrcodeData, 6, ECC_LOW, "00020126580014BR.GOV.BCB.PIX013693edc682-46e1-4f90-8474-adff635fb99952040000530398654042.005802BR5925Jonathan Rafael Teixeira 6009SAO PAULO62140510cCG4mkefWy63046BDF");

  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      uint8_t qrcodeByte = qrcode_getModule(&qrcode, x, y);
      if (qrcodeByte) {
        tft.fillRect(x * 6, y * 6, 6, 6, TFT_BLACK);
      }
    }
  }
}

void loop() {}
