#include "SettingsScreen.h"
#include "Touch.h"

lv_obj_t* SettingsScreen::screenObj;
int16_t SettingsScreen::selectionIndex;
lv_obj_t* SettingsScreen::buttons[3];
lv_obj_t* SettingsScreen::btnOtaData;
lv_obj_t* SettingsScreen::btnOta;
lv_obj_t* SettingsScreen::btnSettingsBack;
lv_style_t SettingsScreen::styleSelected;
lv_style_t SettingsScreen::styleDefault;

bool SettingsScreen::otaActive;

char ssidS[30];
char passwordS[30];
char wifiAccessDataStringS[54];

enum WifiScreenButtons
{
    BUTTON_WIFI_DATA = 0,
    BUTTON_OTA_ACTIVATION = 1,
    BUTTON_BACK = 2  
};

void SettingsScreen::setup() 
{
    screenObj = lv_obj_create(NULL, NULL);

    // Change screen-background-color
    lv_obj_set_style_local_bg_color(screenObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARK);

    static int16_t btnWidth = 140;
    static int16_t btnHeight = 100;
    int16_t spacingX = (SCREEN_WIDTH - btnWidth * 2) / 3;
    int16_t spacingY = (SCREEN_HEIGHT - btnHeight * 2) / 3;    

    buttons[BUTTON_WIFI_DATA] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[BUTTON_WIFI_DATA], spacingX, spacingY);
    lv_obj_set_size(buttons[BUTTON_WIFI_DATA], btnWidth * 2 + spacingX, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[BUTTON_WIFI_DATA], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[BUTTON_WIFI_DATA], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
    lv_obj_set_style_local_radius(buttons[BUTTON_WIFI_DATA], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[BUTTON_WIFI_DATA], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    btnOtaData = lv_label_create(buttons[BUTTON_WIFI_DATA], NULL);    
    lv_obj_set_style_local_text_color(btnOtaData, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

    createSampleData();
    readWifiAccessData(ssidS, passwordS);
    buildWifiStatusString(ssidS, "disconnected", wifiAccessDataStringS);

    lv_label_set_text(btnOtaData, wifiAccessDataStringS);

    buttons[BUTTON_OTA_ACTIVATION] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[BUTTON_OTA_ACTIVATION], spacingX, spacingY * 2 + btnHeight);
    lv_obj_set_size(buttons[BUTTON_OTA_ACTIVATION], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[BUTTON_OTA_ACTIVATION], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[BUTTON_OTA_ACTIVATION], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 3);
    lv_obj_set_style_local_radius(buttons[BUTTON_OTA_ACTIVATION], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[BUTTON_OTA_ACTIVATION], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    btnOta = lv_label_create(buttons[BUTTON_OTA_ACTIVATION], NULL);    
    lv_obj_set_style_local_text_color(btnOta, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(btnOta, "Ota [off]");

    buttons[BUTTON_BACK] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[BUTTON_BACK], spacingX * 2 + btnWidth, spacingY * 2 + btnHeight);
    lv_obj_set_size(buttons[BUTTON_BACK], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[BUTTON_BACK], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[BUTTON_BACK], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
    lv_obj_set_style_local_radius(buttons[BUTTON_BACK], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[BUTTON_BACK], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_t * label4 = lv_label_create(buttons[BUTTON_BACK], NULL);    
    lv_obj_set_style_local_text_color(label4, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(label4, "Back");    
}

void SettingsScreen::showScreen() 
{
    int16_t btnWidth = 140;
    int16_t btnHeight = 100;
    int16_t spacingX = (SCREEN_WIDTH - btnWidth * 2) / 3;
    int16_t spacingY = (SCREEN_HEIGHT - btnHeight * 2) / 3; 

    Touch::clearRegister();    
    Touch::registerArea(1, spacingX, spacingY * 2 + btnHeight, btnWidth, btnHeight);
    Touch::registerArea(2, spacingX * 2 + btnWidth, spacingY * 2 + btnHeight, btnWidth, btnHeight);

    lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
    selectionIndex = 1;
    lv_obj_set_style_local_border_width(buttons[BUTTON_OTA_ACTIVATION], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 3);
    Serial.println("connection status: ");
    Serial.println(WifiManager::connectionFailed);
    if (otaActive && WifiManager::isConnected())
    {
        buildWifiStatusString(ssidS, "connected", wifiAccessDataStringS);
        lv_label_set_text(btnOtaData, wifiAccessDataStringS);
    }
    else if (WifiManager::connectionFailed)
    {
        Serial.println("going in");
        buildWifiStatusString(ssidS, "connection failed", wifiAccessDataStringS);
        lv_label_set_text(btnOtaData, wifiAccessDataStringS);
    }    
    else
    {
        buildWifiStatusString(ssidS, "disconnected", wifiAccessDataStringS);
        lv_label_set_text(btnOtaData, wifiAccessDataStringS);
    }
    
    lv_scr_load(SettingsScreen::screenObj);
}

void SettingsScreen::processButtonPress(ButtonType buttonType, int index) 
{
    if(buttonType == BUTTON_SELECT) 
    {
        if (index == -1)
        {
            lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
            selectionIndex++;
            selectionIndex %= 3;

            if (selectionIndex == 0)
            {
                selectionIndex++;
            }
            
            lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 3);
        }
        else
        {
            lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);        
            selectionIndex = index + 1;
            lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 3);
        }        
    } 
    else 
    {
        switch(selectionIndex) 
        {
            case 0:
            {
                break;
            }  
            case 1:
            {
                if(!otaActive)
                {                   
                    readWifiAccessData(ssidS, passwordS);
                    Serial.println("----------OTA DATA-----------");
                    Serial.println(ssidS);
                    Serial.println(passwordS);
                    Serial.println("OTA successfully activated.");
                    
                    if(WifiManager::connectAccessPoint(ssidS, passwordS))
                    {
                        otaActive = true;
                        OtaManager::setup();
                        WifiScreen::wifiActive = false;
                        lv_label_set_text(btnOta, "Ota [on]");
                    }
                    else
                    {
                        otaActive = false;                        
                    } 
                }
                else
                {
                    lv_label_set_text(btnOta, "Ota [off]");
                    
                    WifiManager::disconnectAccessPoint();
                    otaActive = false;                    
                }  
                break;
            }
            case 2:
            {
                UISystem::currentScreen = OPTION_SCREEN;  
                WifiManager::connectionFailed = false;
                UISystem::setScreen(UISystem::currentScreen);
                break;
            }
        }
    }
}

void SettingsScreen::buildWifiStatusString(char* ssid, char* status, char* output)
{
    output[0] = '\0';
    char s1[] = "SSID: ";
    char s2[] = "\n";
    char s3[] = "state: ";

    Helper::concatCharArrays(output, s1);
    Helper::concatCharArrays(output, ssid);
    Helper::concatCharArrays(output, s2);
    Helper::concatCharArrays(output, s3);
    Helper::concatCharArrays(output, status);
}

void SettingsScreen::readWifiAccessData(char* ssid, char* password)
{
    if (SD.exists(OTA_ACCESS_DATA_PATH))
    {
        File file = SD.open(OTA_ACCESS_DATA_PATH);
        if (file)
        {
            Serial.println("read ota");
            file.readBytesUntil('\n', ssid, 20);
            file.readBytesUntil('\n', password, 20);         
            Serial.println(ssid);
            Serial.println(password);
            file.close();
        }         
    }
}

void SettingsScreen::createSampleData()
{
    if(!SD.exists(OTA_ACCESS_DATA_PATH)) 
    {
        File newFile = SD.open(OTA_ACCESS_DATA_PATH, FILE_APPEND);
        if (newFile)
        {
            newFile.println("Your Network");
            newFile.println("Your Pass");
            newFile.close();        
        }       
    }
}