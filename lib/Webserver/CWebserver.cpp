#include "CWebserver.h"

//Public methods
void CWebserver::ReadControl()
{
    String control = m_server->arg("key");
    if (m_server->method() == HTTP_GET)
    {
        //EDirection { None, Left, Right, Up, Down };
        if (control == "left")
            m_Direction = 1;
        else if (control == "right")
            m_Direction = 2;
        else if (control == "up")
            m_Direction = 3;
        else if (control == "down")
            m_Direction = 4;
        else
            m_Direction = 0;
    }
    else if (m_server->method() == HTTP_POST)
    {
        //TODO: implement POST processing
    }
#ifdef DEBUG
    Serial.println("Read key: " + control);
#endif
}

void CWebserver::HandleRoot()
{
    String response = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"icon\" href=\"data:,\">";
    response += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}.button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;.button2 {background-color: #77878A;}</style></head>";
    response += "<body><h1>ESP8266 Web Server</h1>";
    response += "<p><a href=\"/control?key=up\"><button class=\"button\">Up</button></a></p>";
    response += "<p><a href=\"/control?key=down\"><button class=\"button\">Down</button></a></p>";
    response += "<p><a href=\"/control?key=left\"><button class=\"button\">Left</button></a></p>";
    response += "<p><a href=\"/control?key=right\"><button class=\"button\">Right</button></a></p>";
    response += "</body></html>";

    m_server->send(200, "text/html", response);
#ifdef DEBUG
    Serial.println("Index page called: " + response);
#endif
}

void CWebserver::HandleNotFound()
{
    m_server->send(404, "text/html", "<h1>404: Page Not found</h1>");
#ifdef DEBUG
    Serial.println("Error 404 page called");
#endif    
}