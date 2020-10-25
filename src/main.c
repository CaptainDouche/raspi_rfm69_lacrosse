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
#include <stdbool.h>
#include <stdarg.h>

#define _BSD_SOURCE
#include <unistd.h>
#include <linux/limits.h>

#include "defs.h"
#include "colorprint.h"
#include "lacrosse.h"

#define FORMAT_XDEBUG		"@Y-@d-@m, @H:@M:@S: %t_%x=%v %u"
#define FORMAT_DEBUG		"@H:@M:@S: %x_%t\t%v %u"
#define FORMAT_MQTT			"rf/%x/%t=%v %u"

int  verbosity 				= 0;
const char* pidfile			= NULL;
const char* format 			= FORMAT_DEBUG;

#if (USE_MOSQUITTO == 1)
	
#include <mosquitto.h>
// $ sudo apt-get install libmosquitto-dev
// compile with -lmosquitto

char* host 					= NULL; // "localhost";
unsigned short port 		= 1883;
const char* username		= NULL;
const char* password		= NULL;
const char* client_id		= NULL;
const char* will_topic		= NULL;
const char* will_message	= NULL;
bool retain  				= false;
int keepalive				= 10; // der wert 0 ist wohl nicht erlaubt?
int qos						= 0;
bool clean_session 			= true;

struct mosquitto *mosq		= NULL;
	
void mosq_callback_log(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
	// static const struct num2name loglevelnames[] =
	// {
		// { MOSQ_LOG_DEBUG   , "DEBUG  " },
		// { MOSQ_LOG_INFO    , "INFO   " },
		// { MOSQ_LOG_NOTICE  , "NOTICE " },
		// { MOSQ_LOG_WARNING , "WARNING" },
		// { MOSQ_LOG_ERR     , "ERR    " },
		// { 0                , NULL      },
	// };

	//printf(TSFMT "%s: %s\n", TickStamp(), lookup_name(loglevelnames, level), str);
	//if (verbosity > 0) printf("LOG: %s: %s\n", lookup_name(loglevelnames, level), str);
	if (verbosity > 0) printf("LOG: (LEV_%02x) %s\n", level, str);
	
	// dirty workaround...
	if (strstr(str, "received CONNACK"))
	{
		//mosq_callback_connect_ack(mosq);
	}	
}

void mosq_callback_connect(struct mosquitto *mosq, void *obj, int result)
{
	if (verbosity > 0) printf("connect callback: result=%d\n", result);
}
	
bool mqtt_init(void)
{
	mosquitto_lib_init();

	mosq = mosquitto_new(NULL, clean_session, NULL);
	if (mosq)
	{
		mosquitto_log_callback_set(mosq, mosq_callback_log);
		//mosquitto_message_callback_set(mosq, mosq_callback_message);
		mosquitto_connect_callback_set(mosq, mosq_callback_connect);
		//mosquitto_connect_with_flags_callback_set(mosq, mosq_callback_connect);

		if (username)
		{
			mosquitto_username_pw_set(mosq, username, password);
		}

		// mosquitto_will_set(...)
		if (verbosity > 0) printf("Connecting to %s:%u (keepalive=%u) ... \n", host, port, keepalive);
		
		// https://mosquitto.org/api/files/mosquitto-h.html#mosquitto_connect
		int rc = mosquitto_connect(mosq, host, port, keepalive);
		if (rc == MOSQ_ERR_SUCCESS)
		{
			if (verbosity > 0) printf("connected.\n");
		}
		else 
		{
			eprintf("Connecting to %s:%u failed (%d).\n", host, port, rc);
			if (rc == MOSQ_ERR_INVAL)
			{
				eprintf("MOSQ_ERR_INVAL.\n");
			}
			else 
			if (rc == MOSQ_ERR_ERRNO)		
			{
				eprintf("MOSQ_ERR_ERRNO: %d: %s\n", errno, strerror(errno));
			}
			return false;
		}

		/*
		int loop = mosquitto_loop_start(mosq); // will start a thread
		if (loop != MOSQ_ERR_SUCCESS)
		{
			eprintf("Unable to start loop: %i\n", loop);
			return false;
		}
		*/

		return true;
	}
	else
	{
		eprintf("mosquitto_new failed.\n");
		return false;
	}
}

void mqtt_deinit(void)
{
	if (verbosity > 0) printf("disconnecting ...\n");

	if (mosq)
	{
		mosquitto_disconnect(mosq);
		mosquitto_destroy(mosq);
		mosq = NULL;
	}

	mosquitto_lib_cleanup();
}
	
#endif




