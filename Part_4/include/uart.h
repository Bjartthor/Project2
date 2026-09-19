#ifndef UART_H
#define UART_H

void serial_init(void);
void serial_print_char(char c);
void serial_print(const char* str);
void serial_println(const char* str);

#endif