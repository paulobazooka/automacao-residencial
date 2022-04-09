#include <Dispositivo.h>

Dispositivo::Dispositivo()
{
}

Dispositivo::Dispositivo(String projeto, String nome, String modelo, String firmware, String interface, String chipID)
{
  this->projeto = projeto;
  this->nome = nome;
  this->modelo = modelo;
  this->firmware = firmware;
  this->interface = interface;
  this->chipID = chipID;
}

Dispositivo::~Dispositivo()
{
}

String Dispositivo::getProjeto()
{
  return this->projeto;
}

String Dispositivo::getNome()
{
  return this->nome;
}

String Dispositivo::getModelo()
{
  return this->modelo;
}

String Dispositivo::getFirmware()
{
  return this->firmware;
}

String Dispositivo::getInterface()
{
  return this->interface;
}

String Dispositivo::getChipID()
{
  return this->chipID;
}

unsigned long Dispositivo::getBoot()
{
  return this->boot;
}

void Dispositivo::setProjeto(String projeto)
{
  this->projeto = projeto;
}

void Dispositivo::setNome(String nome)
{
  this->nome = nome;
}

void Dispositivo::setModelo(String modelo)
{
  this->modelo = modelo;
}

void Dispositivo::setFirmware(String firmware)
{
  this->firmware = firmware;
}

void Dispositivo::setInterface(String interface)
{
  this->interface = interface;
}

void Dispositivo::setChipID(String chipID)
{
  this->chipID = chipID;
}

void Dispositivo::setBoot(unsigned long boot)
{
  this->boot = boot;
}