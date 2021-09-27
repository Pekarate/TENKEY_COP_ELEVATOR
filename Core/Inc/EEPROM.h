
#ifndef	_EEPROM_H_
#define	_EEPROM_H_

/************************************************************************************************/
/* Definitions for adresses in ROM																*/
/************************************************************************************************/
#define E_NODE_ID			0x00						/* node id in EEPROM					*/
#define E_VENDOR			0x01						/* vendor ID in EEPROM 		 (4 uint8_t)	*/
#define E_HEARTBEATTIME		0x05						/* heartbeat time in EEPROM  (2 uint8_t)	*/
#define E_PCBTYPE			0x07						/* PCB type adress in EEPROM (2 uint8_t)	*/
#define E_REVISION			0x09						/* revision number in EEPROM (4 uint8_t)	*/
#define E_NUMBER			0x0D						/* serial number in EEPROM   (4 bytes)	*/
#define E_YEAR				0x11						/* production year in EEPROM (4 uint8_t)	*/
#define E_WEEK				0x15						/* production week in EEPROM (4 uint8_t)	*/
#define E_LIFT				0x19						/* lift number for display in EEPROM	*/
#define E_INPOLARITY		0x1A						/* input polarity in EEPROM				*/
#define E_OUTPOLARITY		0x1B						/* output polarity in EEPROM			*/
#define E_INPAR				0x1C						/* input function; 8 * 5 uint8_t			*/
#define E_OUTPAR			0x44						/* output function; 8 * 5 uint8_t			*/

#define	E_ARROW_STATE	0x00			//��ͷ��ʾ״̬
#define	E_FLOOR_NUMBER		0x01		//¥����

#ifdef _EEPROM_C_
uint8_t read_eeprom (uint8_t address);
uint8_t write_eeprom (uint8_t address, uint8_t value);
uint8_t read_eeprom_data (uint8_t startaddress, uint8_t size, uint8_t *value);
uint8_t write_eeprom_data (uint8_t startaddress, uint8_t size, uint8_t *value);

#else
extern	uint8_t read_eeprom (uint8_t address);
extern	uint8_t write_eeprom (uint8_t address, uint8_t value);
extern	uint8_t read_eeprom_data (uint8_t startaddress, uint8_t size, uint8_t *value);
extern	uint8_t write_eeprom_data (uint8_t startaddress, uint8_t size, uint8_t *value);

#endif
#endif

