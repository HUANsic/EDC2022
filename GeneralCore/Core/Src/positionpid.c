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
#define PATH_PID_TOLERANCE 7
#define MAX_SPEED 2000.0
#define MIN_SPEED 750.0

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
	float kp = 15.0;

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
		EstiCoord.x = (float)myCoord.x;
		EstiCoord.y = (float)myCoord.y;
		CoordinateUpdate = 0;
		return 1;
	}
	return 0;
}

float Get_v_x(void)
{
	float v_x = (cmotor_rf.lastSpeed - cmotor_lf.lastSpeed + cmotor_lb.lastSpeed - cmotor_rb.lastSpeed) * 50.0 / 20000;
	return v_x;
}

float Get_v_y(void)
{
	float v_y = (cmotor_rf.lastSpeed + cmotor_lf.lastSpeed + cmotor_lb.lastSpeed + cmotor_rb.lastSpeed) * 60.0 / 20000;
	return v_y;
}

void Position_P(fCoordinate* cur, Coordinate* goal)
{
//	float x_error = goal->x - cur->x;
	float x_error = cur->x - goal->x;
	float y_error = goal->y - cur->y;
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
	uint32_t timestart = HAL_GetTick();
	HAL_Delay(10); // delay 10 ms = 100 Hz
	if(CheckCoord() == 0)
	{
		float lf_v = cmotor_lf.lastSpeed;
		float lb_v = cmotor_lb.lastSpeed;
		float rf_v = cmotor_rf.lastSpeed;
		float rb_v = cmotor_rb.lastSpeed;
//		float v_x = -((rf_v - lf_v + lb_v - rb_v) / 500);
		float v_x = ((rf_v - lf_v + lb_v - rb_v) / 200);
		float v_y = ((rf_v + lf_v + lb_v + rb_v) / 200);
		uint32_t timeend = HAL_GetTick();
		EstiCoord.x = EstiCoord.x + (timeend - timestart) * 0.001 * v_x;
		EstiCoord.y = EstiCoord.y + (timeend - timestart) * 0.001 * v_y;
	}
}
