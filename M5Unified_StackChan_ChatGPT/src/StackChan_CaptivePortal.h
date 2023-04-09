#ifndef SCCP_CAPTIVEPORTAL
#define SCCP_CAPTIVEPORTAL

#include <ESP32WebServer.h>
#include <DNSServer.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <StackChan_Config.h>

class StackChan_CaptivePortal
{
    public:
        StackChan_CaptivePortal();
        void Begin();
        void Update();
    private:
        bool handle_Home();
        bool handle_Get();
        ESP32WebServer server;
        DNSServer dnsServer;
        IPAddress ip;
        StackChan_Config cfg;
};

#endif