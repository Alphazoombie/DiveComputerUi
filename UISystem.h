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
#include "SettingsScreen.h"
#include <Wire.h>
#include "MS5837.h"
#include "FileSystem.h"
#include "Datalogger.h"
#include "Helper.h"
#include "CustomTouchButton.h"

#if DIVE_SIMULATION
#define PI 3.14159
#endif



class UISystem 
{
public:
   static DiveData currentDiveData;

   static float gyroX, gyroY, gyroZ;
   static float accelX, accelY, accelZ;
   static float magnetField;
   static float depth;
   
   static long time;         
   static bool collectingData;

   static char directoryName[10];

   static std::list<DiveData> diveDataSeries;        
   static ScreenType currentScreen;
   static FileSystem fileSystem;
   
   static bool goin;

#if DIVE_SIMULATION
   static int16_t diveAccel;
#endif
        
   static void setup();
   static void start();
   static void initializeButtons(lv_obj_t* screen);
   static void setScreen(ScreenType);      
   static void handleButtons();
   static bool isUnderwater();
   static void handleDiveLogic(); 
};

#endif
