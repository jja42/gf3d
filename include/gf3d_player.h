#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "gf3d_shape.h"
#include "gf3d_entity.h"
#include "simple_logger.h"

Entity *player = NULL;
void gf3d_player_init();
/**
 * @brief move the player in a certain direction
 * @param dir the int that specifies the direction in which to move
 */
void gf3d_moveplayer(int dir);
/**
 * @brief update the player based on what it's touching
 * @param ent a pointer to the entity that is being touched
 */
Entity *gf3d_player_colliding();
void gf3d_player_touch(Entity *ent);
