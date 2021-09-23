/**
 * @author Lucas Balda
 */

#include "StatScreen.h"

lv_obj_t* StatScreen::screenObj;
lv_obj_t* StatScreen::chartObj;
lv_obj_t* StatScreen::lblSerieNameObj;
lv_obj_t* StatScreen::lblSecondsObj;

std::list<NamedChartSerie> StatScreen::namedSerieList = std::list<NamedChartSerie>();
int8_t StatScreen::currentSeriesIndex = 0;

void StatScreen::setup() 
{
    // Create new Screen
    screenObj = lv_obj_create(NULL, NULL);

    // Change screen-background-color
    lv_obj_set_style_local_bg_color(screenObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARK);

    // Initialize chart as line-chart with positioning & alignment
    chartObj = lv_chart_create(screenObj, NULL);
    lv_obj_set_size(chartObj, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 20);
    lv_obj_align(chartObj, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 20);
    lv_chart_set_type(chartObj, LV_CHART_TYPE_LINE);
    lv_obj_set_style_local_bg_color(chartObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_color(chartObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    
    // Gradient transparency effect of the chart
    lv_obj_set_style_local_bg_opa(chartObj, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, LV_OPA_50);
    lv_obj_set_style_local_bg_grad_dir(chartObj, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_obj_set_style_local_bg_main_stop(chartObj, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_bg_grad_stop(chartObj, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 0);

    // Chart ticks style
    lv_chart_set_x_tick_length(chartObj, 5, 2);
    lv_chart_set_y_tick_length(chartObj, 5, 2);

    // Chart padding for space of tick labels
    lv_obj_set_style_local_pad_left(chartObj,  LV_CHART_PART_BG, LV_STATE_DEFAULT, 5 * (LV_DPI / 10));
    lv_obj_set_style_local_pad_bottom(chartObj,  LV_CHART_PART_BG, LV_STATE_DEFAULT, 3 * (LV_DPI / 10));
    lv_obj_set_style_local_pad_right(chartObj,  LV_CHART_PART_BG, LV_STATE_DEFAULT, 2 * (LV_DPI / 10));
    lv_obj_set_style_local_pad_top(chartObj,  LV_CHART_PART_BG, LV_STATE_DEFAULT, 2 * (LV_DPI / 10));

    // Setup series-name-label on top of the screen & changes its font-color
    lblSerieNameObj = lv_label_create(screenObj, NULL);
    lv_obj_align(lblSerieNameObj, NULL, LV_ALIGN_IN_TOP_MID, 0, 2);
    lv_obj_set_style_local_text_color(lblSerieNameObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_BRIGHTER);
    lv_label_set_align(lblSerieNameObj, LV_LABEL_ALIGN_CENTER);
    lv_obj_realign(lblSerieNameObj);

    // Setup seconds-label on bottom of the chart & change its font-color
    lblSecondsObj = lv_label_create(screenObj, NULL);
    lv_label_set_text(lblSecondsObj, "Seconds");
    lv_obj_set_pos(lblSecondsObj, (SCREEN_WIDTH - 7 * 8) / 2, SCREEN_HEIGHT - 24);
    lv_obj_set_style_local_text_color(lblSecondsObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_BRIGHTER);
    
    // Setup the current available series
    addNamedChartSerie("Depth");
    addNamedChartSerie("Temperatur");
    addNamedChartSerie("Brightness");
    addNamedChartSerie("O2-Saturation/Depth");
    addNamedChartSerie("Heart-Frequency/Depth");
    addNamedChartSerie("Heart-Variability");
}

void StatScreen::showScreen() 
{
    Serial.println("1");
    // Update stats after diving
    dataUpdate();
    Serial.println("3");

    lv_scr_load(StatScreen::screenObj);
}

void StatScreen::processButtonPress(ButtonType buttonType) 
{
    // Back to Idle-Screen or show the next series
    if(buttonType == BUTTON_SELECT)
    {
        //UISystem::setScreen(IDLE_SCREEN);
    } 
    else if(buttonType == BUTTON_ACTIVATE) 
    {
        showNextSeries();
    }
}

// Updates all data got from the last dive & inserts them into a chart
void StatScreen::dataUpdate() 
{
    Serial.println("2");

    // Get last shown series
    std::list<NamedChartSerie>::iterator it = namedSerieList.begin();
    std::advance(it, currentSeriesIndex);
    NamedChartSerie serie = *it;

    // Set data count (to display the whole curve)
    lv_chart_set_point_count(chartObj, UISystem::diveDataSeries.size());
    // Hide last shown series
    lv_chart_hide_series(chartObj, serie.series, true);
    // Reset seriesIndex
    currentSeriesIndex = 0;

    //Reset serie-list for new dive-data
    for (NamedChartSerie& serie : namedSerieList) 
    {
        lv_chart_clear_serie(chartObj, serie.series);
        serie.min = 32767;
        serie.max = -32767;
        serie.xAxisLabels = String();
        serie.yAxisLabels = String();
    }

    //Calculate Min/Max Values & Add them to chart-series
    for (DiveData data : UISystem::diveDataSeries) 
    {
        std::list<NamedChartSerie>::iterator it = namedSerieList.begin();
        processDiveData(data.depth, *it++);
        processDiveData(data.temperatur, *it++);
        processDiveData(data.brightness, *it++);
        processDiveData(data.o2saturation, *it++);
        processDiveData(data.heartFrequency, *it++);
        processDiveData(data.heartVariability, *it++);
    }

    //Apply min values for correct chart-curve & put them on chart
    for (DiveData data : UISystem::diveDataSeries) 
    {
        std::list<NamedChartSerie>::iterator it = namedSerieList.begin();
        addNormlizedPointsOnChart(data.depth, *it++);
        addNormlizedPointsOnChart(data.temperatur, *it++);
        addNormlizedPointsOnChart(data.brightness, *it++);
        addNormlizedPointsOnChart(data.o2saturation, *it++);
        addNormlizedPointsOnChart(data.heartFrequency, *it++);
        addNormlizedPointsOnChart(data.heartVariability, *it++);
    }

    // Create axis-strings for labels
    int16_t xGaps = 7;
    int16_t yGaps = 5;

    for (NamedChartSerie& serie : namedSerieList) 
    {
        serie.xAxisLabels += String(1);
        serie.yAxisLabels += String(serie.max);
        for (int i = 0; i < xGaps - 1; i++) 
        {
            serie.xAxisLabels += String("\n");
        }
        for (int i = 0; i < yGaps - 1; i++) 
        {
            serie.yAxisLabels += String("\n");
        }
        serie.xAxisLabels += String(UISystem::diveDataSeries.size());
        serie.yAxisLabels += String(serie.min);
    }

    // Show first series
    NamedChartSerie& startSeries = *namedSerieList.begin();
    lv_chart_set_y_range(chartObj, LV_CHART_AXIS_PRIMARY_Y, 0, startSeries.max - startSeries.min);
    lv_chart_set_x_tick_texts(chartObj, startSeries.xAxisLabels.c_str(), 1, LV_CHART_AXIS_DRAW_LAST_TICK);
    lv_chart_set_y_tick_texts(chartObj, startSeries.yAxisLabels.c_str(), 1, LV_CHART_AXIS_DRAW_LAST_TICK);
    lv_chart_hide_series(chartObj, startSeries.series, false);
    
    // Show Series-Name of first series
    lv_label_set_text(lblSerieNameObj, startSeries.name);
    lv_label_set_align(lblSerieNameObj, LV_LABEL_ALIGN_CENTER);
    lv_obj_realign(lblSerieNameObj);

    // Chart needs to be updated
    lv_chart_refresh(chartObj);
}

// For future use ...
void StatScreen::update() 
{

}

// Create new named-chart-series & add them into the list of available series
void StatScreen::addNamedChartSerie(const char* name) 
{
    //SHIT HAPPENS HERE
    NamedChartSerie serie = NamedChartSerie();
    serie.name = name;
    serie.series = 0;
    serie.series = lv_chart_add_series(chartObj, THEME_PRIMARY_COLOR_NORMAL);
    lv_chart_hide_series(chartObj, serie.series, true);
    namedSerieList.push_back(serie);
}

// Calculate the min/max-values, add next point on chart & write them into the referenced series
void StatScreen::processDiveData(int16_t value, NamedChartSerie& serie) 
{
    serie.max = std::max(value, serie.max);
    serie.min = std::min(value, serie.min);
}

// Apply min value & put point on chart-series
void StatScreen::addNormlizedPointsOnChart(int16_t value, NamedChartSerie serie) 
{
    lv_chart_set_next(chartObj, serie.series, value - serie.min);
}

// Create Tick-Labels on the x- & y-axis
void StatScreen::createAxisLabels(NamedChartSerie& serie, int16_t xGaps, int16_t yGaps) 
{
    serie.xAxisLabels += String(0);
    serie.yAxisLabels += String(serie.min);
    for (int i = 0; i < xGaps; i++) 
    {
        serie.xAxisLabels += String("\n");
    }
    for (int i = 0; i < yGaps; i++) 
    {
        serie.yAxisLabels += String("\n");
    }
    serie.xAxisLabels += String(UISystem::diveDataSeries.size());
    serie.yAxisLabels += String(serie.max);
}

// Switch to next series in the series-list & show them on the chart
void StatScreen::showNextSeries() 
{
    // Get current series
    std::list<NamedChartSerie>::iterator it = namedSerieList.begin();
    std::advance(it, currentSeriesIndex);
    NamedChartSerie& serie = *it;

    // Hide current series on chart
    if(it != namedSerieList.begin())
        lv_chart_hide_series(chartObj, serie.series, true);

    // Get next series
    currentSeriesIndex++;
    currentSeriesIndex %= namedSerieList.size();
    it = namedSerieList.begin();
    std::advance(it, currentSeriesIndex);
    NamedChartSerie& newSerie = *it;
    
    // Update the series on the chart
    lv_chart_set_y_range(chartObj, LV_CHART_AXIS_PRIMARY_Y, 0, newSerie.max - newSerie.min);
    lv_chart_set_x_tick_texts(chartObj, newSerie.xAxisLabels.c_str(), 1, LV_CHART_AXIS_DRAW_LAST_TICK);
    lv_chart_set_y_tick_texts(chartObj, newSerie.yAxisLabels.c_str(), 1, LV_CHART_AXIS_DRAW_LAST_TICK);
    lv_chart_hide_series(chartObj, newSerie.series, false);
    
    // Update Serie-Name-Label
    lv_label_set_text(lblSerieNameObj, newSerie.name);
    lv_obj_realign(lblSerieNameObj);
}
