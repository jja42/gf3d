#include "gf3d_entity.h"

/**
 * @brief process interactions when the player collides with an entity
 * @param player a ptr to the player
 * @param ent the entity that has been collided with
 * @return 0 if entity is to be freed, 1 otherwise
 */
int gf3d_player_touch(Entity *player, Entity *ent);
/**
 * @brief process interactions when an enemy collides with the player
 * @param player a ptr to the player
 */
void gf3d_enemy_touch(Entity *player);
/**
 * @brief process interactions when a boss collides with the player
 * @param player a ptr to the player
 */
void gf3d_boss_touch(Entity *player);
