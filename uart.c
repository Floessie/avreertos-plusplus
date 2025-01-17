#include <stdio.h>
#include <avr/io.h>

#define BAUD 9600

#include <util/setbaud.h>

#include "uart.h"

static int uart_putchar(char c, FILE* stream)
{
    if (c == '\n') {
        uart_putchar('\r', stream);
    }

    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;

    return 0;
}

static int uart_getchar(FILE* stream)
{
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}

void uart_init(void)
{
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0); /* Enable RX and TX */

	static FILE uart_io = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

	stdin = &uart_io;
	stdout = &uart_io;
	stderr = &uart_io;
}
