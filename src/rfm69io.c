#include "rfm69io.h"

#if (defined __RASPI__)
	#include "raspi_gpio.h"
	#include <stdio.h>
	#include <unistd.h>
	#include <stdint.h>
	//#define USE_RFM69_IRQ_THREAD		1 // nur für Testzwecke nötig
	#define TEST_SPI_REG_RW				1
	
	#define _delay_ms(MS)		usleep(1000*(MS))
	#define _delay_us(US)		usleep(US)
	#define _delay_s(S)			sleep(1)
	
	#ifndef RFM69_RST_GPIONUM
		#define RFM69_SPI_DEVFILE "/dev/spidev0.0"
	#endif
	
	#ifndef RFM69_IRQ_GPIONUM
		#define RFM69_IRQ_GPIONUM -1
	#endif	
	
	#ifndef RFM69_RST_GPIONUM
		#define RFM69_RST_GPIONUM -1
	#endif
	
	int rfm69_debuglevel		= 2;
	char* rfm69_spi_devfile 	= RFM69_SPI_DEVFILE;
	int rfm69_irq_gpionum 		= RFM69_IRQ_GPIONUM;
	int rfm69_rst_gpionum 		= RFM69_RST_GPIONUM;

	static int rfm69_fd_spi = -1;
	static int rfm69_fd_irq = -1;
	static int rfm69_fd_rst = -1;	
	
#elif (defined __AVR__)
	//#include <string.h>
	#include <stdint.h>
	#include <util/delay.h>
	#include <avr/interrupt.h>
	#include <avr/wdt.h>
	#include <avr/pgmspace.h>
	#include <avr/wdt.h>
	#include "spi.h"
	#include "uart0.h"
	#define _RFMDBG(level, format, ...)			uart0_printf_L("rfm: " format "\n", ## __VA_ARGS__)
	
	#error todo: include an spi backend	
	
#elif (defined __ESP32__)
	
	// https://nodemcu.readthedocs.io/en/master/en/modules/spi/
	// http://www.electronicwings.com/nodemcu/nodemcu-spi-with-arduino-ide
	// https://github.com/rrobinet/RFM69X_Library/blob/master/RFM69X.cpp
	#include <SPI.h>
	
	#ifndef RFM69_SS_GPIONUM
		#define RFM69_SS_GPIONUM	xxxxxxxxxx
	#endif

	#ifndef RFM69_IRQ_GPIONUM
		#define RFM69_IRQ_GPIONUM	xxxxxxxxxx
	#endif	
	
	SPISettings settingsA(SPI_CLOCK_DIV4, MSBFIRST, SPI_MODE0); // Adapted according current SPI library (SPI_HAS_TRANSACTION)
	
#endif


#if (USE_RFM69_IRQ_THREAD == 1)

// #pragma comment(lib, "pthread")
// link using -lpthread
#include <pthread.h>

pthread_t irqthd_pthread;
volatile bool irqthd_dorun   = false;
volatile bool irqthd_isalive = false;

void* irqthd_work(void* argument)
{
	irqthd_isalive = true;
	
	//printf("\nirqthd: thread begin!\n");

	while (irqthd_dorun)
	{
		//printf("\nirqthd: zzz...\n");
		if (pcd_wait(rfm69_irq_gpionum, 1, 0))
		{
			printf("\nirqthd: pin change detect! ****\n");
		}
	}

	//printf("\nirqthd: thread end!\n");

	irqthd_isalive = false;
	return NULL;
}

bool irqthd_init(void)
{
	if (!irqthd_isalive)
	{
		//printf("\nirqthd: creating thread..\n");
		irqthd_dorun = true;
		pthread_create(&irqthd_pthread, NULL, irqthd_work, NULL);
		//printf("\nirqthd: created thread.\n");
	}
	else
	{
		//printf("\nirqthd: already initialized!\n");
	}
}

bool irqthd_deinit(void)
{
	if (irqthd_isalive)
	{
		irqthd_dorun = false;
		pthread_join(irqthd_pthread, NULL);
		//printf("\nirqthd: thread joined!\n");
	}
	else
	{
		//printf("\nirqthd: already deinitialized!\n");
	}
}

#endif

#if (defined __ESP32__)
void rfm69io_isr(void)
{
	#warning todo
	// no spi transactions here!!!
}

void rfm69io_spi_select(void)
{
	SPI.beginTransaction(settingsA); // Adapted according current SPI library (SPI_HAS_TRANSACTION)
	digitalWrite(_slaveSelectPin, LOW);
}

void rfm69io_spi_deselect(void)
{
	digitalWrite(_slaveSelectPin, HIGH);	
	SPI.endTransaction();	// Adapted according current SPI library (SPI_HAS_TRANSACTION)
}

