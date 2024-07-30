#ifdef GAME

#include <LinkedList.h>
#include <MD_MAX72xx.h>
#include <CCommon.h>
#include <CJoystick.h>
#include <CWebserver.h>

const unsigned long TIME_TO_POWER_DOWN = 60000;     //1 minute
const uint8_t MAX_SNAKE_LENGTH = 16;
enum class EState { S_LOAD, S_SHOW, S_CALCULATE };

// abstract base class
class CLedGame
{
public:
    CLedGame(uint8_t csPin, uint8_t iNumDevices, uint8_t iPinAxisX, uint8_t iPinAxisY, uint8_t iPinButton, bool isWifiConnected)
    {
        // initialize variables
#if IS_D1MINI
        m_leds = new MD_MAX72XX(MD_MAX72XX::FC16_HW, D7, D5, csPin, iNumDevices);
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
        m_iCurrentLevel = 0;
        m_lLastTime = millis();
    };
    void StartGame();

    // Data accessors
    uint8_t GetNumDevices()
    {
        return m_iNumDevices;
    };
protected:
    // Fields
    MD_MAX72XX* m_leds;
    CJoystick* m_joystick;
    CWebserver* m_webServer;
    uint8_t m_iNumDevices;
    EDirection m_lastDirectionX, m_lastDirectionY, m_lastDirection;
    int m_iButtonZ;
    uint8_t m_iCurrentLevel;
    unsigned long m_lLastTime;

    // Protected methods
    void ReadUserControls();
    virtual void RefreshAnimation() = 0;   //pure virtual function -> to be implemented in the derived classes
    virtual void GameCalculate() = 0;
    virtual void ResetGame() = 0;
};

// derived class for Tetris game. Finish CLedGame_Tetris.cpp by adding remaining methods RefreshAnimation() and GameCalculate()
class CLedGameTetris : public CLedGame
{
public:
    CLedGameTetris(uint8_t csPin, uint8_t iNumDevices, uint8_t iPinAxisX, uint8_t iPinAxisY, uint8_t iPinButton, bool isWifiConnected) : CLedGame(csPin, iNumDevices, iPinAxisX, iPinAxisY, iPinButton, isWifiConnected)
    {
        // m_leds->setBuffer(1, sizeof(Pieces[0]), Pieces[0]);
        // m_leds->setBuffer(4, sizeof(Pieces[0]), Pieces[1]);
        // // m_leds->transform(4, MD_MAX72XX::TRC);
        // m_leds->setBuffer(7, sizeof(Pieces[0]), Pieces[2]);
        // m_leds->setBuffer(10, sizeof(Pieces[0]), Pieces[3]);
        // m_leds->setBuffer(13, sizeof(Pieces[0]), Pieces[4]);
        // m_leds->setBuffer(16, sizeof(Pieces[0]), Pieces[5]);
        // m_leds->setBuffer(20, sizeof(PieceStick), PieceStick);
        m_state = EState::S_LOAD;
    };

private:
    // fields
    // const uint8_t PROGMEM Pieces[6][2] = 
    // {
    //     {0b00011000, 0b00011000},       //PieceSquare
    //     {0b00010000, 0b00111000},       //PieceTriangle
    //     {0b00011000, 0b00110000},       //PieceLeftTurn
    //     {0b00110000, 0b00011000},       //PieceRightTurn
    //     {0b00001000, 0b00111000},       //PieceLeftGun
    //     {0b00100000, 0b00111000}        //PieceRightGun
    // };
    // const uint8_t PROGMEM PieceStick = 0b00111100;
    
    EState m_state;
    uint8_t m_currentPiece[2];

    // private methods
    virtual void RefreshAnimation();
    virtual void GameCalculate();
    virtual void ResetGame();
};

// derived class for Snake game. Finish CLedGame_Snake.cpp by adding remaining methods RefreshAnimation() and GameCalculate()
class CLedGameSnake : public CLedGame
{
public:
    CLedGameSnake(uint8_t csPin, uint8_t iNumDevices, uint8_t iPinAxisX, uint8_t iPinAxisY, uint8_t iPinButton, bool isWifiConnected) : CLedGame(csPin, iNumDevices, iPinAxisX, iPinAxisY, iPinButton, isWifiConnected)
    {
        m_Snake = new LinkedList<LongCoordinateXY*>();
        ResetGame();                            //implicitly starts with level 3 with 3 dots for the snake
        SetNewEgg();
    };

private:
    LinkedList<LongCoordinateXY*>* m_Snake;
    LongCoordinateXY m_Egg;
    virtual void RefreshAnimation();
    virtual void GameCalculate();
    virtual void ResetGame();
    void SetNextSnakePos(EDirection direction);
    void SetNewEgg();
};

#endif