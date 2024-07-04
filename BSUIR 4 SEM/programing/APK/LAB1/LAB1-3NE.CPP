#include <dos.h>
#include <stdio.h>

#define COM1_PORT 0x3F8
#define COM2_PORT 0x2F8

void init_serial(int port) {
    outp(port + 1, 0x00); // Disable all interrupts
    outp(port + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outp(port + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outp(port + 1, 0x00); //                  (hi byte)
    outp(port + 3, 0x03); // 8 bits, no parity, one stop bit
    outp(port + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outp(port + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

int is_transmit_empty(int port) {
    return inp(port + 5) & 0x20;
}

void write_serial(int port, char a) {
    while (is_transmit_empty(port) == 0);
    outp(port, a);
}

int is_data_ready(int port) {
    return inp(port + 5) & 1;
}

char read_serial(int port) {
    while (is_data_ready(port) == 0);
    return inp(port);
}

int main() {
    char data;

    init_serial(COM1_PORT);
    init_serial(COM2_PORT);

    printf("Enter a character: ");
    scanf("%c", &data);

    write_serial(COM1_PORT, data);

    printf("Sent character: %c\n", data);

    printf("Received character: %c\n", read_serial(COM2_PORT));

    return 0;
}
