#include "WifiScreen.h"

lv_obj_t* WifiScreen::screenObj;
int16_t WifiScreen::selectionIndex;
lv_obj_t* WifiScreen::buttons[4];
lv_obj_t* WifiScreen::labelWifi;
lv_obj_t* WifiScreen::labelWifiAccessData;
lv_obj_t* WifiScreen::labelWifiGenerate;
lv_style_t WifiScreen::styleSelected;
lv_style_t WifiScreen::styleDefault;

char ssid[9] = "DC_";
char password[9];
char wifiAccessDataString[32];

bool WifiScreen::wifiActive = false;

enum WifiScreenButtons
{
    BUTTON_WIFI_DATA = 0,
    BUTTON_REFRESH_DATA = 1,
    BUTTON_WIFI_ACTIVATION = 2,
    BUTTON_BACK = 3  
};

void WifiScreen::setup() 
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
    lv_obj_set_size(buttons[BUTTON_WIFI_DATA], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[BUTTON_WIFI_DATA], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[BUTTON_WIFI_DATA], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
    lv_obj_set_style_local_radius(buttons[BUTTON_WIFI_DATA], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[BUTTON_WIFI_DATA], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    labelWifiAccessData = lv_label_create(buttons[BUTTON_WIFI_DATA], NULL);
    //lv_obj_set_style_local_text_font(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(labelWifiAccessData, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

    readWifiAccessData(ssid, password);
    // check this shit
    buildWifiAccessDataString(ssid, password, wifiAccessDataString);

    lv_label_set_text(labelWifiAccessData, wifiAccessDataString);

    buttons[BUTTON_REFRESH_DATA] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[BUTTON_REFRESH_DATA], spacingX * 2 + btnWidth, spacingY);
    lv_obj_set_size(buttons[BUTTON_REFRESH_DATA], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[BUTTON_REFRESH_DATA], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[BUTTON_REFRESH_DATA], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
    lv_obj_set_style_local_radius(buttons[BUTTON_REFRESH_DATA], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[BUTTON_REFRESH_DATA], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    labelWifiGenerate = lv_label_create(buttons[BUTTON_REFRESH_DATA], NULL);
    //lv_obj_set_style_local_text_font(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(labelWifiGenerate, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(labelWifiGenerate, "Refresh SSID/PW");

    buttons[BUTTON_WIFI_ACTIVATION] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[BUTTON_WIFI_ACTIVATION], spacingX, spacingY * 2 + btnHeight);
    lv_obj_set_size(buttons[BUTTON_WIFI_ACTIVATION], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[BUTTON_WIFI_ACTIVATION], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[BUTTON_WIFI_ACTIVATION], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 3);
    lv_obj_set_style_local_radius(buttons[BUTTON_WIFI_ACTIVATION], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[BUTTON_WIFI_ACTIVATION], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    labelWifi = lv_label_create(buttons[BUTTON_WIFI_ACTIVATION], NULL);
    //lv_obj_set_style_local_text_font(label3, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(labelWifi, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(labelWifi, "Wifi [off]");

    buttons[BUTTON_BACK] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[BUTTON_BACK], spacingX * 2 + btnWidth, spacingY * 2 + btnHeight);
    lv_obj_set_size(buttons[BUTTON_BACK], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[BUTTON_BACK], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[BUTTON_BACK], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
    lv_obj_set_style_local_radius(buttons[BUTTON_BACK], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[BUTTON_BACK], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_t * label4 = lv_label_create(buttons[BUTTON_BACK], NULL);
    //lv_obj_set_style_local_text_font(label4, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(label4, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(label4, "Back");
}

void WifiScreen::showScreen() 
{
    int16_t btnWidth = 140;
    int16_t btnHeight = 100;
    int16_t spacingX = (SCREEN_WIDTH - btnWidth * 2) / 3;
    int16_t spacingY = (SCREEN_HEIGHT - btnHeight * 2) / 3; 

    Touch::clearRegister();
    Touch::registerArea(1, spacingX * 2 + btnWidth, spacingY, btnWidth, btnHeight);
    Touch::registerArea(2, spacingX, spacingY * 2 + btnHeight, btnWidth, btnHeight);
    Touch::registerArea(3, spacingX * 2 + btnWidth, spacingY * 2 + btnHeight, btnWidth, btnHeight);

    lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
    selectionIndex = 2;
    lv_obj_set_style_local_border_width(buttons[BUTTON_WIFI_ACTIVATION], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 3);
    dataUpdate();
    lv_scr_load(WifiScreen::screenObj);
}

void WifiScreen::processButtonPress(ButtonType buttonType, int index) 
{
    if(buttonType == BUTTON_SELECT) 
    {
        if (index == -1)
        {
            lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
            selectionIndex++;
            selectionIndex %= 4;

            if (selectionIndex == 0)
            {
                selectionIndex++;
            }
        
            lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 3);
        }
        else
        {
            lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);        
            selectionIndex = index;
            selectionIndex++;
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
                WifiAccessGenerator::generateSsid(ssid);
                WifiAccessGenerator::generatePassword(password);

                if (saveWifiAccessData(ssid, password))
                {
                    lv_label_set_text(labelWifi, "Wifi [off]");

                    WifiManager::disconnectAccessPoint();
                    WifiManager::createAccessPoint(21, ssid, password);
                    
                    wifiAccessDataString[0] = '\0';
                    buildWifiAccessDataString(ssid, password, wifiAccessDataString);
                    lv_label_set_text(labelWifiAccessData, wifiAccessDataString);
                    lv_label_set_text(labelWifi, "Wifi [on]");

                    Serial.println("Saving new Wifi Access Data successful.");
                }
                else
                {
                    Serial.println("Saving new Wifi Access Data failed.");
                }
                break;
            }
            case 2:
            {
                if(!wifiActive)
                {
                    lv_label_set_text(labelWifi, "Wifi [on]");
                    readWifiAccessData(ssid, password);

                    Serial.println("----------WIFI DATA-----------");
                    Serial.println(ssid);
                    Serial.println(password);
                    Serial.println("WIFI successfully activated.");
                    
                    WifiManager::createAccessPoint(21, ssid, password);
                    wifiActive = true;
                }
                else
                {
                    lv_label_set_text(labelWifi, "Wifi [off]");
                    
                    WifiManager::disconnectAccessPoint();
                    wifiActive = false;
                }  
                break;
            }
            case 3:
            {
                UISystem::currentScreen = OPTION_SCREEN;  
                UISystem::setScreen(UISystem::currentScreen);
                break;
            }
        }
    }
}

