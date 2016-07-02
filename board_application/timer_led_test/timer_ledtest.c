#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define LED_ON_OFF_LEVEL	0xFFE0A408
#define TLED_MAGIC			LED_ON_OFF_LEVEL
#define TLEDIOC_BLINK		_IO(TLED_MAGIC, 1)		
#define TLEDIOC_SETNUM		_IOW(TLED_MAGIC, 2, 1)	

int main(int argc, char **argv)
{
	int tledfd, newnum;
	
	if(argc > 2) {
		fprintf(stderr, "Usage 1 : timer_ledtest\n");
		fprintf(stderr, "Usage 2 : timer_ledtest [num]\n");
		exit(1);
	}

	tledfd = open("/mnt/f0/dev_module/timer_led", O_RDWR);
	if(tledfd < 0) {
		fprintf(stderr, "open failed!!\n");
		exit(1);
	}

	if (argc == 1) {
		if ( ioctl(tledfd, TLEDIOC_BLINK, 0) ) {
			fprintf(stderr, "!!ioctl error\n");
			exit(1);
		}
	}
	else if (argc == 2) {
		newnum = atoi(argv[1]);
		if ( ioctl(tledfd, TLEDIOC_SETNUM, newnum) ) {
			fprintf(stderr, "ioctl error\n");
			exit(1);
		}
	}

	return(0);
}
