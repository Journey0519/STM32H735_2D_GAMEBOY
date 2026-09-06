/*
 * collision.c
 *
 *  Created on: May 3, 2026
 *      Author: PC
 */
#include <stdint.h>
#include <string.h>
#include "collision.h"

extern player_t players[2];
extern obstacle_t obstacles[10];

static const uint8_t NULL_INDEX = -1; //비활성화

collision_t collision_map[9][52] __attribute__((section(".framebuffer_section"), aligned(32)));
//그리드당 16*16픽셀 커버

void collision_map_init() {
	memset(*collision_map, NULL_INDEX, sizeof(collision_t) * 9 * 52);
	add_player_collision_map(players[0].object, 0);
	add_player_collision_map(players[1].object, 1);
};

uint8_t check_player_collision(object_t player, uint8_t index) {
	int x_start_index = (int)(player.position_x) >> DIV_COLLISION_IDX;
	int x_end_index = (int)(player.position_x + player.width) >> DIV_COLLISION_IDX;
	int y_start_index = (int)(player.position_y - player.hegiht) >> DIV_COLLISION_IDX;
	int y_end_index = (int)(player.position_y) >> DIV_COLLISION_IDX;
	int other_index = NULL_INDEX;
	object_t other_player = players[!index].object;
	uint8_t is_hit = 0;


	y_end_index = y_end_index < 0 ? 0 : y_end_index;
	if (y_start_index < 0) {
		return 0;
	}

	is_hit = check_hitbox_collision(player, other_player);

	if (is_hit == 1) {
		return is_hit;
	}

	for (int i = x_start_index; i <= x_end_index; i++) {
		for (int k = 0; k < 2; k++) {
			other_index = collision_map[y_start_index][i].obstacle_index_arr[k];
			if (other_index != NULL_INDEX) {
				is_hit = check_hitbox_collision(player, obstacles[other_index].object);

				if (is_hit == 1) {
					return is_hit;
				}
			}
		}


		if (y_start_index == y_end_index) {
			continue;
		}

		for (int k = 0; k < 2; k++) {
			other_index = collision_map[y_end_index][i].obstacle_index_arr[k];
			if (other_index != NULL_INDEX) {
				is_hit = check_hitbox_collision(player, obstacles[other_index].object);

				if (is_hit == 1) {
					return is_hit;
				}
			}
		}
	}

	return is_hit;
}

uint8_t check_object_collision(object_t object, uint8_t index) {
	int x_start_index = (int)(object.position_x) >> DIV_COLLISION_IDX;
	int x_end_index = (int)(object.position_x + object.width) >> DIV_COLLISION_IDX;
	int y_start_index = (int)(object.position_y - object.hegiht) >> DIV_COLLISION_IDX;
	int y_end_index = (int)(object.position_y) >> DIV_COLLISION_IDX;
	int other_index = NULL_INDEX;

	uint8_t is_hit = 0;


	y_end_index = y_end_index < 0 ? 0 : y_end_index;
	if (y_start_index < 0) {
		return 0;
	}

	is_hit = check_hitbox_collision(object, players[0].object);
	if (is_hit == 1) {
		return is_hit;
	}

	is_hit = check_hitbox_collision(object, players[1].object);
	if (is_hit == 1) {
		return is_hit;
	}

	for (int i = x_start_index; i <= x_end_index; i++) {
		for (int k = 0; k < 2; k++) {
			other_index = collision_map[y_start_index][i].obstacle_index_arr[k];
			if (other_index != NULL_INDEX && other_index != index) {
				is_hit = check_hitbox_collision(object, obstacles[other_index].object);

				if (is_hit == 1) {
					return is_hit;
				}
			}
		}


		if (y_start_index == y_end_index) {
			continue;
		}

		for (int k = 0; k < 2; k++) {
			other_index = collision_map[y_end_index][i].obstacle_index_arr[k];
			if (other_index != NULL_INDEX && other_index != index) {
				is_hit = check_hitbox_collision(object, obstacles[other_index].object);

				if (is_hit == 1) {
					return is_hit;
				}
			}
		}
	}

	return is_hit;
}

