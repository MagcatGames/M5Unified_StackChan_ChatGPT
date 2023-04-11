#include <StackChan_CaptivePortal.h>


StackChan_CaptivePortal::StackChan_CaptivePortal()
{
    ESP32WebServer server(80);
    DNSServer dnsServer;
    StackChan_Config cfg;
}

void StackChan_CaptivePortal::Begin()
{
    IPAddress apIP(192,168,4,1);

    cfg.Open();
    WiFi.mode(WIFI_AP);
    WiFi.disconnect();
    delay(100);

    int n = WiFi.scanNetworks();
    delay(100);

    WiFi.softAP("StackChanAP");
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    dnsServer.start(53,"*", apIP);
    delay(100);

    String ssidList = "<datalist id='ssidList'>";
    for (int i = 0; i < n; ++i) {
        ssidList += sprintf("<option value='%s'>%s</option>",WiFi.SSID(i).c_str());
    }
    ssidList += "</datalist>";
    delay(100);

    server.onNotFound([&](){
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
                "<form method='get' action='settings'>"
                    "<div>SSID</div>"
                    "<div><input type='text' name='wifi-ssid' value='"+ssid+"' autocomplete='on' list='ssidList' >required><span style='color:red'>*</span></div>"
                    +ssidList+
                    "<div>Password</div>"
                    "<div><input type='password' name='wifi_pass'></div>"
                    "<div><input type='submit'></div>"
                "</form>"
            "</body>"
        "</html>"
        );
    });
    server.on("/settings",[&](){
        String ssid = server.arg("wifi_ssid");
        String pass = server.arg("wifi_pass");
        cfg.SetSSID(ssid);
        cfg.SetPassword(pass);
        ESP.restart();
    });
    server.begin();
}

void StackChan_CaptivePortal::Update()
{
    dnsServer.processNextRequest();
    server.handleClient();
}
