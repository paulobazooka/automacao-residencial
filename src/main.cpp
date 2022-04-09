/**
 * @file main.cpp
 * @author Paulo Sérgio do Nascimento (paulosergionas@gmail.com)
 * @brief 
 * @version 0.1
 * @date 
 * 
 * @copyright Copyright (c) 2022
 * 
 *  TODO: 
 *    1 - Terminar a configuração de Wifi
 *    2 - Fazer a tela de acesso/admin em modo access point
 *    3 - Fazer a persistência em arquivo das configurações de wifi e Administrador
 *    4 - Fazer a tela dos botões para acesso aos relés
 *    5 - Desenvolver a comunicação com o MCP23017 para alterar as saídas
 *    6 - Implementar a leitura de temnperatura e umidade na tela de botões
 *    7 - Obter a previsão do tempo de acordo com a localização/IP
 *    8 - Implementar a leitura de previsão do tempo na tela de botões
 */
#include <Arduino.h>

#include <Wire.h>               // Importa a Biblioteca comunicação I2C
#include <DHT.h>                // Importa a Biblioteca para leitura de temperatura e umidade
#include <AsyncWebSocket.h>     // Importa a Biblioteca Async WebSocket
#include <Adafruit_MCP23X17.h>  // Importa a Biblioteca MCP23017
#include <LiquidCrystal_I2C.h>  // Importa a Biblioteca display cristal liquido
#include <ESP8266WiFi.h>

#include <Rele.h>
#include <Display.h>
#include <SensorDHT.h>
#include <Credenciais.h>
#include <Dispositivo.h>
#include <GerenciadorWifi.h>
#include <GerenciadorArquivos.h>

// *****************************************************************************
// Definições
#define BAUDE_RATE 115200   // Define a taxa de transmissão
#define DHTPIN D5           // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22       // DHT 22 (AM2302)

// *****************************************************************************
// Constantes
const char* ARQUIVO_CONFIG_WIFI = "/config/wifi.json";
const char* ARQUIVO_DISPOSITIVO = "/config/dispositivo.json";
const unsigned long INTERVALO_VERIFICAR_SENSOR = 1000 * 5;

// Variáveis
float umidade = 0;
float temperatura = 0;
unsigned long IRAM_ATTR TEMPO_VERIFICAR_SENSOR;

// *****************************************************************************
// Objetos
AsyncWebServer s(80);                             // Servidor HTTP assíncrono
DHT dht(DHTPIN, DHTTYPE);                         // Objeto para leitura de temperatura e umidade
DNSServer dnsServer;                              // DNS para portal cativo
LiquidCrystal_I2C lcd(0x27, 16, 2);               // Define o display
IPAddress IP = IPAddress(192,168,4,1);            // Endereço de IP do AP
IPAddress GATEWAY = IPAddress(192,168,4,1);       // Endereço de IP do Gateway
IPAddress MASK_SUBNET = IPAddress(255,255,255,0); // Mascara de IP da Sub-rede


Credenciais credenciais;                          // Objeto para armazenar as credenciais
Dispositivo dispositivo;                          // Objeto Dispositivo
Display display(&lcd);                            // Objeto para exibição de texto no display
Sensor sensorDHT(&dht);                           // Objeto para leitura de temperatura e umidade DHT22
GerenciadorArquivos gerenciadorArquivos;          // Objeto para gerenciamento de arquivos
GerenciadorWifi gerenciadorWifi(&s, &dnsServer, &IP, &GATEWAY, &MASK_SUBNET, &dispositivo, &credenciais, &temperatura, &umidade); // Objeto para gerenciamento de Wifi
Rele rele;

// *****************************************************************************
// Escopo das Funções
void imprimirLogMain(String msg);
void callBackLogWifi(String msg);
void callBackLogRele(String msg);
void callBackLogSensor(String msg);
void callBackLogDisplay(String msg);
void callBackLogArquivos(String msg);
unsigned long obterTempoAtualEmSegundos();
void alterarConfiguracaoWifi(String ssid, String senha);
void alterarEstadoRele(int rele);

