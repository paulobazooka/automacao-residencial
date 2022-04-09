#ifndef DISPOSITIVO_H
#define DISPOSITIVO_H

#include <Arduino.h>

class Dispositivo
{
private:
   unsigned long boot; // número de boot do dispositivo
   String projeto;     // Nome do projeto
   String nome;        // nome do dispositivo
   String modelo;      // modelo do dispositivo
   String firmware;    // firmware do dispositivo
   String interface;   // interface do dispositivo
   String chipID;      // ID do chip

public:
   Dispositivo();
   Dispositivo(String projeto, String nome, String modelo, String firmware, String interface, String chipID);
   ~Dispositivo();
   unsigned long getBoot();
   String getProjeto();
   String getNome();
   String getModelo();
   String getFirmware();
   String getInterface();
   String getChipID();
   void setProjeto(String projeto);
   void setNome(String nome);
   void setModelo(String modelo);
   void setFirmware(String firmware);
   void setInterface(String interface);
   void setChipID(String chipID);
   void setBoot(unsigned long boot);
};

#endif