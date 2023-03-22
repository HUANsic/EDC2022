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
extern Coordinate want_allyBeacons[3];
extern Rectangle obstacles[5];
extern Order *delivering[5];
extern uint8_t delivering_num;
extern XB_HandleTypeDef hxb;

void go_Charge(void)
{
	Coordinate Beacon;
	Beacon = Get_nearest_Beacon();
	GotoDestination(Beacon, 0);
}

void set_Beacons(void)
{
	uint8_t i;
	for(i = 0;i < 3; i++){
		GotoDestination(want_allyBeacons[i], 0);
		huansic_xb_setBeacon(&hxb);
	}
}

void Cal_Battery_Coord(void)
{
	uint8_t seted = 0;
	if(Find_crash(32552, 0))
	{
		//set a signal
		want_allyBeacons[seted].x = 127;
		want_allyBeacons[seted].y = 40;
		seted += 1;
	}
	if(Find_crash(32572, 0))
	{
		//set a signal
		want_allyBeacons[seted].x = 127;
		want_allyBeacons[seted].y = 60;
		seted += 1;
	}
	if(Find_crash(32592, 0))
	{
		//set a signal
		want_allyBeacons[seted].x = 127;
		want_allyBeacons[seted].y = 80;
		seted += 1;
	}
	if(seted < 3)
	{
		if(Find_crash(54911, 2))
		{
			//set a signal
			want_allyBeacons[seted].x = 214;
			want_allyBeacons[seted].y = 127;
			seted += 1;
		}
	}
	uint8_t x_i = 127;
	uint8_t y_i = 127;
	while(seted < 3)
	{
		if(Find_crash(x_i * 256 + y_i, 2))
		{
			want_allyBeacons[seted].x = x_i;
			want_allyBeacons[seted].y = y_i;
			seted += 1;
		}
		else
		{
			x_i -= 5;
			y_i -= 5;
		}
	}
}

void Get_packet(Coordinate merchant)
{
	GotoDestination(merchant,0);
}

void Send_packet(Coordinate consumer)
{
	GotoDestination(consumer,0);
}

Coordinate Get_nearest_consumer(void)
{
	int16_t mindis = 512;
	int16_t distance;
	uint8_t minindex = 0;
	if(delivering_num == 0)
		return myCoord;
	for(uint8_t i = 0;i < delivering_num; i++)
	{
		distance = abs(myCoord.x - delivering[i]->destCoord.x) + abs(myCoord.y - delivering[i]->destCoord.y);
		if(distance < mindis){
			mindis = distance;
			minindex = i;
		}
	}
	Coordinate nearest = delivering[minindex]->destCoord;
	return nearest;

}

Coordinate Get_nearest_Beacon(void){
	int16_t mindis = 512;
	int16_t distance;
	uint8_t minindex = 0;
	for(uint8_t i = 0;i < 3;i++)
	{

		distance = abs(myCoord.x - allyBeacons[i].x) + abs(myCoord.y - allyBeacons[i].y);
		if(distance < mindis){
			mindis = distance;
			minindex = i;
		}
	}
	Coordinate nearest = allyBeacons[minindex];
	return nearest;
}
