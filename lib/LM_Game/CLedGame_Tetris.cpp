#ifdef GAME

#include "CLedGame.h"

// Protected methods
void CLedGameTetris::RefreshAnimation()
{
    m_leds->clear();
    m_leds->update(MD_MAX72XX::OFF);
    
    //draw falling piece
    IntCoordinateXY* item;
    LinkedList<IntCoordinateXY*>* currPieceCoordinate = m_CurrPiece->GetCoordinates();
    for (int i = 0; i < currPieceCoordinate->size(); i++)
    {
        item = currPieceCoordinate->get(i);
        m_leds->setPoint(item->x, item->y, true);
    }
#ifdef DEBUG
    if (currPieceCoordinate->size() == 0)
        Serial.println("WARNING! Piece size is 0!");
#endif
    currPieceCoordinate->clear();
    delete currPieceCoordinate;

    //draw Base
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < m_iNumDevices * 8; y++)
        {
            if (m_Base[x][y] == true)
                m_leds->setPoint(x, y, true);
        }
    }

    m_leds->update(MD_MAX72XX::ON);
    delay(300);         //this delay can be dynamic by regulating speed according to difficulty level
}

void CLedGameTetris::GameCalculate()
{
    IntCoordinateXY* item;
    LinkedList<IntCoordinateXY*>* currPieceCoordinate = m_CurrPiece->GetCoordinates();

    //check move horizontal X: [-1, +1]
    bool isOk = true;
    int xOffset = 0;
    if (m_lastDirectionX == EDirection::Left ||
        m_lastDirectionX == EDirection::Right)
    {
        for (int i = 0; i < currPieceCoordinate->size(); i++)
        {
            item = currPieceCoordinate->get(i);
            if (m_lastDirectionX == EDirection::Left &&
                item->x - 1 < 0)
            {
                isOk = false;
                break;
            }
            else if (m_lastDirectionX == EDirection::Right &&
                     item->x + 1 >= 8)
            {
                isOk = false;
                break;
            }
            else
            {
                xOffset = (m_lastDirectionX == EDirection::Left) ? -1 : 1;
                isOk &= m_Base[item->x + xOffset][item->y] == false;
            }
        }
        if (isOk)
            m_CurrPiece->MoveHorizontalPiece(m_lastDirectionX);
    }
    
    //check rotation
    isOk = true;
    if (m_ButtonA || m_ButtonB)
    {
        bool clockwise = m_ButtonA ? true : false;
        m_CurrPiece->RotatePiece(clockwise);    //just rotate and check if possible
        for (int i = 0; i < currPieceCoordinate->size(); i++)
        {
            item = currPieceCoordinate->get(i);
            isOk &= m_Base[item->x][item->y] == false;
        }
        if (!isOk)                              //if not possible, rotate to previous orientation
            m_CurrPiece->RotatePiece(!clockwise);
    }

    //check move vertical Y: [+1]. If allowed -> m_currentLevel++ otherwise AddPiece()
    isOk = true;
    for (int i = 0; i < currPieceCoordinate->size(); i++)
    {
        item = currPieceCoordinate->get(i);
        if (item->y + 1 >= m_iNumDevices * 8)
        {
            isOk = false;
            break;
        }
        isOk &= m_Base[item->x][item->y + 1] == false;
    }
    if (isOk)
    {
        m_iCurrentLevel++;
        m_CurrPiece->SetCurrentLevel(m_iCurrentLevel);
    }
    else
    {
        isOk = true;
        for (int i = 0; i < currPieceCoordinate->size(); i++)
        {
            item = currPieceCoordinate->get(i);
            if (item->y == 0)
            {
                isOk = false;
                break;
            }
        }
        //check piece already at top -> ResetGame()
        if (isOk)
        {
            AddPieceToBase();
            RemoveBaseCompleteLines();
            CreateNewPiece(false);
        }
        else
            ResetGame();
    }

    currPieceCoordinate->clear();
    delete currPieceCoordinate;
}

void CLedGameTetris::ResetGame()
{
    CreateNewPiece(true);

    //TODO: wait for button to start new game

}

void CLedGameTetris::AddPieceToBase()
{
    IntCoordinateXY* item;
    LinkedList<IntCoordinateXY*>* currPieceCoordinate = m_CurrPiece->GetCoordinates();
    for (int i = 0; i < currPieceCoordinate->size(); i++)
    {
        item = currPieceCoordinate->get(i);
        m_Base[item->x][item->y] = true;
    }
    currPieceCoordinate->clear();
    delete currPieceCoordinate;
}

void CLedGameTetris::CreateNewPiece(bool resetBase)
{
    m_lastDirection = EDirection::None;
    m_lastDirectionX = EDirection::None;
    m_lastDirectionY = EDirection::None;
    m_iCurrentLevel = 0;
    m_CurrPiece->NewPiece();

    if (resetBase)
    {
        for (int x = 0; x < 8; x++)
            for (int y = 0; y < m_iNumDevices * 8; y++)
                m_Base[x][y] = false;
    }
}

void CLedGameTetris::RemoveBaseCompleteLines()
{
    bool toRemove;
    int yLine;
    LinkedList<int>* linesToRemove = new LinkedList<int>();
    
    for (yLine = m_iNumDevices * 8 - 1; yLine >= 0; yLine--)
    {
        toRemove = true;
        for (int x = 0; x < 8; x++)
            toRemove &= m_Base[x][yLine];

        if (toRemove)
            linesToRemove->add(yLine);
    }

    for (int i = 0; i < linesToRemove->size(); i++)
    {
        yLine = linesToRemove->get(i);
        for (int y = yLine; y >= 1; y--)
        {
            for (int x = 0; x < 8; x++)
                m_Base[x][y] = m_Base[x][y - 1];
        }
    }
    for (int x = 0; x < 8; x++)
        m_Base[x][0] = false;
    
    linesToRemove->clear();
    delete linesToRemove;
}
#endif