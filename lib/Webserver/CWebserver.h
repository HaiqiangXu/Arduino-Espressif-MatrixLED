#if IS_ESP32
#include <WiFi.h>
#include <WebServer.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#endif

class CWebserver
{
public:
    // Constructors
    CWebserver()
    {
        // initialize HTTP Server
#if IS_ESP32        
        m_server = new WebServer(80);
        m_server->on("/", [this]() { HandleRoot(); });
        m_server->on("/control", [this]() { ReadControl(); });
        m_server->onNotFound([this]() { HandleNotFound(); });
        m_server->begin();
#else
        m_Direction = 0;
        m_server = new ESP8266WebServer(80);
        m_server->on("/", [this]() { HandleRoot(); });
        m_server->on("/control", [this]() { ReadControl(); });
        m_server->onNotFound([this]() { HandleNotFound(); });
        m_server->begin();
#endif
#ifdef DEBUG
        Serial.println("HTTP Server started.");
#endif
    };

    // Public methods
    void HandleClient()
    {   
        //handleClient for any new connection/browser connecting to board's webserver
        m_server->handleClient();
    };
    void ResetDirection()
    {
        m_Direction = 0;
    }
    void ReadControl();
    void HandleRoot();
    void HandleNotFound();

    // Data accessors
    int GetDirection()
    {
        return m_Direction;
    };

private:
    // Fields
#if IS_ESP32
    WebServer* m_server;
#else
    ESP8266WebServer* m_server;
#endif
    int m_Direction;

    // Private methods
};