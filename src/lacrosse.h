#ifndef LACROSSE_h
#define LACROSSE_h

#include <stdint.h>
#include <stdbool.h>
#include "defs.h"
#include "rfm69io.h"

enum
{
	FLAG_TEMP,
	FLAG_HUMID,
	FLAG_RAIN,	
	FLAG_WINDSPEED,	
	FLAG_WINDGUST,		
	FLAG_WINDDIR,	
	FLAG_NEWBATT, // data acquisition phase
	FLAG_WEAKBATT,
	FLAG_ERROR,	// e.g. wind sensor not present
	// more types here
};

#define WINDDIR_NAMES	"N  ", "NNE", "NE ", "ENE", "E  ", "ESE", "SE ", "SSE", "S  ", "SSW", "SW ", "WSW", "W  ", "WNW", "NW ", "NNW"
extern const char winddir_names[16][4];



typedef struct _lacsensor_t
{
	int id;
	long int flags; 
	float temp;
	int humid;
	int rain;	
	int windspeed;
	int windgust;	
	int winddir;	
}	lacsensor_t;

bool lacrosse_init(void);

bool lacrosse_receive(lacsensor_t* lacs, int timeout_s);

//void lacrosse_printsensor(const lacsensor_t* lacs);

void lacrosse_demo(void);

#endif // #ifndef LACROSSE_h





