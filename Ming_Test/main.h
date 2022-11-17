#ifndef INC_MAIN_H_
#define INC_MAIN_H_


# include<stdio.h>
# include<stdlib.h>
# include<time.h>
# include<D:\Tsinghua\EDC2022\Ming_Test\ming_types.h>
# include<D:\Tsinghua\EDC2022\Ming_Test\Astar_util.h>
# include<D:\Tsinghua\EDC2022\Ming_Test\huansic_types.h>

void printmap(Map* map);
// init_Map (No expand)
Map* init_map(Rectangle obstacles[5], Coordinate oppoBeacons[3]);

#endif