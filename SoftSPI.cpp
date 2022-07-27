#include <Arduino.h>
#include "SoftSPI.h"

/* SPI端口初始化 */
void spi_init() {
  pinMode(SPI_SCLK, OUTPUT);
  pinMode(SPI_MOSI, OUTPUT);
  pinMode(SPI_MISO, INPUT);
  pinMode(SPI_SS, OUTPUT);
  //SPI MODE1
  digitalWrite(SPI_SCLK, LOW);
  digitalWrite(SPI_MOSI, LOW);
  digitalWrite(SPI_SS, HIGH);
}
/* SPI写一个字节的数据 */
void spi_write_byte(unsigned char b) {
  digitalWrite(SPI_SS, LOW);
  int i;
  for (i = 0; i < 8; i++) {
    //SPI MODE1
    digitalWrite(SPI_SCLK, LOW);
    if (b & 0x80) digitalWrite(SPI_MOSI, HIGH);
    else digitalWrite(SPI_MOSI, LOW);
    b <<= 1;
    digitalWrite(SPI_SCLK, HIGH);
  }
  digitalWrite(SPI_SCLK, LOW);
  digitalWrite(SPI_SS, HIGH);
}
