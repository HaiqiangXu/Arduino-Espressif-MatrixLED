#ifdef GAME

#include "CLedGame.h"

// Protected methods
void CLedGameTetris::RefreshAnimation()
{
    m_leds->clear();
    m_leds->update(MD_MAX72XX::OFF);
    
    //draw falling piece
    IntCoordinateXY* item;
    LinkedList<IntCoordinateXY*>* currPiece = m_CurrPiece->GetCoordinates();
    for (int i = 0; i < currPiece->size(); i++)
    {
        item = currPiece->get(i);
        m_leds->setPoint(item->x, item->y, true);
    }
#ifdef DEBUG
    if (currPiece->size() == 0)
        Serial.println("WARNING! Piece size is 0!");
#endif
    currPiece->clear();
    delete currPiece;

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
    LinkedList<IntCoordinateXY*>* currPiece = m_CurrPiece->GetCoordinates();

    //check move horizontal X: [-1, +1]
    bool isOk = true;
    int xOffset = 0;
    if (m_lastDirectionX == EDirection::Left ||
        m_lastDirectionX == EDirection::Right)
    {
#ifdef DEBUG
        Serial.println("Left/Right movement detected!");
#endif
        for (int i = 0; i < currPiece->size(); i++)
        {
            item = currPiece->get(i);
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
#ifdef DEBUG
        Serial.println("Rotation detected!");
#endif
        bool clockwise = m_ButtonA ? true : false;
        m_CurrPiece->RotatePiece(clockwise);    //just rotate and check if possible
        for (int i = 0; i < currPiece->size(); i++)
        {
            item = currPiece->get(i);
            isOk &= m_Base[item->x][item->y] == false;
        }
        if (!isOk)                              //if not possible, rotate to previous orientation
            m_CurrPiece->RotatePiece(!clockwise);
    }
    currPiece->clear();
    delete currPiece;

    //check move vertical Y: [+1]. If allowed -> m_currentLevel++ otherwise AddPiece()
    if (m_lastDirectionY == EDirection::Up)
    {
#ifdef DEBUG
        Serial.println("Up movement detected!. Move to bottom the piece");
#endif
        int action = 0;
        do
        {
            action = DownPiece();
        } while (action == 0);
    }
    else
        DownPiece();
}

int CLedGameTetris::DownPiece()
{
    IntCoordinateXY* item;
    LinkedList<IntCoordinateXY*>* currPiece = m_CurrPiece->GetCoordinates();
    bool isOk = true;
    int action = 0;

    for (int i = 0; i < currPiece->size(); i++)
    {
        item = currPiece->get(i);
        if (item->y + 1 >= m_iNumDevices * 8)
        {
            isOk = false;
            break;
        }
        isOk &= m_Base[item->x][item->y + 1] == false;
    }

    if (isOk)
    {
        //down one row
        m_CurrentY++;
        m_CurrPiece->SetCurrentY(m_CurrentY);
        action = 0;
    }
    else
    {
        isOk = true;
        for (int i = 0; i < currPiece->size(); i++)
        {
            if (item->y == 0)
            {
                isOk = false;
                break;
            }
        }

        if (isOk)   //piece contacted with Base
        {
            AddPieceToBase();
            RemoveBaseCompleteLines();
            CreateNewPiece(false);
            action = 1;
        }
        else        //piece already at top -> ResetGame()
        {
#ifdef DEBUG
            Serial.println("Reset game!");
#endif
            ResetGame();
            action = 2;
        }
    }
    currPiece->clear();
    delete currPiece;
    
    return action;
}

void CLedGameTetris::ResetGame()
{
    CreateNewPiece(true);

    //TODO: wait for button to start new game

}

void CLedGameTetris::AddPieceToBase()
{
    IntCoordinateXY* item;
    LinkedList<IntCoordinateXY*>* currPiece = m_CurrPiece->GetCoordinates();
    for (int i = 0; i < currPiece->size(); i++)
    {
        item = currPiece->get(i);
        m_Base[item->x][item->y] = true;
    }
    currPiece->clear();
    delete currPiece;
}

void CLedGameTetris::CreateNewPiece(bool resetBase)
{
    m_lastDirection = EDirection::None;
    m_lastDirectionX = EDirection::None;
    m_lastDirectionY = EDirection::None;
    m_CurrentY = 0;
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