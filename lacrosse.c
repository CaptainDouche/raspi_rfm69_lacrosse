#include "lacrosse.h"

// https://jeelabs.net/projects/jeelib/wiki
// https://wiki.fhem.de/wiki/JeeLink
// https://svn.fhem.de/trac/browser/trunk/fhem/contrib/arduino

// http://nikseresht.com/blog/?p=99
// http://fredboboss.free.fr/articles/tx29.php/
// https://github.com/rinie/LaCrosseITPlusReader
// https://github.com/rinie/LaCrosseITPlusReader/blob/master/WS1600.cpp

// https://www.sevenwatt.com/main/wh1080-protocol-v2-fsk/
// https://jeelabs.net/boards/6/topics/1203
// https://www.raspberrypi.org/forums/viewtopic.php?f=37&t=14777&start=175


#include <stdint.h>
#include <stdio.h>

#include "defs.h"
#include "rfm69io.h"

#define PAYLOADSIZE					24 // maximum of any lacross packets (?)

// see https://wiki.fhem.de/wiki/JeeLink#Unterst.C3.BCtzte_Sensoren_und_Aktoren_incl._Wetterstation_WS_1600
#define DATARATE_TX29DTH_IT 		17241
#define DATARATE_WS_1600_TX22		8842

// https://forum.fhem.de/index.php/topic,14786.msg363766.html#msg363766
// Die WS 1080 gibt es (unter gleichem Namen) in einer OOK- und in einer FSK-Version
#define DATARATE_WS_1080  			17241 

#define DATARATE					DATARATE_TX29DTH_IT	

#define FREQUENCY_KHZ				868300




static bool lacrosse_rfmrecv(uint8_t* payload, uint16_t size)
{
    if (rfm69_readReg(RFM69_REG_IRQFLAGS2) & RFM69_IRQFLAGS2_PAYLOADREADY)
	{
		rfm69_standBy();
		rfm69_readFifo(payload, size);
		rfm69_enableReceiver();
		return true;
    }
	else
	{
		return false;
	}
}

