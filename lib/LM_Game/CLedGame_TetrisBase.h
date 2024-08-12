#ifdef GAME

#include "CLedGame_TetrisPiece.h"   //contains all common libraries

class CLedGameTetrisBase
{
public:
    CLedGameTetrisBase()
    {
        m_Base = new LinkedList<IntCoordinateXY*>();
    };
    void NewBase();
    void AddPiece(LinkedList<IntCoordinateXY*>* piece, int left, int currentLevel);
    bool CheckPiece(LinkedList<IntCoordinateXY*>* piece);

    // Data accessors
    LinkedList<IntCoordinateXY*>* GetCoordinates()
    {
        return m_Base;
    };
private:
    // fields
    LinkedList<IntCoordinateXY*>* m_Base;

    // private methods
};

#endif