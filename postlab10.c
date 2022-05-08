/*
 * File:   prelab10.c
 * Author: Carlos
 *
 * Created on 4 de mayo de 2022, 01:24 PM
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <stdint.h>
#include <xc.h>
//CONSTANTES
#define _XTAL_FREQ 1000000
#define LEN_MSG 9
//VARIABLES
uint8_t leerp = 0;
uint8_t sel = 0;
uint8_t indice = 0;
uint8_t banderas = 1;
uint8_t envc = 0;
//PROTOTIPO DE FUNCIONES
void setup(void);
void cuerdas(char word[]);

//INTERRUPCIONES
void __interrupt() isr (void){
   
    if(PIR1bits.RCIF){
        sel = RCREG;
        switch (sel){
        case (0x31):
            cuerdas("\r El valor del potenciometro es: \r");
            TXREG = ADRESH;
            banderas = 1;
            break;
        case(0x32):
            cuerdas("\r Ingrese el caracter que quiera enviar en ASCII: \r");
            PORTB = 0;
            RCREG = 0;
            envc = 1;
            while (envc == 1){
                PORTB = RCREG;
                if (PORTB != 0){
                    envc = 0;
                }
            }
            PIR1bits.RCIF = 0;
            banderas = 1;
            break;
        default:
            cuerdas("\r ERROR: \r Solamente se puede ingresar el valor 1 o 2, vuelva a intentar \r");
                    }  
    }
    /*
    if(PIR1bits.ADIF){
        if(ADCON0bits.CHS == 0){
            while (leerp == 1){
            TXREG = ADRESH;
            leerp = 0;
            }
            PIR1bits.ADIF = 0;
        }
    }
    */
    return;
}
void setup(void){
    ANSEL = 0b00000001;
    ANSELH = 0;
    TRISA = 0b00000001;
    TRISB = 0;
    PORTB = 0;
    PORTA = 0;
    
    OSCCONbits.IRCF = 0b100;
    OSCCONbits.SCS = 1;
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 25;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;
    TXSTAbits.TX9 = 0;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIR1bits.ADIF = 0;
    PIE1bits.RCIE = 1;
    PIE1bits.ADIE = 1;
  
  
    ADCON0bits.ADCS = 0b01;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON0bits.CHS = 0b0000;
    ADCON1bits.ADFM = 0;
    ADCON0bits.ADON = 1;
    __delay_us(40);
 
    return;
    
}
//CICLO PRINCIPAL
void main(void) {
    setup();
    while(1){
        if (banderas == 1){
            cuerdas("\r Hola, elija una de las siguientes opciones (Escriba 1 o 2): \r");
            cuerdas("\r 1. Leer potenciometro \r");
            cuerdas("\r 2. Enviar ASCII \r ");
            banderas = 0;
            
        }
        if(ADCON0bits.GO == 0){             // No hay proceso de conversion
            ADCON0bits.GO = 1;              // Iniciamos proceso de conversi n?
        }
    }
    return;
}



void cuerdas (char word[]){
    indice = 0;
    while (word[indice] != '\0'){
   
        TXREG = word[indice];
        indice++;
        __delay_ms(200);
        
    }
    return;
}