bool lacrosse_init(void)
{
	//RFM69_VERBOSITY(10);
	
	if (!rfm69io_init())
		return false;
	
	static const uint8_t iniregs[] =
	{
		RFM69_REG_OPMODE, 			RFM69_OPMODE_SEQUENCER_ON | RFM69_OPMODE_LISTEN_OFF | RFM69_OPMODE_MODE_STDBY,
		RFM69_REG_DATAMODUL, 		RFM69_DATAMODUL_DATAMODE_PACKET | RFM69_DATAMODUL_MODULATIONTYPE_FSK | RFM69_DATAMODUL_MODULATIONSHAPING_FSK_NONE,
		RFM69_REG_FDEVMSB, 			RFM69_FDEVMSB_90000,
		RFM69_REG_FDEVLSB, 			RFM69_FDEVLSB_90000,
		RFM69_REG_PALEVEL, 			RFM69_PALEVEL_PA0_ON | RFM69_PALEVEL_PA1_OFF | RFM69_PALEVEL_PA2_OFF | RFM69_PALEVEL_OUTPUTPOWER_11111,
		RFM69_REG_OCP, 				RFM69_OCP_OFF,
		RFM69_REG_RXBW, 			RFM69_RXBW_DCCFREQ_010 | RFM69_RXBW_MANT_16 | RFM69_RXBW_EXP_2,
		RFM69_REG_DIOMAPPING1, 		RFM69_DIOMAPPING1_DIO0_01,
		RFM69_REG_IRQFLAGS2, 		RFM69_IRQFLAGS2_FIFOOVERRUN, // -> clear fifo
		RFM69_REG_RSSITHRESH, 		220,
		RFM69_REG_SYNCCONFIG, 		RFM69_SYNC_ON | RFM69_SYNC_FIFOFILL_AUTO | RFM69_SYNC_SIZE_2 | RFM69_SYNC_TOL_0,
		RFM69_REG_SYNCVALUE1, 		0x2D,
		RFM69_REG_SYNCVALUE2, 		0xD4,
		RFM69_REG_PACKETCONFIG1, 	RFM69_PACKET1_CRCAUTOCLEAR_OFF,
		RFM69_REG_PAYLOADLENGTH, 	PAYLOADSIZE,
		RFM69_REG_FIFOTHRESH, 		RFM69_FIFOTHRESH_TXSTART_FIFONOTEMPTY | RFM69_FIFOTHRESH_VALUE,
		RFM69_REG_PACKETCONFIG2, 	RFM69_PACKET2_RXRESTARTDELAY_2BITS | RFM69_PACKET2_AUTORXRESTART_ON | RFM69_PACKET2_AES_OFF,
		RFM69_REG_TESTDAGC, 		RFM69_DAGC_IMPROVED_LOWBETA0,		
		
		/*
		RFM69_REG_OPMODE, 			RFM69_OPMODE_SEQUENCER_ON | RFM69_OPMODE_LISTEN_OFF | RFM69_OPMODE_STANDBY,
		RFM69_REG_DATAMODUL, 		RFM69_DATAMODUL_DATAMODE_PACKET | RFM69_DATAMODUL_MODULATIONTYPE_FSK | RFM69_DATAMODUL_MODULATIONSHAPING_00,
		RFM69_REG_FDEVMSB, 			RFM69_FDEVMSB_90000,
		RFM69_REG_FDEVLSB, 			RFM69_FDEVLSB_90000,
		RFM69_REG_PALEVEL, 			RFM69_PALEVEL_PA0_ON | RFM69_PALEVEL_PA1_OFF | RFM69_PALEVEL_PA2_OFF | RFM69_PALEVEL_OUTPUTPOWER_11111,
		RFM69_REG_OCP, 				RFM69_OCP_OFF,
		RFM69_REG_RXBW, 			RFM69_RXBW_DCCFREQ_010 | RFM69_RXBW_MANT_16 | RFM69_RXBW_EXP_2,
		RFM69_REG_DIOMAPPING1, 		RFM69_DIOMAPPING1_DIO0_01,
		RFM69_REG_IRQFLAGS2, 		RFM69_IRQFLAGS2_FIFOOVERRUN, // -> clear fifo
		RFM69_REG_RSSITHRESH, 		220,
		RFM69_REG_SYNCCONFIG, 		RFM69_SYNC_ON | RFM69_SYNC_FIFOFILL_AUTO | RFM69_SYNC_SIZE_2 | RFM69_SYNC_TOL_0,
		RFM69_REG_SYNCVALUE1, 		0x2D,
		RFM69_REG_SYNCVALUE2, 		0xD4,
		RFM69_REG_PACKETCONFIG1, 	RFM69_PACKET1_CRCAUTOCLEAR_OFF,
		RFM69_REG_PAYLOADLENGTH, 	PAYLOADSIZE,
		RFM69_REG_FIFOTHRESH, 		RFM69_FIFOTHRESH_TXSTART_FIFONOTEMPTY | RFM69_FIFOTHRESH_VALUE,
		RFM69_REG_PACKETCONFIG2, 	RFM69_PACKET2_RXRESTARTDELAY_2BITS | RFM69_PACKET2_AUTORXRESTART_ON | RFM69_PACKET2_AES_OFF,
		RFM69_REG_TESTDAGC, 		RFM69_DAGC_IMPROVED_LOWBETA0,
		*/
	};
	
	
	for (int i=0; i<ELEMCNT(iniregs); i += 2)
	{
		rfm69_writeReg(iniregs[i], iniregs[i+1]);
	}
	
	rfm69_setFrequency(FREQUENCY_KHZ);
	rfm69_setDataRate(DATARATE);
	rfm69_clearFifo();
	
	rfm69_readTemperature(0);
	
	rfm69_enableReceiver();
	//RFM69_VERBOSITY(0);
	
	return true;
}

void lacrosse_deinit(void)
{
	rfm69io_deinit();
}

static uint8_t lacrosse_chrckcrc(const uint8_t* data, uint16_t len) // (Generatorplonom x8 + x5 + x4 + 1) ?
{
	uint16_t i, j;
	uint8_t res = 0;
	for (j = 0; j < len; j++) 
	{
		uint8_t val = data[j];
		for (i = 0; i < 8; i++) 
		{
			uint8_t tmp = (uint8_t)((res ^ val) & 0x80);
			res <<= 1;
			if (0 != tmp) 
			{
				res ^= 0x31;
			}
			val <<= 1;
		}
	}
	return res;
}


const char winddir_names[16][4] = { WINDDIR_NAMES };

