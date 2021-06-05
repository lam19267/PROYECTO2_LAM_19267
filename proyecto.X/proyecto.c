/*
 * File:   proyecto.c
 * Author: gelp0
 *
 * Created on 21 de mayo de 2021, 04:49 PM
 */


//-------------------------- Bits de configuraci”n -----------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT            // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF                       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF                      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF                      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF                         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF                        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF                      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF                       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = ON                       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON                         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V                   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF                        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ 8000000
#include <stdint.h>
#include <xc.h>
#include <stdio.h> 

//------------------------------ Variables ------------------------------------- 
void setup(void);                          
void bitb1(void);
void bitb2(void);
void bitb3(void);
void putch(char data);   
void text(void);
char val;
int val1;
int val2;
int val3;
int val4;
int val5;
int lectura1;
int lectura2;
int lectura3;
int lectura4;
int lectura5;
int lectura6;
int dato;
int direccion;
void write_eeprom (int dato, int direccion);
char read_eeprom (int direccion);


//--------------------------- Interrupciones -----------------------------------
void __interrupt() isr(void){ //interrupciones
    
    if(PIR1bits.ADIF == 1)
       {
        if(ADCON0bits.CHS == 0){
            CCPR1L = (ADRESH >> 1) + 124;
            val1 = ADRESH;
        }    
        else if(ADCON0bits.CHS == 1){ 
            CCPR2L = (ADRESH >> 1) + 124;
            val2 = ADRESH;
        }
        else if (ADCON0bits.CHS == 2)
        {
               val = ADRESH;
            if (val <= 85){
                bitb1();
                 }
           if ((val <= 170)&&(val >= 86)){
                bitb2();
                 }
            if (val >= 171){
                bitb3();
                 }
        }   
           PIR1bits.ADIF = 0;
           
    }
    
    if (RCIF == 1) {
    if (RCREG == 'w'){ 
        __delay_ms(500);
        printf("\r Carro prendido \r");
        PORTDbits.RD0 = 1;
        PORTEbits.RE0 = 1;
        PORTEbits.RE1 = 0;
        printf("\r Apache s para apagar \r");
        printf("\r Apache a para girar izquierda \r");
        printf("\r Apache d para girar derecha \r");}
    if (RCREG == 's'){
        PORTDbits.RD0 = 0;
        PORTEbits.RE0 = 0;
        PORTEbits.RE1 = 1;
        printf("\r Se apago el motor \r");
        printf("\r Apache w para prender el motor \r");
        printf("\r Apache a para girar izquierda \r");
        printf("\r Apache d para girar derecha \r");}
    if (RCREG == 'a'){
        bitb1();
        printf("\r Girando izquierda \r");
        __delay_ms(100);
        printf("\r Apache s apagar el motor \r");
        printf("\r Apache a para girar izquierda \r");
        printf("\r Apache d para girar derecha \r");}
    if (RCREG == 'd'){
        bitb3();
        printf("\r Girando derecha \r");
        __delay_ms(100);
        printf("\r Apache s apagar el motor \r");
        printf("\r Apache a para girar izquierda \r");
        printf("\r Apache d para girar derecha \r");}
    if (RCREG == 'q'){
            write_eeprom (val,  0x17); // Aqui grabo la variable y la direccion
            write_eeprom (val1, 0x18); // de los rangos de bitbanging
            write_eeprom (val2, 0x19);
            write_eeprom (val3, 0x20); // Estos son lo pwm
            write_eeprom (val4, 0x21); // no guardar valores de +-90
            write_eeprom (val5, 0x22);
            __delay_ms(3000);
            printf("\r Grabando \r");}
    if (RCREG == 'e'){
            lectura1 = read_eeprom (0x17);    // Leo los valores guardados
            lectura2 = read_eeprom (0x18);
            lectura3 = read_eeprom (0x19);
            lectura4 = read_eeprom (0x20);
            lectura5 = read_eeprom (0x21);
            lectura6 = read_eeprom (0x22);
            __delay_ms(3000);
            printf("\r Cargando \r");
            CCPR1L = (lectura2>>1)+124;
            CCPR2L = (lectura3>>1)+124;
            PORTEbits.RE0 = lectura4;
            PORTEbits.RE1 = lectura5;
            PORTDbits.RD0 = lectura6;
    
    }
    else{ 
        NULL;}  
        }
    
    
        
    
    

if (RBIF == 1)  
    {
        if (PORTBbits.RB0 == 0)             
        {
            PORTDbits.RD0 = 1;    
            PORTEbits.RE0 = 1;
            PORTEbits.RE1 = 0;
            val3 = PORTEbits.RE0;
            val4 = PORTEbits.RE1;
            val5 = PORTDbits.RD0;
            
        }
        else if  (PORTBbits.RB1 == 0)           
        {    
            PORTDbits.RD0 = 0;   
            PORTEbits.RE0= 0;
            PORTEbits.RE1= 1;
            val3 = PORTEbits.RE0;
            val4 = PORTEbits.RE1;
            val5 = PORTDbits.RD0;
        }
        INTCONbits.RBIF = 0;                
        
    }
}
void main(void) 
{
    
    setup();                                
    ADCON0bits.GO = 1;                      
    PORTEbits.RE1 = 1;
    printf("\r Apache w para prender motor \r");
    while(1)  
    {
        
        if (ADCON0bits.GO == 0)
        {
            if(ADCON0bits.CHS == 0){
                ADCON0bits.CHS =1;
            }
            else if (ADCON0bits.CHS == 1)
            {
                ADCON0bits.CHS = 2;
            }
            else
                ADCON0bits.CHS = 0;
            __delay_us(50);
            ADCON0bits.GO = 1;
        } 
        

    }}

