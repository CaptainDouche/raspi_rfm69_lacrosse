#ifndef _RASPI_GPIO_H_
#define _RASPI_GPIO_H_

#if (defined __RASPI__)

typedef enum
{
	EDGETYPE_NONE,	
	EDGETYPE_FALLING,
	EDGETYPE_RISING,
	EDGETYPE_BOTH,
}	edgetype_t;

#include <stdint.h>
#include <stdbool.h>
#include "defs.h"

char* byte2bits(uint8_t x);

int gpio_export(int gpionum);
int gpio_setdatadir(int gpionum, bool input);
int gpio_unexport(int gpionum);
int gpio_read(int gpionum);
int gpio_write(int gpionum, bool high);

int pcd_open(int gpionum, edgetype_t edgetype);
bool pcd_wait(int gpionum, int timeout_secs, int timeout_us);
int pcd_close(int gpionum);
void pcd_demo(int gpionum); // use gpionum=25

#if (defined _LINUX)
	#define _BSD_SOURCE
	#include <unistd.h>
	#define sleep_us(X)	usleep(X)
	#define sleep_ms(X)	usleep(1000ul * (X))
	#define sleep_s(X)	Sleep(X)

	#include <linux/spi/spidev.h>

#endif

int spi_open(const char* device, uint32_t speed, uint8_t mode);
int spi_io(int fd, const uint8_t *idata, uint8_t *odata, int length);
void spi_close(int fd);
void spi_demo(void);

#endif // __RASPI__


#endif // #ifndef _RASPI_GPIO_H_