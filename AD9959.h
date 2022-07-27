#ifndef _AD9959_H_
#define _AD9959_H_
#include <EEPROM.h>
#include <Arduino.h>
//#include "SoftSPI.h"
#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long
//AD9959寄存器地址定义
#define CSR_ADD   0x00   //CSR 通道选择寄存器
#define FR1_ADD   0x01   //FR1 功能寄存器1
#define FR2_ADD   0x02   //FR2 功能寄存器2
#define CFR_ADD   0x03   //CFR 通道功能寄存器
#define CFTW0_ADD 0x04   //CTW0 通道频率转换字寄存器
#define CPOW0_ADD 0x05   //CPW0 通道相位转换字寄存器
#define ACR_ADD   0x06   //ACR 幅度控制寄存器
#define LSRR_ADD  0x07   //LSR 通道线性扫描寄存器
#define RDW_ADD   0x08   //RDW 通道线性向上扫描寄存器
#define FDW_ADD   0x09   //FDW 通道线性向下扫描寄存器
//AD9959管脚宏定义
/*为了和其他的模块使用时不冲突，设计了两组引脚连接方式，默认使用Module3
  如果使用module4 需要在Ad9959初始化程序中修改SPI.setModule(4);
*/
//setmodule(3)******************************************
//SPI硬件接口 不可修改
//#define SCLK  3
//#define SDIO0 5//SDO
//#define CS    4
//这部分引脚可以根据占用情况自行修改
#define UPDATE  2 //IOU
#define AD9959_PWR  7 //PDN 接地
#define Reset   8 //RST
#define SDIO3   6 //经测试不需要与控制板连接，直接接地
//其余不需要与控制板连接，直接悬空
void IntReset(void);	  //AD9959复位
void IO_Update(void);   //AD9959更新数据
void Intserve(void);		   //IO口初始化�
void WriteData_AD9959(u8 RegisterAddress, u8 NumberofRegisters, u8 *RegisterData, u8 temp);
void Init_AD9959(void);
void Write_frequence(u8 Channel, u32 Freq);
void Write_Amplitude(u8 Channel, u16 Ampli);
void Write_Phase(u8 Channel, u16 Phase);
void Channel_ParameterSet(u8 Channel, u32 Freq, u16 Phase, u16 Ampli);
uint32_t EEPROM_Read32(uint16_t site) ;
void EEPROM_Write32(uint16_t site, uint32_t pha32b);
void EEPROM_WriteCH(uint8_t CH);
void EEPROM_ReadCH(uint8_t CH);
void Recover_FinalSet(void);
#endif
