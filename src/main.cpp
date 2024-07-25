#ifdef GAME
#include <CLedGameController.h>

// Replace with your network credentials and local config
const char* ssid     = "bf16162423";
const char* password = "fr4r8uhma6";
const long Connect_Timeout = 20000;      //20 sec
IPAddress local_IP(192, 168, 7, 144);
IPAddress gateway(192, 168, 7, 1);
IPAddress subnet(255, 255, 255, 0);

#elif MARQUEE
#include <CLedMarquee.h>
#elif TEST_HW
#include <main.h>
#endif

// Type uint8_t is equivalent to unsigned byte/char and it's native/fastest data type for Atmel 8-bit controllers (ATMega 328P). int is signed 16-bit and long signed 32-bit for these MCUs
// const uint8_t CLK_PIN = D13;    //SCK  [Master Clock]:        13
// const uint8_t DATA_PIN = D11;   //MOSI [Master Out Slave In]: 11
#if IS_ESP32
const uint8_t CS_PIN = SS;
#else
const uint8_t CS_PIN = D10;
#endif
const uint8_t IN_AXIS_X = 17;   //A3
const uint8_t IN_AXIS_Y = 18;   //A4
const uint8_t IN_BUTTON = 2;    //WakeUp Input -> press Joystick button if in sleep to wake the game up
const uint8_t NUM_DEVICES = 2;  //number of Matrix leds attached
const char TEXT[] = "Hello world";

#ifdef GAME
CLedGameController* m_ledsController;
#elif MARQUEE
CLedMarquee* m_ledsController;
#endif

void setup()
{
#ifdef GAME
    #ifdef DEBUG
        Serial.begin(9600);
        Serial.println("Debugging Led Matrix game");
    #endif

    #ifdef GAME_TETRIS    
        m_ledsController = new CLedGameController(CS_PIN, NUM_DEVICES, IN_AXIS_X, IN_AXIS_Y, IN_BUTTON, EGame::Tetris);
    #elif GAME_SNAKE
        m_ledsController = new CLedGameController(CS_PIN, NUM_DEVICES, IN_AXIS_X, IN_AXIS_Y, IN_BUTTON, EGame::Snake);
        if (WiFi.config(local_IP, gateway, subnet)) // Configures static IP address
        {
            bool isTimeout = false;
            unsigned long lLastTime = millis();

            WiFi.begin(ssid, password);             //connect to remote router
            while (WiFi.status() != WL_CONNECTED && !isTimeout)
            {
                delay(500);
                isTimeout = millis() - lLastTime >= Connect_Timeout;
                lLastTime = millis();
            }

            if (!isTimeout)
            {
                CWebserver* webServer = new CWebserver();
                m_ledsController->SetWebServer(webServer);
            }
        }
    #endif
#elif MARQUEE
    #ifdef DEBUG
        Serial.begin(9600);
        Serial.println("Debugging LM Marquee");
    #endif

    m_ledsController = new CLedMarquee(CS_PIN, NUM_DEVICES, EMarqueeStyle::Test);
    m_ledsController->SetText(TEXT);

#endif
}

void loop()
{
#ifdef GAME
    m_ledsController->StartGame();
#elif MARQUEE
    while (true)
    {
        m_ledsController->SetMarqueeStyle(EMarqueeStyle::Test);
        m_ledsController->ShowMarquee();
        delay(300);
        m_ledsController->SetMarqueeStyle(EMarqueeStyle::Text);
        m_ledsController->ShowMarquee();
        delay(300);
        m_ledsController->SetMarqueeStyle(EMarqueeStyle::Pacman);
        m_ledsController->ShowMarquee();
        delay(300);
    }
#endif
}