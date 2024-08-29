#ifdef GAME

#include <LinkedList.h>
#include <MD_MAX72xx.h>
#include <CCommon.h>
#include <CJoystick.h>
#include <CWebserver.h>
#ifndef IS_ESP32
#include <ESP8266TrueRandom.h>
#endif

enum class EPieceType { Triangle, BigLine, Square, LInverted, LShape, ZShape, ZInverted };

class CLedGameTetrisPiece
{
public:
    CLedGameTetrisPiece()
    {
        m_CurrPiece = new LinkedList<IntCoordinateXY*>();
    };
    void NewPiece();
    void RotatePiece(bool clockwise);
    void MoveHorizontalPiece(EDirection direction);

    // Data accessors
    LinkedList<IntCoordinateXY*>* GetCoordinates()
    {
        IntCoordinateXY* item;
        IntCoordinateXY* newItem;
        LinkedList<IntCoordinateXY*>* coordinates = new LinkedList<IntCoordinateXY*>();
        for (int i = 0; i < m_CurrPiece->size(); i++)
        {
            item = m_CurrPiece->get(i);
            newItem = new IntCoordinateXY { m_CurrPieceBoxLeft + item->x, m_iCurrentLevel + item->y };
            coordinates->add(newItem);
        }

        return coordinates;
    };

    void SetCurrentLevel(int iCurrentLevel)
    {
        m_iCurrentLevel = iCurrentLevel;
    };

private:
    // fields
    LinkedList<IntCoordinateXY*>* m_CurrPiece;
    EPieceType m_CurrPieceType;
    int m_CurrPieceBoxLeft;
    int m_CurrPieceBoxWidth;
    int m_CurrRotation;
    int m_iCurrentLevel;

    // private methods
};

#endif