// *****************************************************************************
// Configurações iniciais
void setup() {
  Serial.begin(BAUDE_RATE);
  delay(800);

  imprimirLogMain("Automação Residencial");

  rele.setCallbackLog(&callBackLogRele);
  display.setCallbackLog(&callBackLogDisplay);
  sensorDHT.setCallbackLog(&callBackLogSensor);
  gerenciadorWifi.setCallbackLog(&callBackLogWifi);
  gerenciadorArquivos.setCallbackLog(&callBackLogArquivos);
  gerenciadorWifi.callbackReconfiguracaoWifi(&alterarConfiguracaoWifi);
  gerenciadorWifi.callbackEstadoRele(&alterarEstadoRele);

  // Inicia a partição de arquivos
  if(!gerenciadorArquivos.iniciarParticao())
    while(true);
  // Verifica a integridade dos arquivos
  if(!gerenciadorArquivos.particaoIntegra())
    while(true);
  
  // Inicializa o display LCD 16x02
  if(!display.iniciar())
    while(true); 

  display.imprimir("Automacao      ", 0, 0);
  display.imprimir("    Residencial", 1, 0);
  delay(500);
  display.imprimir("Particao de     ", 0, 0);
  display.imprimir("     Arquivos OK", 1, 0);
  delay(500);

  // Obter os dados do dispositivo 
  gerenciadorArquivos.obterDispositivo(ARQUIVO_DISPOSITIVO, &dispositivo);

  // Obter as credenciais para conectar no roteador wifi
  gerenciadorArquivos.obterCredenciais(ARQUIVO_CONFIG_WIFI, &credenciais);

  // Tentativa de conexão default
  display.limpar();
  display.imprimir("Conectando em   ", 0, 0);  
  display.imprimir(credenciais.getSsid(), 1, 0);
  gerenciadorWifi.modoStation(); 
  gerenciadorWifi.conectarWiFi(credenciais.getSsid(), credenciais.getPass());
  delay(500);

  if(gerenciadorWifi.conectado() == false){
    gerenciadorWifi.modoAccessPoint();
    display.imprimir("Modo Access     ", 0, 0);
    display.imprimir("   Point Ativado", 1, 0);
  } else {
    display.imprimir("Modo Station    ", 0, 0);
    display.imprimir("         Ativado", 1, 0); 
  }
  delay(500);

  // Configura o servidor webserver
  gerenciadorWifi.configurarServidor();
  display.imprimir("Configurando    ", 0, 0);    
  display.imprimir("        Servidor", 1, 0);  
  delay(500);
  
  // Inicia o sensor DHT22
  if(!sensorDHT.iniciar()){
    display.imprimir("*ERRO no Sensor*", 1, 0);
    while(true);
  }

  if(!rele.iniciar()){
    display.imprimir("*ERRO no Rele*", 1, 0);
    while(true);
  }
 
  TEMPO_VERIFICAR_SENSOR = millis();
  display.limpar();
}

void loop() {
 
  display.imprimirScroll(gerenciadorWifi.getStatus(), 0);
  
  if (gerenciadorWifi.obterModoOperacao() == gerenciadorWifi.MODO_ACCESS_POINT && !gerenciadorWifi.conectado())
    gerenciadorWifi.capturarRequisicoes();
  
  // Se está no intervalo de realizar a leitura do sensor DHT22
  if ((millis() - TEMPO_VERIFICAR_SENSOR) > INTERVALO_VERIFICAR_SENSOR){
    TEMPO_VERIFICAR_SENSOR = millis();
    temperatura = sensorDHT.getTemperatura();
    umidade = sensorDHT.getUmidade();

    byte tipoTermometro = 0;
    if (temperatura <= 18)
      tipoTermometro = display.TERMOMETRO_VAZIO;
    if (temperatura > 18 && temperatura < 30)
      tipoTermometro = display.TERMOMETRO_MEDIO;
    if (temperatura >= 30)
      tipoTermometro = display.TERMOMETRO_CHEIO;

    display.imprimirIcone(tipoTermometro, 1, 0);
    display.imprimir(String(temperatura,1), 1, 1);
    display.imprimirIcone(display.ICONE_GRAU);
    display.imprimir("C");

    byte tipoGota = 0;
    if (umidade <= 30)
      tipoGota = display.GOTA_VAZIA;
    if (umidade > 30 && umidade <= 65)
      tipoGota = display.GOTA_MEDIA;      
    if (umidade > 65)
      tipoGota = display.GOTA_CHEIA;

    display.imprimirIcone(tipoGota, 1, 10);
    display.imprimir(String(umidade,1), 1, 11);
    display.imprimir("%");

    //imprimirLogMain(String(temperatura,1) + "ºC\t" + String(umidade,1) + "%");
  } 
}


void alterarConfiguracaoWifi(String ssid, String senha){
  if (!ssid.isEmpty() && !senha.isEmpty()){
    Serial.printf("[%li]\t[WIFI]\t[RECONFIGURACAO] ssid[%s] senha[%s]\n", millis(), ssid.c_str(), senha.c_str());
    credenciais.setSsid(ssid);
    credenciais.setPass(senha);
    bool result = gerenciadorArquivos.setarCredenciais(ARQUIVO_CONFIG_WIFI ,&credenciais);
    if (result){
      imprimirLogMain("reconfiguração do wifi concluída com sucesso!");
      display.limpar();
      display.imprimir("Resetando...");
      delay(2500);
      ESP.reset();
    }
  }
}

void callBackLogRele(String msg){
  Serial.printf("[%li]\t[RELE]\t%s\n", obterTempoAtualEmSegundos(), msg.c_str());
}

void alterarEstadoRele(int relay){
  Serial.printf("[%li]\t[WIFI] [RELE]\t%i\n", obterTempoAtualEmSegundos(), relay);
  imprimirLogMain("Estado do rele " + String(relay) + " é " + rele.getEstadoRele(relay));
  rele.setRele(relay);
}

void callBackLogWifi(String msg){
  Serial.printf("[%li]\t[WIFI]\t%s\n", obterTempoAtualEmSegundos(), msg.c_str());
}

void callBackLogDisplay(String msg){
  Serial.printf("[%li]\t[LCD]\t%s\n", obterTempoAtualEmSegundos(), msg.c_str());
}

void callBackLogSensor(String msg){
  Serial.printf("[%li]\t[DHT]\t%s\n", obterTempoAtualEmSegundos(), msg.c_str());
}

void callBackLogArquivos(String msg){
  Serial.printf("[%li]\t[FILE]\t%s\n", obterTempoAtualEmSegundos(), msg.c_str());
}

void imprimirLogMain(String msg){
  Serial.printf("[%li]\t[MAIN]\t%s\n", obterTempoAtualEmSegundos(), msg.c_str());
}

unsigned long obterTempoAtualEmSegundos(){
  return millis()/1000;
}