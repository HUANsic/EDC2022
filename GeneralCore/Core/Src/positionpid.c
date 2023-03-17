 /*
 * positionpid.c
 *
 *  Created on: 2023年2月20日
 *      Author: ming
 */
#include "positionpid.h"

extern Coordinate myCoord;
extern fCoordinate EstiCoord;
extern uint8_t CoordinateUpdate;
#define PATH_PID_TOLERANCE 10
#define MAX_SPEED 1000
#define MIN_SPEED 500

extern float initangleZ;
extern JY62_HandleTypeDef himu;
extern Motor_HandleTypeDef cmotor_lf, cmotor_rf, cmotor_lb, cmotor_rb;
extern Lane pathlane;

uint8_t GotoDestination(Coordinate Destination, uint8_t mode)
{
	if(mode == 1)
	{
		uint8_t isGetAroad = mingyan_pathfind_avoidObstacle(&myCoord, &Destination);
		if(isGetAroad != 0)
		{
			// success get a road
			for(uint8_t i = 0; i < pathlane.Length; i ++)
			{
				while(1)
				{
					Position_P(&EstiCoord, &pathlane.buffer[pathlane.Head + i]);
					CheckCoord();
					if(abs(EstiCoord.x - pathlane.buffer[pathlane.Head + i].x) + abs(EstiCoord.y - pathlane.buffer[pathlane.Head + i].y) <= PATH_PID_TOLERANCE)
					{
						chao_move_angle(0,0);
						return 1;
					}
				}
			}
		}
		else
		{
			while(1)
			{
				Position_P(&EstiCoord, &Destination);
				CheckCoord();
				if(abs(EstiCoord.x - Destination.x) + abs(EstiCoord.y - Destination.y) <= PATH_PID_TOLERANCE)
				{
					chao_move_angle(0,0);
					return 1;
				}
			}
		}
	}
	else if(mode == 0)
	{
		while(1)
		{
			Position_P(&EstiCoord, &Destination);
			CheckCoord();
			if(abs(EstiCoord.x - Destination.x) + abs(EstiCoord.y - Destination.y) <= PATH_PID_TOLERANCE)
			{
				chao_move_angle(0,0);
				return 1;
			}
		}
	}
	return 0;
}

float Angle_normalization(float angle)
{
	float raw_angle = angle;
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
	float kp = 2.0;

	float Speed = kp * (abs(x) + abs(y));
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

uint8_t CheckCoord(void)
{
	if(CoordinateUpdate == 1)
	{
		EstiCoord.x = myCoord.x;
		EstiCoord.y = myCoord.y;
		CoordinateUpdate = 0;
		return 1;
	}
	return 0;
}

float Get_v_x(void)
{
	float v_x = (cmotor_rf.goalSpeed - cmotor_lf.goalSpeed + cmotor_lb.goalSpeed - cmotor_rb.goalSpeed) * 50.0 / 4000;
	return v_x;
}

float Get_v_y(void)
{
	float v_y = (cmotor_rf.goalSpeed + cmotor_lf.goalSpeed + cmotor_lb.goalSpeed + cmotor_rb.goalSpeed) * 60.0 / 4000;
	return v_y;
}

void Position_P(Coordinate* cur, Coordinate* goal)
{
	int16_t x_error = goal->x - cur->x;
	int16_t y_error = goal->y - cur->y;
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
	CheckCoord();
	HAL_Delay(10); // delay 10 ms = 100 Hz
	if(CheckCoord() == 0)
	{
		EstiCoord.x = EstiCoord.x + 0.01 * Get_v_x();
		EstiCoord.y = EstiCoord.y + 0.01 * Get_v_y();
	}
}