#endif

bool rfm69io_init(void)
{
	#if (defined __AVR__)
		BIT_SET(RFM69_CS_DDR, RFM69_CS_BITPOS);
		RFM69_CS_DESELECT();
	#elif (defined __RASPI__)
		static bool called = false;
		if (called) { return; } else { called = true; }

		_RFMDBG(1, "hw init spi...");
		rfm69_fd_spi = spi_open(rfm69_spi_devfile, 500000, SPI_MODE_0);
		
		if (rfm69_fd_spi < 0)
		{
			eprintf("rfm69io_init: spi_open failed.\n");
			rfm69io_deinit();
			return false;
		}

		if (rfm69_rst_gpionum > 0)
		{
			_RFMDBG(1, "hw init resetpin gpionum=%d ...", rfm69_rst_gpionum);
			gpio_export(rfm69_rst_gpionum);
			gpio_setdatadir(rfm69_rst_gpionum, false);
			gpio_write(rfm69_rst_gpionum, false);
		}

		if (rfm69_irq_gpionum > 0)
		{
			_RFMDBG(1, "hw init irq gpionum=%d ...", rfm69_irq_gpionum);
			//if (!pcd_open(rfm69_irq_gpionum, EDGETYPE_BOTH))
			if (pcd_open(rfm69_irq_gpionum, EDGETYPE_RISING) < 0)
			{
				eprintf("rfm69io_init: pcd_open failed.\n");
				rfm69io_deinit();
				return false;
			}
			
			#if (USE_RFM69_IRQ_THREAD == 1)
			irqthd_init();
			#endif			
		}
		
		return true;
	#elif (defined __ESP32__)
		
		digitalWrite(RFM69_SS_GPIONUM, HIGH);
		pinMode(RFM69_SS_GPIONUM, OUTPUT);
		SPI.begin();
		
		attachInterrupt(RFM69_IRQ_GPIONUM, rfm69io_isr, RISING);

	#endif
}



void rfm69io_deinit(void)
{
	#if (defined __AVR__)

	#elif (defined __RASPI__)
		static bool called = false;
		if (called) { return; } else { called = true; }

		if (rfm69_fd_spi != -1)
		{
			_RFMDBG(1, "hw deinit spi...");
			spi_close(rfm69_fd_spi);
			rfm69_fd_spi = -1;
		}

		if (rfm69_rst_gpionum > 0)
		{
			_RFMDBG(1, "hw deinit resetpin...");
			gpio_setdatadir(rfm69_rst_gpionum, true);
			gpio_unexport(rfm69_rst_gpionum);
		}

		if (rfm69_irq_gpionum > 0)
		{
			#if (USE_RFM69_IRQ_THREAD == 1)
			_RFMDBG(1, "deinit irqthd...");
			irqthd_deinit();
			#endif

			_RFMDBG(1, "hw deinit irq...");
			pcd_close(rfm69_irq_gpionum);
		}
	#elif (defined __ESP32__)
		
	#endif
}



#if (defined __RASPI__) // register names

typedef struct
{
	int num;
	char* name;
}	regnames_t;

const regnames_t regnames[] = { RFM69_REGNAME_MAP };

const char* reg_num2name(int num)
{
	for (int i=0; regnames[i].name; i++)
	{
		if (num == regnames[i].num)
		{
			return regnames[i].name;
		}
	}
	return NULL;
}

int reg_name2num(const char* name)
{
	for (int i=0; regnames[i].name; i++)
	{
		if STRIEQU(name, regnames[i].name)
			return regnames[i].num;
	}
	return 0xff;
}

#endif


uint8_t rfm69_readReg(uint8_t addr)
{
	#if (defined __RASPI__)
		uint8_t ibuf[] =  {SPI_READ_REG(addr), 0x00};
		uint8_t obuf[] =  {0x00, 0x00};
		spi_io(rfm69_fd_spi, ibuf, obuf, sizeof(obuf));

		//static uint8_t last_addr = 0xff;
		//static uint8_t last_oval = 0xff;
		//if ((last_addr != addr) || (last_oval != obuf[1]))
		{
			_RFMDBG(2, " R[%02hx:%-13s] -> %02hx (%s)", addr, reg_num2name(addr), obuf[1], byte2bits(obuf[1]));
			//last_addr = addr;
			//last_oval = obuf[1];
		}

		return obuf[1];
	#elif (defined __AVR__)
		
		RFM69_CS_SELECT();
		spi_io8(SPI_READ_REG(addr));
		uint8_t value = spi_io8(0x00);
		RFM69_CS_DESELECT();
		_RFMDBG(2, " R[%02hx] -> %02hx", addr, value);
		return value;
		
	#elif (defined __ESP32__)
		
		rfm69io_spi_select();
		SPI.transfer(SPI_READ_REG(addr));
		uint8_t regval = SPI.transfer(0);
		rfm69io_spi_deselect();
		return regval;
		
	#endif
}

