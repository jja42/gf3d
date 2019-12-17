#include "gf3d_entity.h"
#include "gf3d_shape.h"

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
/**
 * @brief process enemy thinking
 * @param player a ptr to the player
 * @param enemy a ptr to the enemy
 * @param currentTime keeps track of the current game time
 * @param enemyLastTime keeps track of the last time the enemy acted, this is for hitstun purposes
 */
void gf3d_enemy_think(Entity *player, Entity*enemy, unsigned int currentTime,unsigned int enemyLastTime);
/**
 * @brief process player collisions
 * @param Box to check against other entities, either player or hitbox
 */