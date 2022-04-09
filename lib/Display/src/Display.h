/**
 * @file Display.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h> // Importa a Biblioteca display cristal liquido


class Display
{
    using Log = void (*)(String mensagem);   // Callback para log

private:
    LiquidCrystal_I2C *lcd;
    Log log = nullptr;
    void imprimirLog(String msg);    // Imprime a mensagem no log
    int coluna_scroll = 0;
    unsigned long tempo_scroll = 0;
    byte iconeGrau[8] = {B00110,
                         B01001,
                         B01001,
                         B00110,
                         B00000,
                         B00000,
                         B00000,
                         B00000}; // icone de grau

    byte iconeGota[8] = {B00000,
                         B00000,
                         B00100,
                         B01010,
                         B10001,
                         B10001,
                         B01110,
                         B00000101}; // icone de gota

    byte iconeGota2[8] = {B00000,
                          B00000,
                          B00100,
                          B01010,
                          B10001,
                          B11111,
                          B01110,
                          B00000}; // icone de gota

    byte iconeGota3[8] = {B00000,
                          B00000,
                          B00100,
                          B01110,
                          B11111,
                          B11111,
                          B01110,
                          B00000}; // icone de gota

    byte iconeTermometro[8] = {B00100,
                               B01010,
                               B01010,
                               B01010,
                               B01010,
                               B11111,
                               B11111,
                               B01110}; // icone de termometro  

    byte iconeTermometro2[8] = {B00100,
                                B01010,
                                B01010,
                                B01010,
                                B01110,
                                B11111,
                                B11111,
                                B01110}; // icone de termometro  

    byte iconeTermometro3[8] = {B00100,
                                B01110,
                                B01110,
                                B01110,
                                B01110,
                                B11111,
                                B11111,
                                B01110}; // icone de termometro                                               

public:
    Display();
    Display(LiquidCrystal_I2C *lcd);
    ~Display();

    byte ICONE_GRAU = 0;
    byte GOTA_VAZIA = 1;
    byte GOTA_MEDIA = 2;
    byte GOTA_CHEIA = 3;
    byte TERMOMETRO_VAZIO = 4;
    byte TERMOMETRO_MEDIO = 5;
    byte TERMOMETRO_CHEIO = 6;

    void limpar();
    boolean iniciar();
    void imprimir(String texto);
    void imprimirScroll(String texto, uint8_t linha);
    void imprimir(String texto, uint8_t linha, uint8_t coluna);
    void imprimirIcone(byte tipo);
    void imprimirIcone(byte tipo, uint8_t linha, uint8_t coluna);
    void setCallbackLog(void (*callBackLogDisplay)(String mensagem));
};
#endif

