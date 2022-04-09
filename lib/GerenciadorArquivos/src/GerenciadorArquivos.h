#ifndef GERENCIADORARQUIVOS_H
#define GERENCIADORARQUIVOS_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include <Dispositivo.h>
#include <Credenciais.h>

#define MODO_LEITURA "r"
#define MODO_ESCRITA "w"
#define MODO_APENDICE "a"

class GerenciadorArquivos
{
  using Log = void (*)(String mensagem);   // Callback para log

private:
  Log log;                                // Callback para log
  void imprimirLog(String msg);           // Imprime o log
  File _abrirArquivo(const char *path);
  String abrirArquivo(const char *path);
  String hexStr(const unsigned long &h, const byte &l = 8);

public:
  GerenciadorArquivos();
  ~GerenciadorArquivos();

  String deviceID();
  void listarArquivos();
  bool iniciarParticao();
  void encerrarParticao();
  boolean particaoIntegra();
  String obtemConteudoArquivo(const char* path_arquivo);
  void criarArquivo(const char* path_arquivo, const char* conteudo);
  void setCallbackLog(void (*callBackLogArquivos) (String mensagem));
  void obterCredenciais(const char* path_credenciais, Credenciais *credenciais);
  bool setarCredenciais(const char* path_credenciais, Credenciais *credenciais);
  void obterDispositivo(const char* path_dispositivo, Dispositivo *dispositivo);
};

#endif