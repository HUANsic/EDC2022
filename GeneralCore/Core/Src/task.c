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

void set_Beacons(void)
{
	uint8_t i;
	for(i = 0;i < 3; i++){
		GotoDestination(want_allyBeacons[i], 0);
		huansic_xb_setBeacon(&hxb);
		HAL_Delay(100);
	}
}

void Cal_Battery_Coord(void)
{
	uint8_t seted = 0;
	//set a signal
	want_allyBeacons[seted].x = 127;
	want_allyBeacons[seted].y = 40;
	seted += 1;
	//set a signal
	want_allyBeacons[seted].x = 127;
	want_allyBeacons[seted].y = 100;
	seted += 1;
	//set a signal
	want_allyBeacons[seted].x = 127;
	want_allyBeacons[seted].y = 160;
	seted += 1;
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
		if(delivering[i]->timeLimit < 7000){
			minindex = i;
			break;
		}
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