void usage(const char* progname)
{
	printf("Usage: %s [OPTIONS]\n", progname);
	printf("  -irq <GPIONUM>       use GPIO Pin <GPIONUM> for IRQ.\n");
	printf("  -noirq               dont use an GPIO Pin for IRQ. (uses polling)\n");	
	printf("  -ux                  unexport all GPIO ressources (if after crashes sth is blocked).\n");	
	printf("  -format <FORMAT>     format the output. may contain ...\n");
	printf("     %%x = sensor id: <hex integer>"                                     "\n");
	printf("     %%i = sensor id: <dec integer>"                                     "\n");
	printf("     %%t = sensor type: temp|humid|rain|wdir|wspeed|wgust|pressure|..."  "\n");
	printf("     %%v = sensor value: <float num>"                                    "\n");
	printf("     %%u = sensor value unit: C|%|m/s|bar|..."                           "\n");
	printf("     %%b = battery status: ok|low"                                       "\n");
	printf("     %%n = is_new status: 0|1"                                           "\n");
	printf("     %%e = error_status: 0|1"                                            "\n");
	printf("     %%y = unix timestamp"                                               "\n");
	printf("     @X  = will be passed to strftime as %%X format"                     "\n");	
	printf("     \\n = newline (and other C-string-style escapes)"                   "\n");	
#if (USE_MOSQUITTO == 1)
	printf("  -mqtt:host <HOST>           connect to a MQTT broker at <HOST>\n");	
	printf("  -mqtt:port <PORT>           \n");
	printf("  -mqtt:username <USERNAME>   \n");
	printf("  -mqtt:password <PASSWORD>   \n");
#endif
	printf("Version: " COMPILE_TIMESTAMP " \n");	
}

