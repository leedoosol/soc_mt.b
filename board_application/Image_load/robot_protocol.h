/* ------------------------------------------------------------------------ */
/*                  KAIST BATTLE ROBOT CONTROL PROGRAM						*/
/*                   														*/
/*          Coding by YKH      												*/
/*          file : robot_protocol.h    										*/
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
/*                  KAIST BATTLE ROBOT CONTROL PROGRAM            			*/
/*                   														*/
/*          Modified by Goon-Ho Choi, HANOOL ROBOTICS 2003.07.16     		*/
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
#ifndef __ROBOT_PROTOCOL_H__
#define __ROBOT_PROTOCOL_H__

#define Header0	0xff
#define Header1 0xff
#define Packet	0x09
#define pID	0xfd
#define CMD	0x16
#define DATA1	0x00
///////////////////////////////////////////////////////////////////////////////
void DelayLoop(int delay_time);
void Send_Command(unsigned char CS1, unsigned char CS2, unsigned char DATA0);
unsigned char Receive_Ack(void);
void Motion(unsigned char DATA0);
void init_robot(void);
void attack_forward_kick();
void attack_low_kick();
void attack_middle_kick();
void attack_punch();
void move_steady();
void move_walk_back();
void move_walk_head_attack();
void move_walk_head();
void ready_steady_from_walk_head_attack();
void ready_walk_head_punch();
void search_left();
void search_right();
void turn_left();
void turn_right();
////////////////////////////////////////////////////////////////////////////////

#endif // __ROBOT_PROTOCOL_H__

