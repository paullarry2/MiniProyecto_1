/*
 * File:   main_m.c
 * Author: Larry Paúl Fuentes
 * Carné 18117
 * Mini Proyecto 2
 * Master
 * Digital 2
 *
 * Created on 22 de febrero de 2021, 4:52 PM
 */
//******************************************************************************
//Librerias
//******************************************************************************
#pragma config FOSC = INTRC_NOCLKOUT // Oscillator Selection bits (RC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON      // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma    config WRT = OFF 

#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "I2C.h"
#include "BMP280.h"
#include "UART.h"

#define _XTAL_FREQ (8000000) 
#define address_r 0xEF
#define address_w 0xEE
#define p_msb 0xF7
#define p_lsb 0xF7

#ifndef BMP280_I2C_ADDRESS
  #define BMP280_I2C_ADDRESS  0xEC
#endif

#define BMP280_CHIP_ID        0x58

#define BMP280_REG_DIG_T1     0x88
#define BMP280_REG_DIG_T2     0x8A
#define BMP280_REG_DIG_T3     0x8C

#define BMP280_REG_DIG_P1     0x8E
#define BMP280_REG_DIG_P2     0x90
#define BMP280_REG_DIG_P3     0x92
#define BMP280_REG_DIG_P4     0x94
#define BMP280_REG_DIG_P5     0x96
#define BMP280_REG_DIG_P6     0x98
#define BMP280_REG_DIG_P7     0x9A
#define BMP280_REG_DIG_P8     0x9C
#define BMP280_REG_DIG_P9     0x9E

#define BMP280_REG_CHIPID     0xD0
#define BMP280_REG_SOFTRESET  0xE0

#define BMP280_REG_STATUS     0xF3
#define BMP280_REG_CONTROL    0xF4
#define BMP280_REG_CONFIG     0xF5
#define BMP280_REG_PRESS_MSB  0xF7

//******************************************************************************
//Variables
//******************************************************************************
uint8_t presmas;
signed long temperature;
unsigned long pressure;
char RX_Trans;
uint32_t temp;
char dec_t;
char uni_t;
char deci_t;
char centi_t;


typedef enum {
    MODE_SLEEP = 0x00, // sleep mode
    MODE_FORCED = 0x01, // forced mode
    MODE_NORMAL = 0x03 // normal mode
} BMP280_mode;

// oversampling setting. osrs_t[2:0], osrs_p[2:0]

typedef enum {
    SAMPLING_SKIPPED = 0x00, //skipped, output set to 0x80000
    SAMPLING_X1 = 0x01, // oversampling x1
    SAMPLING_X2 = 0x02, // oversampling x2
    SAMPLING_X4 = 0x03, // oversampling x4
    SAMPLING_X8 = 0x04, // oversampling x8
    SAMPLING_X16 = 0x05 // oversampling x16
} BMP280_sampling;

// filter setting filter[2:0]

typedef enum {
    FILTER_OFF = 0x00, // filter off
    FILTER_2 = 0x01, // filter coefficient = 2
    FILTER_4 = 0x02, // filter coefficient = 4
    FILTER_8 = 0x03, // filter coefficient = 8
    FILTER_16 = 0x04 // filter coefficient = 16
} BMP280_filter;

// standby (inactive) time in ms (used in normal mode), t_sb[2:0]

typedef enum {
    STANDBY_0_5 = 0x00, // standby time = 0.5 ms
    STANDBY_62_5 = 0x01, // standby time = 62.5 ms
    STANDBY_125 = 0x02, // standby time = 125 ms
    STANDBY_250 = 0x03, // standby time = 250 ms
    STANDBY_500 = 0x04, // standby time = 500 ms
    STANDBY_1000 = 0x05, // standby time = 1000 ms
    STANDBY_2000 = 0x06, // standby time = 2000 ms
    STANDBY_4000 = 0x07 // standby time = 4000 ms
} standby_time;

struct {
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;

    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
} BMP280_calib;

char buffer[17];

//******************************************************************************
//Prototipo de Funciones
//******************************************************************************
void setup(void);
void Enviar_temp(void);

//******************************************************************************
//Main Loop 
//******************************************************************************

void main(void) {
    setup();
    if(BMP280_begin(MODE_NORMAL, SAMPLING_X1, SAMPLING_X1, FILTER_OFF, STANDBY_0_5) == 0)
{  // connection error or device address wrong!
        PORTBbits.RB2 = 1;
    while (1) {}}
    while (1) {
    BMP280_readTemperature(&temperature);  // read temperature
    BMP280_readPressure(&pressure);    
    Enviar_temp();
    UART_send_string("#");
    Uart_send_char(dec_t);
    Uart_send_char(uni_t);
    UART_send_string(".");
    Uart_send_char(deci_t);
    Uart_send_char(centi_t);
   
    
    if (RX_Trans == 0b00000000){
        PORTBbits.RB6 = 0; //LED ROJO
    }
    else if (RX_Trans == 0b00000001){
        PORTBbits.RB6 = 1; //LED ROJO
    }
    else if (RX_Trans == 0b00000010){
        PORTBbits.RB7 = 0; //LED AZUL
    }
    else if (RX_Trans == 0b00000011){
        PORTBbits.RB7 = 1; //LED AZUL
    }
    }
}


//******************************************************************************
//Funciones
//******************************************************************************

void setup(void) {
    ANSEL = 0;
    ANSELH = 0;
    TRISC = 0;
    TRISB = 0;
    TRISE = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    I2C_Master_Init(100000); // Inicializar Comuncación I2C
    Uart_conf();
}
void Enviar_temp(void){
    temp = temperature;
    dec_t = temp /1000;
    temp = temp - (dec_t*1000);
    uni_t = temp /100;
    temp = temp - (uni_t*100);
    deci_t = temp /10;
    temp = temp - (deci_t*10);
    centi_t = temp;
}

//******************************************************************************
//Interrupciones
//******************************************************************************

void __interrupt() isr(void) {
    if (PIR1bits.RCIF == 1) {
        RX_Trans = UART_get_char(); //Aqui recibimos el dato de la recepcion
        PIR1bits.RCIF = 0;
    }
}