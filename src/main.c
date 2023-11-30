#include "uart.h"
#include "shell.h"
#include "string.h"
#define CMD_LEN 256

int main(int argc, char *argv[]) {

    uart_init();
    char cmd[CMD_LEN];
    
    while(1) {
        uart_puts("\nshell> ");
        readcmd(cmd);
        uart_send('\n');
        if (strcmp(cmd, "exit")) {
            uart_puts("Exit from osc shell\n");
            break;
        }
        shell(cmd);
    }
    return 0;
}