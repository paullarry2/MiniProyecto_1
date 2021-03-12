 /*
 * File            : UART
 * Author          : Larry Paúl Fuentes
 * Project         : MiniPeoject2
 * Microcontroller : PIC 16F877A
 * Created on 10/03/2021
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#define __UART_H_
#define	__UART_H_

#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic16f887.h>
#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

void Uart_conf();
void Uart_send_char(char bt);
void UART_send_string(char* st_pt);
char UART_get_char();


