#include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/vfs.h>

#define PRG_NAME        "oled_demo"
#define PRG_VERSION     "1.1"

// Instantiate the display
ArduiPi_OLED display;

// Config Option
struct s_opts
{
	int oled;
	int verbose;
} ;

int sleep_divisor = 1 ;

// default options values
s_opts opts = {
	OLED_ADAFRUIT_SPI_128x32,	// Default oled
  false										// Not verbose
};


/* ======================================================================
Function: usage
Purpose : display usage
Input 	: program name
Output	: -
Comments:
====================================================================== */
void usage( char * name)
{
	printf("%s\n", name );
	printf("Usage is: %s --oled type [options]\n", name);
	printf("  --<o>led type\nOLED type are:\n");
	for (int i=0; i<OLED_LAST_OLED;i++)
		printf("  %1d %s\n", i, oled_type_str[i]);

	printf("Options are:\n");
	printf("  --<v>erbose  : speak more to user\n");
	printf("  --<h>elp\n");
	printf("<?> indicates the equivalent short option.\n");
	printf("Short options are prefixed by \"-\" instead of by \"--\".\n");
	printf("Example :\n");
	printf( "%s -o 1 use a %s OLED\n\n", name, oled_type_str[1]);
	printf( "%s -o 4 -v use a %s OLED being verbose\n", name, oled_type_str[4]);
}


/* ======================================================================
Function: parse_args
Purpose : parse argument passed to the program
Input 	: -
Output	: -
Comments:
====================================================================== */
void parse_args(int argc, char *argv[])
{
	static struct option longOptions[] =
	{
		{"oled"	  , required_argument,0, 'o'},
		{"verbose", no_argument,	  	0, 'v'},
		{"help"		, no_argument, 			0, 'h'},
		{0, 0, 0, 0}
	};

	int optionIndex = 0;
	int c;

	while (1)
	{
		/* no default error messages printed. */
		opterr = 0;

    c = getopt_long(argc, argv, "vho:", longOptions, &optionIndex);

		if (c < 0)
			break;

		switch (c)
		{
			case 'v': opts.verbose = true	;	break;

			case 'o':
				opts.oled = (int) atoi(optarg);

				if (opts.oled < 0 || opts.oled >= OLED_LAST_OLED )
				{
						fprintf(stderr, "--oled %d ignored must be 0 to %d.\n
", opts.oled, OLED_LAST_OLED-1);
						fprintf(stderr, "--oled set to 0 now\n");
						opts.oled = 0;
				}
			break;

			case 'h':
				usage(argv[0]);
				exit(EXIT_SUCCESS);
			break;

			case '?':
			default:
				fprintf(stderr, "Unrecognized option.\n");
				fprintf(stderr, "Run with '--help'.\n");
				exit(EXIT_FAILURE);
		}
	} /* while */

	if (opts.verbose)
	{
		printf("%s v%s\n", PRG_NAME, PRG_VERSION);
		printf("-- OLED params -- \n");
		printf("Oled is    : %s\n", oled_type_str[opts.oled]);
		printf("-- Other Stuff -- \n");
		printf("verbose is : %s\n", opts.verbose? "yes" : "no");
		printf("\n");
	}
}

// define function to get local ip address.
char* GetLocalIp()
{
    int MAXINTERFACES=16;
    char *ip = NULL;
    int fd, intrface;
    struct ifreq buf[MAXINTERFACES];
    struct ifconf ifc;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)
    {
        ifc.ifc_len = sizeof(buf);
        ifc.ifc_buf = (caddr_t)buf;
        if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc))
        {
            intrface = ifc.ifc_len / sizeof(struct ifreq);

            while (intrface-- > 0)
            {
                if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface])))
                {
                    ip=(inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
                    break;
                }
            }
        }
        close (fd);
        return ip;
    }
}

// get system disk information, free space in bytes.
int get_system_tf_free(double *free)
{
    if(free == NULL)
            return -1;
    struct statfs diskInfo;
    statfs("/",&diskInfo);
    unsigned long long totalBlocks = diskInfo.f_bsize;
    unsigned long long freeDisk = diskInfo.f_bfree*totalBlocks;

    *free = freeDisk;
    return 0;
}

/* ======================================================================
Function: main
Purpose : Main entry Point
Input 	: -
Output	: -
Comments:
====================================================================== */
int main(int argc, char **argv)
{
//	int i;

	// Oled supported display in ArduiPi_SSD1306.h
	// Get OLED type
	parse_args(argc, argv);

	// SPI
	if (display.oled_is_spi_proto(opts.oled))
	{
		// SPI change parameters to fit to your LCD
		if ( !display.init(OLED_SPI_DC,OLED_SPI_RESET,OLED_SPI_CS, opts.oled) )
			exit(EXIT_FAILURE);
	}
	else
	{
		// I2C change parameters to fit to your LCD
		if ( !display.init(OLED_I2C_RESET,opts.oled) )
			exit(EXIT_FAILURE);
	}

	display.begin();

  // init done
  display.clearDisplay();   // clears the screen  buffer
  display.display();   		// display it (clear display)

	if (opts.oled == 5)
	{
		// showing on this display is very slow (the driver need to be optimized)
		sleep_divisor = 4;

		for(char i=0; i < 12 ; i++)
		{
			display.setSeedTextXY(i,0);  //set Cursor to ith line, 0th column
			display.setGrayLevel(i); //Set Grayscale level. Any number between 0 - 15.
			display.putSeedString("Hello World"); //Print Hello World
		}

		sleep(2);

	}

  display.clearDisplay();

	// display time information
while(1)
{
  double disku = 0.0;
  time_t timep;
  struct tm *p;
  FILE *fp;
  FILE *fp2;
  char list[30];
  char dict[30];

if((fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r")) == NULL)
 {
  printf("You may not have permission to access /sys/class/thermal/thermal_zone0/temp file, you need
root permission.");
   exit(1);
 }

if((fp2 = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq", "r")) == NULL)
 {
  printf("error to open file, have no rights to open /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur
_freq");
   exit(1);
 }
 get_system_tf_free(&disku);

  time (&timep);
  p = localtime(&timep);
  fread(list, sizeof( char ), 25, fp );
  fread(dict, sizeof( char ), 25, fp2 );

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setTextColor(WHITE); // 'inverted' text
  display.printf("%d:%d:%d\n", p->tm_hour,p->tm_min,p->tm_sec); //  show time
  display.printf("IP:%s\n", GetLocalIp());  // show ip address
  display.printf("TEMP:%2.2f degC\n",(float)((float)atoi(list) / 1000)); //show temperature of CPU.
  display.drawVerticalBargraph(114,0,8,(int16_t) display.height(),1, (float)((float)atoi(list) / 1000
)); // show temperature bar graph.
  display.printf("CPU:%01.2f GHz\n",(float)((float)atoi(dict) / 1000000.0));  //show cpu's frequences
.
  display.printf("DISK :%2.2f MB\n", disku / 1024 / 1024); //show free disk size in MB.
  display.setTextSize(1);
  display.display();
}

	// clear and close display.
	display.close();

}
