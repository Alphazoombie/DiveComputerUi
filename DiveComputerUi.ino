/**
 * @author Manuel Bäcker
 * @author Jan Tröger
 * @author Tim Tröger
 * @author Benjamin Bube
 */

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "UISystem.h"
#include "Touch.h"
#include "TouchButton.h"
#include "WifiManager.h"

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

int m_chipSelect = 14;

bool isUp = true;

TouchButton diveButton = TouchButton(T7);

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
    Serial.println("BuildDirectoryName Call:");
    Serial.print("Current Data Param: ");
    Serial.println(UISystem::fileSystem.m_current_date);
    Serial.print("Directory Name Param: ");
    Serial.println(UISystem::directoryName);
    Helper::concatCharArrays(UISystem::directoryName, "/", UISystem::fileSystem.m_current_date);
    Serial.println(UISystem::directoryName);
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
    Serial.println("Booting...");
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

    UISystem::fileSystem.createWifiDataFile();
    UISystem::fileSystem.initializeMetaData();
    buildDirectoryName(); 

    /*Initialize User Interface and Touch handling*/
    UISystem::setup();
    Touch::setup();

    UISystem::setScreen(IDLE_SCREEN);
}

void loop() 
{
    /*UI updater for refreshing the display*/
    UISystem::start();

    diveButton.addTouchEvent([](void)
    { 
        if (isUp)
        {
            Serial.println("Starting dive...");
            UISystem::depth = 2.0;
            isUp = false;
            UISystem::goin = true;
        }
        else
        {
            Serial.println("Stopping dive...");
            UISystem::depth = 0.0;
            UISystem::currentDiveData.depth = 0;
            isUp = true;
            UISystem::goin = true;
        }
    });
    
    if (WifiManager::wifiServerActive)
    {
        Serial.println("FTP Server listening...");
        WifiManager::m_ftpServer.handleFTP(); 
    }

    if (SettingsScreen::otaActive)
    {
        Serial.println("OTA running...");
        OtaManager::checkForUpdates();
    }

    Touch::handleTouch();
}
