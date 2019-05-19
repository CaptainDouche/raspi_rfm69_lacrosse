// http://www.mikrocontroller.net/articles/RFM69
// http://www.netzmafia.de/skripten/hardware/RasPi/RasPi_GPIO.html
// http://www.netzmafia.de/skripten/hardware/RasPi/RasPi_SPI.html
// https://www.kernel.org/doc/Documentation/gpio/sysfs.txt
// https://github.com/dekay/DavisRFM69
// http://www.davis-wetterstationen.de/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

#define _BSD_SOURCE
#include <unistd.h>
#include <linux/limits.h>


#include "defs.h"
#include "colorprint.h"


#include "lacrosse.h"


int  verbosity 				= 0;
bool isdaemon  				= false;
const char* pidfile			= NULL;
//const char* format 		= "{%n - %y: %t[%x]=%v %u }\\n";
//const char* format 		= "%y: %t_%x=%v %u\\n";

#define FORMAT_XDEBUG		"@Y-@d-@m, @H:@M:@S: %t_%x=%v %u\\n"
#define FORMAT_DEBUG		"@H:@M:@S: %x_%t\t%v %u\\n"
#define FORMAT_MQTT			"%x/%t=%v %u\\n"


const char* format 			= FORMAT_DEBUG;


const char* outfilename		= NULL;

void usage(const char* progname)
{
	printf("Usage: %s [OPTIONS]\n", progname);
	printf("  --format <FORMAT> may contain ...\n");
	printf("    %%x = sensor id: <hex integer>"                                     "\n");
	printf("    %%i = sensor id: <dec integer>"                                     "\n");
	printf("    %%t = sensor type: temp|humid|rain|wdir|wspeed|wgust|pressure|..."  "\n");
	printf("    %%v = sensor value: <float num>"                                    "\n");
	printf("    %%u = sensor value unit: C|%|m/s|bar|..."                           "\n");
	printf("    %%b = battery status: ok|low"                                       "\n");
	printf("    %%n = is_new status: 0|1"                                           "\n");
	printf("    %%e = error_status: 0|1"                                            "\n");
	printf("    %%y = unix timestamp"                                               "\n");
	printf("    @X  = will be passed to strftime as %%X format"                     "\n");	
	printf("    \\n = newline (and other C-string-style escapes)"                   "\n");	

	printf("  -u        unexport all GPIO ressources.\n");
	/*
	printf("  --daemon \n");
	printf("     run in background \n");
	printf("  --spidev <SPI_DEVICE_FILE> \n");
	printf("     e.g. %s \n", g_spi_devfile);
	printf("  --pcd <PIN_CHANGE_DETECT_GPIO_PINNUM> \n");
	printf("     e.g. %u \n", g_pcd_gpiopin);
	*/
	printf("Version: " COMPILE_TIMESTAMP " \n");	
}

