/*
 * bullet.c
 *
 *  Created on: May 3, 2026
 *      Author: PC
 */
#include <resource/texture/bullet_texture.h>
#include <string.h>
#include "bullet.h"
#include "collision.h"
#include "player.h"
#include "render.h"
#include "flying.h"

#define BULLET_COUNT (30)
#define BULLET_DAMAGE (100)
#define BULLET_SPEED (0.4f)
#define BULLET_X_RIGHT_OFFSET (37)
#define BULLET_X_LEFT_OFFSET (-3)
#define BULLET_Y_OFFSET (8)

extern player_t players[2];
extern object_t objects[15];
extern collision_t collision_map[9][52];
extern obstacle_t obstacles[10];
extern flying_t flyings[2];

static const uint8_t NULL_INDEX = -1;
static const texture_t bullet_texture = { image_data_bullet_3_3, 3, 0, 200, 3, 3, 0, 1, 0 };

bullet_t bullets[BULLET_COUNT];

void init_bullets() {
	memset(bullets, 0, sizeof(bullet_t) * BULLET_COUNT);
}

void render_and_update_bullets(uint32_t delta_time) {
	for (int i = 0; i < BULLET_COUNT; i++) {
		if (bullets[i].is_active == 0) {
			continue;
		}

		render_texture_no_blend(bullets[i].obj.position_x, bullets[i].obj.position_y, 0, 0, &bullet_texture, delta_time);


		for (int i = 0; i < 2; i++) {
			if (flyings[i].active == 1 && check_hitbox_collision(bullets[i].obj, flyings[i].obj) == 1) {
				bullets[i].is_active = 0;
				flyings[i].health -= BULLET_DAMAGE;
				goto next_bullet;
			}
		}


		bullets[i].obj.position_x +=  (bullets[i].is_right * BULLET_SPEED * delta_time) - (bullets[i].is_left * BULLET_SPEED  * delta_time);
		bullets[i].obj.position_y += (bullets[i].is_down * BULLET_SPEED  * delta_time) - (bullets[i].is_up * BULLET_SPEED  * delta_time);

		if (bullets[i].obj.position_x < 0.0f || bullets[i].obj.position_x > 1654.0f) {
			bullets[i].is_active = 0;

			continue;
			//todo: 사라지는 이펙트
		} else if (bullets[i].obj.position_y < 0.0f || bullets[i].obj.position_y > 272.0f) {
			bullets[i].is_active = 0;

			continue;
		}

		//check bullet - player collision
		int x_index = (int)(bullets[i].obj.position_x) >> DIV_COLLISION_IDX;
		int y_index = (int)(bullets[i].obj.position_y) >> DIV_COLLISION_IDX;

		collision_t collision_box = collision_map[y_index][x_index];

		for (int j = 0; j < 2; j++) {
			if (collision_box.player_index_arr[j] == NULL_INDEX) { //bullet-player
				continue;
			}
			int player_index = collision_box.player_index_arr[j];
			int is_hit = check_hitbox_collision(bullets[i].obj, players[player_index].object);
			if (is_hit) {
				players[player_index].health.health -= BULLET_DAMAGE;
				players[player_index].health.health = players[player_index].health.health < 0 ? 0 : players[player_index].health.health;
				bullets[i].is_active = 0;
				goto next_bullet;
			}
		}

		for (int j = 0; j < 2; j++) {
			if (collision_box.obstacle_index_arr[j] == NULL_INDEX) { //bullet-obstacle
				continue;
			}
			int obstacle_index = collision_box.obstacle_index_arr[j];
			int is_hit = check_hitbox_collision(bullets[i].obj, obstacles[obstacle_index].object);
			if (is_hit) {
				obstacles[obstacle_index].health -= BULLET_DAMAGE;
				obstacles[obstacle_index].health = obstacles[obstacle_index].health < 0 ? 0 : obstacles[obstacle_index].health;
				bullets[i].is_active = 0;
				obstacles[obstacle_index].hit = 1;
				break;
			}
		}

next_bullet:
	}
}

void add_bullet(uint8_t bullet_state, object_t player_obj) {
	for (int i = 0; i < BULLET_COUNT; i++) {
		if (bullets[i].is_active == 0) {
			*(uint8_t*)&bullets[i] =  bullet_state;
			bullets[i].is_active = 1;
			bullets[i].obj.hegiht = 3;
			bullets[i].obj.width = 3;

			if (bullets[i].is_left == 1) {
				bullets[i].obj.position_x = player_obj.position_x + BULLET_X_LEFT_OFFSET;
			} else {
				bullets[i].obj.position_x = player_obj.position_x + BULLET_X_RIGHT_OFFSET;
			}

			bullets[i].obj.position_y = player_obj.position_y - 32 + BULLET_Y_OFFSET;

			return;
		}
	}
}

void render_bullets(uint32_t delta_time) {
	for (int i = 0; i < BULLET_COUNT; i++) {
		if (bullets[i].is_active == 0) {
			continue;
		}


	}

}
