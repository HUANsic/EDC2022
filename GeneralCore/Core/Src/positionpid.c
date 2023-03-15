 /*
 * positionpid.c
 *
 *  Created on: 2023年2月20日
 *      Author: ming
 */
#include "positionpid.h"

extern Coordinate myCoord;
#define PATH_PID_TOLERANCE 1
#define MAX_SPEED 2500
#define MIN_SPEED 500

extern float initangleZ;
extern JY62_HandleTypeDef himu;
extern Lane pathlane;

uint8_t GotoDestination(Coordinate Destination)
{

}

float Angle_normalization(float raw_angle)
{
	while(raw_angle < 0)
	{
		raw_angle += 360;
	}
	while(raw_angle > 360)
	{
		raw_angle -= 360;
	}
	return raw_angle;
}

float CalSpeed(int16_t x, int16_t y)
{
	float kp = 20.0;

	uint16_t Speed = kp * (abs(x) + abs(y));
	if(Speed > MAX_SPEED)
	{
		Speed = MAX_SPEED;
	}
	if(Speed < MIN_SPEED)
	{
		Speed = MIN_SPEED;
	}
	return Speed;
}

void Position_P(Coordinate cur, Coordinate goal)
{
	int16_t x_error = goal.x - cur.x;
	int16_t y_error = goal.y - cur.y;
	float kp = 20;
	if (y_error == 0)
	{
		if(x_error < 0)
		{
			chao_move_angle(270, CalSpeed(x_error, 0));
		}
		else if(x_error > 0)
		{
			chao_move_angle(90, CalSpeed(x_error, 0));
		}
		else
		{
			chao_move_angle(0, 0);
		}
	}
	else
	{
		float azimuth = atan((float)(x_error)/(y_error));
		if(y_error < 0)
		{
			azimuth += M_PI;
		}
		azimuth = azimuth * 360.0 / (2 * M_PI);
		azimuth = Angle_normalization(azimuth);
		float angle = azimuth - Angle_normalization(initangleZ - himu.theta[2]);
		angle = Angle_normalization(angle);
		chao_move_angle(angle, CalSpeed(x_error, y_error));
	}
}
