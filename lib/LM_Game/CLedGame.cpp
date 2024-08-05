#ifdef GAME

#include "CLedGame.h"

#pragma region Public methods

void CLedGame::StartGame()
{
    // consider idle if Joystick hasn't moved to call Power Down. Wake it up if Joystick button is pressed
    if (m_joystick->GetDirectionX() != EDirection::None ||
        m_joystick->GetDirectionY() != EDirection::None ||
        m_iButtonZ == LOW)
    {
        //reset count to PowerDown
        m_lLastTime = millis();
    }
    
    //show animation, read user movement with joystick and calculate game mechanics for next animation iteration
    this->RefreshAnimation();
    ReadUserControls();
    this->GameCalculate();

    // check if enter Power Down to save battery
    if (millis() - m_lLastTime >= TIME_TO_POWER_DOWN)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        m_leds->clear();
        //TODO: replace with LowerPower for ESP8266/ESP32:
        //AtmelPower.PowerDownInt0();
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
        Serial.println("Reading Direction from webServer: " + String(direction));
#endif            
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
    }
    else
    {
#ifdef DEBUG
        Serial.println("No webServer as WiFi connection is offline. Read direction from CJoystick...");
#endif
        digitalWrite(LED_BUILTIN, HIGH);

        // Read inputs from user
        m_iButtonZ = m_joystick->ReadButton();

        //we don't need values for axis (X, Y) because we just need direction
        m_joystick->ReadAxisX();
        m_joystick->ReadAxisY();
        m_lastDirectionX = m_joystick->GetDirectionX(); //default is 1 (Left) when no Joystick connected
        m_lastDirectionY = m_joystick->GetDirectionY(); //default is 3 (Up) when no Joystick connected
#ifdef DEBUG
    Serial.print("Reading Direction from Joystick X: " + String((int)m_lastDirectionX));
    Serial.println(". Y: " + String((int)m_lastDirectionY));
#endif
    }
}

#endif