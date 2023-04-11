#include <M5Unified.h>
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


bool StackChan_CaptivePortal::HasKey(){return pref.isKey("SCCP");}

String StackChan_CaptivePortal::GetSSID(){return pref.getString("wifi_ssid");}
String StackChan_CaptivePortal::GetPassword(){return pref.getString("wifi_pass");}

void StackChan_CaptivePortal::SetSSID(String ssid){pref.putString("wifi_ssid",ssid);}
void StackChan_CaptivePortal::SetPassword(String pass){pref.putString("wifi_pass",pass);}

void StackChan_CaptivePortal::Clear(){pref.clear();}

void StackChan_CaptivePortal::Begin()
{
    IPAddress apIP(192,168,4,1);

    WiFi.mode(WIFI_AP);
    WiFi.disconnect();
    delay(100);

    int n = WiFi.scanNetworks();
    M5.Lcd.print("ssid : "+n);
    delay(100);

    WiFi.softAP("StackChanAP");
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    dnsServer.start(53,"*", apIP);
    delay(100);

    /*
    String ssidList;
    ssidList = "<datalist id='ssidList'>";
    for (int i = 0; i < n && i < 10; ++i) {
        ssidList += sprintf("<option value='%s'>%s</option>",WiFi.SSID(i).c_str());
    }
    ssidList += "</datalist>";
    */
    delay(100);

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
                    "<div><input type='text' name='wifi-ssid' value='"+ssid+"' required/><span style='color:red'>*</span></div>"
                    //"<div><input type='text' name='wifi-ssid' value='"+ssid+"' autocomplete='on' list='ssidList' required/><span style='color:red'>*</span></div>"
                    //""+ssidList+""
                    "<div>Password</div>"
                    "<div><input type='password' name='wifi_pass'/></div>"
                    "<div><input type='submit'/></div>"
                "</form>"
            "</body>"
        "</html>"
        );
    });
    server.on("/settings",[&](){
        String ssid = server.arg("wifi_ssid");
        String pass = server.arg("wifi_pass");
        SetSSID(ssid);
        SetPassword(pass);
        ESP.restart();
    });
    server.begin();
}

void StackChan_CaptivePortal::Update()
{
    dnsServer.processNextRequest();
    server.handleClient();
}

