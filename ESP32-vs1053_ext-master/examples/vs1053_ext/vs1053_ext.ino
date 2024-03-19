#include <vs1053_ext.h>
#include "Arduino.h"
#include <ESP8266WiFi.h>
//#include "WiFi.h"
#include "SPI.h"
//#include "SD.h"
//#include "FS.h"

// Digital I/O used
//#define SD_CS        5
#define VS1053_MOSI   11
#define VS1053_MISO   12
#define VS1053_SCK    13
#define VS1053_CS      6
#define VS1053_DCS     7
#define VS1053_DREQ    2

String ssid =     "Rede Casa";
String password = "gerlianexavier23";

int volume=15;

VS1053 mp3(VS1053_CS, VS1053_DCS, VS1053_DREQ, VSPI, VS1053_MOSI, VS1053_MISO, VS1053_SCK);

//The setup function is called once at startup of the sketch
void setup() {
    //pinMode(SD_CS, OUTPUT);      digitalWrite(SD_CS, HIGH);
    Serial.begin(115200);
    SPI.begin(VS1053_SCK, VS1053_MISO, VS1053_MOSI);
    //SD.begin(SD_CS);
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) delay(1500);
    mp3.begin();
    mp3.setVolume(volume);
    mp3.connecttohost("streambbr.ir-media-tec.com/berlin/mp3-128/vtuner_web_mp3/");
    //mp3.connecttohost("stream.landeswelle.de/lwt/mp3-192");                 // mp3 192kb/s
    //mp3.connecttohost("http://radio.hear.fi:8000/hear.ogg");                // ogg
    //mp3.connecttohost("tophits.radiomonster.fm/320.mp3");                   // bitrate 320k
    //mp3.connecttohost("http://star.jointil.net/proxy/jrn_beat?mp=/stream"); // chunked data transfer
    //mp3.connecttohost("http://stream.srg-ssr.ch/rsp/aacp_48.asx");          // asx
    //mp3.connecttohost("www.surfmusic.de/m3u/100-5-das-hitradio,4529.m3u");  // m3u
    //mp3.connecttohost("https://raw.githubusercontent.com/schreibfaul1/ESP32-audioI2S/master/additional_info/Testfiles/Pink-Panther.wav"); // webfile
    //mp3.connecttohost("http://stream.revma.ihrhls.com/zc5060/hls.m3u8");    // HLS
    //mp3.connecttohost("https://mirchiplaylive.akamaized.net/hls/live/2036929/MUM/MEETHI_Auto.m3u8"); // HLS transport stream
    //mp3.connecttoFS(SD, "320k_test.mp3"); // SD card, local file
    //mp3.connecttospeech("Wenn die Hunde schlafen, kann der Wolf gut Schafe stehlen.", "de");
}

// The loop function is called in an endless loop
void loop()
{
    mp3.loop();
}

// next code is optional:
void vs1053_info(const char *info) {                // called from vs1053
    Serial.print("DEBUG:        ");
    Serial.println(info);                           // debug infos
}
void vs1053_showstation(const char *info){          // called from vs1053
    Serial.print("STATION:      ");
    Serial.println(info);                           // Show station name
}
void vs1053_showstreamtitle(const char *info){      // called from vs1053
    Serial.print("STREAMTITLE:  ");
    Serial.println(info);                           // Show title
}
void vs1053_showstreaminfo(const char *info){       // called from vs1053
    Serial.print("STREAMINFO:   ");
    Serial.println(info);                           // Show streaminfo
}
void vs1053_eof_mp3(const char *info){              // called from vs1053
    Serial.print("vs1053_eof:   ");
    Serial.print(info);                             // end of mp3 file (filename)
}
void vs1053_bitrate(const char *br){                // called from vs1053
    Serial.print("BITRATE:      ");
    Serial.println(String(br)+"kBit/s");            // bitrate of current stream
}
void vs1053_commercial(const char *info){           // called from vs1053
    Serial.print("ADVERTISING:  ");
    Serial.println(String(info)+"sec");             // info is the duration of advertising
}
void vs1053_icyurl(const char *info){               // called from vs1053
    Serial.print("Homepage:     ");
    Serial.println(info);                           // info contains the URL
}
void vs1053_eof_speech(const char *info){           // called from vs1053
    Serial.print("end of speech:");
    Serial.println(info);
}
void vs1053_lasthost(const char *info){             // really connected URL
    Serial.print("lastURL:      ");
    Serial.println(info);
}
