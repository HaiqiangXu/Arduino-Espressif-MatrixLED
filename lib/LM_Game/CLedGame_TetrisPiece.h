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
    void MoveHorizontalPiece(EDirection direction)
    {
        if (direction == EDirection::Left)
        {
            if (m_CurrPieceBoxLeft > 0)
                m_CurrPieceBoxLeft--;
        }
        else if (direction == EDirection::Right)
        {
            if (m_CurrPieceBoxLeft + m_CurrPieceBoxWidth <= 7)
                m_CurrPieceBoxLeft++;
        }
    };

    // Data accessors
    LinkedList<IntCoordinateXY*>* GetCoordinates()
    {
        //TODO: improve how to access to coordinates of the piece
        return m_CurrPiece;
    };

    int GetLeft()
    {
        return m_CurrPieceBoxLeft;
    };
    int GetHeight()
    {
        return m_CurrPieceBoxHeight;
    };

private:
    // fields
    LinkedList<IntCoordinateXY*>* m_CurrPiece;
    EPieceType m_CurrPieceType;
    int m_CurrPieceBoxLeft;
    int m_CurrPieceBoxWidth;
    int m_CurrPieceBoxHeight;
    int m_CurrRotation;

    // private methods
};

#endif