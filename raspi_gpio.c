// http://www.netzmafia.de/skripten/hardware/RasPi/RasPi_GPIO.html
// http://www.netzmafia.de/skripten/hardware/RasPi/RasPi_SPI.html
// https://www.kernel.org/doc/Documentation/gpio/sysfs.txt

#include "raspi_gpio.h"

#if (defined __RASPI__)

#include "colorprint.h"

#if 1 // helpers

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>                // Needed for SPI port
#include <sys/ioctl.h>            // Needed for SPI port

char* byte2bits(uint8_t x)
{
	static char str[9] = "76543210\0";
	for (int i=0; i<8; i++)
	{
		str[i] = (x & (1<<i)) ? ('7'-i) : '.';
	}
	return str;
}

int write_intstr_to_file(const char* filename, int gpionum)
{
	int fd;
	char str[8];
	
	//printf("writing \"%u\" to %s...\n", gpionum, filename);

	fd = open(filename, O_WRONLY);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}

	int len = snprintf(str, sizeof(str), "%u", gpionum);
	int res = write(fd, str, len);
	if (res < 0)
	{
		perror("write");
		return -1;
	}
	close(fd);

	return 0;
}

int write_str_to_file(const char* filename, const char* str, int len)
{
	int fd;
	
	//printf("writing \"%s\" to %s...\n", str, filename);

	fd = open(filename, O_WRONLY);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}

	int res = write(fd, str, len);
	if (res < 0)
	{
		perror("write");
		return -1;
	}
	close(fd);

	return 0;
}

#endif

#if 1 // GPIO output

#define GPIO_NUM_MAX	32
int gpio_value_fds[GPIO_NUM_MAX];

int gpio_export(int gpionum)
{
	// export
	if (write_intstr_to_file("/sys/class/gpio/export", gpionum))
	{
		perror("gpio_export: Kann Pin nicht exportieren!\n");
		// kein grund aufzugeben, vermutlich ist der pin bereits exportiert!
		return -1;
	}
	
	// open filehandle to pinvalue device file
	char str[96];
	snprintf(str, sizeof(str), "/sys/class/gpio/gpio%u/value", gpionum);
	//fd = open(str, O_RDONLY | O_NONBLOCK);
	//int fd = open(str, O_RDONLY);
	int fd = open(str, O_RDWR);	
	if (fd < 0)
	{
		perror("pcd_open: Kann von GPIO nicht lesen (open)!\n");
		return -1;
	}
	
	/* eventuell anstehende Interrupts loeschen */
	lseek(fd, 0, SEEK_SET);
	read(fd, str, sizeof(str));
	
	gpio_value_fds[gpionum] = fd;

	return 0;
}

int gpio_setdatadir(int gpionum, bool input)
{
	char str[96];
	
	// set data direction	
	snprintf(str, sizeof(str), "/sys/class/gpio/gpio%u/direction", gpionum);
	if (write_str_to_file(str, input ? "in" : "out", input ? LLEN("in") : LLEN("out")))
	{
		perror("gpio_setdatadir: Kann Datenrichtung nicht setzen!\n");
		return -1;
	}
	return 0;	
}


/* vom GPIO-Pin lesen
 * Ergebnis: -1 = Fehler, 0/1 = Portstatus
  */
  
 // scheint nicht zu gehen wenn das das dev-gpio-value-file immer auf&zumachen!
int gpio_read(int gpionum)
{
	char result[16] = "";

	if (read(gpio_value_fds[gpionum], result, sizeof(result)) < 0)
	{
		perror("gpio_read: failed!\n");
		return -1;
	}
	else
	{
		int i = (*result - '0');
		printf("gpio_read(%u) -> (%s) \n", gpionum, result);
		return i;		
	}
}

int gpio_write(int gpionum, bool high)
{
	int res = res = write(gpio_value_fds[gpionum], high ? "1" : "0", 1);
	return (res == 1) ? 0 : -1;
}