void update_player_collision_map(object_t before_obj, object_t next_obj, uint8_t player_index) {
	int before_x_start_index = (int)(before_obj.position_x) >> DIV_COLLISION_IDX;
	int before_x_end_index = (int)(before_obj.position_x + before_obj.width) >> DIV_COLLISION_IDX;
	int before_y_start_index = (int)(before_obj.position_y - before_obj.hegiht) >> DIV_COLLISION_IDX;
	int before_y_end_index = (int)(before_obj.position_y) >> DIV_COLLISION_IDX;

	int next_x_start_index = (int)(next_obj.position_x) >> DIV_COLLISION_IDX;
	int next_x_end_index = (int)(next_obj.position_x + next_obj.width) >> DIV_COLLISION_IDX;
	int next_y_start_index = (int)(next_obj.position_y - next_obj.hegiht) >> DIV_COLLISION_IDX;
	int next_y_end_index = (int)(next_obj.position_y) >> DIV_COLLISION_IDX;

	before_y_end_index = before_y_end_index < 0 ? 0 : before_y_end_index;
	next_y_end_index = next_y_end_index < 0 ? 0 : next_y_end_index;

	if (before_y_start_index < 0) {
		return;
	}

	if (before_x_start_index == next_x_start_index && before_x_end_index == next_x_end_index
			&& before_y_start_index == next_y_start_index && before_y_end_index == next_y_end_index) {
		return;
	}

	for (int i = before_x_start_index; i <= before_x_end_index; i++) {
		collision_map[before_y_start_index][i].player_index_arr[player_index] = NULL_INDEX; //비활성화
		collision_map[before_y_end_index][i].player_index_arr[player_index] = NULL_INDEX; //비활성화
	}
	if (before_y_end_index - before_y_start_index > 1) {
		for (int i = before_y_start_index; i <= before_y_end_index; i++) {
			collision_map[i][before_x_start_index].player_index_arr[player_index] = NULL_INDEX; //비활성화
			collision_map[i][before_x_end_index].player_index_arr[player_index] = NULL_INDEX; //비활성화
		}
	}


	if (next_y_start_index < 0) {
		return;
	}

	for (int i = next_x_start_index; i <= next_x_end_index; i++) {
		collision_map[next_y_start_index][i].player_index_arr[player_index] = player_index; //활성화
		collision_map[next_y_end_index][i].player_index_arr[player_index] = player_index; //활성화
	}

	if (next_y_end_index - next_y_start_index > 1) {
		for (int i = next_y_start_index; i <= next_y_end_index; i++) {
			collision_map[i][next_x_start_index].player_index_arr[player_index] = player_index; //활성화
			collision_map[i][next_x_end_index].player_index_arr[player_index] = player_index; //활성화
		}
	}
}

