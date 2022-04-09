/**
 * @file Display.cpp
 * @author Paulo Sérgio do Nascimento (paulosergionas@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "Display.h"

Display::Display(){}

Display::Display(LiquidCrystal_I2C *lcd)
{
    this->lcd = lcd;
}

Display::~Display(){}

boolean Display::iniciar(){
    if (this->lcd != nullptr){
        this->lcd->init();
        this->lcd->noDisplay();
        this->lcd->display();
        this->lcd->backlight();
        this->lcd->clear();
        this->lcd->createChar(this->ICONE_GRAU, iconeGrau);
        this->lcd->createChar(this->GOTA_VAZIA, iconeGota);
        this->lcd->createChar(this->GOTA_MEDIA, iconeGota2);
        this->lcd->createChar(this->GOTA_CHEIA, iconeGota3);
        this->lcd->createChar(this->TERMOMETRO_VAZIO, iconeTermometro);
        this->lcd->createChar(this->TERMOMETRO_MEDIO, iconeTermometro2);
        this->lcd->createChar(this->TERMOMETRO_CHEIO, iconeTermometro3);
        this->imprimirLog("Display inicializado com sucesso!");
        return true;
    }    
    this->imprimirLog("ERRO ao inicializar o display!");
    return false;  
}

void Display::imprimirScroll(String texto, uint8_t linha)
{
    if (this->lcd != nullptr)
    {
        if (millis() - this->tempo_scroll > 400)
        {
            this->tempo_scroll = millis();
            this->coluna_scroll += 1;
            if (this->coluna_scroll > texto.length())
                this->coluna_scroll = 0;    
               
            this->lcd->setCursor(0, linha);
            for (byte i=0; i<16; i++)
            {
                if (i+this->coluna_scroll < texto.length())
                    this->lcd->print(texto.charAt(i+this->coluna_scroll));
                else
                    this->lcd->print(" ");
            }
        }
         
    } else {
        this->imprimirLog("ERRO ao imprimir no display!");
    }
}

void Display::imprimir(String texto, uint8_t linha, uint8_t coluna)
{
    if (this->lcd != nullptr)
    {
        this->lcd->setCursor(coluna, linha);
        this->lcd->print(texto);
         
    } else {
        this->imprimirLog("ERRO ao imprimir no display!");
    }
}

void Display::imprimir(String texto)
{
    if (this->lcd != nullptr)
        this->lcd->print(texto);
     else 
        this->imprimirLog("ERRO ao imprimir no display!");
}

void Display::imprimirIcone(byte tipo){
    if (this->lcd != nullptr)
        this->lcd->write(byte(tipo));
     else 
        this->imprimirLog("ERRO ao imprimir no display!");
}

void Display::imprimirIcone(byte tipo, uint8_t linha, uint8_t coluna)
{
    if (this->lcd != nullptr)
    {
        this->lcd->setCursor(coluna, linha);
        this->lcd->write(byte(tipo));
    } else {
        this->imprimirLog("ERRO ao imprimir no display!");
    }
}

void Display::limpar()
{
    if (this->lcd != nullptr)
    {
        this->lcd->clear();
    } else {
        this->imprimirLog("ERRO ao limpar o display!");
    }
}

void Display::setCallbackLog(void (*callBackLogDisplay) (String mensagem)){
    this->log = callBackLogDisplay;
}

void Display::imprimirLog(String msg){
    if (this->log != nullptr)
        this->log(msg);
}