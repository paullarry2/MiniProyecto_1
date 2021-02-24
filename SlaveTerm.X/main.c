/*
 * File:   main.c
 * Author: Larry Paúl Fuentes
 * Carné 18117
 * Mini Proyecto 1
 * Slave Termometro
 * Digital 2
 *
 * Created on 22 de febrero de 2021, 6:45 PM
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
//#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma    config WRT = OFF 

#include <xc.h>
#include "Adc_int_.h"
#include "SPI.h"
#define _XTAL_FREQ (8000000)
#define led_r RD2
#define led_a RD1
#define led_g RD0

//******************************************************************************
//Variables
//******************************************************************************
int term;
int adc_fin;


//******************************************************************************
//Prototipos
//******************************************************************************
void conf_but(void);

//******************************************************************************
//Main
//******************************************************************************

void main(void) {
    conf_but();
    adc_fin=0;
    confADC();
    term = 0;
    conf_ch(0);
    
    while(1){
        if (adc_fin == 0) {
            adc_fin = 1;
            __delay_ms(10); // Acquisition time
            ADCON0bits.GO = 1; // Enciende la conversion   
        }  
        
        if (term < 100){
            led_g = 1;
            led_a = 0;
            led_r = 0;
        }
        else if (100 < term & term < 114){
            led_a = 1;
            led_g = 0;
            led_r = 0;
        }
        else if (114 < term){
            led_a = 0;
            led_g = 0;
            led_r = 1;
        }
    }
}


//******************************************************************************
//FUNCIONES
//******************************************************************************
void conf_but(void){
    // CONFIGURACION PUERTOS
    INTCONbits.GIE = 1; //Habilito mis interrupciones
    INTCONbits.PEIE = 1; //Habilita interrupciones perifericas 
    ANSEL = 0;// Indicar que el ansel y el anselh esten en 0, (digirales)
    ANSELH = 0;
    ANSELbits.ANS0 = 1; //Excepto el pin AN0 (Term)
    TRISB=0x00; //Pone los puertos como outputs, en b los prim 2 pin input
    TRISD=0x00;
    TRISE=0x00;
    TRISA=0;
    TRISC = 0;
    TRISC4 = 1;
    TRISAbits.TRISA0 = 1;//habilita como entrada el puerto analogico (pot)
    PORTD = 0;
    PORTB = 0;
    PORTC = 0;
    PORTE = 0;
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    //Configuracion de mi slave,  SSPSTAT IGUAL A 0, habilitado el SSPEN, CKP = 0
}  
//******************************************************************************
//Interrupcion
//******************************************************************************

void __interrupt() ISR(void) {//Interrupciones

    if (PIR1bits.ADIF == 1) { //Termino de convevrtir?
        //Chequea la bandera del ADC
        term = ADRESH; //Copia el valor de la conversion al puerto C
        adc_fin = 0; //Apagar bandera de copiando 

    }
    PIR1bits.ADIF = 0; //Apagar bandera de conversion
    
    if (SSPIF == 1) {
        spiWrite(term);
        SSPIF = 0;
    }
}