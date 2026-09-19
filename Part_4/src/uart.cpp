#include "uart.h"
#include <avr/io.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define BRC ((F_CPU / 16 / BAUD) - 1)

void serial_init(void) {
    // 1. Set the Baud Rate
    // UBRR0 is a 16-bit register split into High (H) and Low (L) bytes.
    UBRR0H = (BRC >> 8); 
    UBRR0L = BRC;

    // 2. Enable the Transmitter (TXEN0) and Receiver (RXEN0)
    // This physically connects pins 0 and 1 to the USART hardware.
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // 3. Set Frame Format: 8 data bits, 1 stop bit, no parity
    // UCSZ01 and UCSZ00 configure the character size to 8 bits.
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void serial_print_char(char c) {
    // Wait for the Transmit Buffer to be empty.
    // The UDRE0 (USART Data Register Empty) bit goes to 1 when it's safe to write.
    while (!(UCSR0A & (1 << UDRE0))) {
        // Do nothing until the buffer clears
    }
    
    // Put the character into the data register. The hardware immediately sends it.
    UDR0 = c;
}

void serial_print(const char* str) {
    // Loop through the character array until we hit the null terminator ('\0')
    while (*str) {
        serial_print_char(*str); // Print the current character
        str++;                   // Move to the next character memory address
    }
}

void serial_println(const char* str) {
    serial_print(str);
    serial_print_char('\r'); // Carriage return
    serial_print_char('\n'); // Line feed
}