void update_objcect_collision_map(object_t before_obj, object_t next_obj, uint8_t obj_index, type_t type) {
	int before_x_start_index = (int)(before_obj.position_x) >> DIV_COLLISION_IDX;
	int before_x_end_index = (int)(before_obj.position_x + before_obj.width) >> DIV_COLLISION_IDX;
	int before_y_start_index = (int)(before_obj.position_y - before_obj.hegiht) >> DIV_COLLISION_IDX;
	int before_y_end_index = (int)(before_obj.position_y) >> DIV_COLLISION_IDX;

	int next_x_start_index = (int)(next_obj.position_x) >> DIV_COLLISION_IDX;
	int next_x_end_index = (int)(next_obj.position_x + next_obj.width) >> DIV_COLLISION_IDX;
	int next_y_start_index = (int)(next_obj.position_y - next_obj.hegiht) >> DIV_COLLISION_IDX;
	int next_y_end_index = (int)(next_obj.position_y) >> DIV_COLLISION_IDX;

	before_y_start_index = before_y_start_index < 0 ? 0 : before_y_start_index;
	next_y_start_index = next_y_start_index < 0 ? 0 : next_y_start_index;

	before_y_end_index = before_y_end_index < 0 ? 0 : before_y_end_index;
	next_y_end_index = next_y_end_index < 0 ? 0 : next_y_end_index;

	if (before_y_start_index < 0) {
		return;
	}

	if (before_x_start_index == next_x_start_index && before_x_end_index == next_x_end_index
			&& before_y_start_index == next_y_start_index && before_y_end_index == next_y_end_index) {
		return;
	}

	for (int i = before_x_start_index; i <= before_x_end_index; i++) {
		for (int k = 0; k < 2; k++) {
			switch (*(uint8_t*)&type) {
			case 1:
				if (collision_map[before_y_start_index][i].obstacle_index_arr[k] == obj_index) {
					collision_map[before_y_start_index][i].obstacle_index_arr[k] = NULL_INDEX; //비활성화
					goto loop_update_before_y_start_end;
				}

				break;
			default:
				assert(0);
			}
		}
loop_update_before_y_start_end:

		if (before_y_start_index == before_y_end_index) {
			continue;
		}

		for (int k = 0; k < 2; k++) {
			switch (*(uint8_t*)&type) {
			case 1:
				if (collision_map[before_y_end_index][i].obstacle_index_arr[k] == obj_index) {
					collision_map[before_y_end_index][i].obstacle_index_arr[k] = NULL_INDEX; //비활성화
					goto loop_update_before_y_end_end;
				}

				break;
			default:
				assert(0);
			}
		}
loop_update_before_y_end_end:
	}

	if (before_y_start_index + 1 <= before_y_end_index - 1) {
		for (int i = before_y_start_index + 1; i <= before_y_end_index - 1; i++) {
			for (int k = 0; k < 2; k++) {
				switch (*(uint8_t*) &type) {
				case 1:
					if (collision_map[i][before_x_start_index].obstacle_index_arr[k] == obj_index) {
						collision_map[i][before_x_start_index].obstacle_index_arr[k] = NULL_INDEX; //비활성화
						goto loop_update_before_x_start_end;
					}

					break;
				default:
					assert(0);
				}
			}
loop_update_before_x_start_end:

			if (before_x_start_index == before_x_end_index) {
				continue;
			}

			for (int k = 0; k < 2; k++) {
				switch (*(uint8_t*) &type) {
				case 1:
					if (collision_map[i][before_x_end_index].obstacle_index_arr[k] == obj_index) {
						collision_map[i][before_x_end_index].obstacle_index_arr[k] = NULL_INDEX; //비활성화
						goto loop_update_before_x_end_end;
					}

					break;
				default:
					assert(0);
				}
			}
loop_update_before_x_end_end:
		}
	}


	if (next_y_start_index < 0) {
		return;
	}

	for (int i = next_x_start_index; i <= next_x_end_index; i++) {
		for (int k = 0; k < 2; k++) {
			switch (*(uint8_t*)&type) {
			case 1:
				if (collision_map[next_y_start_index][i].obstacle_index_arr[k] == NULL_INDEX) {
					collision_map[next_y_start_index][i].obstacle_index_arr[k] = obj_index; //활성화
					goto loop_update_next_y_start_end;
				}

				break;
			default:
				assert(0);
			}
		}
loop_update_next_y_start_end:

		if (next_y_start_index == next_y_end_index) {
			continue;
		}

		for (int k = 0; k < 2; k++) {
			switch (*(uint8_t*)&type) {
			case 1:
				if (collision_map[next_y_end_index][i].obstacle_index_arr[k] == NULL_INDEX) {
					collision_map[next_y_end_index][i].obstacle_index_arr[k] = obj_index; //활성화
					goto loop_update_next_y_end_end;
				}

				break;
			default:
				assert(0);
			}
		}
loop_update_next_y_end_end:
	}

	if (next_y_start_index + 1 > next_y_end_index - 1) {
		return;
	}

	for (int i = next_y_start_index + 1; i <= next_y_end_index - 1; i++) {
		for (int k = 0; k < 2; k++) {
			switch (*(uint8_t*)&type) {
			case 1:
				if (collision_map[i][next_x_start_index].obstacle_index_arr[k] == NULL_INDEX) {
					collision_map[i][next_x_start_index].obstacle_index_arr[k] = obj_index; //활성화
					goto loop_update_next_x_start_end;
				}

				break;
			default:
				assert(0);
			}
		}
loop_update_next_x_start_end:

		if (next_x_start_index == next_x_end_index) {
			continue;
		}

		for (int k = 0; k < 2; k++) {
			switch (*(uint8_t*)&type) {
			case 1:
				if (collision_map[i][next_x_end_index].obstacle_index_arr[k] == NULL_INDEX) {
					collision_map[i][next_x_end_index].obstacle_index_arr[k] = obj_index; //활성화
					goto loop_update_next_x_end_end;
				}

				break;
			default:
				assert(0);
			}
		}
loop_update_next_x_end_end:
	}
}

