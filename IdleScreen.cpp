/**
 * @author Lucas Balda
 */

#include "IdleScreen.h"

lv_obj_t* IdleScreen::screenObj;
lv_obj_t* IdleScreen::lblTime;

void IdleScreen::setup() {
    screenObj = lv_obj_create(NULL, NULL);

    // Change screen-background-color
    lv_obj_set_style_local_bg_color(screenObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARK);

    static lv_obj_t* contTimeObj = lv_cont_create(screenObj, NULL);
    lv_obj_set_size(contTimeObj, SCREEN_WIDTH*0.8, SCREEN_HEIGHT*0.8);
    lv_obj_set_pos(contTimeObj, (SCREEN_WIDTH - SCREEN_WIDTH*0.8) / 2, (SCREEN_HEIGHT - SCREEN_HEIGHT*0.8) / 2);
    lv_obj_set_style_local_bg_color(contTimeObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_radius(contTimeObj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(contTimeObj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_shadow_color(contTimeObj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_shadow_opa(contTimeObj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);
    lv_obj_set_style_local_shadow_width(contTimeObj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_shadow_spread(contTimeObj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_shadow_ofs_x(contTimeObj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 3);
    lv_obj_set_style_local_shadow_ofs_y(contTimeObj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, -3);
    //lv_obj_set_style_local_border_color(contTimeObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

    //lv_label
    lblTime = lv_label_create(contTimeObj, NULL);
    lv_obj_set_style_local_text_font(lblTime, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &digital_numbers_40px);
    lv_obj_set_style_local_text_color(lblTime, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_align(lblTime, LV_ALIGN_CENTER);
    lv_obj_align(lblTime, NULL, LV_ALIGN_CENTER, 0, 0);
}

void IdleScreen::showScreen() {
    dataUpdate();
    lv_scr_load(IdleScreen::screenObj);
}

void IdleScreen::processButtonPress(ButtonType buttonType) {
    if(buttonType == BUTTON1) {
        if(UISystem::currentDiveData.time > 10) {
            //UISystem::setScreen(STAT_SCREEN);
        }
    } else if(buttonType == BUTTON2) {
        //UISystem::setScreen(OPTION_SCREEN);
    }
}

void IdleScreen::dataUpdate() {
    long time = UISystem::time;
    long s = time % 60;
    long m = (time % 3600) / 60;
    long h = (time % 86400) / 3600;
    lv_label_set_text_fmt(lblTime, "%02d:%02d:%02d", h, m, s);
    lv_obj_realign(lblTime);
}

void IdleScreen::update() {

    dataUpdate();

    if(UISystem::currentDiveData.depth >= 1) {
        //UISystem::setScreen(DIVE_SCREEN);
    }
}
