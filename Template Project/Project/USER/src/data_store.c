#include "data_store.h"
#include "SEEKFREE_AT24C02.h"
#include "headfile.h"
//uint8 类型
//void store_uint8(uint8 addr, uint8 my_data){
//	//低位在前，读取时注意
//	//at24c02_write_byte(addr, my_data);
////	iap_write_bytes(addr,my_data,1);
//}
uint8 Get_uint8(uint8 addr){
	//return at24c02_read_byte(addr);
	uint8 temp;
	uint8 buff[1];
	//iap_read_bytes(addr,my_data,1);

	iap_read_bytes(addr,buff,1);
	//at24c02_read_bytes(addr, buff, 2);
	temp = buff[0];
	return temp;
}

//int16类型
void store_int16(uint8 addr, int16 my_data){
	uint8 temp[2];
	temp[0] = my_data;
	temp[1] = my_data>>8;
	//低位在前，读取时注意
//	at24c02_write_byte(addr, temp[0]);
//	at24c02_write_byte((uint8)(addr+1), temp[1]);
	extern_iap_write_bytes(addr,temp,1);
	extern_iap_write_bytes((uint8)addr+1,temp+1,1);
}
int16 Get_Int16(uint8 addr){
	int16 temp ;
	uint8 buff[2];
	iap_read_bytes(addr,buff,2);
	//at24c02_read_bytes(addr, buff, 2);
	temp = buff[0] + (buff[1]<<8);
	return temp;
}

//uint16类型
void store_uint16(uint8 addr, uint16 my_data){
	uint8 temp[2];
	temp[0] = my_data;
	temp[1] = my_data>>8;
	//低位在前，读取时注意
	extern_iap_write_bytes(addr,temp,1);
	extern_iap_write_bytes((uint8)addr+1,temp+1,1);
	//at24c02_write_byte(addr, temp[0]);
	//at24c02_write_byte((uint8)(addr+1), temp[1]);
}
uint16 Get_uInt16(uint8 addr){
	int16 temp ;
	uint8 buff[2];
	iap_read_bytes(addr,buff,2);
	//at24c02_read_bytes(addr, buff, 2);
	temp = buff[0] + (buff[1]<<8);
	return temp;
}


//float类型
void store_float(uint8 addr, float my_data){
	uint8 temp[4];
	//强制取址
	temp[0] = ((uint8*)&my_data)[0];
	temp[1] = ((uint8*)&my_data)[1];
	temp[2] = ((uint8*)&my_data)[2];
	temp[3] = ((uint8*)&my_data)[3];
	//at24c02_write_bytes(addr, temp, 4);
	extern_iap_write_bytes(addr,temp,4);
}

float Get_Float(uint8 addr){
	uint8 i;
	float temp ;
	uint8 buff[4];
	iap_read_bytes(addr,buff,4);
	//at24c02_read_bytes(addr, buff, 4);
	for(i = 0; i < 4;i++){
		((uint8*)&temp)[i] = buff[i];
	}
	return temp;
}

//uint32类型
void store_uint32(uint8 addr, uint32 my_data){
	//多字节写入
	uint8 temp[4];
	temp[0] = my_data;
	temp[1] = my_data >> 8;
	temp[2] = my_data >> 16;
	temp[3] = my_data >> 24;
	extern_iap_write_bytes(addr,temp,4);
	//at24c02_write_bytes(addr, temp, 4);
}