int gpio_unexport(int gpionum)
{
	// export
	if (write_intstr_to_file("/sys/class/gpio/unexport", gpionum))
	{
		perror("gpio_export: Kann Pin nicht un-exportieren!\n");
		// kein grund aufzugeben, vermutlich ist der pin bereits exportiert!
		return -1;
	}
	
	// close filehandle for pinvalue device file
	int fd = gpio_value_fds[gpionum];
	if (fd != -1)
	{
		close(fd);
	}
	
	return 0;	
}

#endif


#if 1 // PinChangeDetect

// http://www.netzmafia.de/skripten/hardware/RasPi/RasPi_GPIO_C.html#flanke
// https://www.kernel.org/doc/Documentation/gpio/sysfs.txt

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/select.h>

int pcd_open(int gpionum, edgetype_t edgetype)
{
	//int fd;
	char str[96];
	
	// export
	if (gpio_export(gpionum)) return -1;
	
	// set data direction	
	if (gpio_setdatadir(gpionum, true)) return -1;	

	// set edge sensitivity
	char* edgestr = "none";
	switch (edgetype)
	{
		case EDGETYPE_NONE   : { edgestr = "none"; 		break; }
		case EDGETYPE_FALLING: { edgestr = "falling"; 	break; }
		case EDGETYPE_RISING : { edgestr = "rising"; 	break; }
		case EDGETYPE_BOTH   : { edgestr = "both"; 		break; }
	}
	snprintf(str, sizeof(str), "/sys/class/gpio/gpio%u/edge", gpionum);	
	if (write_str_to_file(str, edgestr, strlen(edgestr)))		
	{
		perror("pcd_open: Kann Pin-Sensitivity nicht setzen!\n");
		return -1;
	}
	
	return 0;
}

bool pcd_wait(int gpionum, int timeout_secs, int timeout_us)
{
	struct timeval timeout = {timeout_secs, timeout_us};
	
	int fd = gpio_value_fds[gpionum];
	fd_set fd_except;
	FD_ZERO(&fd_except);
	FD_SET(fd, &fd_except);	
	
	lseek(fd, 0, SEEK_SET); // eventuell anstehende Interrupts loeschen
	char dummy[128];
	read(fd, dummy, sizeof(dummy));
	
	// https://www.kernel.org/doc/Documentation/gpio/sysfs.txt
	// "If you use select(2), set the file descriptor in exceptfds."
	int s = select(fd + 1, NULL, NULL, &fd_except, &timeout);
	
	if (s > 0)
	{
		return FD_ISSET(fd, &fd_except);	
	}
	else if (s == 0)
	{
		//printf("Zzz\n");
		return false;
	}
	else
	{
		perror("pinchg_wait - select");
		sleep(1);
		return false;		
	}
}

int pcd_close(int gpionum)
{
	char str[96];
	
	// set edge sensitivity
	snprintf(str, sizeof(str), "/sys/class/gpio/gpio%u/edge", gpionum);
	if (write_str_to_file(str, "none", LLEN("none")))	
	{
		perror("pcd_close: Kann Pin-Sensitivity nicht setzen!\n");
	}
	
	// unexport
	if (gpio_unexport(gpionum)) return -1;
	
	return 0;
}

void pcd_demo(int gpionum)
{
	int res = pcd_open(gpionum, EDGETYPE_FALLING);
	if (res < 0)
	{
		eprintf("pcd_open(%u) failed.\n", gpionum);
		return;		
	}
	
	time_t t_end = time(NULL) + 20;
	
	int n = 0;
	while (time(NULL) < t_end)
	{
		if (pcd_wait(gpionum, 1, 0))
		{
			printf("pin change detect!\n");
		}
		
		n++;
		if (n==20) break;
	}

	printf("closing pin change detection.\n");
	pcd_close(gpionum);
}

#endif // PinChangeDetect

