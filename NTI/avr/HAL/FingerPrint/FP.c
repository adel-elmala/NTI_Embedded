#include "FP_Private.h"
#include "FP_Interface.h"
#include "../../MCAL/UART/UART_Interface.h"
#include "../../MCAL/TIMER/timer.h"
#include "../LCD/LCD_Interface.h"
#include "../../LIB/Queue/Queue.h"

UART_Config_t uart_conf_fp = {
    57600, // uint16 baud_rate;
    // 9600,                                           // uint16 baud_rate;
    UART_DISABLE_DOUBLE_TRANSMISSION_SPEED,         // uint8 enable_double_transmission_speed;
    UART_DISABLE_MULTIPROCESSOR_COMMUNICATION_MODE, // uint8 enable_multiprocessor_mode;
    UART_DISABLE_RECEIVE_COMPLETE_INTERRUPT,        // uint8 enable_recevie_complete_interrupt;
    UART_DISABLE_TRANSMIT_COMPLETE_INTERRUPT,       // uint8 enable_transmit_complete_interrupt;
    UART_DISABLE_DATA_REG_EMPTY_COMPLETE_INTERRUPT, // uint8 enable_data_reg_empty_complete_interrupt;
    UART_ENABLE_RECEIVER,                           // uint8 enable_receiver;
    UART_ENABLE_TRANSMITER,                         // uint8 enable_transmiter;
    UART_8BIT_CHAR_SIZE,                            // uint8 frame_data_bits_size;
    UART_DISABLE_PARITY,                            // uint8 parity_mode;
    UART_1_BIT_STOP                                 // uint8 stop_bits_mode;

};

UART_Config_t uart_conf_fp_async = {
    57600, // uint16 baud_rate;
    // 9600,                                           // uint16 baud_rate;
    UART_DISABLE_DOUBLE_TRANSMISSION_SPEED,         // uint8 enable_double_transmission_speed;
    UART_DISABLE_MULTIPROCESSOR_COMMUNICATION_MODE, // uint8 enable_multiprocessor_mode;
    UART_ENABLE_RECEIVE_COMPLETE_INTERRUPT,         // uint8 enable_recevie_complete_interrupt;
    UART_ENABLE_TRANSMIT_COMPLETE_INTERRUPT,        // uint8 enable_transmit_complete_interrupt;
    UART_DISABLE_DATA_REG_EMPTY_COMPLETE_INTERRUPT, // uint8 enable_data_reg_empty_complete_interrupt;
    UART_ENABLE_RECEIVER,                           // uint8 enable_receiver;
    UART_ENABLE_TRANSMITER,                         // uint8 enable_transmiter;
    UART_8BIT_CHAR_SIZE,                            // uint8 frame_data_bits_size;
    UART_DISABLE_PARITY,                            // uint8 parity_mode;
    UART_1_BIT_STOP                                 // uint8 stop_bits_mode;

};

void FP_Init()
{
    // UART_vInit(uart_conf_fp);
    UART_vInit(uart_conf_fp_async);
    TIMER0_Delay_ms_with_Blocking(200);
}

bool send_packet(FP_Packet_t packet)
{
    // send header
    for (int i = PACKET_HEADER_SIZE - 1; i >= 0; --i) // send high bytes first
        // for (int i = 0; i < PACKET_HEADER_SIZE; ++i)
        UART_vTransmit_poll(packet.header[i]);

    // send_adder
    for (int i = PACKET_ADDER_SIZE - 1; i >= 0; --i)
        // for (int i = 0; i < PACKET_ADDER_SIZE; ++i)
        UART_vTransmit_poll(packet.adder[i]);

    // send pid
    UART_vTransmit_poll(packet.id);

    // send length
    for (int i = PACKET_LEN_SIZE - 1; i >= 0; --i)
        // for (int i = 0; i < PACKET_LEN_SIZE; ++i)
        UART_vTransmit_poll(packet.len[i]);

    // length of the data + (checksum : 2 bytes)
    uint16 p_length = ((uint16)(packet.len[0])) | (((uint16)(packet.len[1])) << 8);

    // length of the data only
    p_length -= PACKET_CHECKSUM_SIZE;

    // send data
    // for (int i = p_length - 1; i >= 0; --i)
    for (int i = 0; i < p_length; ++i)
        UART_vTransmit_poll(packet.content[i]);

    // send checksum
    for (int i = PACKET_CHECKSUM_SIZE - 1; i >= 0; --i)
        // for (int i = 0; i < PACKET_CHECKSUM_SIZE; ++i)
        UART_vTransmit_poll(packet.checksum[i]);
    return true;
}

