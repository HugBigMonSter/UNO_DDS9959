#ifndef __SoftSPI_H__
#define __SoftSPI_H__
#define SPI_SS   4    //定义SS所对应的GPIO接口编号
#define SPI_MOSI 5     //定义MOSI(SD0)所对应的GPIO接口编号  主机发送
#define SPI_SCLK 3     //定义SCLK所对应的GPIO接口编号
#define SPI_MISO 9     //定义MISO所对应的GPIO接口编号  主机接受

void spi_init();
void spi_write_byte(unsigned char b) ;
#endif
