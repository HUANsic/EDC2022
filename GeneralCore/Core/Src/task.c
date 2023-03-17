/*
 * task.c
 *
 *  Created on: 2023年3月16日
 *      Author: Ming
 */
#include "task.h"

extern Coordinate myCoord;
extern fCoordinate EstiCoord;
extern uint8_t CoordinateUpdate;
extern Coordinate allyBeacons[3];
extern Rectangle obstacles[5];

void go_Charge()
{
	// TODO:可以排个序
	for(uint8_t i = 0; i < 3; i++)
	{
		GotoDestination(allyBeacons[i], 1);
		// set a signal : this is a beacon
	}
}

void Cal_Battery_Coord()
{
	uint8_t seted = 0;
	if(Find_crash(32552, 2))
	{
		//set a signal
		allyBeacons[seted].x = 127;
		allyBeacons[seted].y = 40;
		seted += 1;
	}
	if(Find_crash(32726, 2))
	{
		//set a signal
		allyBeacons[seted].x = 127;
		allyBeacons[seted].y = 214;
		seted += 1;
	}
	if(Find_crash(10367, 2))
	{
		//set a signal
		allyBeacons[seted].x = 40;
		allyBeacons[seted].y = 127;
		seted += 1;
	}
	if(seted != 3)
	{
		if(Find_crash(54911, 2))
		{
			//set a signal
			allyBeacons[seted].x = 214;
			allyBeacons[seted].y = 127;
			seted += 1;
		}
	}
	uint8_t x_i = 127;
	uint8_t y_i = 127;
	while(seted != 3)
	{
		if(Find_crash(x_i * 256 + y_i, 2))
		{
			allyBeacons[seted].x = x_i;
			allyBeacons[seted].y = y_i;
			seted += 1;
		}
		else
		{
			x_i -= 5;
			y_i -= 5;
		}
	}
}

void Get_packet()
{

}

void Send_packet()
{

}
