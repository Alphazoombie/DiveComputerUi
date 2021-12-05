#include "Touch.h"

#define STMPE_CS 32
#define TFT_CS   15
#define TFT_DC   33
#define SD_CS    14

#define TS_MINX 3800
#define TS_MAXX 100
#define TS_MINY 100
#define TS_MAXY 3750

Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

void Touch::setup()
{
    if (!ts.begin()) 
    {
        Serial.println("Failed to initialize touchscreen controller");
        while (1);
    }
}

void Touch::registerArea(int id, int x, int y, int width, int height)
{
    areas.push_back(Area());
    areas.back().id = id;
    areas.back().x = x;
    areas.back().y = y;
    areas.back().width = width;
    areas.back().height = height;
};

void Touch::clearRegister()
{
    areas.clear();
};

int Touch::areaTouched()
{
    TS_Point p = ts.getPoint();
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
    p.x = 240 - p.x;
  
    if(ts.touched() && p.x > 0 && p.y > 0)
    {
        for(int i = 0; i < areas.size(); i++)
        {
            if((areas[i].x <= p.y && p.y <= areas[i].x + areas[i].width)
                && (areas[i].y <= p.x && p.x <= areas[i].y + areas[i].height))
            {        
                return i; 
            }       
        }
    }
  
    return -1;
}

void Touch::handleTouch()
{
    int areaIndex = Touch::areaTouched();

    if (UISystem::currentScreen == OPTION_SCREEN)
    {
        if (OptionScreen::selectionIndex == areaIndex)
        {
            OptionScreen::processButtonPress(BUTTON_ACTIVATE, areaIndex); 
        }
        else
        {
            OptionScreen::processButtonPress(BUTTON_SELECT, areaIndex); 
        }
    }
    else if (UISystem::currentScreen == WIFI_SCREEN)
    {
        if (WifiScreen::selectionIndex == (areaIndex + 1))
        {
            WifiScreen::processButtonPress(BUTTON_ACTIVATE, areaIndex); 
        }
        else
        {
            WifiScreen::processButtonPress(BUTTON_SELECT, areaIndex); 
        }
    }
    else if (UISystem::currentScreen == SETTINGS_SCREEN)
    {
        if (SettingsScreen::selectionIndex == (areaIndex + 1))
        {
            SettingsScreen::processButtonPress(BUTTON_ACTIVATE, areaIndex); 
        }
        else
        {
            SettingsScreen::processButtonPress(BUTTON_SELECT, areaIndex); 
        }
    }               
    else
    {
        UISystem::setScreen(OPTION_SCREEN);
    }   
}