/* 
 * File:   Adc_int.h
 * Author: Larry Paúl Fuentes
 * Carné 18117
 * Digital 2
 * Lab2
 * Created on 7 de febrero de 2021, 01:19 PM
 */

#ifndef __LCD_H_
#define	__LCD_H_

#include <xc.h>
#include <stdint.h>
//******************************************************************************
//Llamo Funciones
//******************************************************************************
void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear();
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init();
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Shift_Right();
void Lcd_Shift_Left();

#endif	/* XC_HEADER_TEMPLATE_H */


