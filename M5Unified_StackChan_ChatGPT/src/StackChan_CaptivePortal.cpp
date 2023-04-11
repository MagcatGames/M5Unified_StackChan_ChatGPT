#include <StackChan_CaptivePortal.h>
/*
#include <ESP32WebServer.h>
#include <DNSServer.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Preferences.h>
*/

StackChan_CaptivePortal::StackChan_CaptivePortal()
{
    ESP32WebServer server(80);
    DNSServer dnsServer;
    Preferences pref;
    pref.begin("SCCP");
}


bool StackChan_CaptivePortal::HasKey(){return pref.isKey("wifi_ssid");}

String StackChan_CaptivePortal::GetSSID(){return pref.getString("wifi_ssid");}
String StackChan_CaptivePortal::GetPassword(){return pref.getString("wifi_pass");}

void StackChan_CaptivePortal::SetSSID(String ssid){pref.putString("wifi_ssid",ssid);}
void StackChan_CaptivePortal::SetPassword(String pass){pref.putString("wifi_pass",pass);}

void StackChan_CaptivePortal::Clear(){pref.clear();}

void StackChan_CaptivePortal::Begin()
{
    IPAddress apIP(192,168,4,1);

    WiFi.mode(WIFI_MODE_STA);
    WiFi.disconnect();
    delay(100);

    int n = WiFi.scanNetworks();
    delay(100);

    String ssidList;
    ssidList = "<datalist id='ssidList'>";
    for (int i = 0; i < n && i < 10; ++i) {
        ssidList += "<option value='";
        ssidList += WiFi.SSID(i);
        ssidList += "'>";
        ssidList += WiFi.SSID(i);
        ssidList += "</option>";
    }
    ssidList += "</datalist>";
    delay(100);

    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("StackChanAP");
    WiFi.mode(WIFI_MODE_AP);
    dnsServer.start(53,"*", WiFi.softAPIP());
    delay(100);

    server.onNotFound([&](){
        String s =
            "<h1>Wi-Fi Settings</h1><p>Please enter your password by "
            "selecting the SSID.</p>";
            s += "<form method=\"get\" action=\"setap\"><label>SSID: "
            "</label><select name=\"ssid\">";
            s += ssidList;
            s += "</select><br>Password: <input name=\"pass\" length=64 "
            "type=\"password\"><input type=\"submit\"></form>";
        server.send(200, "text/html", makePage("Wi-Fi Settings", s));
    });
    
    server.on("/setap",[&](){
        String ssid = urlDecode(server.arg("ssid"));
        String pass = urlDecode(server.arg("pass"));

        pref.putString("WIFI_SSID", ssid);
        pref.putString("WIFI_PASSWD", pass);

        String s = "<h1>Setup complete.</h1><p>device will be connected to \"";
            s += ssid;
            s += "\" after the restart.";
        server.send(200, "text/html", makePage("Wi-Fi Settings", s));
        delay(2000);
        ESP.restart();
    });

    /*
    server.on("/settings",[&](){
        String ssid = server.arg("wifi_ssid");
        String pass = server.arg("wifi_pass");
        SetSSID(ssid);
        SetPassword(pass);
        ESP.restart();
    });

    server.onNotFound([&](){
        String ssid = GetSSID();
        server.send(200, "text/html",
        "<!DOCTYPE html>"
        "<html lang='ja'>"
            "<head>"
                "<meta charset='UTF-8'>"
                "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                "<title>StackChan WiFi Setup</title>"
            "</head>"
            "<body>"
                "<h1>StackChan WiFi Setup</h1>"
                "<hr style='width:80%'>"
                "<form method='get' action='settings'>"
                    "<div>SSID</div>"
                    "<div><input type='text' name='wifi-ssid' autocomplete='on' list='ssidList' required/><span style='color:red'>*</span></div>"
                    +ssidList+
                    "<div>Password</div>"
                    "<div><input type='password' name='wifi_pass'/></div>"
                    "<div><input type='submit'/></div>"
                "</form>"
            "</body>"
        "</html>"
        );
    });
    */
    server.begin();
}

void StackChan_CaptivePortal::Update()
{
    dnsServer.processNextRequest();
    server.handleClient();
}


String StackChan_CaptivePortal::makePage(String title, String contents) {
  String s = "<!DOCTYPE html><html><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<title>";
  s += title;
  s += "</title></head><body>";
  s += contents;
  s += "</body></html>";
  return s;
}

String StackChan_CaptivePortal::urlDecode(String input){
  String s = input;
  s.replace("%20", " ");
  s.replace("+", " ");
  s.replace("%21", "!");
  s.replace("%22", "\"");
  s.replace("%23", "#");
  s.replace("%24", "$");
  s.replace("%25", "%");
  s.replace("%26", "&");
  s.replace("%27", "\'");
  s.replace("%28", "(");
  s.replace("%29", ")");
  s.replace("%30", "*");
  s.replace("%31", "+");
  s.replace("%2C", ",");
  s.replace("%2E", ".");
  s.replace("%2F", "/");
  s.replace("%2C", ",");
  s.replace("%3A", ":");
  s.replace("%3A", ";");
  s.replace("%3C", "<");
  s.replace("%3D", "=");
  s.replace("%3E", ">");
  s.replace("%3F", "?");
  s.replace("%40", "@");
  s.replace("%5B", "[");
  s.replace("%5C", "\\");
  s.replace("%5D", "]");
  s.replace("%5E", "^");
  s.replace("%5F", "-");
  s.replace("%60", "`");
  return s;
}