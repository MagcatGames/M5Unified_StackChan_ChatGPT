#ifndef SCCP_CAPTIVEPORTAL
#define SCCP_CAPTIVEPORTAL

#include <WiFi.h>
#include <ESP32WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>


class StackChan_CaptivePortal
{
    public:
        StackChan_CaptivePortal();

        String GetSSID();
        String GetPassword();
        void SetSSID(String ssid);
        void SetPassword(String pass);
        bool HasKey();
        void Clear();

        void Begin();
        void Update();

        String makePage(String title, String contents);
        String urlDecode(String input);
    private:
        ESP32WebServer server;
        DNSServer dnsServer;
        Preferences pref;
        String ssidList;
};

#endif