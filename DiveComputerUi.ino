/**
   @author Benjamin Bube
*/

#include <lvgl.h>
#include "UISystem.h"
#include <TFT_eSPI.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <SPI.h>
//#include <ArduinoOTA.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "Touch.h"
#include "CustomTouchButton.h"


TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

int m_chipSelect = 14;

bool isUp = true;

CustomTouchButton diveButton = CustomTouchButton(T7);

//int port = 21;
//WiFiServer wifiServer;


void initializeSD() 
{
    Serial.print("Initializing SD card...");
    // see if the card is present and can be initialized:
    SD.begin(m_chipSelect);
    if (!SD.begin(m_chipSelect)) 
    {
      Serial.println("Card failed, or not present");
      // don't do anything more:
      while (1);
    }
    Serial.println(" card initialized.");
}



void buildDirectoryName()
{
    Helper::concatCharArrays(UISystem::directoryName, "/", UISystem::fileSystem.m_current_date);
}

#if USE_LV_LOG != 0
/* Serial debugging */
void my_print(lv_log_level_t level, const char * file, uint32_t line, const char * dsc)
{

  Serial.printf("%s@%d->%s\r\n", file, line, dsc);
  Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
   uint32_t w = (area->x2 - area->x1 + 1);
   uint32_t h = (area->y2 - area->y1 + 1);   
   tft.startWrite();
   tft.setAddrWindow(area->x1, area->y1, w, h);
   tft.pushColors(&color_p->full, w * h, true);
   tft.endWrite();   
   lv_disp_flush_ready(disp);
}

void setup()
{
  void SetMillis();
  Serial.begin(115200); /* prepare for possible serial debug */
  Serial.println("Booting");
  SPI.begin(5, 19, 18);
  lv_init();
#if USE_LV_LOG != 0
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

  tft.begin(); /* TFT init */
  tft.setRotation(1); /* Landscape orientation */

  initializeSD();

  uint16_t calData[5] = { 275, 3620, 264, 3532, 1 };
  tft.setTouch(calData);

  lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

  /*Initialize the display*/
  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 320;
  disp_drv.ver_res = 240;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  //indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  delay(5000);

  UISystem::fileSystem.createWifiDataFile();
  UISystem::fileSystem.initializeMetaData();

  buildDirectoryName();


  // wifiServer = WiFiServer(port);
  // WiFi.mode(WIFI_AP);
  // WiFi.softAP("Test1", "123");
  
  /*
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  
  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();
  
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  */


  /*The User Interface Initializer*/
  UISystem::setup();
  //Touch::touchEvent();
  //Touch::registerArea(1, 5, 5, 40, 40);

  UISystem::setScreen(IDLE_SCREEN);
}

void loop() 
{
  /* UI updater for refreshing the display*/
  UISystem::start();

  diveButton.btnClickEventListener([](void)
  { 
      if(isUp)
      {
          UISystem::depth = 2.0;
          isUp = false;
      }
      else
      {
          UISystem::depth = 0.0;
          isUp = true;
      }
  });

  

  

  //Touch::drawAreas();
  /*Search for updates*/
  /*This Method will later be called manually in the settings, to decrease energy consumption*/
  //ArduinoOTA.handle()
}
