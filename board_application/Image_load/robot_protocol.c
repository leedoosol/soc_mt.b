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
#define ATTACK_FORWARD_KICK 0x00
#define ATTACK_LOW_KICK 0x01
#define ATTACK_MIDDLE_KICK 0x02
#define ATTACK_PUNCH_KICK 0x03
#define MOVE_STEADY 0x04
#define MOVE_WALK_BACK 0x05
#define MOVE_WALK_HEAD 0x06
#define MOVE_WALK_HEAD_ATTACT 0x07
#define READY_STEADY_FROM_WALK_HEAD_ATTACT 0x08
#define READY_WALK_HEAD_PUNCH 0x09
#define SEARCH_LEFT 0x0a
#define SEARCH_RIGHT 0x0b
#define TURN_LEFT 0x0c
#define TURN_RIGHT 0x0d

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
	DelayLoop(15000000);		// 3second delay
}

void attack_forward_kick()
{
	Motion(ATTACK_FORWARD_KICK);
}

void attack_low_kick(){
	Motion(ATTACK_LOW_KICK);
}

void attack_middle_kick(){
	Motion(ATTACK_MIDDLE_KICK);
}

void attack_punch(){
	Motion(ATTACK_PUNCH_KICK);
}
void move_steady(){
	Motion(MOVE_STEADY);
}

void move_walk_back(){
	Motion(MOVE_WALK_BACK);
}

void move_walk_head_attack(){
	Motion(MOVE_WALK_HEAD_ATTACT);
}

void move_walk_head(){
	Motion(MOVE_WALK_HEAD);
}

void ready_steady_from_walk_head_attack(){
	Motion(READY_STEADY_FROM_WALK_HEAD_ATTACT);
}


void ready_walk_head_punch(){
	Motion(READY_WALK_HEAD_PUNCH);
}

void search_left(){
	Motion(SEARCH_LEFT);
} 

void search_right(){
	Motion(SEARCH_RIGHT);
}
void turn_left(){
	Motion(TURN_LEFT);
}
void turn_right(){
	Motion(TURN_RIGHT);
}