#include "UART_Interface.h"
#include "../../LIB/Calcbit.h"

uint8 char_bit_size;

void UART_vInit(UART_Config_t conf)
{
    // The initialization process normally consists of setting the baud rate, setting frame format and enabling the
    // Transmitter or the Receiver depending on the usage.
    UART_calc_baud_rate_reg_value(conf.baud_rate, conf.enable_double_transmission_speed);

    if (conf.enable_double_transmission_speed == UART_ENABLE_DOUBLE_TRANSMISSION_SPEED)
        setbit(UCSRA, UCSRA_U2X);
    if (conf.enable_multiprocessor_mode == UART_ENABLE_MULTIPROCESSOR_COMMUNICATION_MODE)
        setbit(UCSRA, UCSRA_MPCM);

    if (conf.enable_recevie_complete_interrupt == UART_ENABLE_RECEIVE_COMPLETE_INTERRUPT)
        setbit(UCSRB, UCSRB_RXCIE);

    if (conf.enable_transmit_complete_interrupt == UART_ENABLE_TRANSMIT_COMPLETE_INTERRUPT)
        setbit(UCSRB, UCSRB_TXCIE);

    if (conf.enable_data_reg_empty_complete_interrupt == UART_ENABLE_DATA_REG_EMPTY_COMPLETE_INTERRUPT)
        setbit(UCSRB, UCSRB_UDRIE);
    if (conf.enable_receiver == UART_ENABLE_RECEIVER)
        setbit(UCSRB, UCSRB_RXEN);

    if (conf.enable_transmiter == UART_ENABLE_TRANSMITER)
        setbit(UCSRB, UCSRB_TXEN);

    UART_set_char_size(conf.frame_data_bits_size);
    UART_set_parity_mode(conf.parity_mode);
    UART_set_stop_bits_mode(conf.stop_bits_mode);
}

void UART_calc_baud_rate_reg_value(uint16 baud_rate, uint8 enable_double_transmission_speed)
{
    volatile uint16 reg_value = 0;
    unsigned long br = baud_rate;
    if (enable_double_transmission_speed == UART_ENABLE_DOUBLE_TRANSMISSION_SPEED)
    {
        reg_value = (F_CPU / (8.0 * br)) - 1;
    }
    else
    {
        reg_value = (F_CPU / (16.0 * br)) - 1;
        // reg_value = 51; // delete later
    }
    // select UBRRH register
    clearbit(UBRRH, UBRRH_URSEL);
    UBRRH = (uint8)(reg_value >> 8);
    UBRRL = (uint8)(reg_value);
    setbit(UBRRH, UBRRH_URSEL); // back to UCSRC
}
void UART_set_stop_bits_mode(uint8 stop_bits_mode)
{
    if (stop_bits_mode == UART_1_BIT_STOP)
        clearbit(UCSRC, UCSRC_USBS);
    else
        setbit(UCSRC, UCSRC_USBS);
}

void UART_set_parity_mode(uint8 parity_mode)
{
    switch (parity_mode)
    {
    case UART_DISABLE_PARITY:
    {
        clearbit(UCSRC, UCSRC_UPM0);
        clearbit(UCSRC, UCSRC_UPM1);
        break;
    }

    case UART_EVEN_PARITY:
    {
        clearbit(UCSRC, UCSRC_UPM0);
        setbit(UCSRC, UCSRC_UPM1);
        break;
    }

    case UART_ODD_PARITY:
    {
        setbit(UCSRC, UCSRC_UPM0);
        setbit(UCSRC, UCSRC_UPM1);
        break;
    }

    default:
        break;
    }
}

void UART_set_char_size(uint8 frame_data_bits_size)
{
    switch (frame_data_bits_size)
    {
    case UART_5BIT_CHAR_SIZE:
    {
        clearbit(UCSRC, UCSRC_UCSZ0);
        clearbit(UCSRC, UCSRC_UCSZ1);
        clearbit(UCSRB, UCSRB_UCSZ2);
        char_bit_size = UART_5BIT_CHAR_SIZE;

        break;
    }
    case UART_6BIT_CHAR_SIZE:
    {
        setbit(UCSRC, UCSRC_UCSZ0);
        clearbit(UCSRC, UCSRC_UCSZ1);
        clearbit(UCSRB, UCSRB_UCSZ2);
        char_bit_size = UART_6BIT_CHAR_SIZE;

        break;
    }
    case UART_7BIT_CHAR_SIZE:
    {
        clearbit(UCSRC, UCSRC_UCSZ0);
        setbit(UCSRC, UCSRC_UCSZ1);
        clearbit(UCSRB, UCSRB_UCSZ2);
        char_bit_size = UART_7BIT_CHAR_SIZE;

        break;
    }
    case UART_8BIT_CHAR_SIZE:
    {
        setbit(UCSRC, UCSRC_UCSZ0);
        setbit(UCSRC, UCSRC_UCSZ1);
        clearbit(UCSRB, UCSRB_UCSZ2);
        char_bit_size = UART_8BIT_CHAR_SIZE;
        break;
    }
    case UART_9BIT_CHAR_SIZE:
    {
        setbit(UCSRC, UCSRC_UCSZ0);
        setbit(UCSRC, UCSRC_UCSZ1);
        setbit(UCSRB, UCSRB_UCSZ2);
        char_bit_size = UART_9BIT_CHAR_SIZE;
        break;
    }

    default:
        break;
    }
}

void UART_vTransmit_poll(uint16 data)
{
    while (getbit(UCSRA, UCSRA_UDRE) == 0)
        ; // wait

    if (char_bit_size == UART_9BIT_CHAR_SIZE)
    {
        UCSRB &= ~(1 << UCSRB_TXB8); // clear last data's 9th bit
        if (data & 0x0100)
        {
            UCSRB |= (1 << UCSRB_TXB8); // clear last data's 9th bit
        }
    }
    UDR = (uint8)data;

    // while (!(UCSRA & (1 << UCSRA_UDRE)))
    //     ;
    // /* Put data into buffer, sends the data */
    // UDR = data;
}
uint16 UART_u16Receive_poll()
{
    unsigned char status, resh, resl;
    /* Wait for data to be received */
    while (!(UCSRA & (1 << UCSRA_RXC)))
        ;
    if (char_bit_size == UART_9BIT_CHAR_SIZE)
    {
        /* Get status and 9th bit, then data */
        /* from buffer */
        status = UCSRA;
        resh = UCSRB;
        resl = UDR;
        /* If error, return -1 */
        if (status & ((1 << UCSRA_FE) | (1 << UCSRA_DOR) | (1 << UCSRA_PE)))
            return -1;
        /* Filter the 9th bit, then return */
        resh = (resh >> 1) & 0x01;
        return ((resh << 8) | resl);
    }
    else
        return UDR;

    // while (!(UCSRA & (1 << UCSRA_RXC)))
    //     ;
    // return UDR;
}

void UART_Flush(void)
{
    volatile unsigned char dummy;
    while (UCSRA & (1 << UCSRA_RXC))
        dummy = UDR;
}
void UART_vTransmitString(char *str)
{
    while (*str)
    {
        UART_vTransmit_poll(*str);
        str++;
    }
}