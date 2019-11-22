/*
 * File:   3Practica_Teclado3.c
 * Author: Aftak
 *
 * Created on 20 de noviembre de 2019, 10:07 PM
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE =ON      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
#include <xc.h>
#include <string.h>
#define _XTAL_FREQ 8000000
#define KEY_PORT PORTB //estable el puerto que sera usado por el teclado
#define TRIS_KEY_PORT TRISB //usado para configurar entradas y salidas del puerto

unsigned char contador=0;
int check=0;
unsigned char password[4];
unsigned const char unlock[4]={'8','8','8','8'};     // El arreglo es la contraseña
unsigned char Teclado_LeeOrdenTecla(void);
unsigned char Teclado_LeeHex(void);
void Teclado_EsperaDejePulsar(void);

unsigned const char Tabla_Caracter[16] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'
};

unsigned char pass[4];
unsigned char passing[4];
int aux=0;
unsigned char ver=0;

unsigned char Teclado_LeeOrdenTecla(void)
{
    unsigned char Bit_Rotacion = 0b11111110;
    unsigned char Tecla_Orden = 0;
    while (1)
    {
        KEY_PORT = Bit_Rotacion;
        if (!PORTBbits.RB4)
            break;
        Tecla_Orden++;
        if (!PORTBbits.RB5)
            break;
        Tecla_Orden++;
        if (!PORTBbits.RB6)
            break;
        Tecla_Orden++;
        if (!PORTBbits.RB7)
            break;
        Tecla_Orden++;
        if (Tecla_Orden < 16)
        {
            Bit_Rotacion = (Bit_Rotacion << 1) | 0x01;
            continue;
        }
    }
    return Tecla_Orden;
}
unsigned char Teclado_LeeHex(void)
{
    unsigned char OrdenTecla;
    OrdenTecla = Teclado_LeeOrdenTecla();
    return Tabla_Caracter[OrdenTecla];
}
void Teclado_EsperaDejePulsar(void)
{
    const unsigned char Puerto_init = 0b11110000; //este es el valor que debe tener el KEY_PORT cuando no se presione boton alguno.
    unsigned char a;
    KEY_PORT = Puerto_init;
    while (1)
    {
        __delay_ms(20);
        a = KEY_PORT;
        if (Puerto_init == a)
            break;
    }
}



void __interrupt() teclado(void)
{
    //PORTAbits.RA0=1;
    char caracter;
    caracter=Teclado_LeeHex();
    Teclado_EsperaDejePulsar();
    pass[aux]=caracter;
    if(aux==3){
        for(int i=0;i<3;i++){
            if(pass[i] == unlock[i]){
            check++;
            }
        }
        if(check==3){
            PORTAbits.RA0=1;
            PORTAbits.RA1=0;
            aux=-1;
            check=0;
        }else{
            PORTAbits.RA0=0;
            PORTAbits.RA1=1;
            aux=-1;
            check=0;
        }
    }

    aux++;
    INTCONbits.RBIF=0;
}


void main(void){
    OSCCON=0xff;
    ANSEL= 0;
    ANSELH = 0;
    KEY_PORT = 0x00;
    PORTA=0x00;
    TRIS_KEY_PORT = 0b11110000;
    TRISA=0x00;
    OPTION_REGbits.nRBPU = 0;
    INTCON = 0b10001000; 
    Teclado_EsperaDejePulsar();
    IOCB = 0b11110000; 
     while(1)
        SLEEP();
}