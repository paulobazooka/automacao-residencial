#include <SensorDHT.h>

Sensor::Sensor()
{
    
}

Sensor::Sensor(DHT *sensor)
{
    this->sensor = sensor;
}

Sensor::~Sensor()
{
    
}

boolean Sensor::iniciar(){
    if (this->sensor != nullptr){
        this->sensor->begin();
        delay(3000);
        if (this->sensor->read() != true){
            this->imprimiLog("ERRO ao inicializar o sensor DHT!");
            return false;
        }
        this->imprimiLog("Sensor DHT inicializado com sucesso!");
        return true;
    } else{
        this->imprimiLog("ERRO ao inicializar o sensor DHT!");
        return false;  
    }  
}

float Sensor::getTemperatura()
{
    if (this->sensor != nullptr)
    {   
        float temperatura = this->sensor->readTemperature();
        if (isnan(temperatura)){
            this->imprimiLog("ERRO ao ler a temperatura!");
            return 0;
        }
        else
            return temperatura;
    }
    this->imprimiLog("ERRO ao ler a temperatura! Sensor não inicializado!");
    return 0.0f;
}

float Sensor::getUmidade()
{
    if (this->sensor != nullptr)
    {   
        float umidade = this->sensor->readHumidity();     
        if (isnan(umidade)){
            this->imprimiLog("ERRO ao ler a umidade! ");
            return 0;
        }
        else
            return umidade;
    }
    this->imprimiLog("ERRO ao ler a umidade! Sensor não inicializado!");
    return 0.0f;
}

String Sensor::getTemperaturaString(){
    if (this->sensor != nullptr)
    {   
        float temperatura = this->sensor->readTemperature();
        if (isnan(temperatura)){
            this->imprimiLog("ERRO ao ler a temperatura! " + String(temperatura));
            return "--";
        }
        else
            if ((temperatura <= 9.0f && temperatura >= 0.0f))
                return "0" + String(temperatura, 1);
            else
                return String(temperatura, 1);
    }
    this->imprimiLog("ERRO ao ler a temperatura! Sensor não inicializado!");
    return "--";
}

String Sensor::getUmidadeString(){
    if (this->sensor != nullptr)
    {   
        float umidade = this->sensor->readHumidity();     
        if (isnan(umidade)){
            this->imprimiLog("ERRO ao ler a umidade! " + String(umidade));
            return "--";
        }
        else
            if ((umidade <= 9.0f && umidade >= 0.0f))
                return "0" + String(umidade, 1);
            else
                return String(umidade, 1);
    }
    this->imprimiLog("ERRO ao ler a umidade! Sensor não inicializado!");
    return "--";
}

void Sensor::imprimiLog(String msg){
    if (this->log != nullptr)
        this->log(msg);
}

void Sensor::setCallbackLog(void (*callBackLogSensor) (String mensagem)){
    this->log = callBackLogSensor;
}