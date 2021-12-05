/**
 * @author Manuel Bäcker
 * @author Jan Tröger
 * @author Tim Tröger
 */

#ifndef CUSTOMTOUCHBUTTON_H
#define CUSTOMTOUCHBUTTON_H

#include <Arduino.h>
#include <functional>
#include <Ticker.h>

class CustomTouchButton
{
public:
    CustomTouchButton() {}

    CustomTouchButton(int pin) : m_pin(pin) {}
    
    void btnClickEventListener(std::function<void()> onClickFunction);

private:
    int m_pin;
    bool m_b1 = false; 
    bool m_b2 = false;
    int m_touchCount = 0;
    int m_count = 1;
    bool m_inCycle = false;
    bool m_verifiedTouch = false;
};

#endif