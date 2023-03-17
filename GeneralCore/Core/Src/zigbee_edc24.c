/*
 * zigbee_edc24.c
 *
 *  Created on: 2023年3月17日
 *      Author: Administrator
 */
#include "zigbee_edc24.h"
#include "main.h"

#define MAX_MSG_LEN 200
#define MAX_INFO_LEN 100
#define MAX_STATUS_LEN 150

uint8_t zigbeeMessage[MAX_MSG_LEN];    //原始数据
uint8_t gameInfoMessage[MAX_INFO_LEN]; //游戏信息数据（0x01)
uint8_t gameStatusMessage[MAX_STATUS_LEN]; //游戏状态数据（0x05）
uint8_t zigbeeSend[2][6]={{0x55,0xAA,0x00,0x00,0x00,0x00},
                          {0x55,0xAA,0x02,0x00,0x00,0x00}};        //小车可能发送的信息（0x00:请求游戏信息 0x02:设置充电桩）

extern uint8_t receive_flag;
extern UART_HandleTypeDef* zigbee_huart;
extern Coordinate myCoord;
extern int32_t myCharge;

//void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
//{
//  if (huart == zigbee_huart)
//  {
//    __HAL_UNLOCK(zigbee_huart);
//    receive_flag=0;
//    HAL_UARTEx_ReceiveToIdle_DMA(zigbee_huart,zigbeeMessage,MAX_MSG_LEN);
//  }
//}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart,uint16_t Size)
{
    if(huart == zigbee_huart)
    {
        receive_flag=1;
        zigbeeMessageRecord();
        myCoord = getVehiclePos();
        order_append(getLatestPendingOrder());
        myCharge = getRemainDist();
    }
}

static uint8_t CalculateChecksum(const uint8_t data[], int32_t count)
{
  uint8_t checksum = 0;
  for (int32_t i = 0; i < count; ++i)
  {
    checksum ^= data[i];
  }
  return checksum;
}

static float change_float_data(uint8_t* dat)
{
    float float_data;
    float_data=*((float*)dat);
    return float_data;
}

//接口函数
void zigbee_Init(UART_HandleTypeDef *huart)
{
    memset(zigbeeMessage,0x00,MAX_MSG_LEN);
    memset(gameInfoMessage,0x00,MAX_INFO_LEN);
    memset(gameStatusMessage,0x00,MAX_STATUS_LEN);
    zigbee_huart = huart;
    receive_flag=0;
    HAL_UARTEx_ReceiveToIdle_DMA(zigbee_huart, zigbeeMessage, MAX_MSG_LEN);
}

void zigbeeMessageRecord()
{
    uint16_t msgIndex=0;
    for(msgIndex=0;msgIndex<MAX_MSG_LEN;msgIndex++)
    {
        if(zigbeeMessage[msgIndex]==0x55&&zigbeeMessage[msgIndex+1]==0xAA)
        {
            int16_t tmpnum;
            tmpnum=*((int16_t*)(&zigbeeMessage[msgIndex+3]));
            if(tmpnum<0||(tmpnum>MAX_INFO_LEN&&tmpnum>MAX_STATUS_LEN)) continue;
            if(msgIndex+tmpnum+6>=MAX_MSG_LEN) break;
            uint8_t tmpchecksum;
            tmpchecksum=CalculateChecksum(&zigbeeMessage[msgIndex+6],tmpnum);
            if(tmpchecksum==zigbeeMessage[msgIndex+5])
            {
                if(zigbeeMessage[msgIndex+2]==0x01)
                {
                    memset(gameInfoMessage,0x00,MAX_INFO_LEN);
                    for(int32_t i=0;i<tmpnum;i++)
                    {
                        gameInfoMessage[i]=zigbeeMessage[msgIndex+6+i];
                    }
                    continue;
                }
                else if(zigbeeMessage[msgIndex+2]==0x05)
                {
                    memset(gameStatusMessage,0x00,MAX_STATUS_LEN);
                    for(int32_t i=0;i<tmpnum;i++)
                    {
                        gameStatusMessage[i]=zigbeeMessage[msgIndex+6+i];
                    }
                    continue;
                }
            }
        }
    }
    memset(zigbeeMessage,0x00,MAX_MSG_LEN);
    receive_flag=0;
    HAL_UARTEx_ReceiveToIdle_DMA(zigbee_huart,zigbeeMessage,MAX_MSG_LEN);
}

int32_t getGameTime()
{
    int32_t time;
    time=*((int32_t*)(&gameStatusMessage[1]));
    return time;
}

