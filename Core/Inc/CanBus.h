
#ifndef _CANBUS_H_
#define	_CANBUS_H_

#include "stdint.h"
#define	RX_SIZE		8
#define	TX_SIZE		8

#define SDO_TIMER			0x0A						/* time out for SDO segment transfer	*/

/************************************************************************************************/
/* Definitions for configuration register CANCON												*/
/************************************************************************************************/
#define CAN_MODE_BITS		0xE0		   				/* Mode bits in configuration register	*/
#define CAN_MODE_CONFIG		0x80		   				/* Configuration mode					*/
#define CAN_MODE_LISTEN		0x60		   				/* Listen Only mode						*/
#define CAN_MODE_NORMAL1		0x00		   				/* Normal mode							*/

/************************************************************************************************/
/* Definitions for interrupt code in CANSTAT register											*/
/************************************************************************************************/
#define INTCODE				0x0E		   				/* Mask for interrupt code bits			*/
#define WAKEINT				0x0E		   				/* Wake up interrupt					*/
#define RXB0INT				0x0C		   				/* RX buffer 0 interrupt				*/
#define RXB1INT				0x0A		   				/* RX buffer 1 interrupt				*/
#define TXB0INT				0x08		   				/* TX buffer 0 interrupt				*/
#define TXB1INT				0x06		   				/* TX buffer 1 interrupt				*/
#define TXB2INT				0x04		   				/* TX buffer 2 interrupt				*/
#define ERRORINT			0x02		   				/* error interrupt						*/
#define NOINT				0x00		   				/* kein interrupt						*/

/************************************************************************************************/
/* Definitions for Win bits in CANCON register													*/
/************************************************************************************************/
#define TXB0REG				0x08		   				/* TX buffer 0 register access			*/
#define TXB1REG				0x06		   				/* TX buffer 1 register access			*/

#define	BPS_125K			0
#define	BPS_50K				1

#ifdef	_CANBUS_C_

volatile uint8_t			rc;		//RX message counter
volatile uint8_t 		tc;		//TX message counter
uint8_t 							ro;				//pointer for FIFO read
volatile uint8_t 		ri;		//pointer for FIFO write
volatile uint8_t 		to;		//pointer for TX FIFO read
uint8_t							ti;				//pointer for TX FIFO write


volatile uint8_t 		rx[RX_SIZE][10];				// RX message buffer
volatile uint8_t 		tx[TX_SIZE][10];				// TX message buffer

/************************************************************************************************/
/* change from number to gray code																*/
/************************************************************************************************/
uint8_t const graycode [] = {
	 0, 1, 3, 2, 6, 7, 5, 4,12,13,15,14,10,11, 9, 8,24,25,27,26,30,31,29,28,20,21,23,22,18,19,17,16,
	48,49,51,50,54,55,53,52,60,61,63,62,58,59,57,56,40,41,43,42,46,47,45,44,36,37,39,38,34,35,33,32
};

void Init_Can (void);
void read_rx (void);
void abort_sdo (uint32_t errorcode);
void sdo_response (uint8_t command, uint16_t index, uint8_t subindex, uint32_t value);
void sdo_segment (uint8_t command, uint8_t size, uint8_t *value);
void lss_response (uint8_t command, uint8_t value);
void transmit_error (void);
void transmit_in (uint8_t *input);
void transmit_load (void);
uint8_t check_for_call (uint8_t value);
void set_output (uint8_t *virt);
void set_io_config (void);
void CAN_transmit_heartbeat(void);


#else
extern	volatile uint8_t			rc;		//RX message counter
extern	volatile uint8_t 		rx[RX_SIZE][10];				// RX message buffer
extern	uint8_t 					ro;				//pointer for FIFO read
extern	volatile uint8_t 		tc;

extern	void Init_Can (void);
extern	void read_rx (void);
extern	void abort_sdo (uint32_t errorcode);
extern	void transmit_error (void);
extern	void transmit_in (uint8_t *input);
extern	void transmit_load (void);
extern	void ListenCAN(uint8_t baudrate);

extern	uint8_t check_for_call (uint8_t value);
extern	void set_output (uint8_t *virt);
extern	void set_io_config (void);
extern 	void CAN_transmit_heartbeat(void);


#endif
#endif

