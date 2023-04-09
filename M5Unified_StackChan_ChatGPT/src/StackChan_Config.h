#ifndef SCCP_CONFIG
#define SCCP_CONFIG

#include <Preferences.h>

class StackChan_Config
{
    public:
        StackChan_Config();
        void Open();
        String GetSSID();
        String GetPassword();
        void SetSSID(String ssid);
        void SetPassword(String pass);
        bool HasKey();
        void Clear();
    private:
        Preferences pref;
};

#endif