bool scanargs(int argc, char** argv)
{
	for (int i = 1; i < argc; i++)
	{
		char* arg = argv[i];
		
		#define ARG_EQU(SHORTARG, LONGARG) \
			(STREQU(arg, "-"SHORTARG) || STREQU(arg, "--"LONGARG))

		#define IF_ARG_ASSIGN(SHORTARG, LONGARG, VAR, VAL) \
			if ARG_EQU(SHORTARG, LONGARG) { VAR = VAL; }

		#define IF_ARG_ASSIGN_STR(SHORTARG, LONGARG, STRVAR)	\
			if ARG_EQU(SHORTARG, LONGARG) { i++; STRVAR = argv[i]; }

		#define IF_ARG_ASSIGN_INT(SHORTARG, LONGARG, INTVAR)	\
			if ARG_EQU(SHORTARG, LONGARG) { i++; INTVAR = atoi(argv[i]); }

		#define IF_ARG_ASSIGN_CHAR(SHORTARG, LONGARG, CHARVAR)	\
			if ARG_EQU(SHORTARG, LONGARG) { i++; CHARVAR = *(argv[i]); }

		#define IF_ARG_ASSIGN_FLT(SHORTARG, LONGARG, FLOATVAR) \
			if ARG_EQU(SHORTARG, LONGARG) { i++; FLOATVAR = atof(argv[i]); }

		if STREQU(arg, "--") // ignore args from here
		{
			return false;
		}
		else 		
		IF_ARG_ASSIGN("d", "daemon",			isdaemon, true)
		else
		IF_ARG_ASSIGN_STR("pid", "pidfile", 	pidfile)
		else
		IF_ARG_ASSIGN_INT("v", "verbosity", 	verbosity)
		else
		IF_ARG_ASSIGN_STR("fmt", "format", 		format)
		else
		IF_ARG_ASSIGN_STR("o", "output", 		outfilename)
		else
			
		IF_ARG_ASSIGN("mqtt", "mqtt",			format, FORMAT_MQTT)
		else
		IF_ARG_ASSIGN("xdebug", "xdebug",		format, FORMAT_XDEBUG)
		else			
		
		if ARG_EQU("u", "unexport")
		{
			rfm69io_init();			
			rfm69io_deinit();
			exit(EXIT_FAILURE);
			return true;			
		}		
		else
		if ARG_EQU("h", "help")
		{
			usage(argv[0]);
			exit(EXIT_SUCCESS);
		}
		else
		{
			eprintf("unrecognized argument: %s\n", arg);
			usage(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	return false;
}

#if 1 // signal, pidfile

#include <signal.h>

volatile bool run = true;

void sighandler(int signum)
{
	if (verbosity > 0) printf("\n\n***received process signal %d ***\n", signum);
	run = false;
}

void signals_init(void)
{
	signal(SIGINT,  sighandler);
	signal(SIGTERM, sighandler);

	#ifdef _LINUX
	signal(SIGPIPE, sighandler);		
	//signal(SIGPIPE, SIG_IGN); // http://stackoverflow.com/questions/108183/how-to-prevent-sigpipes-or-handle-them-properly
	#endif
}

void signals_deinit(void)
{
}

void pidfile_init(void)
{
	if (pidfile)
	{
		FILE* f = fopen(pidfile, "wt");
		if (f)
		{
			#if (defined _WIN32)
			fprintf(f, "%u", GetCurrentProcessId());
			#elif (defined _LINUX)
			fprintf(f, "%u", getpid());
			#endif
			fclose(f);
		}
	}
}

void pidfile_deinit(void)
{
	if (pidfile)
	{
		unlink(pidfile);
	}
}

#endif

/*
char* datetimestr(time_t t, char* str, int len, const char* fmt)
{
	struct tm * timeinfo = localtime(&t);
	strftime(str, len, fmt, timeinfo);
	return str;
}

char* nowstr(void)
{
	static char str[32];
	//datetimestr(time(NULL), str, sizeof(str), "%Y-%d-%m, %H:%M:%S");
	datetimestr(time(NULL), str, sizeof(str), "%H:%M:%S");
	return str;
}
*/

char unesc(char c)
{
    const char* xlat_src = "r"   "n"   "t"   "0"   "\\"  "_"  "\"";
    const char* xlat_dst = "\r"  "\n"  "\t"  "\0"  "\\"  " "  "\"";

    for (int i=0; xlat_src[i]; i++)
    {
        if (c == xlat_src[i])
            return xlat_dst[i];
    }

    return c;
}





void print_sensor_formatted(
	FILE* outfile, const char* format, 
	const char* type, int id, 
	const char* valstr, const char* unit
)
{
    static int n = 0;

    time_t t_now = time(NULL);
    struct tm * timeinfo = NULL;
	
    const char* p = format;
    char c;
    while ((c = *p++) != '\0')
    {
    	if (c == '%')
        {
        	c = *p++;
            switch (c)
            {
                case 't':
                {
                    fputs(type, outfile); break;
                }				
                case 'x':
                {
                    fprintf(outfile, "%02x", id); break;
                }
                case 'i':
                {
                    fprintf(outfile, "%i", id); break;
                }
                case 'v': // value
                {
                    fputs(valstr, outfile); break;
                }
                case 'u': // unit
                {
                    fputs(unit, outfile); break;
                }				
                case 'y': // unixtime
                {
                    fprintf(outfile, "%lu", t_now); break;
                }
                case 'n':
                {
                    fprintf(outfile, "%i", n); break;
                }
                default:
                {
                    fputc(c, outfile); break;
                }
            }
        }
        else
    	if (c == '@')
        {
        	c = *p++;
            char strf[3] = "%_\0";
            strf[1] = c;
            char strbuf[32] = "";
            if (!timeinfo)
            {
            	timeinfo = localtime(&t_now);
            }
            if (strftime(strbuf, sizeof(strbuf), strf, timeinfo))
			{
            	fputs(strbuf, outfile);
            }
        }
        else
    	if (c == '\\')
        {
        	c = *p++;
            fputc(unesc(c), outfile);
        }
        else
        {
            fputc(c, outfile);
        }
    }

    n++;
}

void lacrosse_printsensor(FILE* outfile, const lacsensor_t* lacs)
{
	char valstr[32];

	#define PRN_SENS_FMT(TYP, VFMT, VAL, UNIT) \
		{ \
			snprintf(valstr, sizeof(valstr), VFMT, VAL); \
			print_sensor_formatted(outfile, format, TYP, lacs->id, valstr, UNIT); \
		}	
	
	#define IF_FLAG_PRN_SENS_FMT(FLAG, TYP, VFMT, VAL, UNIT) \
		if (BIT_GET(lacs->flags, FLAG)) \
		PRN_SENS_FMT(TYP, VFMT, VAL, UNIT)

	IF_FLAG_PRN_SENS_FMT(FLAG_TEMP, 		"temp", 	"%+5.1f", 	lacs->temp, 					"C");
	IF_FLAG_PRN_SENS_FMT(FLAG_HUMID, 		"humid", 	"%02u", 	lacs->humid, 					"%");
	IF_FLAG_PRN_SENS_FMT(FLAG_RAIN, 		"rain", 	"%u", 		lacs->rain, 					"mm"); // which unit?
	IF_FLAG_PRN_SENS_FMT(FLAG_WINDDIR, 		"wdir", 	"%s", 		winddir_names[lacs->winddir],	"");
	IF_FLAG_PRN_SENS_FMT(FLAG_WINDSPEED, 	"wspeed",	"%u", 		lacs->windspeed, 				"m/s"); // which unit?
	IF_FLAG_PRN_SENS_FMT(FLAG_WINDGUST, 	"wgust", 	"%u", 		lacs->windgust, 				"m/s"); // which unit?	
	
	PRN_SENS_FMT("batt_new",  "%c",  BIT_GET(lacs->flags, FLAG_NEWBATT)  ? '1':'0', "");	
	PRN_SENS_FMT("batt_weak", "%c",  BIT_GET(lacs->flags, FLAG_WEAKBATT) ? '1':'0', "");	
}



void lacrosse_loop(void)
{
	if (!lacrosse_init())
	{
		run = false;
		return;
	}	
	
	FILE* outfile = stdout;

	if (outfilename)
	{
		fclose(stdout);
		outfile = fopen(outfilename, "wt");
		setvbuf(outfile, NULL, _IONBF, 0);
		if (verbosity > 0) fprintf(outfile, "#printing to %s ...\n", outfilename);
	}

	while (run)
	{
		lacsensor_t lacs;
		if (lacrosse_receive(&lacs, 1))
		{		
			lacrosse_printsensor(outfile, &lacs);
		}
	}
	
	rfm69io_deinit();
	
	if (outfilename)
	{
		fclose(outfile);
		outfile = NULL;
	}
}

int main(int argc, char* argv[])
{
	int res = EXIT_SUCCESS;
	
	scanargs(argc, (const char**)(argv));

	if (isdaemon)
	{
		verbosity = 0;
		daemon(false, false);
	}

	pidfile_init();

	signals_init();
	
	//lacrosse_demo();
	lacrosse_loop();
	
	signals_deinit();

	pidfile_deinit();

	return res;
}