#include <ESP8266WiFi.h>

class CWebserver
{
public:
    // Constructors
    CWebserver()
    {
        // initialize local HTTP Server
        m_server = new WiFiServer(80);
        m_server->begin();
    };

    // Public methods
    void WaitForClient();
    void ReadData();

    // Data accessors

private:
    // Fields
    WiFiServer* m_server;
    WiFiClient* m_client;
    String header;          // stores HTTP request
//EDirection m_DirectionX, m_lastDirectionY;

    // Private methods

};