/*



*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
    
const char* ssid = "Rede Casa";
const char* password = "gerlianexavier23";

WebServer server(80);


#define IMMEDIATE_NEW   0     // if 1 will immediately display a new message
#define USE_POT_CONTROL 1
#define PRINT_CALLBACK  0

#define PRINT(s, v) { Serial.print(F(s)); Serial.print(v); }

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

// GPIO pins
#define CLK_PIN   18 // VSPI_SCK
#define DATA_PIN  23 // VSPI_MOSI
#define CS_PIN    5  // VSPI_SS

// SPI hardware interface
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary pins
//MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// Scrolling parameters
#if USE_POT_CONTROL
#define SPEED_IN  A5
#else
#define SCROLL_DELAY  75  // in milliseconds
#endif // USE_POT_CONTROL

#define CHAR_SPACING  1 // pixels between characters

// Global message buffers shared by Serial and Scrolling functions
#define BUF_SIZE  75
uint8_t curMessage[BUF_SIZE] = { "  Web Mesage On ESP32   " };
uint8_t newMessage[BUF_SIZE];
bool newMessageAvailable = false;

uint16_t  scrollDelay;  // in milliseconds


// set size of null Spaces
String nullSpace = "    ";

void scrollDataSink(uint8_t dev, MD_MAX72XX::transformType_t t, uint8_t col)
// Callback function for data that is being scrolled off the display
{
#if PRINT_CALLBACK
  Serial.print("\n cb ");
  Serial.print(dev);
  Serial.print(' ');
  Serial.print(t);
  Serial.print(' ');
  Serial.println(col);
#endif
}

uint8_t scrollDataSource(uint8_t dev, MD_MAX72XX::transformType_t t)
// Callback function for data that is required for scrolling into the display
{
  static uint8_t* p = curMessage;
  static enum { NEW_MESSAGE, LOAD_CHAR, SHOW_CHAR, BETWEEN_CHAR } state = LOAD_CHAR;
  static uint8_t  curLen, showLen;
  static uint8_t  cBuf[15];
  uint8_t colData = 0;    // blank column is the default

#if IMMEDIATE_NEW
  if (newMessageAvailable)  // there is a new message waiting
  {
    state = NEW_MESSAGE;
    mx.clear(); // clear the display
  }
#endif

  // finite state machine to control what we do on the callback
  switch(state)
  {
    case NEW_MESSAGE:   // Load the new message
      memcpy(curMessage, newMessage, BUF_SIZE);  // copy it in
      newMessageAvailable = false;    // used it!
      p = curMessage;
      state = LOAD_CHAR;
      break;

    case LOAD_CHAR: // Load the next character from the font table
      showLen = mx.getChar(*p++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);
      curLen = 0;
      state = SHOW_CHAR;

      // if we reached end of message, opportunity to load the next
      if (*p == '\0')
      {
        p = curMessage;     // reset the pointer to start of message
#if !IMMEDIATE_NEW
        if (newMessageAvailable)  // there is a new message waiting
        {
          state = NEW_MESSAGE;    // we will load it here
          break;
        }
#endif
      }
      // !! deliberately fall through to next state to start displaying

    case SHOW_CHAR: // display the next part of the character
      colData = cBuf[curLen++];
      if (curLen == showLen)
      {
        showLen = CHAR_SPACING;
        curLen = 0;
        state = BETWEEN_CHAR;
      }
      break;

    case BETWEEN_CHAR: // display inter-character spacing (blank columns)
      colData = 0;
      curLen++;
      if (curLen == showLen)
        state = LOAD_CHAR;
      break;

    default:
      state = LOAD_CHAR;
  }

  return(colData);
}

 void scrollText(void)
{
  static uint32_t prevTime = 0;

  // Is it time to scroll the text?
  if (millis()-prevTime >= scrollDelay)
  {
    mx.transform(MD_MAX72XX::TSL);  // scroll along - the callback will load all the data
    prevTime = millis();      // starting point for next time
  }
}

uint16_t getScrollDelay(void)
{
#if USE_POT_CONTROL
  uint16_t  t;

  t = analogRead(SPEED_IN);
  t = map(t, 0, 1023, 25, 250);

  return(t);
#else
  return(SCROLL_DELAY);
#endif
}


void setup() {
  Serial.begin(9600);
  
  delay(2000);

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
  //Printing IP Address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start Web Server 
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
    html += "<h2>Mensagem:</h2>";
    html += "<form action='/input' method='get'>";
    html += "<div class='form-group'>";
    html += "<input type='text' class='form-control' name='data'>";
    html += "</div>";
    html += "<button type='submit' class='btn btn-primary'>Enviar</button>";
    html += "</form>";
    html += "</div>";
    html += "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js'></script>";
    html += "<script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.16.0/umd/popper.min.js'></script>";
    html += "<script src='https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js'></script>";
    html += "</body>";
    html += "</html>";
    server.send(200, "text/html", html);
  });

  server.on("/input", HTTP_GET, []() {
    if (server.hasArg("data")) {
      String data = server.arg("data");
      //debug data from server.arg 
      //Serial.println(data);
      server.sendHeader("Location", "/");
      //Server response if it receives the message via the GET method      
      //
      //server.send(200, "text/plain", "Data received successfully!");
      //You can send a message from any browser on your local network using
      //YourIpEsp32/
      server.send(302, "text/plain", "");
    } else {
      server.send(400, "text/plain", "Erro: Nenhum dado recebido");
    }
  });

  server.begin();
  Serial.println("Servidor iniciado");
  mx.begin();
  mx.setShiftDataInCallback(scrollDataSource);
  mx.setShiftDataOutCallback(scrollDataSink);

  #if USE_POT_CONTROL
    pinMode(SPEED_IN, INPUT);
  #else
    scrollDelay = SCROLL_DELAY;
  #endif
  newMessage[0] = '\0';
}

void readWebServer() {
  if (server.hasArg("data")) {
    String data = nullSpace + server.arg("data") + nullSpace;
    // Copiar os dados de data.c_str() para newMessage
    memcpy(newMessage, data.c_str(), data.length() + 1); // +1 para incluir o caractere nulo de término de string
    newMessageAvailable = true;
  }
} 

void loop() {
  server.handleClient();
  readWebServer();
  scrollDelay = getScrollDelay();
  //readSerial();
  scrollText();
}
