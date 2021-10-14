/**
 * @author Lucas Balda
 */

#ifndef STAT_SCREEN_H
#define STAT_SCREEN_H

#include <lvgl.h>
#include <list>
#include <math.h>
#include <Arduino.h>
#include "Touch.h"
#include "UISystem.h"
#include "ScreenType.h"
#include "ButtonType.h"
#include "DiveData.h"
#include <ArduinoJson.h>

struct NamedChartSerie 
{
    const char* name;
    lv_chart_series_t* series;
    int16_t max, min;
    String xAxisLabels;
    String yAxisLabels;
};

class StatScreen 
{
public:
    static lv_obj_t* screenObj;
    static lv_obj_t* chartObj;
    static lv_obj_t* lblSerieNameObj;
    static lv_obj_t* lblSecondsObj;
    static std::list<SmallDiveData> diveData;
    static std::list<NamedChartSerie> namedSerieList;
    static int8_t currentSeriesIndex; 
      
    static void setup();
    static void showScreen();
    static void processButtonPress(ButtonType);
    static void dataUpdate();
    static void update();
    static void addNamedChartSerie(const char* name);
    static void processDiveData(int16_t value, NamedChartSerie& serie);
    static void addNormlizedPointsOnChart(int16_t value, NamedChartSerie serie);
    static void createAxisLabels(NamedChartSerie& serie, int16_t xGaps, int16_t yGaps);
    static void showNextSeries();
    static void getData();
    static SmallDiveData extractJson(char* json);
};

#endif
