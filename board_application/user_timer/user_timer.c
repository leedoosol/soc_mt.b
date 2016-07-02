#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "eagle_timer_api.h"

#define USING_TIMER 2

int main(int argc, char **argv)
{
	int ret;
	int count_num, count_back;
	unsigned int time_interval=0;

	if(argc != 2) {
		fprintf(stderr, "Usage 1 : user_timer [usec_interval]\n");
		exit(1);
	}

	ret = eagle_timer_open();
	if (ret < 0) {
		printf("Eagle User Timer Open Error!\n");
        return EXIT_FAILURE;
    }

	eagle_timer_int_disable(USING_TIMER);
    eagle_timer_stop(USING_TIMER);
    eagle_timer_count_clear(USING_TIMER);

	time_interval = atoi(argv[1]);
	if(time_interval < 4) {
		printf("Error!, usec time interval must be over 4\n");
		exit(1);
	}

    ret = eagle_timer_config(USING_TIMER, time_interval);
	if (ret < 0) {
		printf("Eagle User Timer Config Error!\n");
        return EXIT_FAILURE;
    }

	printf("Timer %d, Start....\n", USING_TIMER);

	count_num = -1;
	count_back = -1;
    eagle_timer_int_enable(USING_TIMER);
    eagle_timer_run(USING_TIMER);
	while(1) {
		count_num = eagle_timer_read_count(USING_TIMER);
		if(count_num != count_back) {
			printf("count_num = %d\n", count_num);
			count_back = count_num;
		}
		if(count_num>=10) break;		
	}

    eagle_timer_int_disable(USING_TIMER);
    eagle_timer_stop(USING_TIMER);
    eagle_timer_count_clear(USING_TIMER);

	printf("Timer %d, End....\n", USING_TIMER);

	eagle_timer_close();

	return(0);
}