void WifiScreen::dataUpdate() 
{

}

void WifiScreen::update() 
{
    dataUpdate();
}

void WifiScreen::buildWifiAccessDataString(char* ssid, char* password, char* output)
{

    char s1[] = "SSID: ";
    char s2[] = "\n";
    char s3[] = "PW: ";

    Helper::concatCharArraysTwoArguments(output, s1);
    Helper::concatCharArraysTwoArguments(output, ssid);
    Helper::concatCharArraysTwoArguments(output, s2);
    Helper::concatCharArraysTwoArguments(output, s3);
    Helper::concatCharArraysTwoArguments(output, password);
}

void WifiScreen::readWifiAccessData(char* ssid, char* password)
{
    if (SD.exists(WIFI_ACCESS_DATA_PATH))
    {
        File file = SD.open(WIFI_ACCESS_DATA_PATH);
        if (file)
        {
            file.readBytesUntil('\0', ssid, 9);
            ssid[8] = '\0';
            file.read();
            file.readBytesUntil('\0', password, 9);
            password[8] = '\0';

            file.close();
        }         
    }
}

bool WifiScreen::saveWifiAccessData(char* ssid, char* password)
{
    if (SD.exists(WIFI_ACCESS_DATA_PATH))
    {
        SD.remove(WIFI_ACCESS_DATA_PATH);
        Serial.println("Old Wifi File deleted.");

        File newFile = SD.open(WIFI_ACCESS_DATA_PATH, FILE_APPEND);
        if (newFile)
        {
            Serial.println("New File created.");
            newFile.println(ssid);
            newFile.println(password);
            newFile.close();
            //return true;
        }

        // only test
        File newFile2 = SD.open(WIFI_ACCESS_DATA_PATH);
        if (newFile2)
        {
            Serial.println("nur gucken ob richtig gespeicher...");
            Serial.println(newFile2.readStringUntil('\n'));
            Serial.println(newFile2.readStringUntil('\n'));
            newFile2.close();
        }
        return true;
    }
    return false;
}
