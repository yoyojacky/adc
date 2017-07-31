#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int i;
	int fd;
	char list[5];

	if ((fd = open("/sys/class/gpio/gpio21/value", O_RDONLY)) == 0)
	{
		printf("can not open /sys/class/gpio/gpio21/value");
		exit(1);
	}

	while (1)
	{
		read(fd,list, sizeof(list));
		i = atoi(list);
		if (i == 1)
		{
			printf("Button is pressed!\n");
			sleep(0.2);
		}
		else
		{
			printf("Button is not  pressed!\n");
			sleep(0.2);
		}
	}
}

