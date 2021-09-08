#include "CustomTouchButton.h"

void CustomTouchButton::btnClickEventListener(std::function<void()> onClickFunction) 
{
    //we have to use this print statement at this point to prevent the ESP
    //from doing stupid shit
    Serial.print("");
    if(m_verifiedTouch)
    {
        if(touchRead(m_pin) > 60)
        {
            onClickFunction();
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
    
    

    /*
    int touchValue = touchRead(m_pin);
    if(!m_b1 && touchValue<35 && touchValue>7) 
    {
        Serial.println("step 1");
        Serial.print("value: ");
        Serial.println(touchValue);
        m_b1 = true;
    }
    else if(m_b1 && !m_b2 && touchValue<35 && touchValue>7) 
    {
        Serial.println("step 2");
        Serial.print("value: ");
        Serial.println(touchValue);m_b2 = true;
    }
    else if(m_b2 && touchValue > 60) 
    {      
        Serial.println("step 3");
        Serial.print("value: ");
        Serial.println(touchValue);
        m_b1 = false;
        m_b2 = false;
        onClickFunction();     
    }
    */
}

/*

static void CustomTouchButton::Callback(void* this_pointer) {
    CustomTouchButton* self = static_cast<CustomTouchButton*>(this_pointer);
    self->isStillTouched();
}

void CustomTouchButton::isStillTouched() 
{
    if(touchRead(m_pin) > 60)
    {
        if((millis() - m_touchTime) >= 200)
        {
            m_verifiedTouch = true;
            Serial.println("verified touch!");
        }
        m_touched = false;
        m_ticker.detach();
    }
}

void CustomTouchButton::isTouched() 
{
    if(!m_touched) 
    {
        m_ticker.attach_ms(100, callback);
        m_touchTime = millis();
        m_touched = true;
    }
}
*/