bool lacrosse_decode(const uint8_t* data, uint16_t len, lacsensor_t* lacs)
{
	static unsigned int cnt = 0;
	
	#define HINIB(I8)	((I8) >> 4)
	#define LONIB(I8)	((I8)  & 0x0f)

	/*
	printf("~cls\n");	
	for (int i=0; i<len; i++)
	{
		printf("%02x ", data[i] & 0xff);
	}
	printf(".\n");
	*/
	
	if (len < 5) 
	{	
		eprintf("Data too short!\n");
		return false;
	}
	
	lacs->flags = 0;
	
	if (HINIB(data[0]) == 0x9)
	{
		// Message Format:
		// 
		// .- [0] -. .- [1] -. .- [2] -. .- [3] -. .- [4] -.
		// |       | |       | |       | |       | |       |
		// SSSS.DDDD DDN_.TTTT TTTT.TTTT WHHH.HHHH CCCC.CCCC
		// |  | |     ||  |  | |  | |  | ||      | |       |
		// |  | |     ||  |  | |  | |  | ||      | `--------- CRC
		// |  | |     ||  |  | |  | |  | |`-------- Humidity
		// |  | |     ||  |  | |  | |  | |
		// |  | |     ||  |  | |  | |  | `---- weak battery
		// |  | |     ||  |  | |  | |  |
		// |  | |     ||  |  | |  | `----- Temperature T * 0.1
		// |  | |     ||  |  | |  |
		// |  | |     ||  |  | `---------- Temperature T * 1
		// |  | |     ||  |  |
		// |  | |     ||  `--------------- Temperature T * 10  + 40 (to avoid negative numbers)
		// |  | |     | `--- new battery
		// |  | `---------- ID
		// `---- START = 9
		// 
		
		bool crcok = lacrosse_chrckcrc(data, 4) == data[4];
		
		if (crcok)
		{
			lacs->flags = _BV(FLAG_TEMP) | _BV(FLAG_HUMID);
			
			lacs->id = (LONIB(data[0])<<4) | (HINIB(data[1]) & 0b1100); // id is shown this way in display!
			
			if (BIT_GET(data[1], 5))
				BIT_SET(lacs->flags, FLAG_NEWBATT);
			
			if (BIT_GET(data[3], 7))
				BIT_SET(lacs->flags, FLAG_WEAKBATT);

			float temp = (100 * LONIB(data[1])) + (10 * HINIB(data[2])) + (1 * LONIB(data[2]));
			temp = (temp - 400) / 10;			
			
			lacs->temp = temp;
			lacs->humid = data[3] & 0x7f;

			return true;
		}
		else		
		{
			eprintf("SensorType 0x9 - CRC fault!\n");
			return false;
		}
	}
	else
	if (HINIB(data[0]) == 0xa)
	{
		// ws1600 - TX22 - datarate= 
		
		// uint8_t ws1600_testdata = { 0xa5, 0xa5, 0x06, 0x28, 0x10, 0x33, 0x20, 0x00, 0x3e, 0x00, 0x40, 0x00, 0xbd};
		
		// Data - organized in nibbles - are structured as follows (exammple with blanks added for clarity):
		//  a 5a 5 0 628 1 033 2 000 3 e00 4 000 bd
		// [0 01 1 2 233
		// data always start with "a"
		// from next 1.5 nibbles (here 5a) the 6 msb are identifier of transmitter,
		// bit 1 indicates acquisition/synchronizing phase (so 5a >> 58 thereafter)
		// bit 0 will be 1 in case of error (e.g. no wind sensor 5a >> 5b)
		// next nibble (here 5) is count of quartets to betransmitted
		// up to 5 quartets of data follow
		// each quartet starts with type indicator (here 0,1,2,3,4)
		// 0: temperature, 3 nibbles bcd coded tenth of °c plus 400 (here 628-400 = 22.8°C)
		// 1: humidity, 3 nibbles bcd coded (here 33 %rH), meaning of 1st nibble still unclear
		// 2: rain, 3 nibbles, counter of contact closures
		// 3: wind, first nibble direction of wind vane (multiply by 22.5 to obtain degrees,
		//    here 0xe*22.5 = 315 degrees)
		// 	next two nibbles wind speed in m per sec (i.e. no more than 255 m/s; 9th bit still not found)
		// 4: gust, speed in m per sec (yes, TX23 sensor does measure gusts and data are transmitted
		//    but not displayed by WS1600), number of significant nibbles still unclear
		// next two bytes (here bd) are crc.
		// During  acquisition/synchronizing phase (abt. 5 hours) all 5 quartets are sent, see examplke above. Thereafter
		// data strings contain only a few ( 1 up ton 3) quartets, so data strings are not! always of
		// equal length.
		// After powering on, the complete set of data will be transmitted every 4.5 secs for 5 hours during acquisition phase.
		// Lateron only selected sets of data will be transmitted.
		
		lacs->id = (LONIB(data[0])<<4) | (HINIB(data[1]) & 0b1100);
		
		if (BIT_GET(data[1], 5))
			BIT_SET(lacs->flags, FLAG_NEWBATT);		
		
		if (BIT_GET(data[1], 4))
			BIT_SET(lacs->flags, FLAG_ERROR);	
		
		int datasets = LONIB(data[1]);
		
		if (datasets > 5)
		{
			eprintf("SensorType 0xa - too many data sets?\n");
			return false;
		}		
		
		int length   = 2 + datasets*2;
		
		bool crcok = lacrosse_chrckcrc(data, length) == data[length];
		
		if (crcok)
		{

			
			for (int i=0; i<datasets; i++)
			{
				int j = 2 + 2*i;
				switch(HINIB(data[j]))
				{
					case 0:
					{
						BIT_SET(lacs->flags, FLAG_TEMP);					
						float temp = (100 * LONIB(data[j])) + (10 * HINIB(data[j+1])) + (1 * LONIB(data[j+1]));
						temp = (temp - 400) / 10;
						lacs->temp = temp;
						break;
					}
					case 1:
					{
						BIT_SET(lacs->flags, FLAG_HUMID);					
						lacs->humid = (10 * HINIB(data[j+1])) + (1 * LONIB(data[j+1]));
						break;
					}
					case 2:
					{
						BIT_SET(lacs->flags, FLAG_RAIN);
						#warning Encoding of rain not clearly specified!
						// vgl. https://github.com/rinie/LaCrosseITPlusReader/blob/master/WS1600.cpp
						lacs->rain = ((data[j] & 0x0F) << 8) | (data[j+1]);
						break;
					}
					case 3:
					{
						BIT_SET(lacs->flags, FLAG_WINDDIR);
						BIT_SET(lacs->flags, FLAG_WINDSPEED);	
						lacs->winddir   = LONIB(data[j]);
						lacs->windspeed = data[j+1];
						break;
					}
					case 4:
					{
						BIT_SET(lacs->flags, FLAG_WINDGUST);
						#warning Encoding of wind gust not clearly specified! 
						// vgl. https://github.com/rinie/LaCrosseITPlusReader/blob/master/WS1600.cpp
						lacs->windgust = ((data[j] & 0x0F) << 8) | (data[j+1]);
						break;
					}
				}
				
			}		
			return true;
		}
		else		
		{
			eprintf("SensorType 0xa - CRC fault!\n");
			return false;
		}
	}
	else
	{
		/*
		for (int i=0; i<len; i++)
		{
			printf("%02x ", data[i] & 0xff);
		}
		printf(";\n");
		*/
	}

	return false;
}

