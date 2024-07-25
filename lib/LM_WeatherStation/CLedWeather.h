#include <MD_MAX72xx.h>
#include <CCommon.h>
#include <CJoystick.h>

//TODO: made these constants parameters of the constructors and set in the main.cpp instead of here
const uint8_t IN_AXIS_X = 17;   //A3
const uint8_t IN_AXIS_Y = 18;   //A4
const uint8_t IN_BUTTON = 2;
const unsigned long TIME_TO_POWER_DOWN = 60000;     //1 minute

class CLedWeather
{
public:
    // Constructors
    CLedWeather(uint8_t csPin, uint8_t iNumDevices, uint8_t iPinAxisX, uint8_t iPinAxisY, uint8_t iPinButton)
    {
        // initialize variables
        m_leds = new MD_MAX72XX(MD_MAX72XX::FC16_HW, csPin, iNumDevices);
        m_leds->begin();
        m_iNumDevices = iNumDevices;

        m_joystick = new CJoystick(IN_AXIS_X, IN_AXIS_Y, IN_BUTTON);
        m_lLastTime = millis();
    };

    // Data accessors
	uint8_t GetNumDevices()
	{
		return m_iNumDevices;
	};

private:
    // Fields
    MD_MAX72XX* m_leds;
    CJoystick* m_joystick;
    uint8_t m_iNumDevices;
    unsigned long m_lLastTime;

    // Private methods
    void ReadSensors();
};