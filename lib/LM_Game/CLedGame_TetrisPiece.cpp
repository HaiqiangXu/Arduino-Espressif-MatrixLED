#ifdef GAME

#include "CLedGame_TetrisPiece.h"

void CLedGameTetrisPiece::NewPiece()
{
#ifdef IS_ESP32
    m_CurrPieceType = static_cast<EPieceType>(random(7));
#else
    m_CurrPieceType = static_cast<EPieceType>(ESP8266TrueRandom.random(7));
#endif
    m_CurrRotation = 0;
    IntCoordinateXY* item;
    for (int i = m_CurrPiece->size() - 1; i >= 0; i--)
    {
        item = m_CurrPiece->remove(i);   //remove and destroy all objects inside list
        delete item;
    }
    
    switch (m_CurrPieceType)
    {
        case EPieceType::Triangle:
            item = new IntCoordinateXY {1, 0};
            m_CurrPiece->add(item);
            item = new IntCoordinateXY {0, 1};
            m_CurrPiece->add(item);
            item = new IntCoordinateXY {1, 1};
            m_CurrPiece->add(item);
            item = new IntCoordinateXY {2, 1};
            m_CurrPiece->add(item);

            m_CurrPieceBoxLeft = 3;
            m_CurrPieceBoxWidth = 3;
            m_CurrPieceBoxHeight = 2;
            break;

        case EPieceType::BigLine:
            item = new IntCoordinateXY {0, 0};
            m_CurrPiece->add(item);
            item = new IntCoordinateXY {1, 0};
            m_CurrPiece->add(item);
            item = new IntCoordinateXY {2, 0};
            m_CurrPiece->add(item);
            item = new IntCoordinateXY {3, 0};
            m_CurrPiece->add(item);

            m_CurrPieceBoxLeft = 2;
            m_CurrPieceBoxWidth = 4;
            m_CurrPieceBoxHeight = 1;
            break;

        case EPieceType::Square:
            item = new IntCoordinateXY {0, 0};
            m_CurrPiece->add(item);
            item = new IntCoordinateXY {1, 0};
            m_CurrPiece->add(item);
            item = new IntCoordinateXY {0, 1};
            m_CurrPiece->add(item);
            item = new IntCoordinateXY {1, 1};
            m_CurrPiece->add(item);

            m_CurrPieceBoxLeft = 3;
            m_CurrPieceBoxWidth = 2;
            m_CurrPieceBoxHeight = 2;
            break;

        case EPieceType::LInverted:
            item = new IntCoordinateXY {1, 0};
            m_CurrPiece->add(item);
            item = new IntCoordinateXY {1, 1};
            m_CurrPiece->add(item);
            item = new IntCoordinateXY {1, 2};
            m_CurrPiece->add(item);
            item = new IntCoordinateXY {0, 2};
            m_CurrPiece->add(item);

            m_CurrPieceBoxLeft = 3;
            m_CurrPieceBoxWidth = 2;
            m_CurrPieceBoxHeight = 3;
            break;

        case EPieceType::LShape:
            item = new IntCoordinateXY {0, 0};
            m_CurrPiece->add(item);
            item = new IntCoordinateXY {0, 1};
            m_CurrPiece->add(item);
            item = new IntCoordinateXY {0, 2};
            m_CurrPiece->add(item);
            item = new IntCoordinateXY {1, 2};
            m_CurrPiece->add(item);

            m_CurrPieceBoxLeft = 3;
            m_CurrPieceBoxWidth = 2;
            m_CurrPieceBoxHeight = 3;
            break;

        case EPieceType::ZShape:
        case EPieceType::ZInverted:
            item = new IntCoordinateXY {1, 0};
            m_CurrPiece->add(item);
            item = new IntCoordinateXY {1, 1};
            m_CurrPiece->add(item);
            if (m_CurrPieceType == EPieceType::ZShape)
            {
                item = new IntCoordinateXY {0, 0};
                m_CurrPiece->add(item);
                item = new IntCoordinateXY {2, 1};
                m_CurrPiece->add(item);
            }
            else            //Z-inverted
            {
                item = new IntCoordinateXY {0, 1};
                m_CurrPiece->add(item);
                item = new IntCoordinateXY {2, 0};
                m_CurrPiece->add(item);
            }

            m_CurrPieceBoxLeft = 3;
            m_CurrPieceBoxWidth = 3;
            m_CurrPieceBoxHeight = 2;
            break;

        default:
#ifdef DEBUG
            Serial.println("WARNING! Unknown piece type selected!");
#endif
            break;
    }
}

