/************************************************************************
  Title     : Robot Body Protocol Source File
  File name : robot_protocol.c    

  Author    : adc inc. (oxyang@adc.co.kr)
  History
		+ v0.0  2007/2/14
		+ v1.0  2008/8/6
************************************************************************/
#include <stdio.h>
#include <string.h>
#include "robot_protocol.h"
#include "uart_api.h"
//////////////////////////////////////////////////// Protocol Test
#define ATTACK_FORWARD_WALK_PUNCH 0x00
#define ATTACK_FORWARD_WALK_PUNCH_NEED_READY 0x01
#define ATTACK_FRONT_KICK 0x02
#define ATTACK_LOW_KICK 0x03
#define ATTACK_MIDDLE_KICK 0x04
#define ATTACK_PUNCH 0x05
#define MOVE_WALK_BACKWARD_WITH_PUNCH 0x06
#define MOVE_WALK_FORWARD 0x07
#define POSE_STEADY 0x08
#define READY_PUNCH_TO_STEADY 0x09
#define READY_STEADY_TO_PUNCH 0x0a
#define SEARCH_LEFT 0x0b
#define SEARCH_RIGHT 0x0c
#define TURN_LEFT 0x0d
#define TURN_RIGHT 0x0e

void DelayLoop(int delay_time)
{
	while(delay_time)
		delay_time--;
}
void init_robot()
{
	unsigned char Init_Buffer[10] = {0,};
	Init_Buffer[0] = 0xff;
	Init_Buffer[1] = 0xff;
	Init_Buffer[2] = 0x0a;
	Init_Buffer[3] = 0xfe;
	Init_Buffer[4] = 0x03;
	Init_Buffer[5] = 0xa2;
	Init_Buffer[6] = 0x5c;
	Init_Buffer[7] = 0x34;
	Init_Buffer[8] = 0x01;
	Init_Buffer[9] = 0x60;
	uart1_buffer_write(Init_Buffer, 10);
}
void Send_Command(unsigned char CS1, unsigned char CS2, unsigned char DATA0)
{
	int i;
	unsigned char Command_Buffer[9] = {0,};

	Command_Buffer[0] = 0xff;	
	Command_Buffer[1] = 0xff;
	Command_Buffer[2] = 0x09;
	Command_Buffer[3] = 0xfd;
	Command_Buffer[4] = 0x16;
	Command_Buffer[5] = CS1;
	Command_Buffer[6] = CS2;
	Command_Buffer[7] = DATA0;
	Command_Buffer[8] = 0x00;

	uart1_buffer_write(Command_Buffer, 9);
}

#define ERROR 0
#define OK	1

/* Command Function */

void Motion(unsigned char DATA0)
{
	unsigned char CS1;
	unsigned char CS2;

	CS1 = (Packet^pID^CMD^DATA0^DATA1) & 0xfe;
	CS2 = (~(Packet^pID^CMD^DATA0^DATA1)) & 0xfe;
	Send_Command(CS1, CS2, DATA0);
	// printf("CS1=%x\n", CS1);
	// printf("CS2=%x\n", CS2);
	 DelayLoop(2000000);		// 3second delay
}

