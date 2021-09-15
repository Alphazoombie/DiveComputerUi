#ifndef TOUCH_H
#define TOUCH_H

using namespace std;
#include <lvgl.h>
#include <vector>
#include <SPI.h>
#include <Wire.h> 
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include <Adafruit_STMPE610.h>

struct Area {
    int id;
    int x;
    int y;
    int width;
    int height;
};

static vector<Area> areas;

class Touch{
  public:
    //static vector<Area> areas;
    static void touchEvent();
    static void registerArea(int id, int x, int y, int width, int height);
    static void clearRegister();
    static void drawAreas();
    static int areaTouched(int x, int y);
};

#endif