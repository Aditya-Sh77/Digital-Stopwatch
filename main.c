#include <xc.h>

#define _XTAL_FREQ 16000000        // Define crystal oscillator frequency

#pragma config OSC = HS, LVP = OFF, WDT = OFF  // Configuration bits

// Define Ports and Pins
#define ldata PORTC                // Data lines connected to PORTC (RC7-RC4 for D7-D4)
#define RS RB3                     // RS pin connected to RB3
#define EN RB2                     // Enable pin connected to RB2

short i, fr1, fr2, sec1, sec2, mn1, mn2, hr1, hr2;
// Function Prototypes
void LCD_Init(void);
void LCD_Command(unsigned char cmd);
void LCD_Char(unsigned char dat);
void LCD_String(const char *msg);
void LCD_String_xy(char row, char pos, const char *msg);
void LCD_Clear(void);

// LCD Initialization Function
void LCD_Init() {
    ldata = 0;                   // Clear PORTC
    TRISC = 0x00;                // Set PORTC as output (RC7-RC4 for data)
    TRISBbits.TRISB2 = 0;        // Set RB2 as output (Enable pin)
    TRISBbits.TRISB3 = 0;        // Set RB3 as output (RS pin)

    __delay_ms(15);              // 15ms power-on delay
    LCD_Command(0x02);           // Initialize in 4-bit mode
    LCD_Command(0x28);           // 2-line, 5x8 matrix in 4-bit mode
    LCD_Command(0x01);           // Clear display
    LCD_Command(0x0C);           // Display ON, cursor OFF
    LCD_Command(0x06);           // Increment cursor to the right
}

// Function to Send Command to LCD
void LCD_Command(unsigned char cmd) {
    ldata = (ldata & 0x0F) | (cmd & 0xF0);  // Send higher nibble
    RS = 0;                                 // RS = 0 for command
    EN = 1;                                 // High-to-low pulse on Enable
    __delay_us(1);
    EN = 0;
    __delay_us(100);

    ldata = (ldata & 0x0F) | (cmd << 4);    // Send lower nibble
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_ms(2);                          // Command execution delay
}

// Function to Send Character to LCD
void LCD_Char(unsigned char dat) {
    ldata = (ldata & 0x0F) | (dat & 0xF0);  // Send higher nibble
    RS = 1;                                 // RS = 1 for data
    EN = 1;                                 // High-to-low pulse on Enable
    __delay_us(1);
    EN = 0;
    __delay_us(100);

    ldata = (ldata & 0x0F) | (dat << 4);    // Send lower nibble
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_ms(2);                          // Data execution delay
}

// Function to Print a String
void LCD_String(const char *msg) {
    while (*msg) {
        LCD_Char(*msg);                     // Send each character
        msg++;
    }
}

// Function to Print String at Specific Position
void LCD_String_xy(char row, char pos, const char *msg) {
    char location = 0;
    if (row == 0) {
        location = (0x80) | (pos & 0x0F);   // 1st row
    } else if (row == 1) {
        location = (0xC0) | (pos & 0x0F);   // 2nd row
    }
    LCD_Command(location);                  // Set cursor position
    LCD_String(msg);
}

// Function to Clear LCD Display
void LCD_Clear() {
    LCD_Command(0x01);                      // Clear display
    __delay_ms(2);                          // Delay for command execution
}


void stop(){
    LCD_Clear();
    while(1){
            LCD_String_xy(0, 0, "Stopped..."); // Adjusted row and column indices

            LCD_String_xy(1, 7, ":");
            LCD_String_xy(1, 10, ":");
            LCD_String_xy(1, 13, ":");
            LCD_String_xy(1, 14, (char[]){fr2, '\0'});
            LCD_String_xy(1, 15, (char[]){fr1, '\0'});
            LCD_String_xy(1, 12, (char[]){sec1, '\0'});
            LCD_String_xy(1, 11, (char[]){sec2, '\0'});
            LCD_String_xy(1, 9, (char[]){mn1, '\0'});
            LCD_String_xy(1, 8, (char[]){mn2, '\0'});
            LCD_String_xy(1, 6, (char[]){hr1, '\0'});
            LCD_String_xy(1, 5, (char[]){hr2, '\0'});
    }
}
void start() {
    LCD_Command(0x01);  // Clear LCD
    LCD_String_xy(0, 0, "Running..."); // Adjusted row and column indices

    do {

        LCD_String_xy(1, 7, ":");
        LCD_String_xy(1, 10, ":");
        LCD_String_xy(1, 13, ":");

        LCD_String_xy(1, 12, (char[]){sec1, '\0'});
        LCD_String_xy(1, 11, (char[]){sec2, '\0'});
        LCD_String_xy(1, 9, (char[]){mn1, '\0'});
        LCD_String_xy(1, 8, (char[]){mn2, '\0'});
        LCD_String_xy(1, 6, (char[]){hr1, '\0'});
        LCD_String_xy(1, 5, (char[]){hr2, '\0'});

        for (i = 1; i <= 100; i++) {
            if (PORTCbits.RC1 == 1) {
                stop();
                
            }
            __delay_ms(1);
            LCD_String_xy(1, 14, (char[]){fr2, '\0'});
            LCD_String_xy(1, 15, (char[]){fr1, '\0'});
            fr1++;
            if ((i % 10) == 0) {
                fr2++;
                fr1 = 48;
            }
        }

        fr1 = 48;
        fr2 = 48;

        sec1++;
        if (sec1 > 57) {
            sec1 = 48;
            sec2++;
        }
        if (sec2 > 53) {
            sec2 = 48;
            mn1++;
        }
        if (mn1 > 57) {
            mn1 = 48;
            mn2++;
            if (mn2 > 53) {
                mn2 = 48;
                hr1++;
                if (hr1 > 57) {
                    hr1 = 48;
                    hr2++;
                }
            }
        }
        if (hr2 == 49 && hr1 == 51) {
            hr2 = hr1 = 48;
        }
    } while (1);
}


// Main Function

void main() {
    TRISC = 0x0F;  // Set PORTC as input
    INTCON2bits.INTEDG0 = 1;  // Set to rising edge
    INTCON = 0b10010000;

    fr1 = fr2 = sec1 = sec2 = mn1 = mn2 = hr1 = hr2 = 48;
    LCD_Init();
    LCD_Command(0x0C);  // Turn off cursor
    LCD_String_xy(0, 0, "Stopwatch"); // Adjusted row and column indices
    LCD_String_xy(1, 7, ":");
    LCD_String_xy(1, 10, ":");
    LCD_String_xy(1, 13, ":");
    LCD_String_xy(1, 14, (char[]){fr2, '\0'});
    LCD_String_xy(1, 15, (char[]){fr1, '\0'});
    LCD_String_xy(1, 12, (char[]){sec1, '\0'});
    LCD_String_xy(1, 11, (char[]){sec2, '\0'});
    LCD_String_xy(1, 9, (char[]){mn1, '\0'});
    LCD_String_xy(1, 8, (char[]){mn2, '\0'});
    LCD_String_xy(1, 6, (char[]){hr1, '\0'});
    LCD_String_xy(1, 5, (char[]){hr2, '\0'});
    while (1) {
        if (PORTCbits.RC0 == 1)
            start();
    }
}

