/**
   @author Lucas Balda
   modified by Benjamin Bube
*/

#include "UISystem.h"
MS5837 sensor;

DiveData UISystem::currentDiveData = DiveData();
bool UISystem::collectingData = false;
std::list<DiveData> UISystem::diveDataSeries = std::list<DiveData>();

FileSystem UISystem::fileSystem;
Datalogger datalogger;

long UISystem::time = 0;

CustomTouchButton buttonActivate = CustomTouchButton(T5); 
CustomTouchButton buttonSelect = CustomTouchButton(T0);

char jsonString[200];
char UISystem::directoryName[10];
char tempFileName[6];
char directoryPathAndDirectoryName[19];
char fullFilePath[28];
char fileName[10];
char diveID[4];

float UISystem::depth = 0.0;

bool simulateDive = false;
bool isPathBuilt = false;
bool isInDive = false;


ScreenType UISystem::currentScreen;

void UISystem::setup() 
{
  lv_init();

  DiveScreen::setup();
  IdleScreen::setup();
  OptionScreen::setup();
  StatScreen::setup();
  WifiScreen::setup();
  SettingsScreen::setup();

  lv_task_create([](lv_task_t* task) 
  {
    // sensor.read();
    // currentDiveData.depth =sensor.depth();
    // currentDiveData.time++;
    // currentDiveData.temperatur = sensor.temperature();

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
      case SETTINGS_SCREEN:
        SettingsScreen::update();
        break;
    }
  }, 500, LV_TASK_PRIO_MID, NULL);

  lv_task_create([](lv_task_t* task) 
  {
    time = lv_tick_get() / 1000;

    if (true)//collectingData) 
    {
      currentDiveData.time++;
      //diveDataSeries.push_back(currentDiveData);
    }
  }, 1000, LV_TASK_PRIO_MID, NULL);

  lv_task_create([](lv_task_t* task) 
  {
    // if (isUnderwater())
    // {
    //   // ToDo: Insert here your Data Update Logic
    //   Serial.println("IN DIVE TASK...***()§)(");

    // }
    handleDiveLogic();
  }, 100, LV_TASK_PRIO_MID, NULL);

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
  else if (screenType == SETTINGS_SCREEN)
  {    
    SettingsScreen::showScreen();
  } 
  currentScreen = screenType;
}

void UISystem::start() 
{
  lv_task_handler(); /* let the GUI do its work */
  //setScreen(currentScreen);
  delay(5);
}

void UISystem::handleDiveLogic()
{
  if(isUnderwater())
    {
        if (!UISystem::fileSystem.m_sameSession)
        {
            Serial.println("inside not sameSession");
            UISystem::fileSystem.writeDateToSessionFile(UISystem::fileSystem.m_current_date);
            UISystem::fileSystem.m_sameSession = true;
            Helper::concatCharArrays(directoryPathAndDirectoryName, UISystem::fileSystem.m_directoryPath, directoryName);
            SD.mkdir(directoryPathAndDirectoryName);
            Serial.print("directoryPathAndDirectoryName -> ");
            Serial.println(directoryPathAndDirectoryName);
        }
        if(!isPathBuilt)
        {
            Serial.println("inside isPathBuilt");
            snprintf(diveID, sizeof (diveID), "%i", fileSystem.m_diveID);
            Serial.println("1");
            Serial.println(diveID);
            Helper::concatCharArrays(tempFileName, "/d_", diveID);
            Serial.println("2");
            Serial.println(tempFileName);
            Helper::concatCharArrays(fileName, tempFileName, ".log");
            Serial.println("3");
            Serial.println(fileName);
            Helper::concatCharArrays(directoryPathAndDirectoryName, UISystem::fileSystem.m_directoryPath, directoryName);
            Helper::concatCharArrays(fullFilePath, directoryPathAndDirectoryName, fileName);
            Serial.println("4");
            Serial.println(fullFilePath);
            isPathBuilt = true;
            char time[9];
            PeripheralManager::getCurrentTime(time);
            File file = SD.open(fullFilePath, FILE_APPEND);
            if(file)
            {
              file.println(time);
              file.close();
            }
        }
        Serial.print("fileName -> ");
        Serial.println(fileName);
        Serial.print("fullFilePath -> ");
        Serial.println(fullFilePath);
        datalogger.getData(&fileSystem);
        datalogger.logData(&fileSystem, fullFilePath);
    }
    else
    {
      handleButtons();
    }
}

void UISystem::handleButtons()
{
    /*Setup OnClick EventListeners for the buttons to navigate through the menu*/
      buttonSelect.btnClickEventListener([](void)
      {
          if(UISystem::currentScreen == IDLE_SCREEN)
          {
            UISystem::currentScreen = OPTION_SCREEN;
          }
          else if(UISystem::currentScreen == DIVE_SCREEN)
          {
            UISystem::currentScreen = OPTION_SCREEN;
          }
          else if(UISystem::currentScreen == OPTION_SCREEN)
          {  
            OptionScreen::processButtonPress(BUTTON_ACTIVATE);
          }
          else if(UISystem::currentScreen == STAT_SCREEN)
          {     
            UISystem::currentScreen = OPTION_SCREEN;    
          }
          else if(UISystem::currentScreen == WIFI_SCREEN)
          {     
            WifiScreen::processButtonPress(BUTTON_ACTIVATE); 
          }
          else if(UISystem::currentScreen == SETTINGS_SCREEN)
          {     
            SettingsScreen::processButtonPress(BUTTON_ACTIVATE);     
          }
          UISystem::setScreen(UISystem::currentScreen);
      });

      buttonActivate.btnClickEventListener([](void)
      {
        if(UISystem::currentScreen == OPTION_SCREEN)
        {
          OptionScreen::processButtonPress(BUTTON_SELECT);
        }
        else if(UISystem::currentScreen == WIFI_SCREEN)
        {
          WifiScreen::processButtonPress(BUTTON_SELECT);
        }   
        else if(UISystem::currentScreen == SETTINGS_SCREEN)
        {
          SettingsScreen::processButtonPress(BUTTON_SELECT);
        } 
      });
}

bool UISystem::isUnderwater()
{
    if(UISystem::depth >= 1)
    {
        isInDive = true;
        return true;
    }
    else
    {
        if(isInDive)
        {
            isPathBuilt = false;
            isInDive = false;
            UISystem::fileSystem.m_diveID++;
            UISystem::fileSystem.setDiveID();
        }
        return false;
    }
}