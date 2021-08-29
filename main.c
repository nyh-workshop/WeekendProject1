#include <string.h>

volatile unsigned char column = 0;

unsigned int randomData = 28888;

// Wider ":D" emoticon:
const unsigned char emoticon_0[8] = { 0b11000111, 
                            0b10110101,
                            0b10110111,
                            0b10110101,
                            0b11000111 };
                            
// Smaller ":D" emoticon:                            
const unsigned char emoticon_1[8] = { 0b11100111, 
                            0b11010101,
                            0b11010111,
                            0b11010101,
                            0b11100111 };
                            
// ":o" emoticon:
const unsigned char emoticon_2[8] = { 0b11001111, 
                            0b10110101,
                            0b10110111,
                            0b10110101,
                            0b11001111 };

volatile unsigned char frameActive = 0;
volatile unsigned char* buffer;
unsigned char buffer0[8];
volatile unsigned int count = 0;

void Interrupt() iv 0x0004 ics ICS_AUTO
{
 if (TMR0IF_bit)
  {
    TMR0 = 0x06;
    TMR0IF_bit = 0;
        
    if(column == 0) {
        LATB.LATB5 = 1;
        LATC = buffer[column];
        column++;
        frameActive = 1;
        LATB.LATB4 = 0;
        return;
    } else if(column == 1) {
        LATB.LATB5 = 0;
        LATB.LATB6 = 1;
        LATC = buffer[column];
        column++;
        frameActive = 1;
        return;
    } else if(column == 2) {
        LATB.LATB6 = 0;
        LATB.LATB7 = 1;
        LATC = buffer[column];
        column++;
        frameActive = 1;
        return;
    } else if(column == 3) {
        LATB.LATB7 = 0;
        LATA.LATA4 = 1;
        LATC = buffer[column];
        column++;
        frameActive = 1;
        return;
    } else if(column == 4) {
        LATA.LATA4 = 0;
        LATA.LATA5 = 1;
        LATC = buffer[column];
        column++;
        frameActive = 1;
        return;
    } else {
        LATA.LATA5 = 0;
        column = 0;
        frameActive = 0;
        LATB.LATB4 = 1;
    }  
  }
}

void InitTimer0() 
{    
    TMR0IF_bit = 0;
    INTCON = 0xA0;
    OPTION_REG = 0b00000100;
    INTCON = 0xA0;
    TMR0 = 6;
}

void InitTimer2()
{
    TMR2 = 0x00;
    TMR2IF_bit = 0;
    T2CON = 0b00000111;
    PR2 = 125;
    PIE1 = 0b00000010;
    INTCON = 0xC0;
}

void init() {
    // Internal oscillator 32MHz, PLL enabled.
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    OSCCON = 0b11110000;
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x00;
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
}

void displayFrame(unsigned char* buffer, unsigned char* bitmap, unsigned int timeIn5ms) {
    unsigned int i = 0;
    unsigned char once = 0;
    
    for(i = 0; i < timeIn5ms; i++) {
           while(!frameActive); 
            if(!once)
                memcpy(buffer,bitmap, sizeof(emoticon_0));
            else once = 1;
           while(frameActive);
        }
}

unsigned int generateRandom(unsigned int inpNumber) {
    unsigned char tempVal1 = 0;
    unsigned char tempVal2 = 0;
        
    // data = (data << 1) + (data.15 ^ data.12) 
    // (Myke Predko's Programming and Customizing the PIC Microcontroller, p.891)
    tempVal1 = (inpNumber & (1 << 15)) >> 15;
    tempVal2 = (inpNumber & (1 << 12)) >> 12;
    inpNumber = (inpNumber << 1) + (tempVal1 ^ tempVal2);

    return inpNumber;
}

void swap(unsigned char* a, unsigned char* b) {
    unsigned char temp = *a;
    *a = *b;
    *b = temp;
}

void shuffleArray(unsigned char* arr, unsigned char n) {
    // https://bits.mdminhazulhaque.io/c/shuffle-items-of-array-in-c.html
    unsigned char i;
    for(i = n-1; i > 0; i--) {
        int j = randomData % (i+1);
        swap(&arr[i], &arr[j]);
    }
}

void randomBitmapFrame(unsigned char* buffer, unsigned int timeIn5ms) {
    unsigned char i = 0;
    unsigned char bitmap[8];
    
    for(i = 0; i < 8; i++) {
        randomData = generateRandom(randomData);
        bitmap[i] = (unsigned char)(randomData >> 8);        
    }
    
    shuffleArray(bitmap, sizeof(bitmap));
    
    displayFrame(buffer, bitmap, timeIn5ms);    
}

void main(void)
{
    /* Replace with your application code */
    init();
    // Fill buffer0 first:
    memset(buffer0, 0xff, sizeof(buffer0));
    buffer = buffer0;
    
    InitTimer0();        
    // R1-R7 = PORTC
    // C1 = RB5
    // C2 = RB6
    // C3 = RB7
    // C4 = RA4
    // C5 = RA5    
    while (1)
    {
       //displayFrame(buffer, (unsigned char*)emoticon_0, 10);
       //displayFrame(buffer, (unsigned char*)emoticon_1, 10);
       randomBitmapFrame(buffer, 10);
    }
}