bool send_packet_Async(FP_Packet_t packet)
{
    // send header
    for (int i = PACKET_HEADER_SIZE - 1; i >= 0; --i)
        // for (int i = 0; i < PACKET_HEADER_SIZE; ++i)
        UART_vTransmit_no_poll(packet.header[i]);

    // send_adder
    for (int i = PACKET_ADDER_SIZE - 1; i >= 0; --i)
        // for (int i = 0; i < PACKET_ADDER_SIZE; ++i)
        UART_vTransmit_no_poll(packet.adder[i]);

    // send pid
    UART_vTransmit_no_poll(packet.id);

    // send length
    for (int i = PACKET_LEN_SIZE - 1; i >= 0; --i)
        // for (int i = 0; i < PACKET_LEN_SIZE; ++i)
        UART_vTransmit_no_poll(packet.len[i]);

    // length of the data + (checksum : 2 bytes)
    uint16 p_length = ((uint16)(packet.len[0])) | (((uint16)(packet.len[1])) << 8);

    // length of the data only
    p_length -= PACKET_CHECKSUM_SIZE;

    // send data
    // for (int i = p_length - 1; i >= 0; --i) // send high bytes first
    for (int i = 0; i < p_length; ++i)
        UART_vTransmit_no_poll(packet.content[i]);

    // send checksum
    for (int i = PACKET_CHECKSUM_SIZE - 1; i >= 0; --i)
        // for (int i = 0; i < PACKET_CHECKSUM_SIZE; ++i)
        UART_vTransmit_no_poll(packet.checksum[i]);
    return true;
}
bool receive_packet(FP_Packet_t *packet)
{
    lcd_displayString("waiting for ack");
    lcd_goto_line2();
    // lcd_clearAndHome();
    // uint8 tmp = (uint8)UART_u16Receive_poll();
    // lcd_sendData(tmp);
    // send header
    for (int i = PACKET_HEADER_SIZE - 1; i >= 0; --i)
        // for (int i = 0; i < PACKET_HEADER_SIZE; ++i)
        packet->header[i] = (uint8)UART_u16Receive_poll();

    lcd_displayString("header");
    if (packet->header[0] == 0x01)
        lcd_displayString("0x01");
    if (packet->header[1] == 0xef)
        lcd_displayString("0xef");

    // send_adder
    for (int i = PACKET_ADDER_SIZE - 1; i >= 0; --i)
        // for (int i = 0; i < PACKET_ADDER_SIZE; ++i)
        packet->adder[i] = (uint8)UART_u16Receive_poll();

    lcd_displayString("adder");

    // send pid
    packet->id = (uint8)UART_u16Receive_poll();

    lcd_displayString("pid");

    // send length
    for (int i = PACKET_LEN_SIZE - 1; i >= 0; --i)
        // for (int i = 0; i < PACKET_LEN_SIZE; ++i)
        packet->len[i] = (uint8)UART_u16Receive_poll();

    lcd_displayString("len");

    // length of the data + (checksum : 2 bytes)
    uint16 p_length = ((uint16)(packet->len[0])) | (((uint16)(packet->len[1])) << 8);

    // length of the data only
    p_length -= PACKET_CHECKSUM_SIZE;

    // receive data
    for (int i = 0; i < p_length; ++i)
        packet->content[i] = (uint8)UART_u16Receive_poll();

    lcd_displayString("data");
    // send checksum
    for (int i = PACKET_CHECKSUM_SIZE - 1; i >= 0; --i)
        // for (int i = 0; i < PACKET_CHECKSUM_SIZE; ++i)
        packet->checksum[i] = (uint8)UART_u16Receive_poll();
    lcd_displayString("checksum");

    return true;
}
bool receive_packet_Async(FP_Packet_t *packet)
{
    lcd_displayString("waiting for ack");

    // send header
    for (int i = PACKET_HEADER_SIZE - 1; i >= 0; --i)
    // for (int i = 0; i < PACKET_HEADER_SIZE; ++i)
    {
        uint8 data = (uint8)UART_u16Receive_no_poll();
        if (data != QUEUE_ERROR_EMPTY)
            packet->header[i] = data;
        else
            i++;
        // i--;
    }

    lcd_displayString("header");

    // send_adder
    for (int i = PACKET_ADDER_SIZE - 1; i >= 0; --i)
    // for (int i = 0; i < PACKET_ADDER_SIZE; ++i)
    {
        uint8 data = (uint8)UART_u16Receive_no_poll();
        if (data != QUEUE_ERROR_EMPTY)
            packet->adder[i] = data;
        else
            i++;
        // i--;
    }

    lcd_displayString("adder");

    // send pid
    uint8 pid;
    while ((pid = (uint8)UART_u16Receive_no_poll()) == QUEUE_ERROR_EMPTY)
        ;
    packet->id = pid;

    lcd_displayString("pid");

    // send length
    for (int i = PACKET_LEN_SIZE - 1; i >= 0; --i)
    // for (int i = 0; i < PACKET_LEN_SIZE; ++i)
    {
        uint8 data = (uint8)UART_u16Receive_no_poll();
        if (data != QUEUE_ERROR_EMPTY)
            packet->len[i] = data;
        else
            i++;
    }

    lcd_displayString("len");

    // length of the data + (checksum : 2 bytes)
    uint16 p_length = ((uint16)(packet->len[0])) | (((uint16)(packet->len[1])) << 8);

    // length of the data only
    p_length -= PACKET_CHECKSUM_SIZE;

    // receive data
    for (int i = 0; i < p_length; ++i)
    {
        uint8 data = (uint8)UART_u16Receive_no_poll();
        if (data != QUEUE_ERROR_EMPTY)
            packet->content[i] = data;
        else
            i--;
    }

    lcd_displayString("data");
    // send checksum
    for (int i = PACKET_CHECKSUM_SIZE - 1; i >= 0; --i)
    // for (int i = 0; i < PACKET_CHECKSUM_SIZE; ++i)
    {
        uint8 data = (uint8)UART_u16Receive_no_poll();
        if (data != QUEUE_ERROR_EMPTY)
            packet->checksum[i] = data;
        else
            i++;
    }
    lcd_displayString("checksum");

    return true;
}
uint16 calculate_checksum(FP_Packet_t packet)
{
    uint16 checksum = 0;
    checksum += packet.id;
    for (int i = 0; i < PACKET_LEN_SIZE; ++i)
        checksum += packet.len[i];

    uint16 p_length = ((uint16)(packet.len[0])) | (((uint16)(packet.len[1])) << 8);
    p_length -= PACKET_CHECKSUM_SIZE;

    for (int i = 0; i < p_length; ++i)
        checksum += packet.content[i];
    return checksum;
}

