# include<D:\Tsinghua\EDC2022\Ming_Test\main.h>

Rectangle obstacles[5];
Coordinate oppoBeacons[3];

void printmap(Map* map) {
	uint16_t i= 0;
	uint16_t j = 0;
    for(i = 0;i <= 255; i++) {
		for(j = 0;j <= 255; j++) {
            printf("%d ", map->map_icon[i][j]);
        }
        printf("\n");
    }
}

Map* init_map(Rectangle obstacles[5], Coordinate oppoBeacons[3]) {
	Map* map = (Map*)malloc(sizeof(Map));
	uint16_t i = 0;
	uint16_t j = 0;
	uint8_t k = 0;
	uint16_t distance = 0;
	for(i = 0;i <= 255; i++) {
		for(j = 0;j <= 255; j++) {
			//walls
			if((i >= 38 && i <= 40) && ((j >= 38 && j <= 107) || (j >= 147 && j <= 216))){
				map->map_icon[i][j] = 3;
				continue;
			}
			if((i >= 214 && i <= 216) && ((j >= 38 && j <= 107) || (j >= 147 && j <= 216))){
				map->map_icon[i][j] = 3;
				continue;
			}
			if((j >= 38 && j <= 40) && ((i >= 38 && i <= 107) || (i >= 147 && i <= 216))){
				map->map_icon[i][j] = 3;
				continue;
			}
			if((j >= 214 && j <= 216) && ((i >= 38 && i <= 107) || (i >= 147 && i <= 216))){
				map->map_icon[i][j] = 3;
				continue;
			}
			// obstacles
			for(k = 0;k <= 4; k++){
				if(i >= obstacles[k].coord1.x && j >= obstacles[k].coord1.y && i <= obstacles[k].coord2.x && j <= obstacles[k].coord2.y){
					map->map_icon[i][j] = 3;
					continue;
				}
			}
			// oppoBeacons
			for(k = 0;k <= 2; k++){
				distance = (i - oppoBeacons[k].x)*(i - oppoBeacons[k].x)+(j - oppoBeacons[k].y)*(j - oppoBeacons[k].y);
				if(distance <= 400){
					map->map_icon[i][j] = 3;
					continue;
				}
			}
			map->map_icon[i][j] = 0;
		}
	}
	return map;
}

int main()
{
	uint8_t k = 0; // ��uint8_t��Ϊuint_16
	uint8_t t = 0;
	for(k = 0; k < 5; k++) {
		obstacles[k].coord1.x = 0;
		obstacles[k].coord1.y = 0;
		obstacles[k].coord2.x = 0;
		obstacles[k].coord2.y = 0;
	}
	for(t = 0;t < 3; t++) {
		oppoBeacons[t].x = 0;
		oppoBeacons[t].y = 0;
	}
    Map* globalMap = init_map(obstacles, oppoBeacons);
    //printmap(globalMap);
	Coordinate begin;
	begin.x = 50;
	begin.y = 50;
	Coordinate end;
	end.x = 100;
	end.y = 100;
	if (Find_crash(end.x, end.y) && Find_crash(begin.x, begin.y)) {
		A_Star_Node* endnode = A_Star(&begin, &end, 10);
	}
	else {
		printf("begin/end is ERORR!");
	}
    return 0;
}