/*


WH1080 protocol V2 – FSK
2013/03/022013/03/03 Frank Weather stations	

The Fine Offset weather station switched over to a new RF transmission protocol somewhere in 2012. While the old protocol was a On-Off-Keying (OOK) protocol, the V2 protocol used Frequency-Shift-Keying (FSK). This is natural transmision mode of the RFM02 transmitters and their RFM01 receivers. The RFM12B modules can also receive the same FSK signals. Most of the reverse engineering happened on the Raspberry Pi forum: WH1080 V2 protocol decoded

This post details on the exact transmission protocol.


Package definition:
[
preample 3 bytes 0xAA    synchron word    payload 10 bytes  postample 11bits zero
0xAA    0xAA    0xAA     0x2D    0xD4     nnnnn---nnnnnnnnn 0x00     0x0
101010101010101010101010 0010110111010100 101.............. 00000000 000
]
repeated six times (identical packages) per transmission every 48 seconds
There is no or hardly any spacing between the packages.
Spacing: to be confirmed.
When using the RFM01 or RFM12B, the preample and synchron word will not be in the received data. The preamble is intended to have the frequency synthesiser locked, while the synchron word serves as detection of the proper message.
Open issue: How can an end-of-transmission be detected? Would VDI turn into zero?
The payload contains to types of messages for FO WH1080, and relatives:

Payload definition:
Weather sensor reading Message Format:
AAAABBBBBBBBCCCCCCCCCCCCDDDDDDDDEEEEEEEEFFFFFFFFGGGGHHHHHHHHHHHHIIIIJJJJKKKKKKKK
0xA4    0xF0    0x27    0x47    0x00    0x00    0x03    0xC6    0x0C    0xFE
10100100111100000010011101000111000000000000000000000011110001100000110011111110

with:
AAAA = 1010    Message type: 0xA: sensor readings
BBBBBBBB       Station ID / rolling code: Changes with battery insertion.
CCCCCCCCCCCC   Temperature*10 in celsius. Binary format MSB is sign
DDDDDDDD       Humidity in %. Binary format 0-100. MSB (bit 7) unused.
EEEEEEEE       Windspeed
FFFFFFFF       Wind gust
GGGG           Unknown
HHHHHHHHHHHH   Rainfall cumulative. Binary format, max = 0x3FF,
IIII           Status bits: MSB b3=low batt indicator.
JJJJ           Wind direction
KKKKKKKK       CRC8 - reverse Dallas One-wire CRC

DCF Time Message Format:
AAAABBBBBBBBCCCCDDEEEEEEFFFFFFFFGGGGGGGGHHHHHHHHIIIJJJJJKKKKKKKKLMMMMMMMNNNNNNNN
Hours Minutes Seconds Year       MonthDay      ?      Checksum
0xB4    0xFA    0x59    0x06    0x42    0x13    0x43    0x02    0x45    0x74

with:
AAAA = 1011    Message type: 0xB: DCF77 time stamp
BBBBBBBB       Station ID / rolling code: Changes with battery insertion.
CCCC           Unknown
DD             Unknown
EEEEEE         Hours, BCD
FFFFFFFF       Minutes, BCD
GGGGGGGG       Seconds, BCD
HHHHHHHH       Year, last two digits, BCD
III            Unknown
JJJJJ          Month number, BCD
KKKKKKKK       Day in month, BCD
L              Unknown status bit
MMMMMMM        Unknown
NNNNNNNN       CRC8 - reverse Dallas One-wire CRC
The DCF code is transmitted five times with 48 second intervals between 3-6 minutes past a new hour. The sensor data transmission stops in the 59th minute. Then there are no transmissions for three minutes, apparently to be noise free to acquire the DCF77 signal. On similar OOK weather stations the DCF77 signal is only transmitted every two hours.

The package format was deduced using a long transmision buffer on a JeeNode with some modificaitons in the RF12 driver.
The payload definitions have been described at those pages:
WH1080 V1 OOK protocol
WH1080 V2 FSK protocol
WH1080 V1 OOK and DCF77 message format

This applies at least to the following (later) models:
Fine Offset WH1080
Alecto WS4000
National Geographic 265, at 916MHz
868MHz FSK OOK rf protocol wh1080
7 thoughts to “WH1080 protocol V2 – FSK”

    Rinie says:	
    2013/03/30 at 09:17

    Cool!
    While trying to decode IT+ in http://forum.jeelabs.net/node/110
    I received data from my Alecto WS4000.
    Now your sketch http://jeelabs.net/boards/6/topics/1203
    decodes that. So I tried adding the IT+ decoding (packets starting with 0x9 instead of 0xA), it even uses the same crc calculation! See http://fredboboss.free.fr/tx29/index.php?lang=en for IT+ details.
    After that I took a shot at decoding the WS1600. Guessing from http://www.g-romahn.de/ws1600 that it uses half the speed of the IT+, that also works.
    As both WS1600 and FineOffset use packets with 0xA, the code cannot decode both in the same setting.
    WS1600 packages have a variable length, FineOffset repeats packages of 10 bytes very fast.

    Result is at https://github.com/rinie/weatherstationFSK.
    Thank you very much!
    Reply
    Frank says:	
    2013/03/30 at 20:19

    Hi Rinie,

    This is also very interesting work! I was totally unaware of the jeelabs old forum thread, despite google being my friend. I need some time to catch up. Thanks.

    SevenW
    Reply
    Tom R says:	
    2013/09/15 at 14:40

    Hi Guys,

    Great work, a quick question if I may, I have 2 * WH1081PC indoor units, one of the old type(outside unit got damaged) & also a complete new unit, the new indoor unit is picking up the new outside unit but the old indoor unit is not, is there any way to make both old & new indoor units work with a new outdoor unit? any jumpers or anything I can change in the old unit to accept new protocol?

    Thanks

    Tom
    Reply
    Frank says:	
    2013/09/15 at 17:21

    Hi Tom,

    When your old unit uses the OOK protocol, and your new unit the FSK protocol, then there is no chance that you get the old indoor unit to work with the new outdoor unit. When both use the same protocol, for example both FSK, then it should already work when you follow the regular pairing procedure.

    SevenW
    Reply
    Pingback: Raspberry Pi reading WH1081 weather sensors using an RFM01 and RFM12b | SusaNET
    Marius says:	
    2014/12/12 at 20:51

    Hello,

    Using your code I’m able to correctly receive the data from my station, a WX-2013. But how about the barometric pressure ?
    The indoors console displays it. Could it be the sensor is in the indoor console ?



*/