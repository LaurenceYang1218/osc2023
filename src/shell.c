#include "string.h"
#include "mbox.h"
#include "uart.h"

void readcmd(char* str) {
    int idx = 0;
    char c;
    while((c = uart_getc()) != '\n') {
        uart_send(c);
        str[idx++] = c;
    }
    str[idx] = '\0';
    return;
}

void shell(char* str) {
    if (strcmp(str, "help")) {
        uart_puts("#help\n"); 
        uart_puts("help\t: print this help menu\n");
        uart_puts("hello\t: print Hello World!\n"); 
        uart_puts("info\t: get device info\n");
        uart_puts("reboot\t: reboot the device\n");      
    }else if (strcmp(str, "hello")) {
        uart_puts("Hello world!\n");
    }else if (strcmp(str, "info")) {
        get_board_revision();
        get_memory_info();
    }else if (strcmp(str, "reboot")) {

    }else {
        uart_puts(str);
        uart_puts(": command not found\n");
    }
    return;
}