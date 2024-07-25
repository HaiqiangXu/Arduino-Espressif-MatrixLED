#include "CWebserver.h"

//Public methods
void CWebserver::WaitForClient()
{
    WiFiClient client = m_server->available(); 
    m_client = &client;
    if (m_client)
    {                               // If a new client connects,
        String currentLine = "";    // make a String to hold incoming data from the client
        // currentTime = millis();
        // previousTime = currentTime;
        while (m_client->connected() /*&& currentTime - previousTime <= timeoutTime*/)
        {                           // loop while the client's connected
            //currentTime = millis();         
            if (m_client->available())
            {                                     // if there's bytes to read from the client, read a byte, then
                char c = m_client->read();
                header += c;
                if (c == '\n')
                {                                   // if the byte is a newline character
                // if the current line is blank, you got two newline characters in a row.
                // that's the end of the client HTTP request, so send a response:
                if (currentLine.length() == 0)
                {
                    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                    // and a content-type so the client knows what's coming, then a blank line:
                    m_client->println("HTTP/1.1 200 OK");
                    m_client->println("Content-type:text/html");
                    m_client->println("Connection: close");
                    m_client->println();
                    
                    // turns the GPIOs on and off
                    if (header.indexOf("GET /10/on") >= 0)
                    {
//output10State = "on";
                        digitalWrite(D10, HIGH);
                    }
                    else if (header.indexOf("GET /10/off") >= 0)
                    {
//output10State = "off";
                        digitalWrite(D10, LOW);
                    }
                    
                    // Display the HTML web page
                    m_client->println("<!DOCTYPE html><html>");
                    m_client->println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                    m_client->println("<link rel=\"icon\" href=\"data:,\">");
                    // CSS to style the on/off buttons 
                    // Feel free to change the background-color and font-size attributes to fit your preferences
                    m_client->println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                    m_client->println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
                    m_client->println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                    m_client->println(".button2 {background-color: #77878A;}</style></head>");
                    
                    // Web Page Heading
                    m_client->println("<body><h1>ESP8266 Web Server</h1>");
                    
                    // // Display current state, and ON/OFF buttons for GPIO 10  
                    // m_client->println("<p>GPIO 10 - State " + output10State + "</p>");
                    // if (output10State=="off")
                    //     m_client->println("<p><a href=\"/10/on\"><button class=\"button\">ON</button></a></p>");
                    // else
                    //     m_client->println("<p><a href=\"/10/off\"><button class=\"button button2\">OFF</button></a></p>");
                    
                    m_client->println("</body></html>");
                    
                    // The HTTP response ends with another blank line
                    m_client->println();
                    // Break out of the while loop
                    break;
                }
                else
                {                     // if you got a newline, then clear currentLine
                    currentLine = "";
                }
                }
                else if (c != '\r')
                {                       // if you got anything else but a carriage return character,
                    currentLine += c;     // add it to the end of the currentLine
                }
            }
        }
    }
    // Clear the header variable
    header = "";

    // Close the connection and destroy object
    m_client->stop();
    delete m_client;
}

void CWebserver::ReadData()
{
    if (m_client != NULL)
    {
        //TODO: if connected to a Client, read its data

    }
}