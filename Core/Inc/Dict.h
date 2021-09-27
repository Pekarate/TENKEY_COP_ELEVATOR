
#ifndef __DICT_H_
#define	__DICT_H_

//���峣��
#include "stdint.h"

typedef struct  {
	uint16_t object;
	uint8_t sub;
	uint8_t access;
	uint8_t size;
	void *pointer;
	uint8_t ee_address;
}_dict;


uint32_t read_dict (uint8_t pos, uint8_t subindex);
uint8_t write_dict (uint8_t pos, uint8_t subindex, uint32_t value);
uint32_t search_dict(uint16_t index, uint8_t subindex, uint8_t type, uint8_t *pos);
extern _dict dict [];
#endif

