

#include "communication.h"
#include "asf.h"

void communication_init()
{
    // USART options.
    static usart_rs232_options_t USART_SERIAL_OPTIONS = {
        .baudrate = USART_SERIAL_BAUDRATE,
        .charlength = USART_SERIAL_CHAR_LENGTH,
        .paritytype = USART_SERIAL_PARITY,
        .stopbits = USART_SERIAL_STOP_BIT
    };

    // Initialize usart driver in RS232 mode
    usart_init_rs232(USART_SERIAL, &USART_SERIAL_OPTIONS);
}

#define SERIAL_START_BYTE  0x7E
#define SERIAL_STOP_BYTE   0x7F
#define SERIAL_SUFFED_BYTE 0x7D
#define SERIAL_STUFF_CODE  0x20

static void usart_putchar_stuffed(USART_t *usart, uint8_t c)
{
    if ((c == SERIAL_START_BYTE) || (c == SERIAL_STOP_BYTE)
        || (c == SERIAL_SUFFED_BYTE)) {
        usart_putchar(usart, SERIAL_SUFFED_BYTE);
        usart_putchar(usart, (c ^ SERIAL_STUFF_CODE));
    } else {
        usart_putchar(usart, c);
    }
}

static void usart_send_bytes_stuffed(USART_t *usart, const void *c, size_t size)
{
    const uint8_t *byte = c;
    uint8_t i = 0;

    for(i=0; i<size; i++)
    {
        usart_putchar_stuffed(usart, byte[i]);
    }
}

void send_to_pi(const measurement_packet_t *packet)
{
    usart_putchar(USART_SERIAL, SERIAL_START_BYTE);

    // struct.unpack('<Hfff', bin)
    usart_send_bytes_stuffed(USART_SERIAL, &packet->circuit_id, sizeof(packet->circuit_id));
    usart_send_bytes_stuffed(USART_SERIAL, &packet->real_power, sizeof(packet->real_power));
    usart_send_bytes_stuffed(USART_SERIAL, &packet->irms, sizeof(packet->irms));
    usart_send_bytes_stuffed(USART_SERIAL, &packet->vrms, sizeof(packet->vrms));

    usart_putchar(USART_SERIAL, SERIAL_STOP_BYTE);
}

void send_to_pi_mean_calibration(const uint16_t v_measure, const uint16_t i_measure)
{
    usart_putchar(USART_SERIAL, SERIAL_START_BYTE);
    // struct.unpack('<HH', bin)
    usart_send_bytes_stuffed(USART_SERIAL, &v_measure, sizeof(v_measure));
    usart_send_bytes_stuffed(USART_SERIAL, &i_measure, sizeof(i_measure));
    usart_putchar(USART_SERIAL, SERIAL_STOP_BYTE);
}
void send_to_pi_gain_calibration(const uint32_t v_measure, const uint32_t i_measure)
{
    usart_putchar(USART_SERIAL, SERIAL_START_BYTE);
    // struct.unpack('<II', bin)
    usart_send_bytes_stuffed(USART_SERIAL, &v_measure, sizeof(v_measure));
    usart_send_bytes_stuffed(USART_SERIAL, &i_measure, sizeof(i_measure));
    usart_putchar(USART_SERIAL, SERIAL_STOP_BYTE);
}

uint8_t getchar_from_pi(void)
{
    return usart_getchar(USART_SERIAL);
}

void debug_to_pi(const char *c)
{
   while (*c)
   {
       usart_putchar(USART_SERIAL, *c);
       c++;
   }
   usart_putchar(USART_SERIAL, '\n');
}
