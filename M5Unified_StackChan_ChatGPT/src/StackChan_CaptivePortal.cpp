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
        String s;
        s += "<!DOCTYPE html>";
        s += "<html lang='ja'>";
        s += "<head>";
        s += "<meta charset='UTF-8'>";
        s += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
        s += "<title>StackChan WiFi Setup</title>";
        s += "</head>";
        s += "<body>";
        s += "<h1>StackChan WiFi Setup</h1>";
        s += "<hr style='width:80%'>";
        s += "<form method='get' action='settings'>";
        s += "<div>SSID</div>";
        s += "<div><input type='text' name='wifi-ssid' autocomplete='on' list='ssidList' required/><span style='color:red'>*</span></div>";
        s +=  ssidList;
        s += "<div>Password</div>";
        s += "<div><input type='password' name='wifi_pass'/></div>";
        s += "<div><input type='submit'/></div>";
        s += "</form>";
        s += "</body>";
        s += "</html>";
        server.send(200, "text/html", s);
    });
    
    server.on("/settings",[&](){
        String ssid = urlDecode(server.arg("ssid"));
        String pass = urlDecode(server.arg("pass"));

        SetSSID(ssid);
        SetPassword(pass);

        String s;
        s += "<!DOCTYPE html>";
        s += "<html lang='ja'>";
        s += "<head>";
        s += "<meta charset='UTF-8'>";
        s += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
        s += "<title>StackChan WiFi Setup</title>";
        s += "</head>";
        s += "<body>";
        s += "<h1>StackChan WiFi Setup</h1>";
        s += "<hr style='width:80%'>";
        s += "device will be connected to \"";
        s += ssid;
        s += "\" after the restart.";
        s += "</body>";
        s += "</html>";
        server.send(200, "text/html", s);
        delay(2000);
        ESP.restart();
    });

    server.begin();
}

void StackChan_CaptivePortal::Update()
{
    dnsServer.processNextRequest();
    server.handleClient();
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