#ifndef _DATA_STORE_H_
#define _DATA_STORE_H_

#include "common.h"
void store_uint8(uint8 addr, uint8 my_data);
void store_int16(uint8 addr, int16 my_data);
void store_uint32(uint8 addr, uint32 my_data);
void store_int(uint8 addr, int my_data);
void store_int32(uint8 addr, int32 my_data);
void store_float(uint8 addr, float my_data);
void store_uint16(uint8 addr, uint16 my_data);


extern uint8 Get_uint8(uint8 addr);
extern int16 Get_Int16(uint8 addr);
extern uint16 Get_uInt16(uint8 addr);
extern float Get_Float(uint8 addr);
#endif