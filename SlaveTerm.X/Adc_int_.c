#include "Adc_int_.h"

void confADC(){
    //Reordar Siempre configurar en el main, el TRIS del puerto al que queramos 
    //en 1 es input, y tambien colocar el ANSEL, puerto analógico, "1" para esto
    ADCON0bits.ADCS0 = 1; //Selecciono el reloj conversion, F/8 , 1us para 8MHz
    ADCON0bits.ADCS1 = 0;
    ADCON1 = 0; //Set valores de referencia normales VCC y VDD, Justificado 
    //derecho, mas significativos en ADRESH, izq. 
    ADCON0bits.CHS = 0b0000; // Set del canal analógico, cambiar ese valor si
    // es necesario se utiliza actualmente el AN0 = RA0.
    ADCON0bits.ADON = 1; //habilita el ADC
    PIE1bits.ADIE = 1; //habilita la interrupcion del ADC
    PIR1bits.ADIF = 0; //Limpia bandera del ADC
    
      
}
void conf_ch(int sel){
    if (sel == 0){
        ADCON0bits.CHS = 0b0000;
    }
    if (sel == 1){
        ADCON0bits.CHS = 0b0001;
    }
    
}