#if 1 // SPI

// http://www.netzmafia.de/skripten/hardware/RasPi/RasPi_SPI.html

#include <stdint.h>
#include <fcntl.h>                // Needed for SPI port
#include <sys/ioctl.h>            // Needed for SPI port
#include <linux/spi/spidev.h>     // Needed for SPI port

int spi_open(const char* device, uint32_t speed, uint8_t mode)
{
	//uint8_t mode 	= SPI_MODE_0; // http://lxr.free-electrons.com/source/include/uapi/linux/spi/spidev.h
	uint8_t bits 	= 8;
	//uint32_t speed 	= 500000;
	uint16_t delay;
	int ret, fd;

	/* Device oeffen */
	fd = open(device, O_RDWR);
	if (fd < 0)
	{
		eprintf("spi_open: opening device %s failed.\n", device);
		perror("spi_open: open");
		return -1;
	}
	/* Mode setzen */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret < 0)
	{
		eprintf("");
		perror("spi_open: Fehler Set SPI-Modus");
		return -1;
	}

	/* Mode abfragen */
	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret < 0)
	{
		perror("spi_open: Fehler Get SPI-Modus");
		return -1;
	}

	/* Wortlaenge setzen */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret < 0)
	{
		perror("Fehler Set Wortlaenge");
		return -1;
	}

	/* Wortlaenge abfragen */
	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret < 0)
	{
		perror("Fehler Get Wortlaenge");
		return -1;
	}

	/* Datenrate setzen */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret < 0)
	{
		perror("Fehler Set Speed");
		return -1;
	}

	/* Datenrate abfragen */
	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret < 0)
	{
		perror("Fehler Get Speed");
		return -1;
	}
	
	/* Kontrollausgabe */
	/*
	printf("SPI-Device.....: %s\n", device);
	printf("SPI-Mode.......: %d\n", mode);
	printf("Wortlaenge.....: %d\n", bits);
	printf("Geschwindigkeit: %d Hz (%d kHz)\n", speed, speed/1000);
	*/
	return fd;
}

void spi_close(int fd)
{
	if (fd != -1)
	{
		close(fd);
	}
}

int spi_io(int fd, const uint8_t *idata, uint8_t *odata, int length) //, bool keep_selected)
{
	struct spi_ioc_transfer spi[length]; /* Bibliotheksstruktur fuer Schreiben/Lesen */
	
	memset(spi, 0, sizeof(spi));
	
	/* Daten uebergeben */
	for (int i = 0; i < length; i++)
	{
		spi[i].tx_buf       = (unsigned long)(idata + i); // transmit from "data"
		spi[i].rx_buf       = (unsigned long)(odata + i); // receive into "data"
		spi[i].len          = sizeof(uint8_t);
	}
	
	//spi[length-1].cs_change	= keep_selected;

	int ret = ioctl(fd, SPI_IOC_MESSAGE(length), spi) ;
	if (ret < 0)
	{
		perror("Fehler beim Senden/Empfangen - ioctl");
		sleep(1); // debug!
		return -1;
	}
	return ret;
}

void spi_demo(void)
{
	/*
	printf("opening spi device: %s\n", g_spi_devfile);	
	int fd_spi = spi_open(g_spi_devfile);
	uint8_t spi_buf[] = "\x00\x01\x02\x03\x04\x05\x06\x07\xff\xaa";
	printf("spi in = ["); hexprint(spi_buf, sizeof(spi_buf)); printf("]\n");
	spi_io(fd_spi, spi_buf, sizeof(spi_buf));
	printf("spi out= ["); hexprint(spi_buf, sizeof(spi_buf)); printf("]\n");

	printf("opening gpio pin %u for pin change detection.\n", g_pcd_gpiopin);		
	printf("closing spi device.\n");
	spi_close(fd_spi);
	*/
}

#endif // SPI

#endif // __RASPI__