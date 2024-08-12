#ifdef GAME

#include "CLedGame.h"

const unsigned long TIME_TO_POWER_DOWN = 300000;     //5 minutes

#pragma region Public methods

void CLedGame::RefreshGame()
{
    if (m_lastDirectionX != EDirection::None || m_lastDirectionY != EDirection::None ||
        m_ButtonA || m_ButtonB)
    {
        //reset count to DeepSleep whenever there is user interaction
        m_lLastTime = millis();
    }
    
    //show animation, read user movement with joystick and calculate game mechanics for next animation iteration
    this->RefreshAnimation();
    ReadUserControls();
    this->GameCalculate();
    ResetWebserverDirection();

    // check if enter Power Down to save battery
    if (millis() - m_lLastTime >= TIME_TO_POWER_DOWN)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        m_leds->clear();
#ifdef IS_ESP32        
        //wake-up input using esp_sleep_enable_ext0_wakeup(pin) (GPIO27)
        esp_deep_sleep_start();
#else
        //wake-up input using RST pin
        ESP.deepSleep(0);
#endif
        m_lLastTime = millis();     //it's important to reset the counter after wake up!
        digitalWrite(LED_BUILTIN, LOW);
    }
}

#pragma endregion

// Protected methods
void CLedGame::ReadUserControls()
{
    if (m_webServer != NULL)
    {
        m_webServer->HandleClient();
        int direction = m_webServer->GetDirection();
#ifdef DEBUG
        if (direction != 0)
            Serial.println("Reading Direction from webServer: " + String(direction));
#endif
        //TODO: read from buffer (Queue) of commands
        if (direction == 0)
        {
            m_lastDirectionX = EDirection::None;
            m_lastDirectionY = EDirection::None;
        }
        else if (direction == 1)
        {
            m_lastDirectionX = EDirection::Left;
            m_lastDirectionY = EDirection::None;
        }
        else if (direction == 2)
        {
            m_lastDirectionX = EDirection::Right;
            m_lastDirectionY = EDirection::None;
        }
        else if (direction == 3)
        {
            m_lastDirectionX = EDirection::None;
            m_lastDirectionY = EDirection::Up;
        }
        else if (direction == 4)
        {
            m_lastDirectionX = EDirection::None;
            m_lastDirectionY = EDirection::Down;
        }

        m_ButtonA = m_webServer->GetButtonA();
        m_ButtonB = m_webServer->GetButtonB();
#ifdef DEBUG
        if (m_ButtonA || m_ButtonB)
            Serial.println("A button has been pressed");
#endif        
    }
    else
    {
#ifdef DEBUG
        Serial.println("No webServer as WiFi connection is offline. Read direction from CJoystick...");
#endif
        digitalWrite(LED_BUILTIN, HIGH);

        // Read inputs from user
        m_iButtonZ = m_joystick->ReadButton();

        // //we don't need values for axis (X, Y) because we just need direction
        // m_joystick->ReadAxisX();
        // m_joystick->ReadAxisY();
        // m_lastDirectionX = m_joystick->GetDirectionX(); //default is 1 (Left) when no Joystick connected
        // m_lastDirectionY = m_joystick->GetDirectionY(); //default is 3 (Up) when no Joystick connected
#ifdef DEBUG
    Serial.print("Reading Direction from Joystick X: " + String((int)m_lastDirectionX));
    Serial.println(". Y: " + String((int)m_lastDirectionY));
#endif
    }
}

void CLedGame::ResetWebserverDirection()
{
    bool hasChangedDirection = m_lastDirectionX != EDirection::None || m_lastDirectionY != EDirection::None;
    if (m_webServer != NULL)
    {
        if (hasChangedDirection)
            m_webServer->ResetDirection();
        if (m_webServer->GetButtonA())
            m_webServer->ResetButton(true);
        if (m_webServer->GetButtonB())
            m_webServer->ResetButton(false);
    }
}

#endif