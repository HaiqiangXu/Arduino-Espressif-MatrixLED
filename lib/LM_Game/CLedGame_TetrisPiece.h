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
    CLedGameTetrisPiece(int ledsColumnCount)
    {
        m_ledsColumnCount = ledsColumnCount;
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
    bool MoveVerticalPiece(int currentLevel)
    {
        int maxPieceY = currentLevel + m_CurrPieceBoxHeight;
        if (maxPieceY > m_ledsColumnCount)
        {
            NewPiece();
            return true;
        }

        return false;
    }

    // Data accessors
    LinkedList<IntCoordinateXY*>* GetCoordinates()
    {
        return m_CurrPiece;
    };

    int GetLeft()
    {
        return m_CurrPieceBoxLeft;
    };

private:
    // fields
    LinkedList<IntCoordinateXY*>* m_CurrPiece;
    EPieceType m_CurrPieceType;
    int m_ledsColumnCount;
    int m_CurrPieceBoxLeft;
    int m_CurrPieceBoxWidth;
    int m_CurrPieceBoxHeight;
    int m_CurrRotation;

    // private methods
};

#endif