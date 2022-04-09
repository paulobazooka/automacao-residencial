#ifndef RELE_H
#define RELE_H

#include <Arduino.h>
#include <Adafruit_MCP23X17.h> // Importa a Biblioteca MCP23017


class Rele
{
    using Log = void (*)(String mensagem);   // Callback para log

private:
    Adafruit_MCP23X17 mcp; // Instancia da classe Adafruit_MCP23017
    Log log;
    void imprimiLog(String msg);    // Imprime a mensagem no log
public:
    Rele();
    ~Rele();
    bool iniciar();
    void setCallbackLog(void (*callBackLogRele) (String mensagem));
    void setRele(uint8_t rele);
    u_int8_t getEstadoRele(uint8_t rele);
};

#endif