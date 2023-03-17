/*
 * zigbee_edc24.h
 *
 *  Created on: 2023年3月17日
 *      Author: Administrator
 */

#ifndef INC_ZIGBEE_EDC24_H_
#define INC_ZIGBEE_EDC24_H_

#include "stm32f1xx_hal.h"
#include "string.h"
#include "huansic_types.h"

#define MAX_ORDER_NUM 5
#define MAX_BARRIER_NUM 5
#define MAX_PILE_NUM 3

typedef enum{
    GameStandby,      //暂停
    GameGoing       //进行
}GameStatus_edc24;

typedef enum{
    Prematch,        //赛前
    FirstHalf,       //上半场
    SecondHalf       //下半场
}GameStage_edc24;

typedef struct{
    Coordinate depPos;      //起点位置
    Coordinate desPos;      //终点位置
    int32_t timeLimit;         //时间限制
    int16_t orderId;            //订单编号
    float commission;         //订单分数
}Order_edc24;

extern uint8_t receive_flag;
extern uint32_t error_flag;

void zigbee_Init(UART_HandleTypeDef *huart);        //初始化,开始接收消息
void zigbeeMessageRecord(void);                     //校验并记录消息，重新打开串口

int32_t getGameTime(void);                         //获取游戏时间，单位ms
GameStage_edc24 getGameStage(void);                 //获取比赛阶段
GameStatus_edc24 getGameStatus(void);               //获取游戏状态，进行中或暂停中
float getScore(void);                              //获取小车得分
Coordinate getVehiclePos(void);                 //获取小车的位置
int32_t getRemainDist(void);                        //获取小车剩余里程
int32_t getHalfGameDuration(void);                 //获取半场持续时间
uint8_t getOwnChargingPileNum(void);                //获取自己充电桩数目
uint8_t getOppChargingPileNum(void);                //获取对手充电桩数目
uint8_t getOrderNum(void);                          //获取当前正在运送的订单数目
Order_edc24 getLatestPendingOrder(void);            //获取最新订单
Order_edc24 getOneOrder(uint8_t orderNo);           //获取一个正在运送的订单
Rectangle getOneBarrier(uint8_t barrierNo);     //获取一个障碍物位置
Coordinate getOneOwnPile(uint8_t pileNo);       //获取一个己方充电桩位置
Coordinate getOneOppPile(uint8_t pileNo);       //获取一个对方充电桩位置

void reqGameInfo(void);                             //请求游戏基本信息
void setChargingPile(void);                         //设置充电桩

#endif /* INC_ZIGBEE_EDC24_H_ */
