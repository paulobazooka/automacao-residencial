#include "GerenciadorArquivos.h"

GerenciadorArquivos::GerenciadorArquivos(){}

GerenciadorArquivos::~GerenciadorArquivos()
{
}

bool GerenciadorArquivos::iniciarParticao()
{
  if (LittleFS.begin())
  {
    this->imprimirLog("Partição inicializada com sucesso");
    return true;
  }
  this->imprimirLog("ERRO ao inicializar a partição de arquivos!");
  return false;
}

void GerenciadorArquivos::encerrarParticao()
{
  LittleFS.end();
  this->imprimirLog("Partição encerrada com sucesso");
}

boolean GerenciadorArquivos::particaoIntegra()
{
  if (LittleFS.check())
  {
    this->imprimirLog("Partição íntegra");
    return true;
  }

  this->imprimirLog("ERRO! Partição não íntegra");
  return false;
}

void GerenciadorArquivos::listarArquivos()
{
  
}

void GerenciadorArquivos::criarArquivo(const char *path_arquivo, const char *conteudo)
{
  if (LittleFS.check())
  {
    File file = LittleFS.open(path_arquivo, "w");
    file.print(conteudo);
    file.close();
  }
}

void GerenciadorArquivos::obterDispositivo(const char *path_dispositivo, Dispositivo *dispositivo)
{
  this->imprimirLog("Obtendo informações do dispositivo");
  if (LittleFS.check())
  {
    File file = LittleFS.open(path_dispositivo, "r");
    if (file)
    {
      StaticJsonDocument<512> doc;
      DeserializationError error = deserializeJson(doc, file.readString());
      if (error) 
        this->imprimirLog("ERRO ao obter informações do arquivo " + String(path_dispositivo) + "\n" + error.c_str());
      else{
        dispositivo->setBoot(doc["boot"]);
        dispositivo->setNome(doc["nome"]);
        dispositivo->setProjeto(doc["projeto"]);
        dispositivo->setFirmware(doc["firmware"]);
        dispositivo->setInterface(doc["interface"]);
        dispositivo->setModelo(doc["modelo"]);
        //dispositivo->setChipID(deviceID());
      }
    }
    file.close();
  }
}

void GerenciadorArquivos::obterCredenciais(const char *path_credenciais, Credenciais *credenciais)
{
  this->imprimirLog("Obtendo credenciais wifi");
  if (LittleFS.check())
  {
    File file = LittleFS.open(path_credenciais, "r");
    if (file)
    {
      StaticJsonDocument<192> doc;
      DeserializationError error = deserializeJson(doc, file.readString());
      if (error) 
        this->imprimirLog("ERRO ao obter informações do arquivo " + String(path_credenciais) + "\n" + error.c_str());
      else{
        credenciais->setSsid(doc["ssid"]);
        credenciais->setPass(doc["pass"]);
        credenciais->setAdmin(doc["admin"]);
      }
    }
    file.close();
  }
}

bool GerenciadorArquivos::setarCredenciais(const char *path_credenciais, Credenciais *credenciais)
{
  this->imprimirLog("Setando credenciais wifi");
  if (LittleFS.check())
  {
    File file = LittleFS.open(path_credenciais, "w");
    if (file)
    {
      StaticJsonDocument<192> doc;
      doc["ssid"] = credenciais->getSsid();
      doc["pass"] = credenciais->getPass();
      if(serializeJson(doc, file) > 0){
        file.close();
        return true;
      }
      file.close();
      return false;  
    }
    return false;
  }
  return false;
}

String GerenciadorArquivos::obtemConteudoArquivo(const char *path_arquivo)
{
  this->imprimirLog("Obtendo conteúdo de " + String(path_arquivo));
  String conteudo = "";

  if (LittleFS.check())
  {
    File file = LittleFS.open(path_arquivo, "r");
    if (file)
      conteudo = file.readString();
    else
    {
      this->imprimirLog("Arquivo " + String(path_arquivo) + " não encontrado!\nCriando um novo arquivo...");
      File file = LittleFS.open(path_arquivo, "w");
    }
    file.close();
  }
  else
    this->imprimirLog("ERRO ao abrir " + String(path_arquivo) + "! Partição não íntegra");
  return conteudo;
}

void GerenciadorArquivos::setCallbackLog(void (*callBackLogArquivos)(String mensagem))
{
  this->log = callBackLogArquivos;
}

void GerenciadorArquivos::imprimirLog(String msg)
{
  if (this->log != nullptr)
    this->log(msg);
}

String GerenciadorArquivos::deviceID()
{
  String id = "ESP8266-" + hexStr(ESP.getChipId(), 8);
  this->imprimirLog("ID do dispositivo: " + id);
  return id;
}

String hexStr(const unsigned long &h, const byte &l = 8)
{
  String s = "";
  s = String(h, HEX);
  s.toUpperCase();
  s = ("00000000" + s).substring(s.length() + 8 - l);
  return s;
}