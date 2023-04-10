/*******************************************************************************
Archivo:         Lab7
Dispositivo:     PIC16F887
Autor:           Sergio Vasquez
Compilador:      XC8, MPLABX 6.00
Creado:          10/04/2023
Ultima modificacion: 10/04/2023
*******************************************************************************/

//**************************
// Includes
//**************************
#include <xc.h> // Incluir el archivo de cabecera para el PIC16F887

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF       
#pragma config PWRTE = OFF      
#pragma config MCLRE = OFF      
#pragma config CP = OFF         
#pragma config CPD = OFF        
#pragma config BOREN = OFF      
#pragma config IESO = OFF       
#pragma config FCMEN = OFF      
#pragma config LVP = OFF        

// CONFIG2
#pragma config BOR4V = BOR40V   
#pragma config WRT = OFF        
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// Definición de frecuencia de oscilador y velocidad de transmisión serial
#define _XTAL_FREQ 8000000
#define BAUDRATE 9600

// Definicion de Variables
uint8_t  adc_value = 0;
char duty_cycle;

// Función de inicialización del módulo PWM
void initPWM() {
    TRISCbits.TRISC2 = 0;   // Configurar pin RC2 como salida (PWM)
    PR2 = 249;              // Configurar registro PR2 para generar frecuencia de PWM de 50Hz
    CCPR1L = 0x0F;          // Configurar registro CCPR1L para generar ciclo de trabajo inicial de 50%
    CCP1CONbits.P1M = 0;    // Configurar modo de salida PWM simple
    CCP1CONbits.CCP1M = 0b1100; // Configurar modo de trabajo PWM
    
    T2CONbits.T2CKPS = 0b11; // Configurar pre-escala de 1:16
    T2CONbits.TMR2ON = 1;    // Activar módulo de temporizador TMR2
}

void main() {
    ADCON1bits.ADFM  = 0;   //Justificado a la izquierda
    ADCON1bits.VCFG1 = 0;   
    ADCON1bits.VCFG0 = 0;
    
    TRISA = 0xFF;           // Configurar todos los pines del puerto A como entradas (potenciómetro)
    
    initPWM();              // Inicializar el módulo PWM
    
    while(1) {
        unsigned int adc_value; // Variable para almacenar el valor leído del potenciómetro
        
        ADCON0bits.CHS = 0b0000; // Seleccionar canal AN0 para la conversión analógica
        ADCON0bits.ADON = 1;    // Activar el módulo de conversión analógica
        
        ADCON0bits.GO_nDONE = 1; // Iniciar la conversión analógica
        while(ADCON0bits.GO_nDONE); // Esperar a que la conversión analógica termine
        
        adc_value = (ADRESH << 8) + ADRESL; // Leer el valor convertido del potenciómetro
        
        // Mapear el valor del potenciómetro al rango de ciclo de trabajo del PWM (0-255)
    unsigned char duty_cycle = (unsigned char)((adc_value * 255) / 1023);
    
    CCPR1L = duty_cycle; // Configurar el ciclo de trabajo del PWM
    
    __delay_ms(20); // Retardo para suavizar la respuesta del servo
}
    }