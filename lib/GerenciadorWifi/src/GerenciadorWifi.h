/**
 * @file GerenciadorWifi.h
 * @author Paulo Sérgio do Nascimento (paulosergionas@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-02-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef GERENCIADORWIFI_H
#define GERENCIADORWIFI_H

#include <iostream>                 // std::cout
#include <LittleFS.h>               // Manipular arquivos
#include <DNSServer.h>              // Servidor DNS
#include <ESP8266WiFi.h>            // Importa a Biblioteca Wifi
#include <ESPAsyncTCP.h>            // Importa a Biblioteca Async TCP
#include "AsyncJson.h"              // Importa a Biblioteca Async Json
#include "ArduinoJson.h"            // Biblioteca para manipular Json
#include <AsyncWebSocket.h>         // Importa a Biblioteca Async WebSocket
#include <Dispositivo.h>            // Dispositivo
#include <Credenciais.h>            // Credenciais
#include <GerenciadorArquivos.h>    // Gerenciador de arquivos

class GerenciadorWifi
{
    using EventoWifi = void (*)(String ssid, String senha);   // Evento de alteração de configuração da rede Wifi
    using EventoRele = void (*)(int rele);                    // Evento de alteração de estado do rele
    using Log = void (*)(String mensagem);   // Callback para log

private:
    IPAddress *ipAddress;   // Endereço de IP do AP
    IPAddress *IP;           // Endereço de IP do AP
    IPAddress *GATEWAY;      // Endereço de IP do Gateway
    IPAddress *MASK_SUBNET;  // Mascara de IP da Sub-rede

    DNSServer *dnsServer;                    // DNS para portal cativo
    AsyncWebServer *asyncWebServer;         // Servidor HTTP
    
    Dispositivo *dispositivo;               // Dispositivo
    Credenciais *credenciais;               // Credenciais
    EventoWifi eventoWifi;                  // Evento de alteração de configuração da rede Wifi
    EventoRele eventoRele;                  // Evento de alteração de estado do rele
    Log log;                                // Callback para log

    const String HOSTNAME = "meu_lar";
    const String PASS = "meular123";
    const char *MENSAGEM_CONFIGURACAO_WIFI = "   DESCONECTADO - Conectar na rede meu_lar com a senha meu_lar e acessar o IP 192.168.4.1 no browser para configurar! ";
    const byte DNS_PORT = 53;               // Porta para modo DNS em Access Point
    const byte TENTATIVAS_CONEXAO = 20;     // Número de tentativas de conexão com o retador
    const int TEMPO_REINICIALIZACAO = 3000; // Tempo para reinicialização do modo AP
    float *temperatura;                     // Temperatura do ambiente
    float *umidade;                         // Umidade do ambiente
    String processor(const String &var);    // Processador de variáveis
    void imprimirLog(String msg);           // Imprime o log

public:
    GerenciadorWifi(AsyncWebServer *s, DNSServer *dnsServer, IPAddress *IP, IPAddress *GATEWAY, IPAddress *MASK_SUBNET, Dispositivo *dispositivo);
    GerenciadorWifi(AsyncWebServer *s, DNSServer *dnsServer, IPAddress *IP, IPAddress *GATEWAY, IPAddress *MASK_SUBNET, Dispositivo *dispositivo, float *temperatura, float *umidade);
    GerenciadorWifi(AsyncWebServer *s, DNSServer *dnsServer, IPAddress *IP, IPAddress *GATEWAY, IPAddress *MASK_SUBNET, Dispositivo *dispositivo, Credenciais *credenciais, float *temperatura, float *umidade);
    ~GerenciadorWifi();

    const boolean MODO_STATION = 0;
    const boolean MODO_ACCESS_POINT = 1;

    /**
     * @brief Retorna o status de conexão do dispositivo
     *
     * @return true Se o dispositivo está conectado
     * @return false Se o dispositivo não está conectado
     */
    bool conectado();

    /**
     * @brief Altera o dispositivo para modo Station
     *
     */
    void modoStation();

    /**
     * @brief Altera o dispositivo para modo Access Point
     *
     */
    void modoAccessPoint();

    /**
     * @brief Configura o servidor HTTP com as chamadas HTTP
     *
     */
    void configurarServidor();

    /**
     * @brief Callback para o servidor HTTP
     *
     */
    void capturarRequisicoes();

    /**
     * @brief Obtém o endereço de IP do AP
     *
     * @return IPAddress Endereço de IP
     */
    IPAddress localIp();

    /**
     * @brief Obtém o endereço de IP
     *
     * @return String endereço do IP local
     */
    String getIP();

    /**
     * @brief Obtém o endereço de IP
     *
     * @return String endereço do IP local
     */
    String getStatus();

    /**
     * @brief Conecta com o nome do AP e senha
     *
     * @param ssid String com o nome do SSID
     * @param pass String com a senha do SSID
     */
    void conectarWiFi(String ssid, String pass);

    /**
     * @brief Retorna o modo de operação do dispositivo
     *      0 - Modo Station
     *      1 - Modo Access Point
     * @return boolean 
     */
    boolean obterModoOperacao();

    /**
     * @brief Função callback que será chamada quando houver alteração no wifi
     *
     */
    void callbackReconfiguracaoWifi(void (*alterarConfiguracaoWifi) (String ssid, String pass));

    void callbackEstadoRele(void (*alterarEstadoRele) (int rele));

    void setCallbackLog(void (*callBackLogWifi) (String mensagem));

    void scanearRedes();                    // Scanea as redes disponíveis
};

#endif