#include "Touch.h"


#define STMPE_CS 32
#define TFT_CS   15
#define TFT_DC   33
#define SD_CS    14

Adafruit_ILI9341 tft2 = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

void Touch::touchEvent()
{
   tft2.begin();
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

void Touch::clearRegister(){
  areas.clear();
};

void Touch::drawAreas()
{
   for(int i = 0; i < areas.size(); i++)
   {
     //tft2.drawRect(areas[i].x, areas[i].y, areas[i].width, areas[i].height, ILI9341_MAGENTA);   
     Serial.println(areas[i].x);
   }
};

int Touch::areaTouched(int x, int y)
{
   for(int i = 0; i < areas.size(); i++)
   {
     if((areas[i].x <= x && x <= areas[i].x + areas[i].width)
         && (areas[i].y <= y && y <= areas[i].y + areas[i].height))
     {
        return i; 
     }       
   }
   return -1;
}
