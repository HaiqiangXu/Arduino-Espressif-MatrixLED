#include "CWebserver.h"

//Public methods
void CWebserver::ReadControl()
{
    String control = m_server->arg("key");
    if (m_server->method() == HTTP_GET)
    {
        //equivalent integers to EDirection { None, Left, Right, Up, Down };
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
#ifdef DEBUG
    Serial.print("Key read through GET method.");
    Serial.println("Read key: " + control + ". New direction: " + String(m_Direction));
#endif
        this->HandleRoot(); //just show the main page again as we need to respond something for the GET method, that's why we should use POST for this case
    }
    else if (m_server->method() == HTTP_POST)
    {
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
#ifdef DEBUG
    Serial.print("Key read through POST method.");
    Serial.println("Read key: " + control + ". New direction: " + String(m_Direction));
#endif
    }
}

void CWebserver::HandleRoot()
{
    String response = "<!DOCTYPE html><html><head>";
    response += "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\"><link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css\">";
    //using Fetch API (use Content-Type: application/x-www-form-urlencoded as application/json is not allowed by MCU):
//response += "<script>function proceed(direction) { const json = { key: direction }; fetch(\"/control\", { method: \"POST\", headers: { \"Content-Type\": \"application/json\" }, body: JSON.stringify(json) });document.getElementById(\"lastControl\").innerHTML = direction;}";
    response += "<script>function proceed(direction) { fetch(\"/control\", { method: \"POST\", headers: { \"Content-Type\": \"application/x-www-form-urlencoded\" }, body: 'key=' + direction });document.getElementById(\"lastControl\").innerHTML = direction;}";
    //using XMLHttpRequest (XHR):
//response += "function proceed2(direction) { const xhr = new XMLHttpRequest(); const json = { key: direction }; xhr.open(\"POST\", \"/control\"); xhr.setRequestHeader(\"Content-Type\", \"application/json\"); xhr.send(JSON.stringify(json));document.getElementById(\"lastControl\").innerHTML = direction;}";
    response += "function proceed2(direction) { const xhr = new XMLHttpRequest(); xhr.open(\"POST\", \"/control\"); xhr.setRequestHeader(\"Content-Type\", \"application/x-www-form-urlencoded\"); xhr.send('key=' + direction);document.getElementById(\"lastControl\").innerHTML = direction;}";
    response += "</script></head>";
    response += "<body><p class=\"h1 text-center\">LED Matrix control (running on ESP8266 Web Server)</p>";
    //Using GET Method
    // response += "<p><a href=\"/control?key=up\"><button class=\"button\">Up</button></a></p>";
    // response += "<p><a href=\"/control?key=down\"><button class=\"button\">Down</button></a></p>";
    // response += "<p><a href=\"/control?key=left\"><button class=\"button\">Left</button></a></p>";
    // response += "<p><a href=\"/control?key=right\"><button class=\"button\">Right</button></a></p>";
    //Using POST Method
    response += "<div class=\"container text-center\"><div class=\"row row-cols-3\"><div class=\"col-5\"></div><div class=\"col-2\"><button type=\"button\" class=\"btn btn-primary\" onclick=\"proceed('up');\"><i class=\"fa fa-arrow-up fa-2x\"></i></button></div><div class=\"col-5\"></div></div>";
    response += "<div class=\"row row-cols-3\"><div class=\"col-4\"></div><div class=\"col-1\"><button type=\"button\" class=\"btn btn-primary\" onclick=\"proceed2('left');\"><i class=\"fa fa-arrow-left fa-2x\"></i></button></div><div class=\"col-2\"></div><div class=\"col-1\"><button type=\"button\" class=\"btn btn-primary\" onclick=\"proceed2('right');\"><i class=\"fa fa-arrow-right fa-2x\"></i></button></div><div class=\"col-4\"></div></div>";
    response += "<div class=\"row row-cols-3\"><div class=\"col-5\"></div><div class=\"col-2\"><button type=\"button\" class=\"btn btn-primary\" onclick=\"proceed('down');\"><i class=\"fa fa-arrow-down fa-2x\"></i></button></div><div class=\"col-5\"></div></div><div class=\"row row-cols-3\"><div class=\"col-3\"></div><div class=\"col-6\"><div class=\"text-right\">Control pressed: <div id=\"lastControl\"></div></div></div><div class=\"col-3\"></div></div></div>";
    response += "</body></html>";

    m_server->send(200, "text/html", response);
#ifdef DEBUG
    Serial.println("Main \"/\" page called");
#endif
}

void CWebserver::HandleNotFound()
{
    m_server->send(404, "text/html", "<h2>404: Page Not found</h2>");
#ifdef DEBUG
    Serial.println("Error 404 page called");
#endif
}