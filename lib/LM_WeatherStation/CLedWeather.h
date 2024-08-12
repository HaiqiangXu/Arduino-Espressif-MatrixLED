#include <MD_MAX72xx.h>
#include <CCommon.h>
#include <CJoystick.h>

const unsigned long TIME_TO_POWER_DOWN = 60000;     //1 minute

class CLedWeather
{
public:
    // Constructors
    CLedWeather(int csPin, int iNumDevices, int iPinAxisX, int iPinAxisY, int iPinButton)
    {
        // initialize variables
        m_leds = new MD_MAX72XX(MD_MAX72XX::FC16_HW, csPin, iNumDevices);
        m_leds->begin();
        m_iNumDevices = iNumDevices;

        m_joystick = new CJoystick(iPinAxisX, iPinAxisY, iPinButton);
        m_lLastTime = millis();
    };

    // Public methods  
    void ReadSensors();

    // Data accessors
	int GetNumDevices()
	{
		return m_iNumDevices;
	};

private:
    // Fields
    MD_MAX72XX* m_leds;
    CJoystick* m_joystick;
    int m_iNumDevices;
    unsigned long m_lLastTime;

    // Private methods
    void ReadSensors();
};