void rfm69_writeReg(uint8_t addr, uint8_t value)
{
	#if (defined __RASPI__)
		
		uint8_t ibuf[] =  {SPI_WRITE_REG(addr), value};
		uint8_t obuf[] =  {0x00, 0x00};
		spi_io(rfm69_fd_spi, ibuf, obuf, sizeof(obuf));
		#if (_DBG_DUMP_REG_ACCESS == 1)
		_RFMDBG(2, " W[%02hx:%-13s] <- %02hx (%s)", addr, reg_num2name(addr), value, byte2bits(value));
		#endif
		
	#elif (defined __AVR__)
		
		RFM69_CS_SELECT();
		spi_io8(SPI_WRITE_REG(addr));
		spi_io8(value);
		RFM69_CS_DESELECT();
		#if (_DBG_DUMP_REG_ACCESS == 1)
		_RFMDBG(2, " W[%02hx] <- %02hx", addr, value);
		#endif
		
	#elif (defined __ESP32__)
		
		rfm69io_spi_select();	
		SPI.transfer(SPI_WRITE_REG(addr));
		SPI.transfer(value);
		rfm69io_spi_deselect();		

	#endif
}

/*
void rfm69_writeRegBurst(uint8_t addr, const uint8_t* vals, int len)
{
	#if (defined __RASPI__)
		while (len--)
		{
			rfm69_writeReg(addr, *vals);
			addr++;
			vals++;
		}
	#elif (defined __AVR__)
		RFM69_CS_SELECT();
		spi_io8(SPI_WRITE_REG(addr));
		while (len--)
		{
			spi_io8(*vals);
			#if (_DBG_DUMP_REG_ACCESS == 1)
			_RFMDBG(" R[%02hx] -> %02hx", addr, *vals);
			#endif
			vals++;
		}
		RFM69_CS_DESELECT();
	#endif
}
*/

void rfm69_writeFifo(const uint8_t* vals, int len)
{
	//_RFMDBG("writeFifo...");
	#if (defined __RASPI__)
		while (len--)
		{
			rfm69_writeReg(RFM69_REG_FIFO, *vals);
			vals++;
		}
	#elif (defined __AVR__)
		RFM69_CS_SELECT();
		spi_io8(SPI_WRITE_REG(RFM69_REG_FIFO));
		while (len--)
		{
			uint8_t val = *vals;
			spi_io8(val);
			#if (_DBG_DUMP_REG_ACCESS == 1)
			_RFMDBG(2, " W[%02hx] <- %02hx", RFM69_REG_FIFO, val);
			#endif
			vals++;
		}
		RFM69_CS_DESELECT();
	#endif
	_RFMDBG(2, "writeFifo: done");
}

void rfm69_readFifo(uint8_t* vals, int len)
{
	//_RFMDBG("readFifo...");
	#if (defined __RASPI__)
		while (len--)
		{
			*vals = rfm69_readReg(RFM69_REG_FIFO);
			vals++;
		}
	#elif (defined __AVR__)
		RFM69_CS_SELECT();
		spi_io8(SPI_READ_REG(RFM69_REG_FIFO));
		while (len--)
		{
			uint8_t val = *vals;
			*vals = spi_io8(val);
			#if (_DBG_DUMP_REG_ACCESS == 1)
				
			_RFMDBG(2, " R[%02hx] -> %02hx", RFM69_REG_FIFO, val);
			#endif
			vals++;
		}
		RFM69_CS_DESELECT();
	#endif
	//_RFMDBG("readFifo: done");
}

bool rfm69_waitForIrq(int timeout_secs, int timeout_us)
{
	#if (defined __RASPI__)
	return pcd_wait(rfm69_irq_gpionum, timeout_secs, timeout_us);
	#endif
}

bool rfm69_waitForRegBitSet(uint8_t reg, uint8_t mask, int timeout_ms)
{
	_RFMDBG(1, "waitForRegBitSet(%s, mask=%02x, tout=%u)...", reg_num2name(reg), mask, timeout_ms);
	while (timeout_ms > 0)
	{
		if ((rfm69_readReg(reg) & mask) == mask)
		{
			_RFMDBG(1, "waitForRegBitSet: ok");
			return true;
		}
		_delay_ms(1);
		timeout_ms--;
	}

	_RFMDBG(1, "waitForRegBitSet: TIMEOUT!!!");
	return false;
}

