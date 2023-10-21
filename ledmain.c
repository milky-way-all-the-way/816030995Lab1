/*
 * uplab1led.c
 *
 * Created: 06/10/2023
 * Author : Deandre Radgman 
   ID number : 816030995
 */ 


#include "sam.h"


int main(void)
{
    int i;
	SystemInit();
	REG_PORT_DIR0 |= (1<<21);
    while (1) 
    {
		REG_PORT_OUT0 |= (1<<21);
		for (i = 0; i < 100000; i++)
		{
		}

		REG_PORT_OUT0 &= (~(1<<21));
		for (i = 0; i < 100000; i++)
		{
		}
	}
}
