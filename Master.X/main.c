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
uint8_t temp;

//******************************************************************************
//Prototipos
//******************************************************************************
void setup(void);
uint8_t get_spi(unsigned SS);

//******************************************************************************
//Main
//******************************************************************************

void main(void) {
    setup();
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
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    S_Pot = 1;
    S_Cont = 1;
    S_Term = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}

uint8_t get_spi(unsigned SS){
        SS = 0; //Slave Select
        __delay_ms(1);
        spiWrite(1);
        temp = spiRead();
        __delay_ms(1);
        SS = 1; //Slave Deselect  
        return (temp);
}
