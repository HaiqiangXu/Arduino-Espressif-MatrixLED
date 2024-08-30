#ifdef GAME

#include "CLedGame_TetrisPiece.h"

// abstract base class
class CLedGame
{
public:
    CLedGame(int csPin, int iNumDevices, int iPinAxisX, int iPinAxisY, int iPinButton, bool isWifiConnected)
    {
        // initialize variables
#if IS_D1MINI
        m_leds = new MD_MAX72XX(MD_MAX72XX::FC16_HW, D7, D5, csPin, iNumDevices);
#elif IS_NODEMCU
        m_leds = new MD_MAX72XX(MD_MAX72XX::FC16_HW, D1, D2, csPin, iNumDevices);
#elif IS_ESP32
        m_leds = new MD_MAX72XX(MD_MAX72XX::FC16_HW, MOSI, SCK, csPin, iNumDevices);

        //NOTE: only RTC GPIO pins allowed as wake-up source. These include GPIOs 36, 39, 34, 35, 32, 33, 25, 26, 27, 14, 12, 13, 0, 15, 2, and 4
        //Tested working: 33, 14, 27. Tested not working: 39, 34. Rest RTC GPIOs are not tested
        esp_sleep_enable_ext0_wakeup(GPIO_NUM_27, 0);
        // touchAttachInterrupt(T3, NULL, 15);	//When the value read on T3 (GPIO 15) is lower than the value set on the Threshold variable, the ESP32 wakes up and callback function is executed
        // esp_sleep_enable_touchpad_wakeup();
#else
        m_leds = new MD_MAX72XX(MD_MAX72XX::FC16_HW, csPin, iNumDevices);
#endif
        m_leds->begin();
        m_leds->control(MD_MAX72XX::INTENSITY, MAX_INTENSITY >> 4);
        m_iNumDevices = iNumDevices;
        if (isWifiConnected)
            m_webServer = new CWebserver();
        else
            m_webServer = NULL;

        m_joystick = new CJoystick(iPinAxisX, iPinAxisY, iPinButton);
        m_lastDirectionX = EDirection::None;
        m_lastDirectionY = EDirection::None;
        m_ButtonA = false;
        m_ButtonB = false;
        m_iCurrentLevel = 0;
        m_lLastTime = millis();
    };
    void RefreshGame();

    // Data accessors
    int GetNumDevices()
    {
        return m_iNumDevices;
    };
protected:
    // Fields
    MD_MAX72XX* m_leds;
    CJoystick* m_joystick;
    CWebserver* m_webServer;
    int m_iNumDevices;
    EDirection m_lastDirectionX, m_lastDirectionY, m_lastDirection;
    bool m_ButtonStart, m_ButtonA, m_ButtonB;
    int m_iButtonZ;
    int m_iCurrentLevel;
    unsigned long m_lLastTime;

    // Protected methods
    void ReadUserControls();
    void ResetWebserverDirection();
    virtual void RefreshAnimation() = 0;   //pure virtual function -> to be implemented in the derived classes
    virtual void GameCalculate() = 0;
    virtual void ResetGame() = 0;
};

// derived class for Tetris game. Finish CLedGame_Tetris.cpp by adding remaining methods RefreshAnimation() and GameCalculate()
class CLedGameTetris : public CLedGame
{
public:
    CLedGameTetris(int csPin, int iNumDevices, int iPinAxisX, int iPinAxisY, int iPinButton, bool isWifiConnected) : CLedGame(csPin, iNumDevices, iPinAxisX, iPinAxisY, iPinButton, isWifiConnected)
    {
        m_CurrPiece = new CLedGameTetrisPiece();
        m_Base = new bool*[8];  //fix width of 8 pixels
        for (int x = 0; x < 8; x++)
            m_Base[x] = new bool[m_iNumDevices * 8];
        ResetGame();
    };

private:
    // fields
    CLedGameTetrisPiece* m_CurrPiece;
    bool** m_Base;
    int m_CurrentY;

    // protected methods
    virtual void RefreshAnimation();
    virtual void GameCalculate();
    virtual void ResetGame();

    // private methods
    int DownPiece();
    void AddPieceToBase();
    void CreateNewPiece(bool resetBase);
    void RemoveBaseCompleteLines();
};

// derived class for Snake game. Finish CLedGame_Snake.cpp by adding remaining methods RefreshAnimation() and GameCalculate()
class CLedGameSnake : public CLedGame
{
public:
    CLedGameSnake(int csPin, int iNumDevices, int iPinAxisX, int iPinAxisY, int iPinButton, bool isWifiConnected) : CLedGame(csPin, iNumDevices, iPinAxisX, iPinAxisY, iPinButton, isWifiConnected)
    {
        m_Snake = new LinkedList<IntCoordinateXY*>();
        ResetGame();                            //implicitly starts with level 3 with 3 dots for the snake
        SetNewEgg();
    };

private:
    LinkedList<IntCoordinateXY*>* m_Snake;
    IntCoordinateXY m_Egg;

    // Protected methods
    virtual void RefreshAnimation();
    virtual void GameCalculate();
    virtual void ResetGame();
    // private methods
    void SetNextSnakePos(EDirection direction);
    void SetNewEgg();
};

#endif