void send_array(uint8 *packet, uint8 packet_size)
{
    for (int i = 0; i < packet_size; i++)
        // UART_vTransmit_no_poll(packet[i]);
        UART_vTransmit_poll(packet[i]);
}

void receive_array(uint8 *packet, uint8 packet_size)
{
    for (int i = 0; i < packet_size; i++)
    {
        uint8 data = (uint8)UART_u16Receive_no_poll();
        if (data == QUEUE_ERROR_EMPTY)
            i--;
        else
            packet[i] = data;
    }
}

uint8 FP_Verify_Password(uint32 password)
{
    FP_Packet_t vfyPwd_packet = {0};
    (*(uint16 *)(vfyPwd_packet.header)) = PACKET_START;
    (*(uint32 *)(vfyPwd_packet.adder)) = PACKET_DEFAULT_ADDER;
    vfyPwd_packet.id = 0x01;
    vfyPwd_packet.len[0] = 0x07;
    vfyPwd_packet.content[0] = 0x13;
    *((uint32 *)(vfyPwd_packet.content + 1)) = password;
    (*((uint16 *)vfyPwd_packet.checksum)) = calculate_checksum(vfyPwd_packet);
    // send_packet(vfyPwd_packet);
    send_packet_Async(vfyPwd_packet);
    // uint8 vf_packet[] = {0xEF, 0x1, 0xFF, 0xFF, 0xFF, 0xFF, 0x1, 0x0, 0x7, 0x13, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1B};
    // send_array(vf_packet, 16);
    lcd_clearAndHome();
    lcd_displayString("sent");

    FP_Packet_t ack_packet = {0};
    // receive_packet(&ack_packet);
    receive_packet_Async(&ack_packet);
    lcd_displayString("received");

    return ack_packet.content[0];
}

uint8 FP_Collect_Image()
{
    FP_Packet_t getImg_packet = {0};
    // (*(uint16 *)(getImg_packet.header)) = PACKET_START;
    // (*(uint32 *)(getImg_packet.adder)) = PACKET_DEFAULT_ADDER;
    // getImg_packet.id = 0x01;
    // getImg_packet.len[0] = 0x03;
    // getImg_packet.content[0] = 0x01;

    // (*((uint16 *)getImg_packet.checksum)) = calculate_checksum(getImg_packet);
    uint8 getImg_content[] = {0x01};
    fill_packet(&getImg_packet, 0x01, 0x0003, getImg_content);
    // send_packet(getImg_packet);
    send_packet_Async(getImg_packet);

    FP_Packet_t ack_packet = {0};
    // receive_packet(&ack_packet);
    receive_packet_Async(&ack_packet);
    return ack_packet.content[0];
}

uint8 FP_Upload_Image()
{
    FP_Packet_t getImg_packet = {0};
    (*(uint16 *)(getImg_packet.header)) = PACKET_START;
    (*(uint32 *)(getImg_packet.adder)) = PACKET_DEFAULT_ADDER;
    getImg_packet.id = 0x01;
    getImg_packet.len[0] = 0x03;
    getImg_packet.content[0] = 0x0A;

    (*((uint16 *)getImg_packet.checksum)) = calculate_checksum(getImg_packet);
    send_packet_Async(getImg_packet);
    // send_packet(getImg_packet);

    FP_Packet_t ack_packet = {0};
    // receive_packet(&ack_packet);
    receive_packet_Async(&ack_packet);
    return ack_packet.content[0];
}

void fill_packet(FP_Packet_t *packet, uint8 pid, uint16 len, uint8 *data)
{
    (*(uint16 *)(packet->header)) = PACKET_START;
    (*(uint32 *)(packet->adder)) = PACKET_DEFAULT_ADDER;
    packet->id = pid;
    packet->len[0] = (len & 0x00ff);
    packet->len[1] = (len & 0xff00) >> 8;
    for (int i = 0; i < len - PACKET_LEN_SIZE; ++i)
    {
        packet->content[i] = data[i];
    }

    (*((uint16 *)packet->checksum)) = calculate_checksum(*packet);
}