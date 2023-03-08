 /*
 * positionpid.c
 *
 *  Created on: 2023年2月20日
 *      Author: ming
 */
#include "positionpid.h"
#include "stdlib.h"
#include "math.h"

extern Coordinate myCoord;
#define PATH_PID_TOLERANCE 1
#define velocity -3000
extern float initangleZ;
extern float rotation_angle;
extern Motor_HandleTypeDef cmotor_lf, cmotor_rf, cmotor_lb, cmotor_rb;
extern JY62_HandleTypeDef himu;

uint8_t GotoDestination(Coordinate Destination)
{
	int16_t x_error = Destination.x - myCoord.x;
	int16_t y_error = Destination.y - myCoord.y;
	float move_angle = 0;
	while (!(abs(x_error) < PATH_PID_TOLERANCE && abs(y_error) < PATH_PID_TOLERANCE)) {
		// 确定方向
		if(abs(y_error) < PATH_PID_TOLERANCE)
		{
			if(x_error > PATH_PID_TOLERANCE) //右
			{
				move_angle = M_PI / 2;
			}
			else if (x_error < 0 - PATH_PID_TOLERANCE) //左
			{
				move_angle = -M_PI / 2;
			}
			else //到了
			{
				break;
			}
		}
		else if(y_error >= PATH_PID_TOLERANCE) // 因为actan值域在（-pi/2，pi/2）
		{
			if(x_error >= 0) // 右上
				move_angle = atan((float)(x_error)/(y_error));
			else // 左上
				move_angle = M_PI + atan((float)(x_error)/(y_error));
		}
		else
		{
			if(x_error >= 0) //右下
				move_angle = M_PI / 2 + atan((float)(0-y_error)/(x_error));
			else  //左下
				move_angle = M_PI + atan((float)(x_error)/(y_error));
		}
		// 方向确定，调整转角
		uint8_t k_p = 50;
		uint8_t k_i = 2;
		while (1) {
			rotation_angle = himu.theta[2] - initangleZ;
			if(rotation_angle < 5) break;
			else
			{
				cmotor_lf.goalSpeed = k_p * rotation_angle - k_i * himu.omega[2];
				cmotor_lb.goalSpeed = k_p * rotation_angle - k_i * himu.omega[2];
				cmotor_rf.goalSpeed = -k_p * rotation_angle + k_i * himu.omega[2];
				cmotor_rb.goalSpeed = -k_p * rotation_angle + k_i * himu.omega[2];
				HAL_Delay(50);
			}
		}
		// 前进
//		cmotor_lf.goalSpeed = velocity * cos(move_angle) - velocity * sin(move_angle);
//		cmotor_rf.goalSpeed = velocity * cos(move_angle) + velocity * sin(move_angle);
//		cmotor_lb.goalSpeed = velocity * cos(move_angle) + velocity * sin(move_angle);
//		cmotor_rb.goalSpeed = velocity * cos(move_angle) - velocity * sin(move_angle);
//		uint16_t time = 500;
//		HAL_Delay(time);
//		myCoord.x = myCoord.x + velocity * time * cos(move_angle);
//		myCoord.y = myCoord.y + velocity * time * sin(move_angle);
		return 0;
	}
	return 1;
}
