/**
 * @author Lucas Balda
 */

#include "StatScreen.h"

lv_obj_t* StatScreen::screenObj;
lv_obj_t* StatScreen::chartObj;
lv_obj_t* StatScreen::lblSerieNameObj;
lv_obj_t* StatScreen::lblSecondsObj;
std::list<SmallDiveData> StatScreen::diveData;


int16_t StatScreen::xGaps = 7;
int16_t StatScreen::yGaps = 5;

std::vector<NamedChartSerie> StatScreen::namedSerieList = std::vector<NamedChartSerie>();
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
    
    // Create axis-strings for labels
    xGaps = 7;
    yGaps = 5;

    // Setup the current available series
    addNamedChartSerie("Depth", SERIE_COLOR_YELLOW);
    addNamedChartSerie("O2-Saturation", SERIE_COLOR_BLUE);
    addNamedChartSerie("Heart-Frequency", SERIE_COLOR_GREEN);
    addNamedChartSerie("O2-Saturation/Depth", SERIE_COLOR_BLUE);
    addNamedChartSerie("Heart-Frequency/Depth", SERIE_COLOR_GREEN);
}

void StatScreen::showScreen() 
{
    Touch::clearRegister();    
    Touch::registerArea(1, 0, 0, 320, 240);
    Serial.println("1");
    Serial.println("2");

    // Update stats after diving
    StatScreen::getData();
    Serial.println("nach getdata");

    if (!diveData.empty())
    {
        StatScreen::dataUpdate();
    }

    Serial.println("3");

    lv_scr_load(StatScreen::screenObj);
}

void StatScreen::processButtonPress(ButtonType buttonType) 
{    
    // Back to Idle-Screen or show the next series
    if(buttonType == BUTTON_ACTIVATE)
    {
        UISystem::setScreen(IDLE_SCREEN);
    } 
    else if(buttonType == BUTTON_SELECT) 
    {
        Serial.println("processButtonPress");
        showNextSeries();
    }
    
}

// Updates all data got from the last dive & inserts them into a chart
void StatScreen::dataUpdate() 
{
    // Get last shown series
    NamedChartSerie serie = namedSerieList[currentSeriesIndex];

    // Set data count (to display the whole curve)
    lv_chart_set_point_count(chartObj, StatScreen::diveData.size());
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
    for (SmallDiveData data : StatScreen::diveData) 
    {
        processDiveData(namedSerieList[0], data.depth);
        processDiveData(namedSerieList[1], data.o2saturation);
        processDiveData(namedSerieList[2], data.heartFrequency);
        processDiveData(namedSerieList[3], data.o2saturation);
        processDiveData(namedSerieList[4], data.heartFrequency);
    }

    //Apply min values for correct chart-curve & put them on chart
    for (SmallDiveData data : StatScreen::diveData) 
    {
        addNormlizedPointsOnChart(namedSerieList[0], data.depth);
        addNormlizedPointsOnChart(namedSerieList[1], data.o2saturation);
        addNormlizedPointsOnChart(namedSerieList[2], data.heartFrequency);
        addNormlizedPointsOnChart(namedSerieList[3], data.o2saturation);
        addNormlizedPointsOnChart(namedSerieList[4], data.heartFrequency);
    }

    for (NamedChartSerie& serie : namedSerieList) 
    {
        createAxisLabels(serie, serie.min, serie.max);
    }
    
    // Show first series
    NamedChartSerie& startSeries = namedSerieList[0];
    lv_chart_set_y_range(chartObj, LV_CHART_AXIS_PRIMARY_Y, 0, startSeries.max - startSeries.min);    
    lv_chart_set_x_tick_texts(chartObj, startSeries.xAxisLabels.c_str(), 1, LV_CHART_AXIS_DRAW_LAST_TICK);    
    lv_chart_set_y_tick_texts(chartObj, startSeries.yAxisLabels.c_str(), 1, LV_CHART_AXIS_DRAW_LAST_TICK);
    
    // war eigentlich auf false programm ist dadurch abgeschmiert
    lv_chart_hide_series(chartObj, startSeries.series, false);
    
    // Show Series-Name of first series
    lv_label_set_text(lblSerieNameObj, startSeries.name);
    lv_label_set_align(lblSerieNameObj, LV_LABEL_ALIGN_CENTER);
    lv_obj_realign(lblSerieNameObj);

    // Chart needs to be updated
    lv_chart_refresh(chartObj);
    
}

void getLastDive(char* lastDivePath)
{
    File file = SD.open("/lastDive.log", FILE_READ);
    if(file)
    {
        file.readStringUntil('\n').toCharArray(lastDivePath,200);
        file.close();
    }
}

SmallDiveData StatScreen::extractJson(char* json)
{    
    SmallDiveData smallDiveData;

    DynamicJsonDocument jDocument(200);   

    deserializeJson(jDocument, json);                                      
                smallDiveData.depth = jDocument["4"];                                
                smallDiveData.time = jDocument["5"];    
                smallDiveData.heartFrequency = jDocument["9"];
                smallDiveData.o2saturation = jDocument["12"];
    
    return smallDiveData;
}

