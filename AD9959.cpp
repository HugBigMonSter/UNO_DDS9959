#include "AD9959.h"
//#include<SPI.h>
#include "SoftSPI.h"
u8 CSR_DATA0[1] = {0x10};     // 开 CH0
u8 CSR_DATA1[1] = {0x20};     // 开 CH1
u8 CSR_DATA2[1] = {0x40};     // 开 CH2
u8 CSR_DATA3[1] = {0x80};     // 开 CH3

u32 SinFre[5] = {50, 50, 50, 50};
u32 SinAmp[5] = {1023, 1023, 1023, 1023};
u32 SinPhr[5] = {0, 4095, 4095 * 3, 4095 * 2};
u32 SinFre0[4] = {1000};

u8 FR2_DATA[2] = {0x00, 0x00}; //default Value = 0x0000
u8 CFR_DATA[3] = {0x00, 0x03, 0x02}; //default Value = 0x000302

u8 CPOW0_DATA[2] = {0x00, 0x00}; //default Value = 0x0000   @ = POW/2^14*360



u8 LSRR_DATA[2] = {0x00, 0x00}; //default Value = 0x----

u8 RDW_DATA[4] = {0x00, 0x00, 0x00, 0x00}; //default Value = 0x--------

u8 FDW_DATA[4] = {0x00, 0x00, 0x00, 0x00}; //default Value = 0x--------
//EEPROM 各输出通道数据存储地址
uint8_t EEPROM_Site[4][3] = {{0x00, 0x04, 0x08}, {0x0C, 0x10, 0x14}, {0x18, 0x1C, 0x20}, {0x24, 0x28, 0x2C}};
uint32_t Parameter[4][3];
//AD9959初始化
void Init_AD9959(void)
{

  u8 FR1_DATA[3] = {0xD0, 0x00, 0x00}; //20倍频 Charge pump control = 75uA FR1<23> -- VCO gain control =0时 system clock below 160 MHz;
  pinMode(UPDATE, OUTPUT);
  pinMode(AD9959_PWR, OUTPUT);
  pinMode(Reset, OUTPUT);
  pinMode(SDIO3, OUTPUT);
  //  SPI.setModule(3);
  //  SPI.setClockDivider(SPI_CLOCK_DIV16);
  //  SPI.setBitOrder(MSBFIRST);
  //  SPI.setDataMode(SPI_MODE1);
  //  SPI.begin();
  spi_init();


  Intserve();  //IO口初始化
  IntReset();  //AD9959复位

  WriteData_AD9959(FR1_ADD, 3, FR1_DATA, 1); //写功能寄存器1
  //  WriteData_AD9959(FR2_ADD,2,FR2_DATA,0);
  //  WriteData_AD9959(CFR_ADD,3,CFR_DATA,1);
  //  WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
  //  WriteData_AD9959(ACR_ADD,3,ACR_DATA,0);
  //  WriteData_AD9959(LSRR_ADD,2,LSRR_DATA,0);
  //  WriteData_AD9959(RDW_ADD,2,RDW_DATA,0);
  //  WriteData_AD9959(FDW_ADD,4,FDW_DATA,1);
  //写入初始频率�
  //  Write_frequence(3,SinFre[3]);
  //  Write_frequence(0,SinFre[0]);
  //  Write_frequence(1,SinFre[1]);
  //  Write_frequence(2,SinFre[2]);

  Write_frequence(3, 50000000);
  Write_frequence(0, 1);
  Write_frequence(1, 1000);
  Write_frequence(2, 50000000);

  //  Write_Phase(3, 0);
  //  Write_Phase(0, 0);
  //  Write_Phase(1, 0);
  //  Write_Phase(2, 0);

  //    Write_Phase(3, SinPhr[3]);
  //    Write_Phase(0, SinPhr[0]);
  //    Write_Phase(1, SinPhr[1]);
  //    Write_Phase(2, SinPhr[2]);

  Write_Amplitude(3, 1023);
  Write_Amplitude(0, 1);
  Write_Amplitude(1, 1);
  Write_Amplitude(2, 1023);

  //  Write_Amplitude(3, SinAmp[3]);
  //  Write_Amplitude(0, SinAmp[0]);
  //  Write_Amplitude(1, SinAmp[1]);
  //  Write_Amplitude(2, SinAmp[2]);
}
//IO口初始化
void Intserve(void)
{
  digitalWrite(SDIO3, LOW);
  digitalWrite(AD9959_PWR, LOW);
  digitalWrite(UPDATE, LOW);

}
//AD9959复位
void IntReset(void)
{
  digitalWrite(Reset, LOW);
  delayMicroseconds(1);
  digitalWrite(Reset, HIGH);
  delayMicroseconds(30);
  digitalWrite(Reset, LOW);
}
//AD9959更新数据
void IO_Update(void)
{
  digitalWrite(UPDATE, LOW);
  delayMicroseconds(2);
  digitalWrite(UPDATE, HIGH);
  delayMicroseconds(4);
  digitalWrite(UPDATE, LOW);
}
/*--------------------------------------------
  函数功能：控制器通过SPI向AD9959写数据
  RegisterAddress:寄存器地址
  NumberofRegisters: 所含字节数
  RegisterData: 数据起始地址
  temp: 是否更新IO寄存器
  ----------------------------------------------*/
