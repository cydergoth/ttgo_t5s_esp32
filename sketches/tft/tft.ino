#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

#define PIN_LED 5
#define ON LOW
#define OFF HIGH
#define TFT_GREY 0x5AEB

#define TFT_BL_ON HIGH
#define TFT_BL_OFF LOW

void setup() {
  tft.init();
  tft.setRotation(1);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED,ON);
  pinMode(TFT_BL,OUTPUT);
  digitalWrite(TFT_BL,TFT_BL_ON);
}

void loop() {
  // Fill screen with grey so we can see the effect of printing with and without
  // a background colour defined
  tft.fillScreen(TFT_GREY);

  // Set "cursor" at top left corner of display (0,0) and select font 2
  // (cursor will move to next line automatically during printing with 'tft.println'
  //  or stay on the line is there is room for the text with tft.print)
  tft.setCursor(0, 0, 2);
  // Set the font colour to be white with a black background, set text size multiplier to 1
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  tft.setTextSize(1);
  // We can now plot text on screen using the "print" class
  tft.println("Hello World!");


  while (true) {
    delay(1000);
  }
}