GameStatus_edc24 getGameStatus()
{
    uint8_t status;
    status=gameStatusMessage[0];
    if(status==0x00)
    {
        return GameStandby;
    }
    else
    {
        return GameGoing;
    }
}

float getScore()
{
    return change_float_data(&gameStatusMessage[5]);
}

Coordinate getVehiclePos()
{
    Coordinate pos;
    pos.x=*((int16_t*)(&gameStatusMessage[9]));
    pos.y=*((int16_t*)(&gameStatusMessage[11]));
    return pos;
}

int32_t getRemainDist()
{
    int32_t dist;
    dist=*((int32_t*)(&gameStatusMessage[13]));
    return dist;
}

uint8_t getOrderNum()
{
    return gameStatusMessage[17];
}

Order_edc24 getOneOrder(uint8_t orderNo)
{
    Order_edc24 order;
    order.depPos.x=*((int16_t*)(&gameStatusMessage[18+18*orderNo]));
    order.depPos.y=*((int16_t*)(&gameStatusMessage[20+18*orderNo]));
    order.desPos.x=*((int16_t*)(&gameStatusMessage[22+18*orderNo]));
    order.desPos.y=*((int16_t*)(&gameStatusMessage[24+18*orderNo]));
    order.timeLimit=*((int32_t*)(&gameStatusMessage[26+18*orderNo]));
    order.commission=change_float_data(&gameStatusMessage[30+18*orderNo]);
    order.orderId=*((int16_t*)(&gameStatusMessage[34+18*orderNo]));
    return order;
}

Order_edc24 getLatestPendingOrder()
{
    int32_t tmpnum=(int32_t)gameStatusMessage[21];
    Order_edc24 order;
    order.depPos.x=*((int16_t*)(&gameStatusMessage[18+18*tmpnum]));
    order.depPos.y=*((int16_t*)(&gameStatusMessage[20+18*tmpnum]));
    order.desPos.x=*((int16_t*)(&gameStatusMessage[22+18*tmpnum]));
    order.desPos.y=*((int16_t*)(&gameStatusMessage[24+18*tmpnum]));
    order.timeLimit=*((int32_t*)(&gameStatusMessage[26+18*tmpnum]));
    order.commission=change_float_data(&gameStatusMessage[30+18*tmpnum]);
    order.orderId=*((int16_t*)(&gameStatusMessage[34+18*tmpnum]));
    return order;
}

GameStage_edc24 getGameStage()
{
    uint8_t stage;
    stage=gameInfoMessage[0];
    return (GameStage_edc24)stage;
}

Rectangle getOneBarrier(uint8_t barrierNo)
{
    Rectangle barrier;
    barrier.coord1.x=*((int16_t*)(&gameInfoMessage[2+8*barrierNo]));
    barrier.coord1.y=*((int16_t*)(&gameInfoMessage[4+8*barrierNo]));
    barrier.coord2.x=*((int16_t*)(&gameInfoMessage[6+8*barrierNo]));
    barrier.coord2.y=*((int16_t*)(&gameInfoMessage[8+8*barrierNo]));
    return barrier;
}

int32_t getHalfGameDuration()
{
    int32_t time;
    time=*((int32_t*)(&gameInfoMessage[42]));
    return (int32_t)time;
}

uint8_t getOwnChargingPileNum()
{
    return gameInfoMessage[46];
}

uint8_t getOppChargingPileNum()
{
    uint8_t tmp;
    tmp=gameInfoMessage[46];
    return gameInfoMessage[47+4*tmp];
}

Coordinate getOneOwnPile(uint8_t pileNo)
{
    Coordinate pos;
    pos.x=*((int16_t*)(&gameInfoMessage[47+4*pileNo]));
    pos.y=*((int16_t*)(&gameInfoMessage[49+4*pileNo]));
    return pos;
}

Coordinate getOneOppPile(uint8_t pileNo)
{
    uint8_t tmpnum;
    tmpnum=gameInfoMessage[46];
    Coordinate pos;
    pos.x=*((int32_t*)(&gameInfoMessage[48+4*tmpnum+4*pileNo]));
    pos.y=*((int32_t*)(&gameInfoMessage[48+4*tmpnum+4*pileNo]));
    return pos;
}

void reqGameInfo()
{
    HAL_UART_Transmit(zigbee_huart,zigbeeSend[0],6,HAL_MAX_DELAY);
}

void setChargingPile()
{
    HAL_UART_Transmit(zigbee_huart,zigbeeSend[1],6,HAL_MAX_DELAY);
}
