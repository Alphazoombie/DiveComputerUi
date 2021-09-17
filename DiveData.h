#ifndef DIVEDATA_H
#define DIVEDATA_H

struct DiveData 
{
    int16_t depth = 0;
    int16_t temperatur = 0;
    int16_t brightness = 0;
    int16_t o2saturation = 0;
    int16_t heartFrequency = 0;
    int16_t heartVariability = 0;
    float gyroX = 0;
    float gyroY = 0;
    float gyroZ = 0;
    float accelX = 0;
    float accelY = 0;
    float accelZ = 0;
    float magnetField = 0;
    long  time = 0;
};

#endif