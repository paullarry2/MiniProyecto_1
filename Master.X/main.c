/*
 * File:   main.c
 * Author: Larry Paúl Fuentes
 * Carné 18117
 * Mini Proyecto 1
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
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
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
#include "SPI.h"
#include "lcd.h"
#define S_Pot PORTBbits.RB0
#define S_Cont PORTBbits.RB1
#define S_Term PORTBbits.RB2

#define _XTAL_FREQ (8000000)

//******************************************************************************
//Variables
//******************************************************************************
uint8_t pot;
uint8_t termometro;
uint8_t contador;

uint8_t uniPV;
uint8_t decPV;
uint8_t cenPV;
uint8_t uniTV;
uint8_t decTV;
uint8_t cenTV;
uint8_t uniCV;
uint8_t decCV;
uint8_t cenCV;
uint16_t temp;

//******************************************************************************
//Prototipos
//******************************************************************************
void setup(void);
void impr_cont(char uni, char dec, char cen, uint8_t val, int fact);
void impr_Pot(char uni, char dec, char cen, uint8_t val, int fact);

//******************************************************************************
//Main
//******************************************************************************

void main(void) {
    setup();
    Lcd_Init();
    Lcd_Clear();
    while (1) {

        S_Pot = 0; //Slave Select
        __delay_ms(5);
        spiWrite(1);
        pot = spiRead();
        __delay_ms(5);
        S_Pot = 1; //Slave Deselect  

        S_Cont = 0; //Slave Select
        __delay_ms(5);
        spiWrite(1);
        contador = spiRead();
        __delay_ms(5);
        S_Cont = 1; //Slave Deselect   

        S_Term = 0; //Slave Select
        __delay_ms(5);
        spiWrite(1);
        termometro = spiRead();
        __delay_ms(5);
        S_Term = 1; //Slave Deselect  

        //Pasos para creacion del display principal
        Lcd_Set_Cursor(1, 1);
        Lcd_Write_String("S1:");
        Lcd_Set_Cursor(1, 7);
        Lcd_Write_String("S2:");
        Lcd_Set_Cursor(1, 13);
        Lcd_Write_String("S3:");

        impr_Pot(uniPV, decPV, cenPV, pot, 2);
        impr_cont(uniCV, decCV, cenCV, contador, 1);

        if (termometro < 69) {
            temp = termometro * 0.81;
            temp = 55 - temp;
            temp = temp * 10;
            uniTV = temp / 100;
            temp = temp - (uniTV * 100);
            decTV = temp / 10;
            temp = temp - (decTV * 10);
            cenTV = temp;
            
            
            Lcd_Set_Cursor(2, 12);
            Lcd_Write_String("-");
            
            Lcd_Set_Cursor(2, 13);
            uniTV = uniTV + 48;
            Lcd_Write_Char(uniTV);

            Lcd_Set_Cursor(2, 14);
            decTV = decTV + 48;
            Lcd_Write_Char(decTV);

            Lcd_Set_Cursor(2, 15);
            Lcd_Write_String(".");
            
            Lcd_Set_Cursor(2, 16);
            cenTV = cenTV + 48;
            Lcd_Write_Char(cenTV);
        }
        else  {
            temp = termometro * 0.81;
            temp = temp - 55;
            temp = temp*10;
            uniTV = temp / 100;
            temp = temp - (uniTV * 100);
            decTV = temp / 10;
            temp = temp - (decTV * 10);
            cenTV = temp;
            
            Lcd_Set_Cursor(2, 12);
            Lcd_Write_String("+");
            
            Lcd_Set_Cursor(2, 13);
            uniTV = uniTV + 48;
            Lcd_Write_Char(uniTV);

            Lcd_Set_Cursor(2, 14);
            decTV = decTV + 48;
            Lcd_Write_Char(decTV);

            Lcd_Set_Cursor(2, 15);
            Lcd_Write_String(".");
            
            Lcd_Set_Cursor(2, 16);
            cenTV = cenTV + 48;
            Lcd_Write_Char(cenTV);

        }
    }
}


//******************************************************************************
//FUNCIONES
//******************************************************************************

void setup(void) {
    ANSEL = 0;
    ANSELH = 0;
    TRISC = 0;
    TRISC4 = 1;
    TRISB = 0;
    TRISE = 0x00;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    S_Pot = 1;
    S_Cont = 1;
    S_Term = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}

void impr_cont(char uni, char dec, char cen, uint8_t val, int fact) {
    temp = val * fact;
    uni = temp / 100;
    temp = temp - (uni * 100);
    dec = temp / 10;
    temp = temp - (dec * 10);
    cen = temp;


    Lcd_Set_Cursor(2, 7);
    uni = uni + 48;
    Lcd_Write_Char(uni);

    Lcd_Set_Cursor(2, 8);
    dec = dec + 48;
    Lcd_Write_Char(dec);

    Lcd_Set_Cursor(2, 9);
    cen = cen + 48;
    Lcd_Write_Char(cen);
}

void impr_Pot(char uni, char dec, char cen, uint8_t val, int fact) {
    temp = val * fact;
    uni = temp / 100;
    temp = temp - (uni * 100);
    dec = temp / 10;
    temp = temp - (dec * 10);
    cen = temp;


    Lcd_Set_Cursor(2, 1);
    uni = uni + 48;
    Lcd_Write_Char(uni);

    Lcd_Set_Cursor(2, 2);
    Lcd_Write_String(":");

    Lcd_Set_Cursor(2, 3);
    dec = dec + 48;
    Lcd_Write_Char(dec);

    Lcd_Set_Cursor(2, 4);
    cen = cen + 48;
    Lcd_Write_Char(cen);
}