void WriteData_AD9959(u8 RegisterAddress, u8 NumberofRegisters, u8 *RegisterData, u8 temp)
{
  u8  ControlValue = 0;
  u8  ValueToWrite = 0;
  u8  RegisterIndex = 0;
  u8  i = 0;

  ControlValue = RegisterAddress;
  //写入地址�
  //digitalWrite(CS, LOW);
  spi_write_byte(ControlValue);
  // SPI.transfer(ControlValue);
  //写入数据
  for (RegisterIndex = 0; RegisterIndex < NumberofRegisters; RegisterIndex++)
  {
    ValueToWrite = RegisterData[RegisterIndex];
    //    SPI.transfer(ValueToWrite);
    spi_write_byte(ValueToWrite);
  }
  if (temp == 1)IO_Update();
  //digitalWrite(CS, HIGH);
}
/*---------------------------------------
  函数功能：设置通道输出频率�
  Channel:  输出通道
  Freq:     输出频率
  ---------------------------------------*/
void Write_frequence(u8 Channel, u32 Freq)
{
  u8 CFTW0_DATA[4] = {0x00, 0x00, 0x00, 0x00};  //中间变量�
  u32 Temp;
  Temp = (u32)Freq * 8.589934592;  //将输入频率因子分为四个字节  4.294967296=(2^32)/500000000
  CFTW0_DATA[3] = (u8)Temp;
  CFTW0_DATA[2] = (u8)(Temp >> 8);
  CFTW0_DATA[1] = (u8)(Temp >> 16);
  CFTW0_DATA[0] = (u8)(Temp >> 24);
  if (Channel == 0)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 1); //控制寄存器写入CHO通道
    WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1); //CTW0 address 0x04.输出CHO设定频率
  }
  else if (Channel == 1)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 1); //控制寄存器写入CH1通道
    WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1); //CTW0 address 0x04.输出CH1设定频率
  }
  else if (Channel == 2)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 1); //控制寄存器写入CH2通道
    WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1); //CTW0 address 0x04.输出CH2设定频率
  }
  else if (Channel == 3)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 1); //控制寄存器写入CH3通道
    WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 3); //CTW0 address 0x04.输出CH3设定频率
  }

}
/*---------------------------------------
  函数功能：设置通道输出幅度�
  Channel:  输出通道�
  Ampli:    输出幅度
  ---------------------------------------*/
void Write_Amplitude(u8 Channel, u16 Ampli)
{
  u16 A_temp;//=0x23ff;
  u8 ACR_DATA[3] = {0x00, 0x00, 0x00}; //default Value = 0x--0000 Rest = 18.91/Iout

  A_temp = Ampli | 0x1000;
  ACR_DATA[2] = (u8)A_temp;  //低位数据
  ACR_DATA[1] = (u8)(A_temp >> 8); //高位数据
  if (Channel == 0)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 1);
    WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
  }
  else if (Channel == 1)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 1);
    WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
  }
  else if (Channel == 2)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 1);
    WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
  }
  else if (Channel == 3)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 1);
    WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
  }
}
/*---------------------------------------
  函数功能：设置通道输出相位
  Channel:  输出通道
  Phase:    输出相位，范围：0~16383(对应角度：0-360度)
  ---------------------------------------*/
void Write_Phase(u8 Channel, u16 Phase)
{
  u16 P_temp = 0;
  P_temp = (u16)Phase;
  CPOW0_DATA[1] = (u8)P_temp;
  CPOW0_DATA[0] = (u8)(P_temp >> 8);
  if (Channel == 0)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 0);
    WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 0);
  }
  else if (Channel == 1)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 0);
    WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 0);
  }
  else if (Channel == 2)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 0);
    WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 0);
  }
  else if (Channel == 3)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 0);
    WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 0);
  }
}
void Channel_ParameterSet(u8 Channel, u32 Freq, u16 Phase, u16 Ampli) {
  Write_frequence(Channel, Freq);
  Write_Amplitude(Channel, Ampli);
  Write_Phase(Channel, Phase);
}
///////////////EEPROM//////////////////////////////////////
void EEPROM_Write32(uint16_t site, uint32_t pha32b) {
  u8 pha8b = 0;
  pha8b = pha32b;
  EEPROM.write(site, pha8b);
  //delayMicroseconds(20);
  pha32b = pha32b >> 8;
  pha8b = pha32b;
  EEPROM.write(site + 1, pha8b);
  // delayMicroseconds(20);
  pha32b = pha32b >> 8;
  pha8b = pha32b;
  EEPROM.write(site + 2, pha8b);
  // delayMicroseconds(20);
  pha32b = pha32b >> 8;
  pha8b = pha32b;
  EEPROM.write(site + 3, pha8b);
  // delayMicroseconds(20);

}
uint32_t EEPROM_Read32(uint16_t site) {
  uint8_t temp = 0;
  uint32_t pha_tem = 0;
  pha_tem = 0;
  temp = EEPROM.read(site + 3);
  pha_tem |= temp;

  pha_tem = pha_tem << 8;
  temp = EEPROM.read(site + 2);
  pha_tem |= temp;

  pha_tem = pha_tem << 8;
  temp = EEPROM.read(site + 1);
  pha_tem |= temp;

  pha_tem = pha_tem << 8;
  temp = EEPROM.read(site);
  pha_tem |= temp;
  return pha_tem;
}
void EEPROM_WriteCH(uint8_t CH) {
  uint8_t i;
  for (i = 0; i < 3; i++) {
    EEPROM_Write32(EEPROM_Site[CH][i], Parameter[CH][i]);
  }
}

void EEPROM_ReadCH(uint8_t CH) {
  uint8_t i;
  //uint32_t (*p)[3] = Parameter;//定义二维数组指针
  for (i = 0; i < 3; i++) {
    Parameter[CH][i] = EEPROM_Read32(EEPROM_Site[CH][i]);
  }
}

void Recover_FinalSet(void) {
  uint8_t i;
  for (i = 0; i < 4; i++) {
    EEPROM_ReadCH(i);
    Channel_ParameterSet(i , Parameter[i][0], Parameter[i][1], Parameter[i][2]);
  }
}
