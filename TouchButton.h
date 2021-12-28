/**
 * @author Manuel Bäcker
 * @author Jan Tröger
 * @author Tim Tröger
 */

#ifndef TOUCHBUTTON_H
#define TOUCHBUTTON_H

#include <Arduino.h>
#include <functional>

class TouchButton
{
public:
    TouchButton(int pin) : m_pin(pin) { }
    
    void addTouchEvent(std::function<void()> onTouchEventCallback);

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