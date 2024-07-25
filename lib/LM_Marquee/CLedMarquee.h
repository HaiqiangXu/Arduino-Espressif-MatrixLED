#include <MD_MAX72xx.h>

const uint8_t IN_BTN = 2;
const int DELAY_TIME = 100;     // milliseconds
const int TIMEOUT_TIME = 3000;  // 3 seconds

enum class EMarqueeStyle { Test, Text, Pacman };

class CLedMarquee
{
public:
    // Constructors
    CLedMarquee(uint8_t csPin, uint8_t iNumDevices, EMarqueeStyle marquee)
    {
        // initialize devices and variables
        pinMode(IN_BTN, INPUT_PULLUP);
        m_leds = new MD_MAX72XX(MD_MAX72XX::FC16_HW, csPin, iNumDevices);
        m_leds->begin();
        /* Default library's values -> half intensity and auto-update:
            - control(MD_MAX72XX::INTENSITY, MAX_INTENSITY / 2);
            - control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON); */
        m_leds->control(MD_MAX72XX::INTENSITY, MAX_INTENSITY >> 4);
        m_iNumDevices = iNumDevices;
        m_currentMarquee = marquee;
    };

    // Public methods
    void ShowMarquee();

    // Data accessors
	uint8_t GetDevices()
	{
		return m_iNumDevices;
	};

    EMarqueeStyle GetMarqueeStyle()
    {
        return m_currentMarquee;
    };

    void SetMarqueeStyle(EMarqueeStyle style)
    {
        m_currentMarquee = style;
    };

    void SetText(const char* text)
    {
        ///strcpy(m_msgText, text);     //NOTE: m_msgText is a pointer, not a char array with info so strcpy() is not copying
        m_msgText = text;
        m_msgTextIni = text;
    };
private:
    // Fields
    MD_MAX72XX* m_leds;
    EMarqueeStyle m_currentMarquee;
    uint8_t m_iNumDevices;
    const char* m_msgText, *m_msgTextIni;

    // Private methods
    void TestsOneDevice();
    void TestsAdvanced();
    void TestsTransformations();
    void ScrollText();
    void ShowPacman();
    void BlinkEyes();
};