#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>

#include "uart_api.h"
#include "robot_protocol.h"


void Logo(void)
{
  	printf("\n\n");
  	printf("            *****************************************\n");
  	printf("              Welcome to Eagle Robot Platform Board \n");
  	printf("            *****************************************\n"); 
}

void PrintBannerRobot(void)
{
	printf("\n");
	printf("================================================================\n");
	printf("              <<<< Robot Body TEST Selection >>>>               \n");
	printf("----------------------------------------------------------------\n");
	printf("   1. Start motion 		-[a]   |  5. Turn Right 		-[e] \n");
	printf("   2. Front walk 		-[b]   |  6. Ready to walk 		-[f] \n");
	printf("   3. Back walk 		-[c]   |  7. Ready to turn 		-[g]\n"); 
	printf("   4. Turn Left 		-[d]   |  8. init robot 		-[i]\n"); 
	printf("----------------------------------------------------------------\n");
	printf("   19. Quit            -[z]         \n");
	printf("================================================================\n");
	
}


int TestItemSelectRobot(void)
{
   char Item;
 	
	while(1) {
 		PrintBannerRobot();
		printf("\nSelected Test Item :  ");
 		Item = getchar();
		putchar(Item);

		switch(Item) {
			case 'a' : case 'A' : 
				startMotion();		
				break;
			case 'b' : case 'B' : 
				frontWalk();			
				break;
			case 'c' : case 'C' : 
				backWalk();			
				break;
			case 'd' : case 'D' : 
				turnLeft();		
				break;
			case 'e' : case 'E' :
				turnRight();
				break;
			case 'i' : case 'I'	:
				init_robot();
				break;
			case 'z' : case 'Z' :
				return 0;
			case 'f' : case 'F' :
				readyToWalk();
				break;
			case 'g' : case 'G' : 
				readyToTurn();
				break;
			default : printf("\nNo Test Item Selected");	break;
		}
	}
	return 0;
}

#include <termios.h>
static struct termios inittio, newtio;

void init_console(void)
{
    tcgetattr(0, &inittio);
    newtio = inittio;
    newtio.c_lflag &= ~ICANON;
    newtio.c_lflag &= ~ECHO;
    newtio.c_lflag &= ~ISIG;
    newtio.c_cc[VMIN] = 1;
    newtio.c_cc[VTIME] = 0;

    cfsetispeed(&newtio, B115200);

    tcsetattr(0, TCSANOW, &newtio);
}

int main(void)
{
	int ret;

	init_console();

	ret = uart_open();
	if (ret < 0) return EXIT_FAILURE;

	uart_config(UART1, 115200, 8, UART_PARNONE, 1);

	Logo();
	TestItemSelectRobot();

	uart_close();

	return 0;
}