void CLedGameTetrisPiece::RotatePiece(bool clockwise)
{
    if (clockwise)
        m_CurrRotation++;
    else
        m_CurrRotation--;

    IntCoordinateXY* item;
    switch (m_CurrPieceType)
    {
        case EPieceType::Triangle:
            if (m_CurrRotation > 3)         //clockwise
                m_CurrRotation = 0;
            else if (m_CurrRotation < 0)    //anti-clockwise
                m_CurrRotation = 3;

            item = m_CurrPiece->get(0);
            if (m_CurrRotation == 0)
                { item->x = 1; item->y = 0; }
            else if (m_CurrRotation == 1 || m_CurrRotation == 2)
                { item->x = 1; item->y = 1; }
            else if (m_CurrRotation == 3)
                { item->x = 0; item->y = 1; }

            item = m_CurrPiece->get(1);
            if (m_CurrRotation == 0)
                { item->x = 0; item->y = 1; }
            else if (m_CurrRotation == 1)
                { item->x = 0; item->y = 0; }
            else if (m_CurrRotation == 2)
                { item->x = 2; item->y = 0; }
            else if (m_CurrRotation == 3)
                { item->x = 1; item->y = 0; }

            item = m_CurrPiece->get(2);
            if (m_CurrRotation == 0)
                { item->x = 1; item->y = 1; }
            else if (m_CurrRotation == 1)
                { item->x = 0; item->y = 1; }
            else if (m_CurrRotation == 2)
                { item->x = 1; item->y = 0; }
            else if (m_CurrRotation == 3)
                { item->x = 1; item->y = 1; }

            item = m_CurrPiece->get(3);
            if (m_CurrRotation == 0)
                { item->x = 2; item->y = 1; }
            else if (m_CurrRotation == 1)
                { item->x = 0; item->y = 2; }
            else if (m_CurrRotation == 2)
                { item->x = 0; item->y = 0; }
            else if (m_CurrRotation == 3)
                { item->x = 1; item->y = 2; }

            m_CurrPieceBoxWidth = (m_CurrRotation == 0 || m_CurrRotation == 2) ? 3 : 2;
            m_CurrPieceBoxHeight = (m_CurrRotation == 0 || m_CurrRotation == 2) ? 2 : 3;
            break;

        case EPieceType::BigLine:
            if (m_CurrRotation > 1)
                m_CurrRotation = 0;
            else if (m_CurrRotation < 0)
                m_CurrRotation = 1;

            item = m_CurrPiece->get(0);
            item->x = (m_CurrRotation == 0) ? 0 : 1; item->y = 0;
            item = m_CurrPiece->get(1);
            item->x = (m_CurrRotation == 0) ? 1 : 1; item->y = (m_CurrRotation == 0) ? 0 : 1;
            item = m_CurrPiece->get(2);
            item->x = (m_CurrRotation == 0) ? 2 : 1; item->y = (m_CurrRotation == 0) ? 0 : 2;
            item = m_CurrPiece->get(3);
            item->x = (m_CurrRotation == 0) ? 3 : 1; item->y = (m_CurrRotation == 0) ? 0 : 3;

            m_CurrPieceBoxWidth = (m_CurrRotation == 0) ? 4 : 1;
            m_CurrPieceBoxHeight = (m_CurrRotation == 0) ? 1 : 4;
            if (m_CurrRotation == 1)
                m_CurrPieceBoxLeft--;   //x is 1 instead of 0, so apply offset to m_CurrPieceBoxLeft
            break;

        case EPieceType::Square:
            //no rotation -> no changes
            m_CurrRotation = 0;
            break;

        case EPieceType::LInverted:
            if (m_CurrRotation > 3)
                m_CurrRotation = 0;
            else if (m_CurrRotation < 0)
                m_CurrRotation = 3;

            item = m_CurrPiece->get(0);
            if (m_CurrRotation == 0)
                { item->x = 1; item->y = 0; }
            else if (m_CurrRotation == 1)
                { item->x = 2; item->y = 1; }
            else if (m_CurrRotation == 2)
                { item->x = 0; item->y = 2; }
            else if (m_CurrRotation == 3)
                { item->x = 0; item->y = 0; }

            item = m_CurrPiece->get(1);
            if (m_CurrRotation == 0 || m_CurrRotation == 1)
                { item->x = 1; item->y = 1; }
            else if (m_CurrRotation == 2)
                { item->x = 0; item->y = 1; }
            else if (m_CurrRotation == 3)
                { item->x = 1; item->y = 0; }

            item = m_CurrPiece->get(2);
            if (m_CurrRotation == 0)
                { item->x = 1; item->y = 2; }
            else if (m_CurrRotation == 1)
                { item->x = 0; item->y = 1; }
            else if (m_CurrRotation == 2)
                { item->x = 0; item->y = 0; }
            else if (m_CurrRotation == 3)
                { item->x = 2; item->y = 0; }

            item = m_CurrPiece->get(3);
            if (m_CurrRotation == 0)
                { item->x = 0; item->y = 2; }
            else if (m_CurrRotation == 1)
                { item->x = 0; item->y = 0; }
            else if (m_CurrRotation == 2)
                { item->x = 1; item->y = 0; }
            else if (m_CurrRotation == 3)
                { item->x = 2; item->y = 1; }

            m_CurrPieceBoxWidth = (m_CurrRotation == 0 || m_CurrRotation == 2) ? 2 : 3;
            m_CurrPieceBoxHeight = (m_CurrRotation == 0 || m_CurrRotation == 2) ? 3 : 2;
            break;

        case EPieceType::LShape:
            if (m_CurrRotation > 3)
                m_CurrRotation = 0;
            else if (m_CurrRotation < 0)
                m_CurrRotation = 3;

            item = m_CurrPiece->get(0);
            if (m_CurrRotation == 0)
                { item->x = 0; item->y = 0; }
            else if (m_CurrRotation == 1)
                { item->x = 2; item->y = 0; }
            else if (m_CurrRotation == 2)
                { item->x = 1; item->y = 2; }
            else if (m_CurrRotation == 3)
                { item->x = 0; item->y = 1; }

            item = m_CurrPiece->get(1);
            if (m_CurrRotation == 0)
                { item->x = 0; item->y = 1; }
            else if (m_CurrRotation == 1)
                { item->x = 1; item->y = 0; }
            else if (m_CurrRotation == 2 || m_CurrRotation == 3)
                { item->x = 1; item->y = 1; }

            item = m_CurrPiece->get(2);
            if (m_CurrRotation == 0)
                { item->x = 0; item->y = 2; }
            else if (m_CurrRotation == 1)
                { item->x = 0; item->y = 0; }
            else if (m_CurrRotation == 2)
                { item->x = 1; item->y = 0; }
            else if (m_CurrRotation == 3)
                { item->x = 2; item->y = 1; }

            item = m_CurrPiece->get(3);
            if (m_CurrRotation == 0)
                { item->x = 1; item->y = 2; }
            else if (m_CurrRotation == 1)
                { item->x = 0; item->y = 1; }
            else if (m_CurrRotation == 2)
                { item->x = 0; item->y = 0; }
            else if (m_CurrRotation == 3)
                { item->x = 2; item->y = 0; }

            m_CurrPieceBoxWidth = (m_CurrRotation == 0 || m_CurrRotation == 2) ? 2 : 3;
            m_CurrPieceBoxHeight = (m_CurrRotation == 0 || m_CurrRotation == 2) ? 3 : 2;
            break;

        case EPieceType::ZShape:
            if (m_CurrRotation > 1)
                m_CurrRotation = 0;
            else if (m_CurrRotation < 0)
                m_CurrRotation = 1;

            item = m_CurrPiece->get(0);
            item->x = 1; item->y = (m_CurrRotation == 0) ? 0 : 1;
            item = m_CurrPiece->get(1);
            item->x = (m_CurrRotation == 0) ? 1 : 0; item->y = 1;
            item = m_CurrPiece->get(2);
            item->x = (m_CurrRotation == 0) ? 0 : 1; item->y = 0;
            item = m_CurrPiece->get(3);
            item->x = (m_CurrRotation == 0) ? 2 : 0; item->y = (m_CurrRotation == 0) ? 1 : 2;

            m_CurrPieceBoxWidth = (m_CurrRotation == 0) ? 3 : 2;
            m_CurrPieceBoxHeight = (m_CurrRotation == 0) ? 2 : 3;
            break;

        case EPieceType::ZInverted:
            if (m_CurrRotation > 1)
                m_CurrRotation = 0;
            else if (m_CurrRotation < 0)
                m_CurrRotation = 1;

            item = m_CurrPiece->get(0);
            item->x = 1; item->y = (m_CurrRotation == 0) ? 0 : 1;
            item = m_CurrPiece->get(1);
            item->x = (m_CurrRotation == 0) ? 1 : 0; item->y = 1;
            item = m_CurrPiece->get(2);
            item->x = 0; item->y = (m_CurrRotation == 0) ? 1 : 0;
            item = m_CurrPiece->get(3);
            item->x = (m_CurrRotation == 0) ? 2 : 1; item->y = (m_CurrRotation == 0) ? 0 : 2;

            m_CurrPieceBoxWidth = (m_CurrRotation == 0) ? 3 : 2;
            m_CurrPieceBoxHeight = (m_CurrRotation == 0) ? 2 : 3;
            break;
    }

    //correct piece out of bounds
    if (m_CurrPieceBoxLeft + m_CurrPieceBoxWidth > 8)
        m_CurrPieceBoxLeft = 8 - m_CurrPieceBoxWidth;
}

#endif