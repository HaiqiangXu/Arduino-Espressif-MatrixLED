#ifdef GAME

#include "CLedGame.h"

// Protected derived virtual methods
void CLedGameSnake::RefreshAnimation()
{
    IntCoordinateXY* currItem;

    m_leds->clear();
    m_leds->update(MD_MAX72XX::OFF);
    for (int i = 0; i < m_iCurrentLevel; i++)
    {
        currItem = m_Snake->get(i);
        m_leds->setPoint(currItem->x, currItem->y, true);
#ifdef DEBUG
    Serial.println("x " + String(i) + ": " + String(currItem->x) + " | y" + String(i) + ": " + String(currItem->y));
#endif
    }
    m_leds->setPoint(m_Egg.x, m_Egg.y, true);
    m_leds->update(MD_MAX72XX::ON);

    delay(300);         //this delay can be dynamic by regulating speed according to difficulty level
}

void CLedGameSnake::GameCalculate()
{
#ifdef DEBUG
    if (m_lastDirectionX != EDirection::None)
        Serial.println("Previous direction X: " + String((int)m_lastDirection) + ". Set direction X: " + String((int)m_lastDirectionX));
    else if (m_lastDirectionY != EDirection::None)
        Serial.println("Previous direction Y: " + String((int)m_lastDirection) + ". Set direction Y: " + String((int)m_lastDirectionY));
#endif

    SetNextSnakePos(m_lastDirection);               //set last direction by default
    if (m_lastDirectionX != EDirection::None)
    {
        if (m_lastDirection == m_lastDirectionX ||  //accept any direction (3 possibilities) except the opposite direction
            m_lastDirection == EDirection::Up ||
            m_lastDirection == EDirection::Down)
        {
            SetNextSnakePos(m_lastDirectionX);
            m_lastDirection = m_lastDirectionX;
        }
    
        if (m_webServer != NULL)
            m_webServer->ResetDirection();
    }
    else if (m_lastDirectionY != EDirection::None)
    {
        if (m_lastDirection == m_lastDirectionY ||
            m_lastDirection == EDirection::Left ||
            m_lastDirection == EDirection::Right)
        {
            SetNextSnakePos(m_lastDirectionY);
            m_lastDirection = m_lastDirectionY;
        }

        if (m_webServer != NULL)
            m_webServer->ResetDirection();
    }
}

void CLedGameSnake::ResetGame()
{
    IntCoordinateXY* item;
    for (int i = 0; i < m_Snake->size(); i++)
    {
        item = m_Snake->shift();            //destroy all objects inside list
        delete item;
    }
    m_Snake->clear();
    m_iCurrentLevel = 3;                    //default level
    m_lastDirection = EDirection::Left;     //default initial direction

#ifdef DEBUG
    m_Snake->add(new IntCoordinateXY { 4, 12 });
    m_Snake->add(new IntCoordinateXY { 4, 11 });
    m_Snake->add(new IntCoordinateXY { 4, 10 });
#else
#ifdef IS_ESP32
    item = new IntCoordinateXY { 4, random((COL_SIZE * m_iNumDevices) - 3) };
    long randLong = random(2);
#else
    item = new IntCoordinateXY { 4, ESP8266TrueRandom.random((COL_SIZE * m_iNumDevices) - 3) };
    long randLong = ESP8266TrueRandom.random(2);
#endif
    m_Snake->add(item);
    if (randLong == 0l)
    {
        m_Snake->add(new IntCoordinateXY {(item->x) + 1, item->y});
        m_Snake->add(new IntCoordinateXY {(item->x) + 2, item->y});
    }
    else
    {
        m_Snake->add(new IntCoordinateXY {item->x, (item->y) - 1});
        m_Snake->add(new IntCoordinateXY {item->x, (item->y) - 2});
    }
#endif
}

// Private methods
void CLedGameSnake::SetNewEgg()
{
    //TODO: control to not overlay existing snake dot
#ifndef DEBUG
    bool bIsOk = false;
    IntCoordinateXY* item;
    do
    {
        //keep searching while new egg position is not outside the snake
#ifdef IS_ESP32
        m_Egg = IntCoordinateXY { random(ROW_SIZE), random((COL_SIZE * m_iNumDevices)) };
#else
        m_Egg = IntCoordinateXY { ESP8266TrueRandom.random(ROW_SIZE), ESP8266TrueRandom.random((COL_SIZE * m_iNumDevices)) };
#endif
        for (int i = 0; i < m_iCurrentLevel; i++)
        {
            item = m_Snake->get(i);
            if (item->x == m_Egg.x &&
                item->y == m_Egg.y)
            {
                bIsOk |= false;
            }
            else
            {
                bIsOk |= true;
            }
        }
    } while (!bIsOk);
#endif
}

void CLedGameSnake::SetNextSnakePos(EDirection direction)
{
    IntCoordinateXY *firstItem, *newItem, *lastItem;

    lastItem = m_Snake->pop();       //remove last item
    firstItem = m_Snake->get(0);
    newItem = new IntCoordinateXY { firstItem->x, firstItem->y };
    switch (direction)
    {
        case EDirection::Left:
            newItem->y++;
            break;

        case EDirection::Right:
            newItem->y--;
            break;

        case EDirection::Up:
            newItem->x--;
            break;

        case EDirection::Down:
            newItem->x++;
            break;

        default:
            break;
    }
    m_Snake->unshift(newItem);      //set as first item
    
    //Go up 1 level and add egg as last item. If not, destroy last item
    if (newItem->x == m_Egg.x &&
        newItem->y == m_Egg.y && 
        m_iCurrentLevel < MAX_SNAKE_LENGTH)
    {
        m_Snake->add(lastItem);
        SetNewEgg();
        m_iCurrentLevel++;
    }
    else
    {
        delete lastItem;
    }

    //reset game if snake goes outside borders or crashes with itself
    IntCoordinateXY* item;
    bool bIsOk = true;
    for (int i = 1; i < m_iCurrentLevel; i++)
    {
        item = m_Snake->get(i);
        if (item->x == newItem->x &&
            item->y == newItem->y)
        {
            bIsOk = false;
            break;
        }
    }
    if ( !bIsOk ||
         (newItem->y >= (COL_SIZE * m_iNumDevices)) ||
         (newItem->y < 0) ||
         (newItem->x >= ROW_SIZE) ||
         (newItem->x < 0) )
    {
        ResetGame();
    }
}

#endif