void StatScreen::getData()
{
    char fullFilePath[29];
    diveData.clear();
    char data[200];
    int count = 0;
    int index = 0;    
    getLastDive(fullFilePath);

    if (!SD.exists(fullFilePath))
    {
        return;
    }

    File file = SD.open(fullFilePath, FILE_READ);
    if (file)
    {
        while (file.available())
        {
            file.readStringUntil('\n');
            index++;
        }
        int scaling = index / 20;   
        if (scaling == 0)
        {
            scaling = 1;
        }
             
        file.seek(0);
        Serial.print("index: ");
        Serial.println(index);
        file.readStringUntil('\n');
        while(file.available())
        {
            if(count % scaling == 0)
            {
                file.readStringUntil('\n').toCharArray(data,200);
                SmallDiveData small = extractJson(data);
                StatScreen::diveData.push_back(small);
            }
            else
            {
                file.readStringUntil('\n');
            }
            count++;
        }  
        file.close();
    }   
}

// For future use ...
void StatScreen::update() 
{

}

// Create new named-chart-series & add them into the list of available series
void StatScreen::addNamedChartSerie(const char* name, lv_color_t color) 
{
    //SHIT HAPPENS HERE
    NamedChartSerie serie = NamedChartSerie();
    serie.name = name;
    serie.series = 0;
    serie.series = lv_chart_add_series(chartObj, color);
    lv_chart_hide_series(chartObj, serie.series, true);
    namedSerieList.push_back(serie);
}

// Calculate the min/max-values, add next point on chart & write them into the referenced series
void StatScreen::processDiveData(NamedChartSerie& serie, int16_t value) 
{
    serie.max = std::max(value, serie.max);
    serie.min = std::min(value, serie.min);
}

// Apply min value & put point on chart-series
void StatScreen::addNormlizedPointsOnChart(NamedChartSerie& serie, int16_t value) 
{
    lv_chart_set_next(chartObj, serie.series, value - serie.min);
}

// Create Tick-Labels on the x- & y-axis
void StatScreen::createAxisLabels(NamedChartSerie& serie, int16_t min, int16_t max) 
{
    serie.xAxisLabels = String("0");
    serie.yAxisLabels = String(max);
    for (int i = 0; i < xGaps; i++) 
    {
        serie.xAxisLabels += String("\n");
    }
    for (int i = 0; i < yGaps; i++) 
    {
        serie.yAxisLabels += String("\n");
    }
    serie.xAxisLabels += String(StatScreen::diveData.size());
    serie.yAxisLabels += String(min);
}

// Switch to next series in the series-list & show them on the chart
void StatScreen::showNextSeries() 
{
    Serial.print("index -> ");
    Serial.println(currentSeriesIndex);

    // Hide current series on chart
    if(currentSeriesIndex == 0 || currentSeriesIndex == 1)
    {
        Serial.print("serie name: ");
        Serial.println(namedSerieList[currentSeriesIndex].name);
        lv_chart_hide_series(chartObj, namedSerieList[currentSeriesIndex].series, true);
    }
    else if(currentSeriesIndex == 2)
    {
        Serial.print("serie name: ");
        Serial.println(namedSerieList[currentSeriesIndex].name);
        lv_chart_hide_series(chartObj, namedSerieList[currentSeriesIndex].series, true);

        Serial.print("serie name of depth: ");
        Serial.println(namedSerieList[0].name);

        lv_chart_hide_series(chartObj, namedSerieList[0].series, false);
    }
    else if(currentSeriesIndex == 3)
    {
        Serial.print("serie name: ");
        Serial.println(namedSerieList[currentSeriesIndex].name);
        lv_chart_hide_series(chartObj, namedSerieList[currentSeriesIndex].series, true);
    }
    else if(currentSeriesIndex == 4)
    {
        Serial.print("serie name: ");
        Serial.println(namedSerieList[currentSeriesIndex].name);
        lv_chart_hide_series(chartObj, namedSerieList[currentSeriesIndex].series, true);
    }
    
    // Get next series
    currentSeriesIndex++;
    currentSeriesIndex %= namedSerieList.size();
    
    Serial.print("new serie name: ");
    Serial.println(namedSerieList[currentSeriesIndex].name);

    int maxIndex = currentSeriesIndex;
    int minIndex = currentSeriesIndex;

    if(currentSeriesIndex == 3 || currentSeriesIndex == 4)
    {
        if(namedSerieList[0].max > namedSerieList[currentSeriesIndex].max)
        {
            maxIndex = 0;
        }
        if(namedSerieList[0].min < namedSerieList[currentSeriesIndex].min)
        {
            minIndex = 0;
        }
        createAxisLabels(namedSerieList[currentSeriesIndex], namedSerieList[minIndex].min, namedSerieList[maxIndex].max);
    }

    // Update the series on the chart
    lv_chart_set_y_range(chartObj, LV_CHART_AXIS_PRIMARY_Y, 0, namedSerieList[maxIndex].max - namedSerieList[minIndex].min);
    lv_chart_set_x_tick_texts(chartObj, namedSerieList[maxIndex].xAxisLabels.c_str(), 1, LV_CHART_AXIS_DRAW_LAST_TICK);
    lv_chart_set_y_tick_texts(chartObj, namedSerieList[maxIndex].yAxisLabels.c_str(), 1, LV_CHART_AXIS_DRAW_LAST_TICK);
    // was false, the reason why it crashed
    lv_chart_hide_series(chartObj, namedSerieList[currentSeriesIndex].series, false);
    
    // Update Serie-Name-Label
    lv_label_set_text(lblSerieNameObj, namedSerieList[currentSeriesIndex].name);
    lv_obj_realign(lblSerieNameObj);
}
