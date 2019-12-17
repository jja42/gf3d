#ifndef __GF3D_INPUT__
#define __GF3D_INPUT__

#include <SDL.h> 
#include "gfc_matrix.h"
#include "gf3d_entity.h"
#include "gf3d_shape.h"
/**
 * @brief move the player and check for collisions
 * @param dir the direction in which the player is moving
 * @param player the entity denoting the player
 * @param hitbox the hitbox to compare against for collisions
 */
void gf3d_move_char(int dir, Entity* player, Box *a);

#endif
