#include"AD9959.h"
#include <avr/wdt.h>
uint8_t CH;
char Fremod, Phamod, Ampmod, EndFlag;
uint32_t Pha, Amp;
double Fre;
extern uint32_t Parameter[4][3];//保存每个通道的参数 依次为频率 相位 幅度
bool SetFlag = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  wdt_enable(WDTO_2S);//可以设置的时间是15，30，60，120，250，500ms，1，2，4，8s///
  Init_AD9959();
  Recover_FinalSet();
}

void loop() {
  wdt_reset();//看门狗复位
  if (Serial.available() > 0) {
    delay(10);//等待数据传送完毕
    char Chmod = Serial.read();
    Serial.print("Chmod:");
    Serial.println(Chmod);
    if (Chmod == 'C') {
      CH = Serial.parseInt();//读取通道参数，数据范围为0~3
      Fremod = Serial.read();//读取频率模式，只能为M，K，H，L，其中M，K,H表示设置的频率单位，L表示缺省，此时频率值为最近一次设置的频率值
      Fre = Serial.parseFloat();//读取频率数据，浮点数，当频率设置模式为L时，它的值不起作用，但不能没有
      Phamod = Serial.read();//读取相位模式，只能为P，L，其中L表示缺省，此时相位值为最近一次设置的相位值
      Pha = Serial.parseInt();//读取相位数据，整型数据，当相位模式设置模式为L时，它的值不起作用，但不能没有
      Ampmod =  Serial.read();//读取振幅模式，只能为A，L，其中L表示缺省，此时振幅值为最近一次设置的振幅值
      Amp = Serial.parseInt();//读取振幅数据，整型数据，当振幅模式设置模式为L时，它的值不起作用，但不能没有
      EndFlag = Serial.read();//读取命令结束标志，只能为E，否则命令无效
      if (EndFlag == 'E') {
        Serial.println("rec cmd ok");
        SetFlag = true;
      }
      else {
        Serial.println("rec cmd failed");
        SetFlag = false;
      }
      if (SetFlag) {
        if (Fremod == 'M') {
          Parameter[CH][0] = (uint32_t)(Fre * 1000000);
        }
        if (Fremod == 'K') {
          Parameter[CH][0] = (uint32_t)(Fre * 1000);
        }
        if (Fremod == 'H') {
          Parameter[CH][0] = (uint32_t)Fre;
        }
        if (Fremod == 'L') {
          Parameter[CH][0] = Parameter[CH][0];
        }
        if (Phamod == 'L') {
          Parameter[CH][1] = Parameter[CH][1];
        } else {
          Parameter[CH][1] = (uint32_t)Pha;
        }
        if (Ampmod == 'L') {
          Parameter[CH][2] = Parameter[CH][2];
        } else {
          Parameter[CH][2] = (uint32_t)Amp;
        }
        Channel_ParameterSet(CH, Parameter[CH][0], Parameter[CH][1], Parameter[CH][2]);
        EEPROM_WriteCH(CH);

        //如果不需要回显确认 可以把下面的输出语句注释掉
        Serial.print("CH:");
        Serial.println(CH);
        Serial.print("Fre :");
        Serial.println(Parameter[CH][0]);
        Serial.print("Pha :");
        Serial.println(Parameter[CH][1]);
        Serial.print("Amp :");
        Serial.println(Parameter[CH][2]);
      }
      while (Serial.read() >= 0) {}//清空串口缓存
    }
  }
}
