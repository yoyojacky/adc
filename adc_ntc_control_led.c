#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <wiringPi.h>

void main()
{
	wiringPiSetup();
	pinMode(26, OUTPUT);
	pinMode(27, OUTPUT);
	pinMode(28, OUTPUT);
	pinMode(29, OUTPUT);

	void ledoff()
	{
		digitalWrite(26, LOW);
		digitalWrite(27, LOW);
		digitalWrite(28, LOW);
		digitalWrite(29, LOW);
	};

	int file;
	char *bus = "/dev/i2c-1";

	if ((file = open(bus, O_RDWR)) <0)
	{
		printf("Failed to open the but.");
		exit(1);
	}

	ioctl(file, I2C_SLAVE, 0x48);

	char config[3] = {0};
	config[0] = 0x01;
	//config[1] = 0xD4;
	config[1] = 0xC5;
	config[2] = 0x84;
 	write(file, config, 3);
	sleep(1);

	char reg[1] = {0x00};
	write(file, reg, 1);
	char data[2] = {0};
	if(read(file, data, 2) != 2)
	{
		printf("Error: Input/Output Error!");
	}
	else
	{
		int raw_adc = (data[0] * 256 + data[1]);
		if( raw_adc > 32767 )
		{
			raw_adc -= 65535;
		}
		printf("%d \n", raw_adc);
               if( raw_adc > 18800 && raw_adc < 18900)
		{
			ledoff();
               		digitalWrite(26, HIGH);
		}
		else if ( raw_adc > 18900 && raw_adc < 19000)
		{
			ledoff();
			digitalWrite(26, HIGH);
			digitalWrite(27, HIGH);
		}
		else if ( raw_adc > 19000 && raw_adc < 19100)
		{
			ledoff();
			digitalWrite(26, HIGH);
			digitalWrite(27, HIGH);
			digitalWrite(28, HIGH);
		}
		else if ( raw_adc > 19200 && raw_adc < 19700)
		{
			ledoff();
			digitalWrite(26, HIGH);
			digitalWrite(27, HIGH);
			digitalWrite(28, HIGH);
			digitalWrite(29, HIGH);
		}
		else if ((raw_adc < 18750 && raw_adc > 0))
		{
			ledoff();
			digitalWrite(26, LOW);
			digitalWrite(27, LOW);
			digitalWrite(28, LOW);
			digitalWrite(29, LOW);
		}

	}
}
