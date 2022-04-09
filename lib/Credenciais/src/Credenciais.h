#ifndef CREDENCIAIS_H
#define CREDENCIAIS_H

#include <Arduino.h>

class Credenciais
{
private:
    String ssid;
    String pass;
    String admin;
public:
    Credenciais(String ssid, String pass, String admin);
    Credenciais();
    ~Credenciais();
    String getSsid();
    String getPass();
    String getAdmin();
    void setSsid(String ssid);
    void setPass(String pass);
    void setAdmin(String admin);
};

#endif // CREDENCIAIS_H