void putch(char data){    
    while(TXIF == 0);
    TXREG = data; 
    return;
}

//----------------------------- SUB-RUTINAS ------------------------------------
// Sub-rutina de configuraciones generales
void setup(void){
    
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;                  
    OSCCONbits.SCS = 1;
    
    
    ANSEL = 0b00000111;
    ANSELH = 0;
    
    
    TRISA = 0b00000111;
    
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    
    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB5 = 1;

    
    
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;
    
    
    
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    
    
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    
    
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    
    OPTION_REGbits.nRBPU = 0;
    WPUB = 0b00000011;
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    
        
    ADCON1bits.ADFM = 0;                    
    ADCON1bits.VCFG0 = 0;                    
    ADCON1bits.VCFG1 = 0;   
    
    ADCON0bits.ADCS = 0b10;                 
    ADCON0bits.ADON = 1;                    
    ADCON0bits.CHS = 0;                  
    __delay_us(50);
    
    
    TRISCbits.TRISC2 = 1;                   
    TRISCbits.TRISC1 = 1; 
    PR2 = 249;                              
    CCP1CONbits.P1M = 0;                    
    CCP1CONbits.CCP1M = 0b1100;
    CCPR1L = 0x0f;                          
    CCP2CONbits.CCP2M = 0b1100;
    CCPR2L = 0x0f;
    
    CCP1CONbits.DC1B = 0;
    CCP2CONbits.DC2B0 = 0;
    CCP2CONbits.DC2B1 = 0;
    
    PIR1bits.TMR2IF = 0;                    
    T2CONbits.T2CKPS = 0b11;                
    T2CONbits.TMR2ON = 1;
    
    while(PIR1bits.TMR2IF == 0);            
    PIR1bits.TMR2IF = 0;
    TRISCbits.TRISC2 = 0;                   
    TRISCbits.TRISC1 = 0;                   
    
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;                    
    PIE1bits.ADIE = 1;                      
    PIR1bits.ADIF = 0;                      
    INTCONbits.RBIF = 1;
    INTCONbits.RBIE = 1;
    
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 210;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
    TXSTAbits.TXEN = 1;
    
    PIR1bits.RCIF = 0;  
    PIR1bits.TXIF = 0;  

}
    
    void bitb1 (void)
    {
        PORTCbits.RC3 = 1;
        __delay_ms(1);
        PORTCbits.RC3 = 0;
        __delay_ms(19);
    }
    
    void bitb2 (void)
    {
        PORTCbits.RC3 = 1;
        __delay_ms(1.5);
        PORTCbits.RC3 = 0;
        __delay_ms(18.5);
    }

    void bitb3 (void)
    {
        PORTCbits.RC3 = 1;
        __delay_ms(2);
        PORTCbits.RC3 = 0;
        __delay_ms(18);
    }
    
    
    void write_eeprom (int dato, int direccion){
    EEADR = direccion;      
    EEDAT = dato;           
    
    
    INTCONbits.GIE = 0;     
    
    EECON1bits.EEPGD = 0;   
    EECON1bits.WREN = 1;    
    
     
    EECON2 = 0x55;          
    EECON1bits.WREN = 1;    
    
     
    EECON2 = 0x55;          
    EECON1bits.WREN = 1;    
    
     
    EECON2 = 0x55;          
    EECON2 = 0xAA;          
    
    EECON1bits.WR = 1;      
    
    while(PIR2bits.EEIF == 0);
    PIR2bits.EEIF = 0;
    
    EECON1bits.WREN = 0;    
    INTCONbits.GIE = 1;
}

char read_eeprom (int direccion){
    EEADR = direccion;     
    EECON1bits.EEPGD = 0;  
    EECON1bits.RD = 1;     
    int dato = EEDATA;     
    return dato;           
}
    
    

    


