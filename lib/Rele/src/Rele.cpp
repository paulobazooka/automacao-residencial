#include "Rele.h"

Rele::Rele()
{
}

Rele::~Rele()
{
}

bool Rele::iniciar()
{
    if (mcp.begin_I2C())
    {
        delay(100);
        for (uint8_t i = 0; i < 8; i++)
        {
            mcp.pinMode(i, OUTPUT);
            mcp.digitalWrite(i, HIGH);
        }
        this->imprimiLog("Reles inicializados com sucesso!");
        return true;
    }
    this->imprimiLog("ERRO ao inicializar os reles!");
    return false;
}

void Rele::imprimiLog(String msg)
{
    if (this->log != nullptr)
        this->log(msg);
}

void Rele::setCallbackLog(void (*callBackLogRele)(String mensagem))
{
    this->log = callBackLogRele;
}

void Rele::setRele(uint8_t rele){
    uint8_t estado = mcp.digitalRead(rele);
    mcp.digitalWrite(rele, !estado);
}

uint8_t Rele::getEstadoRele(uint8_t rele){
    return mcp.digitalRead(rele);
}