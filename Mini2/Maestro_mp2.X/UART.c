#include "UART.h"

void Uart_conf() {


    //TRANSMISION
    TXSTAbits.TXEN = 1; //Habilitamos la transmision.
    TXSTAbits.SYNC = 0; //Configura como asincrona
    RCSTAbits.SPEN = 1; //Habilita el Eusart 
    TRISCbits.TRISC6 = 0; // Puerto RC6 como salida
    TXSTAbits.TX9 = 0; // No usaremos los 9 bits solo 8
    PIE1bits.TXIE = 0;


    //RECEPCION
    RCSTAbits.CREN = 1; // Habilitamos la recepcion
    PIE1bits.RCIE = 1; //Se habilita la interrupcion de la recepcion
    RCSTAbits.RX9 = 0; // 8Bits
    TRISCbits.TRISC7 = 1; //Set as input

    //BAUD RATE
    
    SPBRG = (_XTAL_FREQ/(16 * 9600)) - 1;    
    SPBRGH = 0;
    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 0;


}

void Uart_send_char(char bt) {
    while (!TXIF); // Mientras mi bandera no se enciende, while inf
    TXREG = bt; // Cuando listo, subir el dato
}

void UART_send_string(char* st_pt) {
    while (*st_pt) // Verificar puntero, si almacena valor, sigue en while
        Uart_send_char(*st_pt++); // Envio cada char por separado
}

char UART_get_char() {
    if (RCSTAbits.OERR == 1) // Chequear errores en UART.
    {
        RCSTAbits.CREN = 0; // En caso de error reseteamos la comunicación. 
        RCSTAbits.CREN = 1;
    }

    while (!PIR1bits.RCIF); // While hasta que la bandera de mi interrupcion encendida 
    return RCREG; // 

}
