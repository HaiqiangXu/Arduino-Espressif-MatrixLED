#ifdef GAME

#include "CLedGame.h"

// Protected methods
void CLedGameTetris::RefreshAnimation()
{
    IntCoordinateXY* item;
    m_leds->clear();
    m_leds->update(MD_MAX72XX::OFF);
    for (int i = 0; i < m_CurrPiece->GetCoordinates()->size(); i++)
    {
        item = m_CurrPiece->GetCoordinates()->get(i);
        m_leds->setPoint(m_CurrPiece->GetLeft() + item->x, m_iCurrentLevel + item->y, true);
    }
#ifdef DEBUG
        if (m_CurrPiece->GetCoordinates()->size() == 0)
            Serial.println("WARNING! Piece size is 0!");
#endif
    for (int i = 0; i < m_Base->size(); i++)
    {
        item = m_Base->get(i);
        m_leds->setPoint(item->x, item->y, true);
    }
    m_leds->update(MD_MAX72XX::ON);

    delay(300);         //this delay can be dynamic by regulating speed according to difficulty level
}

void CLedGameTetris::GameCalculate()
{
    if (CheckPiece())
    {
        //piece can move horizontally or rotate
        if (m_lastDirectionX == EDirection::Left ||
            m_lastDirectionX == EDirection::Right)
            m_CurrPiece->MoveHorizontalPiece(m_lastDirectionX);
        if (m_ButtonA)
            m_CurrPiece->RotatePiece(true);
        if (m_ButtonB)
            m_CurrPiece->RotatePiece(false);
        m_iCurrentLevel++;  //implicitly moves the piece vertically
    }
    else
    {
        //TODO: some of the points can be already added to Base
        AddPieceToBase();
        CreateNewPiece(false);
    }
}

void CLedGameTetris::ResetGame()
{
    CreateNewPiece(true);

    //TODO: wait for button to start new game

}

void CLedGameTetris::AddPieceToBase()
{
    IntCoordinateXY* item;
    for (int i = 0; i < m_CurrPiece->GetCoordinates()->size(); i++)
    {
        item = m_CurrPiece->GetCoordinates()->get(i);
        m_Base->add(new IntCoordinateXY {m_CurrPiece->GetLeft() + item->x, m_iCurrentLevel + item->y - 1});
    }
}

void CLedGameTetris::CreateNewPiece(bool resetBase)
{
    m_lastDirection = EDirection::None;
    m_iCurrentLevel = 0;
    m_CurrPiece->NewPiece();

    if (resetBase)
    {
        IntCoordinateXY* item;
        for (int i = m_Base->size() - 1; i >= 0; i--)
        {
            item = m_Base->remove(i);       //remove and destroy all objects inside list
            delete item;
        }
    }
}

bool CLedGameTetris::CheckPiece()
{
    //TODO: check if piece can move hor. (X: [+1, -1])
    
    //TODO: check vert. (Y: [+1])
    
    //TODO: check rotation
    
    //If can do the movement -> just return true so piece is moved/rotated. If not, call AddPiece(), and if not and also it at top of the screen then call ResetGame() as it's game over

    //
    int maxPieceY = m_iCurrentLevel + m_CurrPiece->GetHeight();    
    if (maxPieceY > m_leds->getColumnCount())   //TODO: add more checkings at Y-axis apart of bottom pixel
        return false;

    return true;
}

#endif