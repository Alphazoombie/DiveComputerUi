#ifndef WIFIACCESSGENERATOR_H
#define WIFIACCESSGENERATOR_H

#include <Arduino.h>
#include <random>

class WifiAccessGenerator
{
public:

    static void generateSsid(char* ssid)
    {
        std::random_device generator;
        std::mt19937 eng(millis());
        std::uniform_int_distribution<int> distribution(0, 9);

        char temp[2];

        for (int i = 3; i < 8; i++)
        {
            snprintf(temp, sizeof(temp), "%i", distribution(eng));
            ssid[i] = temp[0];     
        }
        delay(1);
    }

    static void generatePassword(char* password)
    {
        std::random_device generator;
        std::mt19937 eng(millis());
        std::uniform_int_distribution<int> distribution(0, 9);

        char temp[2];

        for (int i = 0; i < 8; i++)
        {
            snprintf(temp, sizeof(temp), "%i", distribution(eng));
            password[i] = temp[0];
        }
        delay(1);
    }
};

#endif
