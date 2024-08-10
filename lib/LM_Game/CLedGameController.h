#ifdef GAME

#include "CLedGame.h"

enum class EGame { Tetris, Snake };

class CLedGameController
{
public:
    // Constructors
    CLedGameController(int csPin, int iNumDevices, int iPinAxisX, int iPinAxisY, int iPinButton, EGame newGame, bool isWifiConnected)
    {
        m_currentGame = newGame;
        if (newGame == EGame::Tetris)
        {
            m_ledGame = new CLedGameTetris(csPin, iNumDevices, iPinAxisX, iPinAxisY, iPinButton, isWifiConnected);
        }
        else if (newGame == EGame::Snake)
        {
            m_ledGame = new CLedGameSnake(csPin, iNumDevices, iPinAxisX, iPinAxisY, iPinButton, isWifiConnected);
        }
    };

    // Public methods  
    void RefreshGame()
    {
        m_ledGame->RefreshGame();
    };

    // Data accessors
    EGame GetGame()
	{
		return m_currentGame;
	};

    void SetGame(EGame newGame)
    {
        m_currentGame = newGame;
    };
private:
    CLedGame* m_ledGame;
    EGame m_currentGame;
};

#endif