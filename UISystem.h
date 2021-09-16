/**
 * @author Lucas Balda
 */

#ifndef UI_SYSTEM_H
#define UI_SYSTEM_H

#include <lvgl.h>
#include <math.h>
#include <list>

#include "UIConfig.h"
#include "DiveScreen.h"
#include "IdleScreen.h"
#include "OptionScreen.h"
#include "StatScreen.h"
#include "ButtonType.h"
#include "ScreenType.h"
#include "WifiScreen.h"

#if DIVE_SIMULATION
#define PI 3.14159
#endif

struct DiveData 
{
   int16_t depth = 0;
   int16_t time = 0;
   int16_t temperatur = 0;
   int16_t brightness = 0;
   int16_t o2saturation = 0;
   int16_t heartFrequency = 0;
   int16_t heartVariability = 0;
};

class UISystem 
{
public:
   static DiveData currentDiveData;

   static float gyroX, gyroY, gyroZ;
   static float accelX, accelY, accelZ;
   static float magnetField;
   
   static long time;         
   static bool collectingData;

   static std::list<DiveData> diveDataSeries;        
   static ScreenType currentScreen;

#if DIVE_SIMULATION
   static int16_t diveAccel;
#endif
        
   static void setup();
   static void start();
   static void initializeButtons(lv_obj_t* screen);
   static void setScreen(ScreenType);       
};

#endif
