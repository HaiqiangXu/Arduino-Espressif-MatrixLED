#ifdef GAME
#include <CLedGameController.h>

// Replace with your network credentials and local config
const char* ssid     = "---";
const char* password = "---";
const long Connect_Timeout = 10000;      //10 sec
const IPAddress Local_IP(192, 168, 7, 144);
const IPAddress Gateway(192, 168, 7, 1);
const IPAddress Subnet(255, 255, 255, 0);

#elif MARQUEE
#include <CLedMarquee.h>
#endif

// Type uint8_t is equivalent to unsigned byte/char and it's native/fastest data type for Atmel 8-bit controllers (ATMega 328P). int is signed 16-bit and long signed 32-bit for these MCUs
// Type int is 32-bit for ESP8266/ESP32 as they are 32-bit MCU architecture
// D1 R1/Uno:        D1 Mini:
// CLK_PIN = D13;    //D5       //SCK  [Master Clock]
// DATA_PIN = D11;   //D7       //MOSI [Master Out Slave In]   
// CS_PIN = D10;     //D8       //SS   [Slave Select]
#if IS_ESP32
const int CS_PIN = SS;
#elif IS_D1MINI
const int CLK_PIN = D5;
const int DATA_PIN = D7;
const int CS_PIN = D8;
#else
const int CS_PIN = D10;
#endif
const int IN_AXIS_X = 17;   //A3
const int IN_AXIS_Y = 18;   //A4
const int IN_BUTTON = 2;    //WakeUp Input -> press Joystick button if in sleep to wake the game up
const int NUM_DEVICES = 2;  //number of Matrix leds attached
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

    // Configure and init WiFi connection to router with static IP address
    bool isTimeout = false;
#ifdef IS_ESP32
    pinMode(GPIO_NUM_33, INPUT_PULLUP);
#endif
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    if (WiFi.config(Local_IP, Gateway, Subnet))
    {
        unsigned long lLastTime = millis();

#ifdef DEBUG
        Serial.println("Connecting to router...");
#endif
        WiFi.mode(WIFI_STA);            //WIFI_STA = station ('client') mode , WIFI_AP = access point ('hotspot/server') mode
        WiFi.begin(ssid, password);     //connect to remote router
        while (WiFi.status() != WL_CONNECTED && !isTimeout)
        {
            delay(300);
            isTimeout = millis() - lLastTime >= Connect_Timeout;
            lLastTime = millis();
        }
#ifdef DEBUG
        Serial.println("Connection OK.");
        Serial.print("Board IP address: ");
        Serial.println(WiFi.localIP());
#endif
    }
    digitalWrite(LED_BUILTIN, LOW);

    m_ledsController = new CLedGameController(CS_PIN, NUM_DEVICES, IN_AXIS_X, IN_AXIS_Y, IN_BUTTON,
    #ifdef GAME_TETRIS    
        EGame::Tetris, !isTimeout);
    #elif GAME_SNAKE
        EGame::Snake, !isTimeout);
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
    m_ledsController->SetMarqueeStyle(EMarqueeStyle::Test);
    m_ledsController->ShowMarquee();
    delay(300);
    m_ledsController->SetMarqueeStyle(EMarqueeStyle::Text);
    m_ledsController->ShowMarquee();
    delay(300);
    m_ledsController->SetMarqueeStyle(EMarqueeStyle::Pacman);
    m_ledsController->ShowMarquee();
    delay(300);
#endif
}