void add_player_collision_map(object_t player, uint8_t player_index) {
	int x_start_index = (int)(player.position_x) >> DIV_COLLISION_IDX;
	int x_end_index = (int)(player.position_x + player.width) >> DIV_COLLISION_IDX;
	int y_start_index = (int)(player.position_y - player.hegiht) >> DIV_COLLISION_IDX;
	int y_end_index = (int)(player.position_y) >> DIV_COLLISION_IDX;

	for (int i = x_start_index; i <= x_end_index; i++) {
		for (int k = 0; k < 2; k++) {
			if (collision_map[y_start_index][i].player_index_arr[k] == NULL_INDEX) {
				collision_map[y_start_index][i].player_index_arr[k] = player_index; //활성화
				break;
			}
		}

		if (y_start_index == y_end_index) {
			break;
		}

		for (int k = 0; k < 2; k++) {
			if (collision_map[y_end_index][i].player_index_arr[k] == NULL_INDEX) {
				collision_map[y_end_index][i].player_index_arr[k] = player_index; //활성화
				break;
			}
		}

	}

	if (y_start_index + 1 > y_end_index - 1) {
		return;
	}

	for (int i = y_start_index + 1; i <= y_end_index - 1; i++) {
		for (int k = 0; k < 2; k++) {
			if (collision_map[i][x_start_index].player_index_arr[k] == NULL_INDEX) {
				collision_map[i][x_start_index].player_index_arr[k] = player_index; //활성화
				break;
			}
		}

		if (x_start_index == x_end_index) {
			break;
		}

		for (int k = 0; k < 2; k++) {
			if (collision_map[i][x_end_index].player_index_arr[k] == NULL_INDEX) {
				collision_map[i][x_end_index].player_index_arr[k] = player_index; //활성화
				break;
			}
		}

	}
}

void add_object_collision_map(object_t obj, uint8_t obj_index, type_t type) {
	int x_start_index = (int)(obj.position_x) >> DIV_COLLISION_IDX;
	int x_end_index = (int)(obj.position_x + obj.width) >> DIV_COLLISION_IDX;
	int y_start_index = (int)(obj.position_y - obj.hegiht) >> DIV_COLLISION_IDX;
	int y_end_index = (int)(obj.position_y) >> DIV_COLLISION_IDX;

	if (x_end_index > 51) {
		x_end_index = 51;
	}

	if (y_start_index < 0) {
		y_start_index = 0;
	}

	for (int i = x_start_index; i <= x_end_index; i++) {
		for (int k = 0; k < 2; k++) {
			switch (*(uint8_t*)&type) {
			case 1:
				if (collision_map[y_start_index][i].obstacle_index_arr[k] == NULL_INDEX) {
					collision_map[y_start_index][i].obstacle_index_arr[k] = obj_index; //활성화
					goto loop_add_y_start_end;
				}
				break;
			default:
				assert(0);
			}
		}
loop_add_y_start_end:


		if (y_start_index == y_end_index) {
			continue;
		}

		for (int k = 0; k < 2; k++) {
			switch (*(uint8_t*)&type) {
			case 1:
				if (collision_map[y_end_index][i].obstacle_index_arr[k] == NULL_INDEX) {
					collision_map[y_end_index][i].obstacle_index_arr[k] = obj_index; //활성화
					goto loop_add_y_end_end;
				}
				break;
			default:
				assert(0);
			}
		}
loop_add_y_end_end:
	}


	if (y_start_index + 1 > y_end_index - 1) {
		return;
	}

	for (int i = y_start_index + 1; i <= y_end_index - 1; i++) {
		for (int k = 0; k < 2; k++) {
			switch (*(uint8_t*)&type) {
			case 1:
				if (collision_map[i][x_start_index].obstacle_index_arr[k] == NULL_INDEX) {
					collision_map[i][x_start_index].obstacle_index_arr[k] = obj_index; //활성화
					goto loop_add_x_start_end;
				}
				break;
			default:
				assert(0);
			}
		}
loop_add_x_start_end:

		if (x_start_index == x_end_index) {
			continue;
		}

		for (int k = 0; k < 2; k++) {
			switch (*(uint8_t*)&type) {
			case 1:
				if (collision_map[i][x_end_index].obstacle_index_arr[k] == NULL_INDEX) {
					collision_map[i][x_end_index].obstacle_index_arr[k] = obj_index; //활성화
					goto loop_add_x_end_end;
				}

				break;
			default:
				assert(0);
			}
		}
loop_add_x_end_end:

	}
}

