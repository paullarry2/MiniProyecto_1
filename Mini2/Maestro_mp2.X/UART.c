#include "UART.h"

void UART_config() {


    //Para la TRANSMISION
    TXSTAbits.TXEN = 1; //Habilitamos la transmision - La bandera TXIF se pone 1 auto.
    TXSTAbits.SYNC = 0; //Configura EUSART para operacion asincrona
    RCSTAbits.SPEN = 1; //Habilita el EUSART Y PONE TX/CK I/O pin como salida
    TRISCbits.TRISC6 = 0; //Lo ponemos como salida para asegurar
    TXSTAbits.TX9 = 0; // No usaremos los 9 bits solo 8
    PIE1bits.TXIE = 0;


    //Para la RECEPCION

    RCSTAbits.CREN = 1; // Habilitamos la recepcion
    PIE1bits.RCIE = 1; //Se habilita la interrupcion de la recepcion
    RCSTAbits.RX9 = 0; // No usaremos los 9 bits, solo 8
    TRISCbits.TRISC7 = 1; //Lo ponemos como entrada para asegurar

    //BAUD RATE
    // SPBRG = ((_XTAL_FREQ / 16) / Baud_rate) - 1;
    //  SPBRG = (_XTAL_FREQ/(16 * Baud_rate)) - 1;    
    SPBRG = 51;
    SPBRGH = 0;

    //   SPBRG=2.25  ;//funciono maso

    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 0;


}

void UART_send_char(char bt) {
    while (!TXIF); // ESPERAMOS HASTA QUE ESTE LIBRE LA BANDERA
    TXREG = bt; //YA QUE ESTA LIBRE CARGAMOS EL DATO
}

void UART_send_string(char* st_pt) {
    while (*st_pt) //SI HAY ALGUN CHAR
        UART_send_char(*st_pt++); //Lo procesamos como un dato
}

char UART_get_char() {
    if (RCSTAbits.OERR == 1) //Vemos si hay error
    {
        RCSTAbits.CREN = 0; //Si lo hay reseteamos la recepcion
        RCSTAbits.CREN = 1;
    }

    while (!PIR1bits.RCIF); // esperamos hasta que la bandera este libre
    return RCREG; //amos el dato de retorno

}
