// rfid.h

#ifndef _RFIDLOG_h
#define _RFIDLOG_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define RFID_UART_RX_BUFF_SIZE 40
#define BAUD 9600

#define NUM_ACCEPTED_TAGS 43
const char accepted_tags[NUM_ACCEPTED_TAGS][32] =
	{	"030003AA43","03000223D4","030001B060", 
"0300029F64", 
"030002EEE3", 
"300020673", 
"03000318ED", 
"03000233CE", 
"0700EDACB6", 
"0700EDBC30", 
"300033379", 
"03000239EF", 
"030001E1EF", 
"0700EDDA76", 
"030003157D", 
"030001D39E", 
"030001C32C", 
"300022699", 
"03000305D9", 
"030002E6AC", 
"030001E8EB", 
"030001D8DD", 
"030003A97D", 
"300027418", 
"0700EDD527", 
"0700EDB7AE", 
"0300032D8E", 
"0700EE3BFF", 
"0700EE059D", 
"0700EE2D5F", 
"2A006D1DB8", 
"03000358B4", 
"0700EE1F53", 
"0700EDFC8F", 
"0700ED9624", 
"0700EDB5E7", 
"0110175CA8", 
"0110175DA2", 
"110176163", 
"01101768F3", 
"110176391", 
"0700EDF247",
		"test2"
	};


typedef enum {	SET_READER_ACTIVE,
				SET_FORMAT_HEXADECIMAL,
				SET_FORMAT_DECIMAL,
				SET_OUTPUT_MODE_0,
				SET_OUTPUT_MODE_1,
				READ_FIRMWARE_VERSION_CODE,
				READ_LOG_FILE} rfidlog_cmd;

extern volatile int  rfid_ok;	/* received OK flag */
extern volatile int  rcv;		/* received return or <CR> character flag. */
extern volatile int  rfid_tag;  /* Scanned tag is valid */
extern volatile char rfid_uart_rx_buff[RFID_UART_RX_BUFF_SIZE]; /* rx buffer */
extern volatile char rfid_tag_str[RFID_UART_RX_BUFF_SIZE]; /* The rx buffer is copied here */


void rfidlog_uart_init();
void rfidlog_send_command(rfidlog_cmd cmd);
void rfidlog_send_string(char * str);


#endif

