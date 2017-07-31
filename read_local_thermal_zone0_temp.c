#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
	int ch;
	FILE * fp;
	long count = 0;
	fp = fopen("/sys/class/thermal/theraml_zone0/temp", "r");
	ch = getc(fp);

	while(ch != EOF)
	{
	    putchar(ch);
	    ch = getc(fp);
	}
	fclose(fp);
	printf("File %s has %1d characters\n", argv[1], count);
	return 0;
}
