#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <ESPDateTime.h>
#include <string>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "0fab68b8-eb4d-4abc-9d6c-89fd057d5572"
#define CHARACTERISTIC_UUID "d653575e-2b5b-4db2-a6c8-b93ad899690f"
#define CHARACTERISTIC_MILLI_UUID "30989c48-03c0-4faa-8123-d27daee72554"


// Define must come before GxEPD2 includes
#define ENABLE_GxEPD2_GFX 1
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold18pt7b.h>
// 264x176

void timestamp();
void initDisplay(GxEPD2_GFX& display);
void helloWorld(GxEPD2_GFX& display, const char *msg);
GxEPD2_3C<GxEPD2_270c, GxEPD2_270c::HEIGHT> display(GxEPD2_270c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));

#define ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())

#define LED_ON      LOW
#define LED_OFF     HIGH

String deviceName = "ESP_" + String(ESP_getChipId(), HEX);

const int PIN_LED = 22;

BLECharacteristic *pCharacteristicMsg;
BLECharacteristic *pCharacteristicMilli;
void setup() {
  pinMode(PIN_LED, OUTPUT);

  // put your setup code here, to run once:
  Serial.begin(115200);
  DateTime.setTime(1262304000L);
  initDisplay(display);

  Serial.println("Starting BLE work!");

  BLEDevice::init(deviceName.c_str());
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristicMsg = pService->createCharacteristic(
                         CHARACTERISTIC_UUID,
                         BLECharacteristic::PROPERTY_READ |
                         BLECharacteristic::PROPERTY_WRITE
                       );

  pCharacteristicMsg->setValue("Hello World");
  pCharacteristicMilli = pService->createCharacteristic(
                           CHARACTERISTIC_MILLI_UUID,
                           BLECharacteristic::PROPERTY_READ |
                           BLECharacteristic::PROPERTY_WRITE
                         );
  pCharacteristicMilli->setValue("");
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  digitalWrite(PIN_LED, LED_ON);

  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

std::string lastValue = "";

int lastMin = -1;

void loop() {
  std::string milli = pCharacteristicMilli->getValue();
  if (milli != "" ) {
    DateTime.setTime(atol(milli.c_str()));
    pCharacteristicMilli->setValue("");
  }
  // put your main code here, to run repeatedly:
  std::string value = pCharacteristicMsg->getValue();
  if (value != lastValue ) {
    if (value[0] == '!') display.setTextColor(GxEPD_RED);
    else display.setTextColor(GxEPD_BLACK);
    helloWorld(display, value.c_str());
    lastValue = value;
    lastMin = -1;
  }
  int min = DateTime.getParts().getMinutes();
  if (lastMin != min) {
    lastMin = min;
    timestamp();
  }
  delay(1000);
}

void initDisplay(GxEPD2_GFX& display) {
  display.init(115200);
  display.setRotation(1);
  display.setFont(&FreeMonoBold18pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.display(false);
}

void printDigits(byte digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  display.print(":");
  if (digits < 10)
    display.print('0');
  display.print(digits, DEC);
}

void timestamp() {
  display.setPartialWindow(0, 0, 128, 32);
  display.firstPage();
  do {
    display.fillRect(0, 0, 128, 32, GxEPD_WHITE);
    display.drawRect(1, 1, 126, 30, GxEPD_BLACK);
    display.drawRect(2, 2, 125, 29, GxEPD_BLACK);
    display.setCursor(12, 26);
    display.setTextColor(GxEPD_BLACK);
    String nowStr = DateTime.format("%l:%M");
    display.print(nowStr);
  }
  while (display.nextPage());
}

void helloWorld(GxEPD2_GFX& display, const char* msg)
{
  display.setFullWindow();
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(msg, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.fillScreen(GxEPD_WHITE);
  display.setCursor(x, y);
  display.print(msg);
  display.display(false);
}
