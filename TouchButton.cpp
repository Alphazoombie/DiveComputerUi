/**
 * @author Manuel Bäcker
 * @author Jan Tröger
 * @author Tim Tröger
 */

#include "TouchButton.h"

void TouchButton::addTouchEvent(std::function<void()> onTouchEventCallback) 
{
    //the print statement is needed to run the logic below without errors
    Serial.print("");
    if(m_verifiedTouch)
    {
        if(touchRead(m_pin) > 60)
        {
            onTouchEventCallback();
            m_verifiedTouch = false;
            m_count = 1;
        }
    }
    else
    {
        if(touchRead(m_pin) < 35)
        {
            m_touchCount++;
            m_inCycle = true;
        }
        if(m_count == 4) 
        {
            if(m_touchCount >= 3)
            {
                m_verifiedTouch = true;
            }
            m_touchCount = 0;
            m_count = 1;
            m_inCycle = false;
        }
        if(m_inCycle)
        {
            m_count++;
        }
    }
}