#include <StackChan_Config.h>


StackChan_Config::StackChan_Config(){Preferences pref;}

void StackChan_Config::Open(){pref.begin("CaptivePortal");}
bool StackChan_Config::HasKey(){return pref.isKey("CaptivePortal");}

String StackChan_Config::GetSSID(){return pref.getString("wifi_ssid");}
String StackChan_Config::GetPassword(){return pref.getString("wifi_pass");}

void StackChan_Config::SetSSID(String ssid){pref.putString("wifi_ssid",ssid);}
void StackChan_Config::SetPassword(String pass){pref.putString("wifi_pass",pass);}

void StackChan_Config::Clear(){pref.clear();}