bool rfm69_waitForRegBitClear(uint8_t reg, uint8_t mask, int timeout_ms)
{
	_RFMDBG(1, "waitForRegBitClear(%s, mask=%02x, tout=%u)...", reg_num2name(reg), mask, timeout_ms);
	while (timeout_ms > 0)
	{
		if ((rfm69_readReg(reg) & mask) == 0)
		{
			_RFMDBG(1, "waitForRegBitClear: ok");
			return true;
		}
		_delay_ms(1);
		timeout_ms--;
	}

	_RFMDBG(1, "waitForRegBitClear: TIMEOUT!!!");
	return false;
}

#if (TEST_SPI_REG_RW == 1)
bool rfm69_test_reg_rw(void)
{
	_RFMDBG(0, "test_reg_rw...");

	uint8_t Test;
	Test = rfm69_readReg(RFM69_REG_VERSION);
	if (Test == 00 || Test == 0xFF)
	{
		_RFMDBG(1, "error: register r failed!");
		return(false);
	}

	rfm69_writeReg(RFM69_REG_SYNCVALUE1, 0xaa);
	if (rfm69_readReg(RFM69_REG_SYNCVALUE1) != 0xaa)
	{
		_RFMDBG(1, "error: register w/r failed!");
		return(false);
	}

	rfm69_writeReg(RFM69_REG_SYNCVALUE1, 0x55);
	if (rfm69_readReg(RFM69_REG_SYNCVALUE1) != 0x55)
	{
		_RFMDBG(1, "error: register w/r failed!");
		return(false);
	}

	return true;
}
#endif

void rfm69_setFrequency(uint32_t kHz) 
{
	_RFMDBG(1, "set freq %lu kHz", kHz);
	uint32_t f = (((kHz * 1000) << 2) / (32000000L >> 11)) << 6;
	rfm69_writeReg(RFM69_REG_FRFMSB, f >> 16);
	rfm69_writeReg(RFM69_REG_FRFMID, f >> 8);
	rfm69_writeReg(RFM69_REG_FRFLSB, f);
}

void rfm69_setDataRate(uint32_t dataRate) 
{
	_RFMDBG(1, "set datarate %lu kHz", dataRate);
	uint16_t r = ((32000000UL + (dataRate / 2)) / dataRate);
	rfm69_writeReg(RFM69_REG_BITRATEMSB, r >> 8);
	rfm69_writeReg(RFM69_REG_BITRATELSB, r & 0xFF);
}

void rfm69_clearFifo(void) 
{
	_RFMDBG(1, "clear fifo");
	rfm69_writeReg(RFM69_REG_IRQFLAGS2, RFM69_IRQFLAGS2_FIFOOVERRUN);
}

void rfm69_setOpMode(uint8_t opMode) 
{
	static const char* OpModeNames[] = { "0:SLEEP", "1:STDBY", "2:SYNTH", "3:TX", "4:RX", NULL, };
	_RFMDBG(1, "setOpMode %s", OpModeNames[opMode >> 2]);
	
	uint8_t reg = rfm69_readReg(RFM69_REG_OPMODE) & 0xE3;
	rfm69_writeReg(RFM69_REG_OPMODE, reg | opMode);
	
	if (opMode != RFM69_OPMODE_MODE_SLEEP)
	{
		rfm69_waitForRegBitSet(RFM69_REG_IRQFLAGS1, RFM69_IRQFLAGS1_MODEREADY, 1000);
	}
}

void rfm69_powerDown(void) 
{
	rfm69_setOpMode(RFM69_OPMODE_MODE_SLEEP);
}

void rfm69_standBy(void)
{
	rfm69_setOpMode(RFM69_OPMODE_MODE_STDBY);	
}

void rfm69_enableReceiver(void)
{
	rfm69_setOpMode(RFM69_OPMODE_MODE_RX);	
}

void rfm69_enableTransmitter(void)
{
	rfm69_setOpMode(RFM69_OPMODE_MODE_TX);	
}

int8_t rfm69_readTemperature(int8_t CalibrationTempVal)
{
	rfm69_standBy();

	rfm69_writeReg(RFM69_REG_TEMP1, RFM69_TEMP1_TEMPMEASSTART);

	rfm69_waitForRegBitClear(RFM69_REG_TEMP1, RFM69_TEMP1_TEMPMEASRUNNING, 1000);

	//int Temp = CalibrationTempVal - rfm69_readReg(RFM69_REG_TEMP2);
	
	//int Temp = ~rfm69_readReg(RFM69_REG_TEMP2) + CalibrationTempVal; //'complement'corrects the slope, rising temp = rising val

	int8_t deg = ~rfm69_readReg(RFM69_REG_TEMP2) + RFM69_COARSE_TEMP_COEF; 
	
	_RFMDBG(1, "Temperature=%d C", deg);

	return deg;
}
