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
    for (int i = 0; i < m_Base->GetCoordinates()->size(); i++)
    {
        item = m_Base->GetCoordinates()->get(i);
        m_leds->setPoint(item->x, item->y, true);
    }
    m_leds->update(MD_MAX72XX::ON);

    delay(300);         //this delay can be dynamic by regulating speed according to difficulty level
}

void CLedGameTetris::GameCalculate()
{
    //move horizontally and rotate CurrentPiece with no base
    if (m_lastDirectionX == EDirection::Left ||
        m_lastDirectionX == EDirection::Right)
        m_CurrPiece->MoveHorizontalPiece(m_lastDirectionX);
    if (m_ButtonA)
        m_CurrPiece->RotatePiece(true);
    if (m_ButtonB)
        m_CurrPiece->RotatePiece(false);

    m_iCurrentLevel++;    //advance vertically one line
    if (m_CurrPiece->MoveVerticalPiece(m_iCurrentLevel))
    {
        //add piece to Base
        //TODO: some of the points can be already added to Base
        m_Base->AddPiece(m_CurrPiece->GetCoordinates(), m_CurrPiece->GetLeft(), m_iCurrentLevel);
        m_iCurrentLevel = 0;
    }

    //-----------///
    //TODO: implement the correct check piece against Base

}

void CLedGameTetris::ResetGame()
{
    m_lastDirection = EDirection::None; //default initial direction
    m_iCurrentLevel = 0;
    m_CurrPiece->NewPiece();
    m_Base->NewBase();

    //TODO: wait for button to start new game

}
#endif