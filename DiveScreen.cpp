/**
   @author Lucas Balda
*/

#include "DiveScreen.h"


lv_obj_t* DiveScreen::screenObj;

lv_obj_t* DiveScreen::lblTimeObj;
lv_obj_t* DiveScreen::barDepthObj;
//lv_obj_t* DiveScreen::gaugeSpO2Obj;
lv_obj_t* DiveScreen::arcSpO2Obj;

lv_obj_t* DiveScreen::imgHeartFrequencyObj;
lv_obj_t* DiveScreen::imgTimeObj;
lv_obj_t* DiveScreen::imgSpO2Obj;
lv_obj_t* DiveScreen::imgDepthObj;

lv_obj_t* DiveScreen::lblDepth;
lv_obj_t* DiveScreen::lblSpO2Obj;
lv_obj_t* DiveScreen::lblHeartFrequency;

Timer DiveScreen::diveTimer;
//stopwatch::Stopwatch diveTimer;

void DiveScreen::setup() 
{
  screenObj = lv_obj_create(NULL, NULL);
  lv_obj_set_style_local_bg_color(screenObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARK);

  static int16_t contWidth = 140; //200
  static int16_t contHeight = 100; //140
  int16_t spacingX = (SCREEN_WIDTH - contWidth * 2) / 3;
  int16_t spacingY = (SCREEN_HEIGHT - contHeight * 2) / 3;

  lv_obj_t* contTime = lv_cont_create(screenObj, NULL);
  lv_obj_set_size(contTime, 140, 100); //200, 140
  lv_obj_set_pos(contTime, spacingX, spacingY);
  lv_obj_set_style_local_bg_color(contTime, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
  lv_obj_set_style_local_border_color(contTime, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

  lblTimeObj = lv_label_create(contTime, NULL);
  lv_obj_set_pos(lblTimeObj, 10, 10);
  lv_obj_set_size(lblTimeObj, 160, 100); //220,140
  lv_obj_set_style_local_text_font(lblTimeObj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &digital_numbers_40px);
  lv_label_set_align(lblTimeObj, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(lblTimeObj, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_local_text_color(lblTimeObj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

  imgTimeObj = lv_img_create(contTime, NULL);
  lv_img_set_src(imgTimeObj, &sandclock_image);
  lv_obj_align(imgTimeObj, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -2, -4);

  lv_obj_t* contDepth = lv_cont_create(screenObj, NULL);
  lv_obj_set_size(contDepth, contWidth, contHeight); //200,140
  lv_obj_set_pos(contDepth, spacingX * 2 + contWidth, spacingY);
  lv_obj_set_style_local_bg_color(contDepth, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
  lv_obj_set_style_local_border_color(contDepth, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

  barDepthObj = lv_bar_create(contDepth, NULL);
  lv_obj_set_size(barDepthObj, 10, contHeight - 20); //10,120
  lv_obj_align(barDepthObj, NULL, LV_ALIGN_IN_RIGHT_MID, -10, 0);
  lv_bar_set_range(barDepthObj, 0, 20);
  lv_bar_set_anim_time(barDepthObj, 100);
  lv_bar_set_value(barDepthObj, 0, LV_ANIM_ON);
  lv_obj_set_style_local_bg_color(barDepthObj, LV_BAR_PART_BG , LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
  lv_obj_set_style_local_bg_color(barDepthObj, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_NORMAL);
  lv_obj_set_style_local_radius(barDepthObj, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, 0);

  lblDepth = lv_label_create(contDepth, NULL);
  lv_obj_set_pos(lblDepth, 187, 10); //250, 10
  lv_obj_set_size(lblDepth, 160, 100); //220, 140
  lv_obj_set_style_local_text_font(lblDepth, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
  lv_label_set_align(lblDepth, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(lblDepth, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_local_text_color(lblDepth, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

  imgDepthObj = lv_img_create(contDepth, NULL);
  lv_img_set_src(imgDepthObj, &depth_image);
  lv_obj_align(imgDepthObj, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 8, -8);

  lv_obj_t* contSpO2 = lv_cont_create(screenObj, NULL);
  lv_obj_set_size(contSpO2, contWidth, contHeight); //200,140
  lv_obj_set_pos(contSpO2, spacingX, spacingY * 2 + contHeight);
  lv_obj_set_style_local_bg_color(contSpO2, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
  lv_obj_set_style_local_border_color(contSpO2, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

  arcSpO2Obj = lv_arc_create(contSpO2, NULL);
  lv_obj_set_size(arcSpO2Obj, 120, 120);//180,180
  lv_obj_set_style_local_text_color(arcSpO2Obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
  lv_obj_set_style_local_bg_color(arcSpO2Obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
  lv_obj_set_style_local_line_color(arcSpO2Obj, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
  lv_obj_set_style_local_line_color(arcSpO2Obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_NORMAL);
  lv_obj_set_style_local_border_width(arcSpO2Obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_style_local_radius(arcSpO2Obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_arc_set_range(arcSpO2Obj, 0, 100);
  lv_obj_align(arcSpO2Obj, NULL, LV_ALIGN_CENTER, 0, 0);
  /*
    gaugeSpO2Obj = lv_gauge_create(contSpO2, NULL);
    static lv_color_t needleColor[1] = { THEME_PRIMARY_COLOR_DARK };
    lv_gauge_set_needle_count(gaugeSpO2Obj, 1, needleColor);
    lv_obj_set_size(gaugeSpO2Obj, 170, 170);
    lv_obj_align(gaugeSpO2Obj, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_gauge_set_range(gaugeSpO2Obj, 0, 100);
    lv_gauge_set_critical_value(gaugeSpO2Obj, 200);
    lv_obj_set_style_local_text_color(gaugeSpO2Obj, LV_GAUGE_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_set_style_local_bg_color(gaugeSpO2Obj, LV_GAUGE_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARK);
    lv_obj_set_style_local_border_color(gaugeSpO2Obj, LV_GAUGE_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
  */
  lblSpO2Obj = lv_label_create(contSpO2, NULL);
  lv_obj_set_pos(lblSpO2Obj, 10, 210);//10,290
  lv_obj_set_size(lblSpO2Obj, contWidth, contHeight);//220,140
  lv_obj_set_style_local_text_font(lblSpO2Obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
  lv_label_set_align(lblSpO2Obj, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(lblSpO2Obj, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_local_text_color(lblSpO2Obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

  imgSpO2Obj = lv_img_create(contSpO2, NULL);
  lv_img_set_src(imgSpO2Obj, &lungs_image);
  lv_obj_align(imgSpO2Obj, NULL, LV_ALIGN_IN_TOP_RIGHT, -2, 4);

  lv_obj_t* contHeartFrequency = lv_cont_create(screenObj, NULL);
  lv_obj_set_size(contHeartFrequency, contWidth, contHeight); //200,140
  lv_obj_set_pos(contHeartFrequency, spacingX * 2 + contWidth, spacingY * 2 + contHeight);
  lv_obj_set_style_local_bg_color(contHeartFrequency, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
  lv_obj_set_style_local_border_color(contHeartFrequency, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

  lblHeartFrequency = lv_label_create(contHeartFrequency, NULL);
  lv_obj_set_pos(lblHeartFrequency, 250, 290);//250,290
  lv_obj_set_size(lblHeartFrequency, contWidth, contHeight);//220,140
  lv_obj_set_style_local_text_font(lblHeartFrequency, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
  lv_label_set_align(lblHeartFrequency, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(lblHeartFrequency, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_local_text_color(lblHeartFrequency, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

  imgHeartFrequencyObj = lv_img_create(contHeartFrequency, NULL);
  lv_img_set_src(imgHeartFrequencyObj, &heart_image);
  lv_obj_align(imgHeartFrequencyObj, NULL, LV_ALIGN_IN_TOP_LEFT, 8, 8);

  diveTimer = Timer();
  diveTimer.start();
}

void DiveScreen::showScreen() 
{
  Touch::clearRegister();    
  dataUpdate();
  lv_scr_load(DiveScreen::screenObj);
}

void DiveScreen::processButtonPress(ButtonType buttonType) 
{

}

void DiveScreen::dataUpdate() 
{
  int8_t s = UISystem::currentDiveData.time % 60;
  int8_t m = (UISystem::currentDiveData.time - s) / 60;

  // int currentTime = diveTimer.elapsedSeconds();

  // int32_t s = currentTime % 60;
  // int32_t m = (currentTime - s) / 60;

  //lv_gauge_set_value(gaugeSpO2Obj, 0, UISystem::currentDiveData.o2saturation);
  lv_arc_set_value(arcSpO2Obj, UISystem::currentDiveData.o2saturation);
  lv_label_set_text_fmt(lblTimeObj, "%02d:%02d", m, s);
  lv_obj_realign(lblTimeObj);
  lv_label_set_text_fmt(lblDepth, "%d", (int)UISystem::currentDiveData.depth);
  lv_label_set_text_fmt(lblSpO2Obj, "%d", UISystem::currentDiveData.o2saturation);
  lv_label_set_text_fmt(lblHeartFrequency, "%d", UISystem::currentDiveData.heartFrequency);
  lv_bar_set_value(barDepthObj, 20 - UISystem::currentDiveData.depth, LV_ANIM_ON);
  lv_obj_realign(lblSpO2Obj);
  lv_obj_realign(lblTimeObj);
  lv_obj_realign(lblHeartFrequency);
  lv_obj_realign(lblDepth);
}

void DiveScreen::update() 
{
  dataUpdate();

  if (UISystem::currentDiveData.depth < 1) 
  {    
    if (UISystem::currentDiveData.time >= 10) 
    {
      //UISystem::setScreen(STAT_SCREEN);
    } 
    else 
    {
      //UISystem::setScreen(IDLE_SCREEN);
    }
  }
}
