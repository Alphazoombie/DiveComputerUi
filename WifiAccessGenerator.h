#ifndef WIFIACCESSGENERATOR_H
#define WIFIACCESSGENERATOR_H

#include <Arduino.h>
#include <random>

class WifiAccessGenerator
{
public:

    static String generateSsid()
    {
        String ssid("DC_");

        std::random_device generator;
        std::mt19937 eng(millis());
        std::uniform_int_distribution<int> distribution(0, 9);

        for (int i = 0; i < 5; i++)
        {
            ssid += distribution(eng);     
        }
        delay(1);
        return ssid;
    }

    static String generatePassword()
    {
        String password;
        std::random_device generator;
        std::mt19937 eng(millis());
        std::uniform_int_distribution<int> distribution(0, 9);

        for (int i = 0; i < 5; i++)
        {
            password += distribution(eng);
        }
        delay(1);
        return password;
    }
};

#endif