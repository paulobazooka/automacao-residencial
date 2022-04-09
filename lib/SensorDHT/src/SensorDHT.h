/**
 * @file SensorDHT.h
 * @author Paulo Sérgio do Nascimento (paulosergionas@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef SENSOR_H
#define SENSOR_H

#include <DHT.h>

#define DHTPIN D5     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT 22 (AM2302)

class Sensor
{
    using Log = void (*)(String mensagem);   // Callback para log

private:
    DHT *sensor;
    Log log;
    void imprimiLog(String msg);    // Imprime a mensagem no log
public:
    Sensor();
    Sensor(DHT *sensor);
    ~Sensor();
    boolean iniciar();
    float getUmidade();
    float getTemperatura();
    String getTemperaturaString();
    String getUmidadeString();
    void setCallbackLog(void (*callBackLogSensor) (String mensagem));
};



#endif