bool scanargs(int argc, char** argv)
{
	for (int i = 1; i < argc; i++)
	{
		char* arg = argv[i];
		
		while (*arg == '-')
			arg++;

		#define ARG_EQU(LIT) (STREQU(arg, LIT))
		
		if (ARG_EQU("fmt") || ARG_EQU("format"))
		{
			i++;
			format = argv[i];
		}
		else
		if (ARG_EQU("irq") || ARG_EQU("irq_gpio"))
		{
			i++;
			rfm69_irq_gpionum = atoi(argv[i]);
		}
		else
		if (ARG_EQU("noirq") || ARG_EQU("no_irq_gpio"))
		{
			rfm69_irq_gpionum = -1;
		}
		else			
		if (ARG_EQU("v") || ARG_EQU("verbosity"))
		{
			i++;
			verbosity = atoi(argv[i]);
		}
		else
		if (ARG_EQU("pidfile"))
		{
			i++;
			pidfile = argv[i];
		}
		else			
		if (ARG_EQU("debug"))
		{
			format = FORMAT_XDEBUG;
			verbosity = 10;
		}
		else
		if (ARG_EQU("daemon"))
		{
			daemon(false, false);
		}
		else			
		
#if (USE_MOSQUITTO == 1)
		if (ARG_EQU("mqtt")) // will enable mqtt with default settings
		{
			host = "localhost";
			format = FORMAT_MQTT;
		}
		else	
		if (ARG_EQU("mqtt:host"))
		{
			i++;
			host = argv[i];
		}
		else
		if (ARG_EQU("mqtt:port"))
		{
			i++;
			port = atoi(argv[i]);
		}
		else
		if (ARG_EQU("mqtt:username"))
		{
			i++;
			username = argv[i];
		}
		else			
		if (ARG_EQU("mqtt:password"))
		{
			i++;
			password = argv[i];
		}
		else
		if (ARG_EQU("mqtt:client_id"))
		{
			i++;
			client_id = argv[i];
		}
		else
		if (ARG_EQU("mqtt:will_topic"))
		{
			i++;
			will_topic = argv[i];
		}
		else
		if (ARG_EQU("mqtt:will_message"))
		{
			i++;
			will_message = argv[i];
		}
		else
		if (ARG_EQU("mqtt:retain"))
		{
			i++;
			retain = atoi(argv[i]);
		}
		else
		if (ARG_EQU("mqtt:keepalive"))
		{
			i++;
			keepalive = atoi(argv[i]);
		}
		else
		if (ARG_EQU("mqtt:qos"))
		{
			i++;
			qos = atoi(argv[i]);
		}
		else
		if (ARG_EQU("mqtt:clean_session"))
		{
			i++;
			clean_session = atoi(argv[i]);
		}
		else
#endif
		if (ARG_EQU("ux") || ARG_EQU("unexport"))
		{
			rfm69io_init();			
			rfm69io_deinit();
			exit(EXIT_SUCCESS);
			return true;			
		}		
		else
		if (ARG_EQU("h") || ARG_EQU("help"))
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
	char* str, int len,
	const char* format, 	
	const char* type, int id, 
	const char* valstr, const char* unit
)
{
    time_t t_now = time(NULL);
    struct tm * timeinfo = NULL;
	
    const char* p = format;
    char c;
	
	int l = 0;
    while ((c = *p++) != '\0')
    {
    	if (c == '%')
        {
        	c = *p++;
            switch (c)
            {
                case 't':
                {
                    //fputs(type, outfile); break;
                    l += snprintf(str+l, len-l, "%s", type); break;
                }				
                case 'x':
                {
                    //fprintf(outfile, "%02x", id); break;
                    l += snprintf(str+l, len-l, "%02x", id); break;
                }
                case 'i':
                {
                    //fprintf(outfile, "%i", id); break;
                    l += snprintf(str+l, len-l, "%i", id); break;
                }
                case 'v': // value
                {
                    //fputs(valstr, outfile); break;
					l += snprintf(str+l, len-l, "%s", valstr); break;
                }
                case 'u': // unit
                {
                    //fputs(unit, outfile); break;
					l += snprintf(str+l, len-l, "%s", unit); break;
                }				
                case 'y': // unixtime
                {
                    //fprintf(outfile, "%lu", t_now); break;
                    l += snprintf(str+l, len-l, "%lu", t_now); break;
                }
                default:
                {
                    //fputc(c, outfile); break;
                    str[l++] = c;
					str[l]   = '\0';
                }
            }
        }
        else
    	if (c == '@')
        {
            if (!timeinfo)
            {
            	timeinfo = localtime(&t_now);
            }
			
        	c = *p++;
            char strf[3] = "%_\0";
            strf[1] = c;

            //char strbuf[32] = "";
            // if (strftime(strbuf, sizeof(strbuf), strf, timeinfo))
			// {
            	// fputs(strbuf, outfile);
            // }
			l += strftime(str+l, len-l, strf, timeinfo);
			
        }
        else
    	if (c == '\\')
        {
        	c = *p++;
            //fputc(unesc(c), outfile);
			str[l++] = unesc(c);
			str[l]   = '\0';
        }
        else
        {
            //fputc(c, outfile);
			str[l++] = c;
			str[l]   = '\0';			
        }
    }
}



//void lacrosse_printvalue(FILE* outfile, const lacsensor_t* lacs, int flag, const char* type, const char* unit, const char* vfmt, ...)
void lacrosse_printvalue(FILE* outfile, const char* type, const char* unit, int id, const char* valstr)
{
	char line[256];	
	print_sensor_formatted(line, sizeof(line), format, type, id, valstr, unit);
	fprintf(outfile, "%s\n", line);

	if (mosq)
	{
		char* delim = strchr(line, '=');
		if (delim)
		{
			*delim = '\0';
			delim++;
			const char* topic   = line;
			const char* payload = delim;
			char* msgid = NULL; // a real msgid is needed for qos > 0!
			mosquitto_publish(mosq, msgid, topic, strlen(payload), payload, qos, retain);
		}
	}
}

void lacrosse_printsensor(FILE* outfile, const lacsensor_t* lacs)
{
	char valstr[32];
	
	
	#define LAC_PNR_VAL(TYPE, UNIT, VFMT, VAL) \
		{ \
			snprintf(valstr, sizeof(valstr), VFMT, VAL); \
			lacrosse_printvalue(outfile, TYPE, UNIT, lacs->id, valstr); \
		}	
	
	
	#define IF_FLAG_LAC_PNR_VAL(FLAG, TYPE, UNIT, VFMT, VAL) \
		if (BIT_GET(lacs->flags, FLAG)) LAC_PNR_VAL(TYPE, UNIT, VFMT, VAL)


	LAC_PNR_VAL("timestamp", ""   , "%lu"   , time(NULL));
	IF_FLAG_LAC_PNR_VAL(FLAG_TEMP     , "temp"     , "C"  , "%+.1f", lacs->temp                  );
	IF_FLAG_LAC_PNR_VAL(FLAG_HUMID    , "humid"    , "%"  , "%02u" , lacs->humid                 );
	IF_FLAG_LAC_PNR_VAL(FLAG_RAIN     , "rain"     , "mm" , "%u"   , lacs->rain                  );
	IF_FLAG_LAC_PNR_VAL(FLAG_WINDDIR  , "wdir"     , ""   , "%s"   , winddir_names[lacs->winddir]);
	IF_FLAG_LAC_PNR_VAL(FLAG_WINDSPEED, "wspeed"   , "m/s", "%u"   , lacs->windspeed             );
	IF_FLAG_LAC_PNR_VAL(FLAG_WINDGUST , "wgust"    , "m/s", "%u"   , lacs->windgust              );
	LAC_PNR_VAL("batt_new" , ""   , "%c"   , BIT_GET(lacs->flags, FLAG_NEWBATT)  ? '1':'0');
	LAC_PNR_VAL("batt_weak", ""   , "%c"   , BIT_GET(lacs->flags, FLAG_WEAKBATT) ? '1':'0');
	
	
	
}



void lacrosse_loop(void)
{
	if (!lacrosse_init())
	{
		run = false;
		return;
	}	
	
	FILE* outfile = stdout;
	
	setvbuf(outfile, NULL, _IONBF, 0);

	time_t t_prev = 0;
	
	while (run)
	{
		lacsensor_t lacs;
		if (lacrosse_receive(&lacs, 1))
		{		
			lacrosse_printsensor(outfile, &lacs);
		}
		
		mosquitto_loop(mosq, 0, 1);
	}
	
	rfm69io_deinit();
}

int main(int argc, char* argv[])
{
	int res = EXIT_SUCCESS;
	
	scanargs(argc, (const char**)(argv));

	pidfile_init();

	signals_init();
	
	#if (USE_MOSQUITTO == 1)
	if (host)
		mqtt_init();
	#endif

	lacrosse_loop();
	
	#if (USE_MOSQUITTO == 1)
	if (host)
		mqtt_deinit();
	#endif
	
	signals_deinit();

	pidfile_deinit();

	return res;
}