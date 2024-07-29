#if IS_ESP32
#include <WiFi.h>
#include <WebServer.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>
#endif

class CWebserver
{
public:
    // Constructors
    CWebserver()
    {
        // initialize local HTTP Server
#ifdef DEBUG
        Serial.println("Initializing HTTP Server");
#endif
#if IS_ESP32        
        m_server = new WebServer(80);
        m_server->on("/control", [this]() { this->ReadData(); });
        m_server->on("/", [this]() { this->HandleIndex(); });
        m_server->onNotFound([this]() { this->HandleNotFound(); });
        m_server->begin();
#else
        m_server = new ESP8266WebServer(80);
        //m_server->on("/control", [this]() { this->ReadControl(); });
        //m_server->on("/", [this]() { this->HandleRoot(); });
        //m_server->onNotFound([this]() { this->HandleNotFound(); });
        //TODO: there is an exception thrown through the lambda expression to be fixed
        m_server->on("/", []()
        {
            String response = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"icon\" href=\"data:,\">";
            response += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}.button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;.button2 {background-color: #77878A;}</style></head>";
            response += "<body><h1>ESP8266 Web Server</h1>";
            response += "<p><a href=\"/control?key=up\"><button class=\"button\">Up</button></a></p>";
            response += "<p><a href=\"/control?key=down\"><button class=\"button\">Down</button></a></p>";
            response += "<p><a href=\"/control?key=left\"><button class=\"button\">Left</button></a></p>";
            response += "<p><a href=\"/control?key=right\"><button class=\"button\">Right</button></a></p>";
            response += "</body></html>";
#ifdef DEBUG
            Serial.println("Index page called: " + response);
#endif
        });
        m_server->begin();

        // m_server = new AsyncWebServer(80);
        // m_server->on("/control", [this](AsyncWebServerRequest *request) { this->ReadControl(); request->send(200, "text/plain", "OK"); });
        // m_server->on("/", [this](AsyncWebServerRequest *request) { this->HandleRoot(); request->send(200, "text/plain", "OK"); });
        // m_server->onNotFound([this](AsyncWebServerRequest *request) { this->HandleNotFound(); request->send(404, "text/plain", "OK"); });
#endif
#ifdef DEBUG
        Serial.println("HTTP Server started.");
#endif
    };

    // Public methods
    void HandleClient()
    {
#ifdef DEBUG
        Serial.println("HTTP handleClient() called.");
#endif        
        m_server->handleClient();
    };
    void ReadControl();
    void HandleRoot();
    void HandleNotFound();

    // Data accessors
    int ReadDirection()
    {
        return m_Direction;
    };

private:
    // Fields
#if IS_ESP32
    WebServer* m_server;
#else
    ESP8266WebServer* m_server;
    //AsyncWebServer* m_server;
#endif
    int m_Direction;

    // Private methods
};