bool lacrosse_receive(lacsensor_t* lacs, int timeout)
{
	if (rfm69_waitForIrq(timeout, 0))
	{		
		uint8_t payload[PAYLOADSIZE];
		if (lacrosse_rfmrecv(payload, PAYLOADSIZE))
		{
			return lacrosse_decode(payload, PAYLOADSIZE, lacs);
		}
	}	
	
	return false;
}




#include <time.h>

extern volatile bool run;


void lacrosse_demo_printsensor(const lacsensor_t* lacs)
{
	printf("id=%02x ", lacs->id & 0xff);
	
	if (BIT_GET(lacs->flags, FLAG_TEMP))		printf("temp=%+5.1fC ", lacs->temp);
	if (BIT_GET(lacs->flags, FLAG_HUMID))		printf("humid=%2u%% ", lacs->humid);
	if (BIT_GET(lacs->flags, FLAG_RAIN))		printf("rain=%u ", lacs->rain);			
	                                            
	if (BIT_GET(lacs->flags, FLAG_WINDDIR))		printf("wdir=%u(%s) ", lacs->winddir, winddir_names[lacs->winddir]);					
	if (BIT_GET(lacs->flags, FLAG_WINDSPEED))	printf("wspeed=%u ", lacs->windspeed);				
	if (BIT_GET(lacs->flags, FLAG_WINDGUST))	printf("wgust=%u ", lacs->windgust);			

	if (BIT_GET(lacs->flags, FLAG_NEWBATT))		printf("NEW_BATT ");				
	if (BIT_GET(lacs->flags, FLAG_WEAKBATT))	printf("WEAK_BATT ");
	if (BIT_GET(lacs->flags, FLAG_ERROR))		printf("SENS_ERR ");
}

void lacrosse_demo(void)
{
	/*
	
	// test for
	// https://github.com/rinie/LaCrosseITPlusReader/blob/master/WS1600.cpp
	
	lacsensor_t lacs;
	uint8_t ws1600_testdata[] = { 0xa5, 0xa5, 0x06, 0x28, 0x10, 0x33, 0x20, 0x00, 0x3e, 0x00, 0x40, 0x00, 0xbd};
	if (lacrosse_decode(ws1600_testdata, sizeof(ws1600_testdata), &lacs))
	{
		lacrosse_printsensor(&lacs);
	}
	else
	{
		eprintf("ws1600_testdata failed!\n");
	}
	
	return;
	*/

	lacrosse_init();
	
	setvbuf(stdout, NULL, _IONBF, 0);
	time_t t_prev = 0;
	
	while (run)
	{
		lacsensor_t lacs;
		if (lacrosse_receive(&lacs, 1))
		{		
			time_t t = time(NULL);
			printf("time=%lu dt=%u \t", t, (t - t_prev));
			
			lacrosse_demo_printsensor(&lacs);

			printf("\n");
			fflush(stdout);	
			
			t_prev = t;
		}	
	}
	
	rfm69io_deinit();	
}

