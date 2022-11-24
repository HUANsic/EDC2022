#include "stdio.h"
#include "ming_malloc.h"
#include "Astar_util.h"

Rectangle obstacles[5];			// area that depletes charge faster
Coordinate oppoBeacons[3];		// opponent charging station coordinate

int main()
{
	Queue_init();
	List_init();
	int i;
	for (i = 0; i < 5; i++)
	{
		obstacles[i].coord1.x = 0;
		obstacles[i].coord1.y = 0;
		obstacles[i].coord2.x = 0;
		obstacles[i].coord2.y = 0;
	}
	for (i = 0; i < 3; i++)
	{
		oppoBeacons[i].x = 0;
		oppoBeacons[i].y = 0;
	}
	Coordinate start = {50,50};
	Coordinate last = {100,79};
	A_Star(&start, &last, 3);
	return 0;
}