uint8_t check_hitbox_collision(object_t obj_a, object_t obj_b) {
	float a_left = obj_a.position_x;
	float a_right = obj_a.position_x + obj_a.width;
	float a_top = obj_a.position_y - obj_a.hegiht;
	float a_bottom = obj_a.position_y;

	float b_left = obj_b.position_x;
	float b_right = obj_b.position_x + obj_b.width;
	float b_top = obj_b.position_y - obj_b.hegiht;
	float b_bottom = obj_b.position_y;

	if (a_right >= b_left && a_left <= b_right && a_bottom >= b_top && a_top <= b_bottom) {
		return 1;
	}

	return 0;
}


void remove_player_collision_map(uint8_t player_index) {

}

void remove_object_collision_map(object_t obj, uint8_t obj_index, type_t type) {
	int x_start_index = (int)(obj.position_x) >> DIV_COLLISION_IDX;
	int x_end_index = (int)(obj.position_x + obj.width) >> DIV_COLLISION_IDX;
	int y_start_index = (int)(obj.position_y - obj.hegiht) >> DIV_COLLISION_IDX;
	int y_end_index = (int)(obj.position_y) >> DIV_COLLISION_IDX;

	y_start_index = y_start_index < 0 ? 0 : y_start_index;
	y_end_index = y_end_index < 0 ? 0 : y_end_index;


	for (int i = x_start_index; i <= x_end_index; i++) {
		for (int k = 0; k < 2; k++) {
			switch (*(uint8_t*)&type) {
			case 1:
				if (collision_map[y_start_index][i].obstacle_index_arr[k] == obj_index) {
					collision_map[y_start_index][i].obstacle_index_arr[k] = NULL_INDEX; //비활성화
					goto loop_remove_y_start_end;
				}

				break;
			default:
				assert(0);
			}
		}
loop_remove_y_start_end:

		if (y_start_index == y_end_index) {
			continue;
		}

		for (int k = 0; k < 2; k++) {
			switch (*(uint8_t*)&type) {
			case 1:
				if (collision_map[y_end_index][i].obstacle_index_arr[k] == obj_index) {
					collision_map[y_end_index][i].obstacle_index_arr[k] = NULL_INDEX; //비활성화
					goto loop_remove_y_end_end;
				}

				break;
			default:
				assert(0);
			}
		}
loop_remove_y_end_end:
	}

	if (y_start_index + 1 > y_end_index - 1) {
		return;
	}

	for (int i = y_start_index + 1; i <= y_end_index - 1; i++) {
		for (int k = 0; k < 2; k++) {
			switch (*(uint8_t*)&type) {
			case 1:
				if (collision_map[i][x_start_index].obstacle_index_arr[k] == obj_index) {
					collision_map[i][x_start_index].obstacle_index_arr[k] = NULL_INDEX; //비활성화
					goto loop_remove_x_start_end;
				}

				break;
			default:
				assert(0);
			}
		}
loop_remove_x_start_end:

		if (x_start_index == x_end_index) {
			continue;
		}

		for (int k = 0; k < 2; k++) {
			switch (*(uint8_t*)&type) {
			case 1:
				if (collision_map[i][x_end_index].obstacle_index_arr[k] == obj_index) {
					collision_map[i][x_end_index].obstacle_index_arr[k] = NULL_INDEX; //비활성화
					goto loop_remove_x_end_end;
				}

				break;
			default:
				assert(0);
			}
		}
loop_remove_x_end_end:
	}


}



