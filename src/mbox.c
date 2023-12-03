#include "mbox.h"
#include "uart.h"

void mailbox_call(unsigned int* mbox) {
    unsigned int r = (((unsigned long)mbox) & ~0xF) | MBOX_CH_PROP;

    do {
        asm volatile ("nop");
    }while (*MAILBOX_STATUS & MAILBOX_FULL); 

    *MAILBOX_WRITE = r;

    while (1) {
        do {
            asm volatile ("nop");
        }while (*MAILBOX_STATUS & MAILBOX_EMPTY);

        if (r == *MAILBOX_READ) {
            return;
        }
    }
} 

void get_board_revision() {
    unsigned int mailbox[7];
    mailbox[0] = 7 * 4; // buffer size in bytes
    mailbox[1] = REQUEST_CODE;
    // tags begin
    mailbox[2] = GET_BOARD_REVISION; // tag identifier
    mailbox[3] = 4; // maximum of request and response value buffer's length.
    mailbox[4] = TAG_REQUEST_CODE;
    mailbox[5] = 0; // value buffer
    // tags end
    mailbox[6] = END_TAG;
    mailbox_call(mailbox);

    uart_puts("RPI3B+ Board Revision: ");
    uart_hex(mailbox[5]);
    uart_puts("\n");
    
    return;
}

void get_memory_info() {
    unsigned int mailbox[8];
    mailbox[0] = 8 * 4; // buffer size in bytes
    mailbox[1] = REQUEST_CODE;
    // tags begin
    mailbox[2] = GET_ARM_MEMORY; // tag identifier
    mailbox[3] = 8; // maximum of request and response value buffer's length.
    mailbox[4] = TAG_REQUEST_CODE;
    mailbox[5] = 0; // value buffer
    mailbox[6] = 0; // value buffer
    // tags end
    mailbox[7] = END_TAG;
    mailbox_call(mailbox);

    uart_puts("RPI3B+ Memory Info: \n");
    uart_puts("  Base address: ");
    uart_hex(mailbox[5]);
    uart_puts("\n");
    uart_puts("  Size: ");
    uart_hex(mailbox[6]);
    uart_puts("\n");
    
    return;
}