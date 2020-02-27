
#include <SPI.h>
#define ENABLE_GxEPD2_GFX 1

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>

void helloWorld(GxEPD2_GFX& display, const char *msg);
GxEPD2_3C<GxEPD2_270c, GxEPD2_270c::HEIGHT> display(GxEPD2_270c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));

char *ip_to_char(uint32_t ip);

#define COLORED     0
#define UNCOLORED   1

#include <esp_wifi.h>
#include <WiFi.h>
#include <WiFiClient.h>

#define ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())

#define LED_ON      LOW
#define LED_OFF     HIGH

// SSID and PW for Config Portal
String ssid = "ESP_" + String(ESP_getChipId(), HEX);
const char* password = "your_password";

// SSID and PW for your Router
String Router_SSID;
String Router_Pass;

#define USE_AVAILABLE_PAGES     true

#include <ESP_WiFiManager.h>

const int PIN_LED = 22;

void heartBeatPrint(void)
{
  static int num = 1;

  if (WiFi.status() == WL_CONNECTED)
    Serial.print("H");        // H means connected to WiFi
  else
    Serial.print("F");        // F means not connected to WiFi

  if (num == 80)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    Serial.print(" ");
  }
}

void check_status()
{
  static ulong checkstatus_timeout = 0;

  //KH
#define HEARTBEAT_INTERVAL    10000L
  // Print hearbeat every HEARTBEAT_INTERVAL (10) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    heartBeatPrint();
    checkstatus_timeout = millis() + HEARTBEAT_INTERVAL;
  }
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LED_ON);

  // put your setup code here, to run once:
  Serial.begin(115200);

  unsigned long startedAt = millis();

  Serial.println("Hello world");

  delay(100);
  display.init(115200);
  // first update should be full refresh
  helloWorld(display, "Waiting for Network");

  delay(1000);

  digitalWrite(PIN_LED, LED_OFF); // turn the LED on by making the voltage LOW to tell us we are in configuration mode.

  ESP_WiFiManager ESP_wifiManager;
  ESP_wifiManager.setMinimumSignalQuality(-1);
  // Set static IP, Gateway, Subnetmask, DNS1 and DNS2. New in v1.0.5
  ESP_wifiManager.setSTAStaticIPConfig(IPAddress(192, 168, 2, 114), IPAddress(192, 168, 2, 1), IPAddress(255, 255, 255, 0),
                                       IPAddress(192, 168, 2, 1), IPAddress(8, 8, 8, 8));

  // We can't use WiFi.SSID() in ESP32as it's only valid after connected.
  // SSID and Password stored in ESP32 wifi_ap_record_t and wifi_config_t are also cleared in reboot
  // Have to create a new function to store in EEPROM/SPIFFS for this purpose
  Router_SSID = ESP_wifiManager.WiFi_SSID();
  Router_Pass = ESP_wifiManager.WiFi_Pass();

  //Remove this line if you do not want to see WiFi password printed
  Serial.println("Stored: SSID = " + Router_SSID );

  //Check if there is stored WiFi router/password credentials.
  //If not found, device will remain in configuration mode until switched off via webserver.
  Serial.print("Opening configuration portal.");
  digitalWrite(PIN_LED, LED_ON);

  if (Router_SSID != "")
  {
    ESP_wifiManager.setConfigPortalTimeout(60); //If no access point name has been previously entered disable timeout.
    Serial.println("Timeout 60s");
  }
  else
    Serial.println("No timeout");

  // SSID to uppercase
  ssid.toUpperCase();

  //it starts an access point
  //and goes into a blocking loop awaiting configuration
  if (!ESP_wifiManager.startConfigPortal((const char *) ssid.c_str(), password))
    Serial.println("Not connected to WiFi but continuing anyway.");
  else
    Serial.println("WiFi connected...yeey :)");

  digitalWrite(PIN_LED, LED_OFF); // Turn led off as we are not in configuration mode.

  // For some unknown reason webserver can only be started once per boot up
  // so webserver can not be used again in the sketch.
#define WIFI_CONNECT_TIMEOUT        30000L
#define WHILE_LOOP_DELAY            200L
#define WHILE_LOOP_STEPS            (WIFI_CONNECT_TIMEOUT / ( 3 * WHILE_LOOP_DELAY ))

  startedAt = millis();

  while ( (WiFi.status() != WL_CONNECTED) && (millis() - startedAt < WIFI_CONNECT_TIMEOUT ) )
  {
    WiFi.mode(WIFI_STA);
    WiFi.persistent (true);
    // We start by connecting to a WiFi network

    Serial.print("Connecting to ");
    Serial.println(Router_SSID);

    WiFi.begin(Router_SSID.c_str(), Router_Pass.c_str());

    int i = 0;
    while ((!WiFi.status() || WiFi.status() >= WL_DISCONNECTED) && i++ < WHILE_LOOP_STEPS)
    {
      delay(WHILE_LOOP_DELAY);
    }
  }

  Serial.print("After waiting ");
  Serial.print((millis() - startedAt) / 1000);
  Serial.print(" secs more in setup(), connection result is ");

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("connected. Local IP: ");
    Serial.println(WiFi.localIP());
  }
  char *ip_str = ip_to_char(WiFi.localIP());
  Serial.println(ip_str);
  helloWorld(display, ip_str);
}

void loop() {
  // put your main code here, to run repeatedly:
  check_status();
}

static char ipCstring[16]; //size the char array large enough to hold 4 x 3 digit numbers + 3 x dot + null terminator

char *ip_to_char(const uint32_t ip) {
  utoa(((byte*)&ip)[0], ipCstring, 10); //put the first octet in the array
  for (byte octetCounter = 1; octetCounter < 4; ++octetCounter) {
    strcat(ipCstring, ".");
    char octetCstring[4]; //size the array for 3 digit number + null terminator
    utoa(((byte*)&ip)[octetCounter], octetCstring, 10);  //convert the octet to a string
    strcat(ipCstring, octetCstring);
  }
  return ipCstring;
}
void helloWorld(GxEPD2_GFX& display, const char* msg)
{
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(msg, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(msg);
  }
  while (display.nextPage());
}
