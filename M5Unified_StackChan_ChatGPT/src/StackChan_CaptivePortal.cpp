#include <StackChan_CaptivePortal.h>
#include <StackChan_Config.h>

void StackChan_CaptivePortal::handle_Home()
{
    String ssid = cfg.GetSSID();
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
                "<form action='get' method='get'>"
                    "<div>SSID</div>"
                    "<div><input type='text' name='wifi_ssid' value='"+ssid+"' required><span style='color:red'>*</span></div>"
                    "<div>Password</div>"
                    "<div><input type='password' name='wifi_pass'></div>"
                    "<div><input type='submit'></div>"
                "</form>"
            "</body>"
        "</html>"
        );
}

void StackChan_CaptivePortal::handle_Get()
{
    String ssid = server.arg("wifi_ssid");
    String pass = server.arg("wifi_pass");
    cfg.SetSSID(ssid);
    cfg.SetPassword(pass);
}

StackChan_CaptivePortal::StackChan_CaptivePortal()
{
    ESP32WebServer server(80);
    DNSServer dnsServer;
    IPAddress ip = IPAddress(8,8,5,5);
    StackChan_Config cfg;
}

void StackChan_CaptivePortal::Begin()
{
    cfg.Open();
    WiFi.mode(WIFI_AP);
    WiFi.softAP("StackChanAP");
    WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));
    dnsServer.start(53, "*", ip);
    server.onNotFound(std::bind(&StackChan_CaptivePortal::handle_Home, this)); // よく理解してないが動いたのでヨシ！
    server.on("/get", std::bind(&StackChan_CaptivePortal::handle_Get, this)); // よく理解してないが動いたのでヨシ！
    server.begin();
}

void StackChan_CaptivePortal::Update()
{
    dnsServer.processNextRequest();
    server.handleClient();
}