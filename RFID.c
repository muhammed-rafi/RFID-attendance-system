#include <reg51.h>
#include <string.h>

void delay(unsigned int count);
void lcd_cmd(unsigned char a);
void lcd_data(unsigned char b);
void lcd_init(void);
void lcd_str(unsigned char *str);
sbit rs = P1^0;          // Register Select pin for LCD
sbit en = P1^1;          // Enable pin for LCD
sbit Speaker = P1^2;     // Speaker pin
unsigned int i;
unsigned char a[100];    // Array to hold received data
sfr ldata = 0xA0;        // Port 2 for LCD
void clear(void);

void main() {
    Speaker = 1;
    TMOD = 0x20;          //
    TH1 = 0xFD;          //
    SCON = 0x50;         // 8-b1 t
    TR1 = 1;             // r1

    lcd_init();
    lcd_str("   RFID Based   ");
    lcd_cmd(0xc0);
    lcd_str("   Attendance   ");
    
    delay(80);
    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_str("   MUHAMMED    ");
    lcd_cmd(0xc0);
    lcd_str("   RAFI K T   ");
		Speaker = 1;
    delay(100);

    while (1) {
        lcd_cmd(0x01);
        lcd_cmd(0x80);
        lcd_str("   Waiting For   ");
        lcd_cmd(0xc0);
        lcd_str("   RFID Tag   ");
        
        // Read RFID tag
        for (i = 0; i < 10; i++) {
            while (RI == 0);  // Waitreception
            RI = 0;           // Cleflag
            a[i] = SBUF;      // Store array
        }
        a[i] = '\0';  // Null-terminate the string

        // Validate the RFID tag
        if (strcmp("FA5910A6", a) == 0) {
            lcd_cmd(0x01);
            lcd_cmd(0x80);
			lcd_str(" STUDENT 1 ");
            lcd_cmd(0xc0);
            lcd_str(" PRESENT ");
			Speaker = 1;  // Activate speaker for valid tag
            delay(300);
            Speaker = 0;  // Deactivate speaker
			delay(50);
            clear();
        } else if (strcmp("72CDA889", a) == 0) {
            lcd_cmd(0x01);
            lcd_cmd(0x80);
            lcd_str(" STUDENT 2 ");
            lcd_cmd(0xc0);
            lcd_str(" PRESENT ");
           Speaker = 1;  // Activate speaker for valid tag
            delay(300);
            Speaker = 0;  // Deactivate speaker
			delay(50);
            clear();
        } else if (strcmp("2395A89", a) == 0) {
            lcd_cmd(0x01);
            lcd_cmd(0x80);
            lcd_str(" STUDENT 3 ");
            lcd_cmd(0xc0);
            lcd_str(" PRESENT ");
            Speaker = 1;  // Activate speaker for valid tag
            delay(300);
            Speaker = 0;  //
			delay(50);
            clear();
        } else {
            lcd_cmd(0x01);
            lcd_cmd(0x80);
            lcd_str(" INVALID TAG");
            lcd_cmd(0xc0);
            lcd_str(a);
            delay(50);
            clear();
            Speaker = 1;  // Speaker alert for invalid tag
            delay(100);
            Speaker = 0;  
			delay(100);
			Speaker = 1;  
            delay(100);
            Speaker = 0;  
        }
    }
}

void lcd_init() {
    lcd_cmd(0x38);  // Function set: 8-bit, 2 lines, 5x7 dots
    lcd_cmd(0x0C);  // Display on, cursor off
    lcd_cmd(0x01);  // Clear display
    lcd_cmd(0x80);  // Move cursor to beginning
}

void delay(unsigned int count) {
    int i, j;
    for (i = 0; i < count; i++)
        for (j = 0; j < 1275; j++);
}

void lcd_cmd(unsigned char a) {
    rs = 0;            // Command mode
    ldata = a;        // Send command
    en = 1;           // Enable LCD
    delay(1);
    en = 0;           // Disable LCD
    delay(1);
}

void lcd_data(unsigned char b) {
    rs = 1;            // Data mode
    ldata = b;        // Send data
    en = 1;           // Enable LCD
    delay(1);
    en = 0;           // Disable LCD
    delay(1);
}

void lcd_str(unsigned char *str) {
    while (*str) {
        lcd_data(*str++);  // Send each character
    }
}

void clear(void) {
    unsigned int i;
    for (i = 0; i < 100; i++) {
        a[i] = '\0';  // Clear the array
    }
}
