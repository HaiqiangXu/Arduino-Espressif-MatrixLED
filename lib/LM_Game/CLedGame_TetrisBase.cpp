#ifdef GAME

#include "CLedGame_TetrisBase.h"

void CLedGameTetrisBase::NewBase()
{
    IntCoordinateXY* item;
    for (int i = m_Base->size() - 1; i >= 0; i--)
    {
        item = m_Base->remove(i);       //remove and destroy all objects inside list
        delete item;
    }
}

void CLedGameTetrisBase::AddPiece(LinkedList<IntCoordinateXY*>* piece, int left, int currentLevel)
{
    IntCoordinateXY* item;
    for (int i = 0; i < piece->size(); i++)
    {
        item = piece->get(i);
        m_Base->add(new IntCoordinateXY {left + item->x, currentLevel + item->y - 1});
    }
}

bool CLedGameTetrisBase::CheckPiece(LinkedList<IntCoordinateXY*>* piece)
{
    //TODO: check if piece can move hor., vert. and rotation. If no, add piece to base, if yes just return piece moved/rotated

    return false;
}

#endif