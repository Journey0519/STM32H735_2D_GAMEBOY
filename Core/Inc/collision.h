/*
 * collision.h
 *
 *  Created on: May 3, 2026
 *      Author: PC
 */


#ifndef INC_COLLISION_H_
#define INC_COLLISION_H_

#include "player.h"
#include "obstacle.h"

#define COLLISION_WIDTH (52)
#define COLLLISION_HEIGHT (9)
#define DIV_COLLISION_IDX (5)

typedef enum {
	PLAYER = 0,
	OBSTACLE = 1,
} type_t;

typedef struct {
	uint8_t player_index_arr[2]; //캐릭터는 자기 인덱스랑 충돌맵 인덱스 동일
	uint8_t obstacle_index_arr[2]; //오브젝트나 적들은 순회해서 자기 index동일한 애 찾아야함
} collision_t;


void collision_map_init();

uint8_t check_player_collision(object_t player, uint8_t index);

uint8_t check_object_collision(object_t object, uint8_t index);

void update_player_collision_map(object_t before_obj, object_t next_obj, uint8_t player_index);

void update_objcect_collision_map(object_t before_obj, object_t next_obj, uint8_t obj_index, type_t type);

void add_player_collision_map(object_t player, uint8_t player_index);

void add_object_collision_map(object_t obj, uint8_t obj_index, type_t type);

void remove_player_collision_map(uint8_t player_index);
void remove_object_collision_map(object_t obj, uint8_t obj_index, type_t type);


//void update_bullet_collision(bullet_t bullet);

uint8_t check_hitbox_collision(object_t obj_a, object_t obj_b);

#endif /* INC_COLLISION_H_ */
