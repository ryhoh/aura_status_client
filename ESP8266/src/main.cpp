#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "StatusClient.h"

extern "C" {
  #include "DotMatrixLED.h"
}

const String WIFI_SSID = "your_ssid";
const String WIFI_PASSWORD = "your_password";

const uint8_t LED = 13;

Max7219 max7219;
MatrixLED matrixLEDs[8];

void setup() {
  pinMode(LED, OUTPUT);

  initMax7219(&max7219, 5, 15, 12, 2);
  testRunMax7219(&max7219);
  for (uint8_t i = 0; i < 8; ++i) {
    initMatrixLED(&matrixLEDs[i], 8, 8);
  }

  writeAsciisToMatrixLEDs(matrixLEDs, 8, "Hello", 0);
  flushMatrixLEDsByMax7219(&max7219, matrixLEDs, 8);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }
  delay(100);

  configTzTime("JST-9", "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
}

void loop() {
  auto res = postStatusToBoard("ELIC101");
  // if (res.ok()) {
    for (uint8_t i = 0; i < 8; ++i) {
      fillMatrixLED(&matrixLEDs[i], false);
    }
    writeAsciisToMatrixLEDs(matrixLEDs, 8, res.retval.c_str(), 0);
    flushMatrixLEDsByMax7219(&max7219, matrixLEDs, 8);
  // }

  delay(10000);
}
