#include "Credenciais.h"

Credenciais::Credenciais(){}

Credenciais::Credenciais(String ssid, String pass, String admin)
{
    this->ssid = ssid;
    this->pass = pass;
    this->admin = admin;
}

Credenciais::~Credenciais(){}

String Credenciais::getSsid(){return this->ssid;}

String Credenciais::getPass(){return this->pass;}

String Credenciais::getAdmin(){return this->admin;}

void Credenciais::setSsid(String ssid){this->ssid = ssid;}

void Credenciais::setPass(String pass){this->pass = pass;}

void Credenciais::setAdmin(String admin){this->admin = admin;}