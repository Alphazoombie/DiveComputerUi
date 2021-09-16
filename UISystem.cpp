/**
   @author Lucas Balda
   modified by Benjamin Bube
*/

#include "UISystem.h"
#include <Wire.h>
#include "MS5837.h"
MS5837 sensor;

DiveData UISystem::currentDiveData = DiveData();
bool UISystem::collectingData = false;
std::list<DiveData> UISystem::diveDataSeries = std::list<DiveData>();

float UISystem::gyroX = 0, UISystem::gyroY = 0, UISystem::gyroZ = 0;
float UISystem::accelX = 0, UISystem::accelY = 0, UISystem::accelZ = 0;
float UISystem::magnetField = 0;
long UISystem::time = 0;


ScreenType UISystem::currentScreen;

void UISystem::setup() 
{
  lv_init();

  DiveScreen::setup();
  IdleScreen::setup();
  OptionScreen::setup();
  StatScreen::setup();
  WifiScreen::setup();

  lv_task_create([](lv_task_t* task) 
  {
    // ToDo: Insert here your Data Update Logic
    sensor.read();
    currentDiveData.depth =sensor.depth();
    currentDiveData.time++;
    currentDiveData.temperatur = sensor.temperature();

    switch (currentScreen) 
    {
      case DIVE_SCREEN:
        DiveScreen::update();
        break;
      case IDLE_SCREEN:
        IdleScreen::update();
        break;
      case OPTION_SCREEN:
        OptionScreen::update();
        break;
      case STAT_SCREEN:
        StatScreen::update();
        break;
      case WIFI_SCREEN:
        WifiScreen::update();
        break;
    }
  }, 500, LV_TASK_PRIO_MID, NULL);

  lv_task_create([](lv_task_t* task) 
  {
    time = lv_tick_get() / 1000;

    if (collectingData) 
    {
      currentDiveData.time++;
      diveDataSeries.push_back(currentDiveData);
    }
  }, 1000, LV_TASK_PRIO_MID, NULL);

  //setScreen(IDLE_SCREEN);

}

void UISystem::setScreen(ScreenType screenType) 
{
  if (screenType == IDLE_SCREEN)
  {
    IdleScreen::showScreen();
  }
  else if (screenType == DIVE_SCREEN)
  {
    currentDiveData.time = 0;
    collectingData = true;
    diveDataSeries.clear();
    diveDataSeries.push_back(currentDiveData);
    DiveScreen::showScreen();
  }
  else if (screenType == OPTION_SCREEN)
  {
    OptionScreen::showScreen();
  }
  else if (screenType == STAT_SCREEN)
  {
    collectingData = false;
    StatScreen::showScreen();
  } 
  else if (screenType == WIFI_SCREEN)
  {    
    WifiScreen::showScreen();
  } 
  currentScreen = screenType;
}

void UISystem::start() 
{
  lv_task_handler(); /* let the GUI do its work */
  //setScreen(currentScreen);
  delay(5);
}
