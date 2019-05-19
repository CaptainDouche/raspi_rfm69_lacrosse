#ifndef LACROSSE_h
#define LACROSSE_h

#include <stdint.h>
#include "defs.h"


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

bool lacrosse_receive(lacsensor_t* lacs, int timeout);

//void lacrosse_printsensor(const lacsensor_t* lacs);

void lacrosse_demo(void);

#endif // #ifndef LACROSSE_h