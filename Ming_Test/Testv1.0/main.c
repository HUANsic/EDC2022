# include<d:\Tsinghua\EDC2022\Ming_Test\main.h>

Rectangle obstacles[5];
Coordinate oppoBeacons[3];

void printmap(Map* map) {
    for(uint8_t i = 0;i <= 255; i++) {
		for(uint8_t j = 0;j <= 255; j++) {
            printf("%d ", map->map_icon[i][j]);
        }
        printf("\n");
    }
}

Map* init_map(Rectangle obstacles[5], Coordinate oppoBeacons[3]) {
	Map* map = (Map*)malloc(sizeof(Map));
	for(uint8_t i = 0;i <= 255; i++) {
		for(uint8_t j = 0;j <= 255; j++) {
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
			for(uint8_t k = 0;k <= 4; k++){
				if(i >= obstacles[k].coord1.x && j >= obstacles[k].coord1.y && i <= obstacles[k].coord2.x && j <= obstacles[k].coord2.y){
					map->map_icon[i][j] = 3;
					continue;
				}
			}
			// oppoBeacons
			for(uint8_t k = 0;k <= 2; k++){
				uint16_t distance = (i - oppoBeacons[k].x)*(i - oppoBeacons[k].x)+(j - oppoBeacons[k].y)*(j - oppoBeacons[k].y);
				if(distance <= 400){
					map->map_icon[i][j] = 3;
					continue;
				}
			}
		}
	}
	return map;
}

int main()
{
	for(uint8_t k = 0; k < 5; k++) {
		obstacles[k].coord1.x = k;
		obstacles[k].coord1.y = k;
		obstacles[k].coord2.x = k+1;
		obstacles[k].coord2.y = k+1;
	}
	for(uint8_t t = 0;t < 3; t++) {
		oppoBeacons[t].x = t + 32;
		oppoBeacons[t].y = t + 32;
	}
    Map* globalMap = init_map(obstacles, oppoBeacons);
    printmap(globalMap);

    return 0;
}