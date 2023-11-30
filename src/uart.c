#include "gpio.h"

/* Auxilary mini UART registers */
#define AUX_ENABLE      ((volatile unsigned int*)(MMIO_BASE+0x00215004))
#define AUX_MU_IO       ((volatile unsigned int*)(MMIO_BASE+0x00215040))
#define AUX_MU_IER      ((volatile unsigned int*)(MMIO_BASE+0x00215044))
#define AUX_MU_IIR      ((volatile unsigned int*)(MMIO_BASE+0x00215048))
#define AUX_MU_LCR      ((volatile unsigned int*)(MMIO_BASE+0x0021504C))
#define AUX_MU_MCR      ((volatile unsigned int*)(MMIO_BASE+0x00215050))
#define AUX_MU_LSR      ((volatile unsigned int*)(MMIO_BASE+0x00215054))
#define AUX_MU_MSR      ((volatile unsigned int*)(MMIO_BASE+0x00215058))
#define AUX_MU_SCRATCH  ((volatile unsigned int*)(MMIO_BASE+0x0021505C))
#define AUX_MU_CNTL     ((volatile unsigned int*)(MMIO_BASE+0x00215060))
#define AUX_MU_STAT     ((volatile unsigned int*)(MMIO_BASE+0x00215064))
#define AUX_MU_BAUD     ((volatile unsigned int*)(MMIO_BASE+0x00215068))

// Initializes the device and maps it to the GPIO ports 
void uart_init() {
    register unsigned int r;
    *AUX_ENABLE |= 1;
    *AUX_MU_CNTL = 0;   // Disable transmitter and receiver during configuration.
    *AUX_MU_IER = 0;    // Disable interrupt because currently you don’t need interrupt.
    *AUX_MU_LCR = 3;    // Set the data size to 8 bit.
    *AUX_MU_MCR = 0;    // Don’t need auto flow control.
    *AUX_MU_BAUD = 270; // Set baud rate to 115200
    *AUX_MU_IIR = 6;    // No FIFO.
    
    /* Map mini UART to GPIO pins */
    r = *GPFSEL1;
    r &= ~((7<<12)|(7<<15)); // gpio14, gpio15
    r |= (2<<12)|(2<<15);    // alt5
    *GPFSEL1 = r;
    *GPPUD = 0;              // enable pins 14 and 15
    r = 150; while(r--) { asm volatile("nop"); }
    *GPPUDCLK0 = (1<<14)|(1<<15);
    r = 150; while(r--) { asm volatile("nop"); }
    *GPPUDCLK0 = 0;          // flush GPIO setup
    
    *AUX_MU_CNTL = 3;   // Enable the transmitter and receiver.
}

// Send a character
void uart_send(unsigned int c) {
    /* wait until we can send */
    do{
        asm volatile("nop");
    }while(!(*AUX_MU_LSR & 0x20));
    /* write the character to the buffer */
    *AUX_MU_IO=c;
}

// Receive a character
char uart_getc() {
    char r;
    /* wait until something is in the buffer */
    do {
        asm volatile("nop");
    }while (!(*AUX_MU_LSR & 0x01));
    /* read it and return */
    r = (char)(*AUX_MU_IO);
    /* convert carriage return to newline */
    return r == '\r' ? '\n' : r; 
}

// Display a string
void uart_puts(char *s) {
    while(*s) {
        /* convert newline to carrige return + newline */
        if(*s == '\n')
            uart_send('\r');
        uart_send(*s++);
    }
}

void uart_hex(unsigned int d) {
    unsigned int n;
    int c;
    for (c = 28; c >= 0; c -= 4) {
        // get highest tetrad
        n = (d >> c) & 0xF;
        // 0-9 => '0'-'9', 10-15 => 'A'-'F'
        n += n > 9 ? 0x37 : 0x30;
        uart_send(n);
    }
}
