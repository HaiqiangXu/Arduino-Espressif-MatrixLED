#ifdef GAME

#include "CLedGame.h"

// Protected methods
void CLedGameTetris::RefreshAnimation()
{
    if (m_state != EState::S_SHOW)
        return;

    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
    // m_leds->control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::OFF);
    // m_leds->control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::ON);

    if (m_lastDirectionX == EDirection::Left)
        m_leds->transform(2, MD_MAX72XX::TSU);
    else if (m_lastDirectionX == EDirection::Right)
        m_leds->transform(2, MD_MAX72XX::TSD);
    else if (m_lastDirectionX == EDirection::None || //TODO: to be deleted
             m_lastDirectionY == EDirection::None ||
             m_lastDirectionY == EDirection::Down)
        m_leds->transform(2, MD_MAX72XX::TSL);

    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

    delay(250);
}

void CLedGameTetris::GameCalculate()
{
    if (m_state == EState::S_LOAD)
    {
        //uint16_t index = TrueRandom.random(7);
        // if (index == 7)
        //     m_currentPiece = &Pieces[index];         //TODO: set to m_leds->setBuffer()
        // else
        //     m_currentPiece = &PieceStick;

        m_state = EState::S_SHOW;
    }
}

#endif