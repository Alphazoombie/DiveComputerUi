/**
 * @author Lucas Balda
 */

#include "OptionScreen.h"

lv_obj_t* OptionScreen::screenObj;
int16_t OptionScreen::selectionIndex;
lv_obj_t* OptionScreen::buttons[4];
lv_style_t OptionScreen::styleSelected;
lv_style_t OptionScreen::styleDefault;

enum OptionScreenButtons
{
    BUTTON_WIFI = 0,
    BUTTON_STAT_SCREEN = 1,
    BUTTON_DIVE_SCREEN = 2,
    BUTTON_IDLE_SCREEN = 3    
};

void OptionScreen::setup() 
{
    screenObj = lv_obj_create(NULL, NULL);

    // Change screen-background-color
    lv_obj_set_style_local_bg_color(screenObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARK);

    static int16_t btnWidth = 140;
    static int16_t btnHeight = 100;
    int16_t spacingX = (SCREEN_WIDTH - btnWidth * 2) / 3;
    int16_t spacingY = (SCREEN_HEIGHT - btnHeight * 2) / 3;

    buttons[BUTTON_WIFI] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[BUTTON_WIFI], spacingX, spacingY);
    lv_obj_set_size(buttons[BUTTON_WIFI], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[BUTTON_WIFI], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[BUTTON_WIFI], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 3);
    lv_obj_set_style_local_radius(buttons[BUTTON_WIFI], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[BUTTON_WIFI], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_t * label1 = lv_label_create(buttons[BUTTON_WIFI], NULL);
    lv_obj_set_style_local_text_font(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(label1, LV_SYMBOL_WIFI);

    buttons[BUTTON_STAT_SCREEN] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[BUTTON_STAT_SCREEN], spacingX * 2 + btnWidth, spacingY);
    lv_obj_set_size(buttons[BUTTON_STAT_SCREEN], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[BUTTON_STAT_SCREEN], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[BUTTON_STAT_SCREEN], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
    lv_obj_set_style_local_radius(buttons[BUTTON_STAT_SCREEN], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[BUTTON_STAT_SCREEN], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_t * label2 = lv_label_create(buttons[BUTTON_STAT_SCREEN], NULL);
    lv_obj_set_style_local_text_font(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(label2, "Statistic");

    buttons[BUTTON_DIVE_SCREEN] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[BUTTON_DIVE_SCREEN], spacingX, spacingY * 2 + btnHeight);
    lv_obj_set_size(buttons[BUTTON_DIVE_SCREEN], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[BUTTON_DIVE_SCREEN], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[BUTTON_DIVE_SCREEN], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
    lv_obj_set_style_local_radius(buttons[BUTTON_DIVE_SCREEN], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[BUTTON_DIVE_SCREEN], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_t * label3 = lv_label_create(buttons[BUTTON_DIVE_SCREEN], NULL);
    lv_obj_set_style_local_text_font(label3, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(label3, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(label3, LV_SYMBOL_SETTINGS);

    buttons[BUTTON_IDLE_SCREEN] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[BUTTON_IDLE_SCREEN], spacingX * 2 + btnWidth, spacingY * 2 + btnHeight);
    lv_obj_set_size(buttons[BUTTON_IDLE_SCREEN], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[BUTTON_IDLE_SCREEN], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[BUTTON_IDLE_SCREEN], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
    lv_obj_set_style_local_radius(buttons[BUTTON_IDLE_SCREEN], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[BUTTON_IDLE_SCREEN], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_t * label4 = lv_label_create(buttons[BUTTON_IDLE_SCREEN], NULL);
    lv_obj_set_style_local_text_font(label4, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(label4, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(label4, LV_SYMBOL_HOME);
}

void OptionScreen::showScreen() 
{
    int16_t btnWidth = 140;
    int16_t btnHeight = 100;
    int16_t spacingX = (SCREEN_WIDTH - btnWidth * 2) / 3;
    int16_t spacingY = (SCREEN_HEIGHT - btnHeight * 2) / 3; 

    Touch::clearRegister();
    Touch::registerArea(1, spacingX, spacingY, btnWidth, btnHeight);
    Touch::registerArea(2, spacingX * 2 + btnWidth, spacingY, btnWidth, btnHeight);
    Touch::registerArea(3, spacingX, spacingY * 2 + btnHeight, btnWidth, btnHeight);
    Touch::registerArea(4, spacingX * 2 + btnWidth, spacingY * 2 + btnHeight, btnWidth, btnHeight);

    lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
    selectionIndex = 0;
    lv_obj_set_style_local_border_width(buttons[BUTTON_WIFI], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 3);
    lv_scr_load(OptionScreen::screenObj);
}

void OptionScreen::processButtonPress(ButtonType buttonType, int index) 
{
    if(buttonType == BUTTON_SELECT) 
    {
        if (index == -1)
        {
            lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
            selectionIndex++;
            selectionIndex %= 4;
            lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 3);
        }
        else
        {
            lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);        
            selectionIndex = index;
            lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 3);
        }        
    } 
    else 
    {
        switch(selectionIndex) 
        {
            case 0:
                UISystem::currentScreen = WIFI_SCREEN;  
                UISystem::setScreen(UISystem::currentScreen);
                break;
            case 1:
                UISystem::currentScreen = STAT_SCREEN;  
                UISystem::setScreen(UISystem::currentScreen);
                break;
            case 2:
                UISystem::currentScreen = SETTINGS_SCREEN;  
                UISystem::setScreen(UISystem::currentScreen);
                break;
            case 3:
                UISystem::currentScreen = IDLE_SCREEN;  
                UISystem::setScreen(UISystem::currentScreen);
                break;
        }
    }
}