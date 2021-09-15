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

void WifiScreen::setup() 
{
    screenObj = lv_obj_create(NULL, NULL);

    // Change screen-background-color
    lv_obj_set_style_local_bg_color(screenObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARK);

    static int16_t btnWidth = 140;
    static int16_t btnHeight = 100;
    int16_t spacingX = (SCREEN_WIDTH - btnWidth * 2) / 3;
    int16_t spacingY = (SCREEN_HEIGHT - btnHeight * 2) / 3;    

    buttons[0] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[0], spacingX, spacingY);
    lv_obj_set_size(buttons[0], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[0], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[0], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
    lv_obj_set_style_local_radius(buttons[0], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[0], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    labelWifiAccessData = lv_label_create(buttons[0], NULL);
    //lv_obj_set_style_local_text_font(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(labelWifiAccessData, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

    readWifiAccessData(ssid, password);
    // check this shit
    buildWifiAccessDataString(ssid, password, wifiAccessDataString);


    lv_label_set_text(labelWifiAccessData, wifiAccessDataString);

    buttons[1] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[1], spacingX * 2 + btnWidth, spacingY);
    lv_obj_set_size(buttons[1], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[1], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[1], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
    lv_obj_set_style_local_radius(buttons[1], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[1], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    labelWifiGenerate = lv_label_create(buttons[1], NULL);
    //lv_obj_set_style_local_text_font(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(labelWifiGenerate, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(labelWifiGenerate, "Refresh SSID/PW");

    buttons[2] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[2], spacingX, spacingY * 2 + btnHeight);
    lv_obj_set_size(buttons[2], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[2], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[2], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 3);
    lv_obj_set_style_local_radius(buttons[2], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[2], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    labelWifi = lv_label_create(buttons[2], NULL);
    //lv_obj_set_style_local_text_font(label3, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(labelWifi, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(labelWifi, "Wifi [off]");

    buttons[3] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[3], spacingX * 2 + btnWidth, spacingY * 2 + btnHeight);
    lv_obj_set_size(buttons[3], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[3], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[3], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
    lv_obj_set_style_local_radius(buttons[3], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[3], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_t * label4 = lv_label_create(buttons[3], NULL);
    //lv_obj_set_style_local_text_font(label4, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(label4, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(label4, "Back");
}

void WifiScreen::showScreen() 
{
    lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
    selectionIndex = 2;
    lv_obj_set_style_local_border_width(buttons[2], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 3);
    dataUpdate();
    lv_scr_load(WifiScreen::screenObj);
}

void WifiScreen::processButtonPress(ButtonType buttonType) 
{
    if(buttonType == BUTTON1) 
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

                    //TODO: TOGGLE WIFI ACCESS POINT OFF
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
