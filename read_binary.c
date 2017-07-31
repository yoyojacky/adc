#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

char * FILE_NAME = "/home/pi/yoyo/download.bin";
unsigned char file_buffer[20];


int main()
{
	FILE * file_fd;
	int read_len, i;

	file_fd = fopen(FILE_NAME, "rb");
	if(file_fd == NULL)
	{
		perror("errno");
	}
	else
	{
		printf("file open successed!\n");
	}

	while(1)
	{
		read_len = fread(file_buffer,1,16, file_fd);
		if(read_len == -1)
		{
			printf("File read error!\n");
			perror("errno");
			exit(0);
		}
		else if(read_len == 0)
		{
			printf("File open successed!\n");
			break;
		}
		else
		{
			printf("Read %d Byte From download.bin:", read_len);
			for(i = 0; i < read_len; i++)
			{
				printf(" %02x", file_buffer[i]);
			}
			printf("\n");
		}

		usleep(20000);
	}

	fclose(file_fd);
	return 0;

}
