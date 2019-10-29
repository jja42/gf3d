#ifndef __GF3D_TOUCH_H__
#define __GF3D_TOUCH_H__
#include "gf3d_entity.h"
#include "simple_logger.h"
#include "gfc_matrix.h"
int gf3d_player_touch(Entity *player, Entity *ent){
if (strcmp(&ent->name,"health") == 0){
    slog("%f",player->health);
    slog("Health Up");
    player->health += 50;
    slog("%f",player->health);
    ent->_set = 0;
    return 0;
}
if (strcmp(&ent->name,"armor") == 0){
    slog("Player Armor:%f",player->armor);
    slog("Armor Up");
    player->armor += 50;
    slog("Player Armor:%f",player->armor);
    ent->_set = 0;
    return 0;
}
if (strcmp(&ent->name,"katana") == 0){
    slog("Player Damage:%f",player->damage);
    slog("Damage Up");
    player->damage += 15;
    slog("Player Damage:%f",player->damage);
    ent->_set = 0;
    return 0;
}
return 1;
}
void gf3d_enemy_touch(Entity *player){
    if(player->armor > 0){
    slog("Current Armor: %f",player->armor);
    slog("Losing Armor");
    player->armor -= 25;
    slog("Current Armor:%f",player->armor);
    }
    else{
    slog("Current Health:%f",player->health);
    slog("Taking Damage");
    player->health -= 25;
    slog("Current Health:%f",player->health);
    }
}
void gf3d_boss_touch(Entity *player){
    if(player->armor > 0){
    slog("Current Armor: %f",player->armor);
    slog("Losing Armor");
    player->armor -= 35;
    slog("Current Armor:%f",player->armor);
    }
    else{
    slog("Current Health:%f",player->health);
    slog("Taking Damage");
    player->health -= 35;
    slog("Current Health:%f",player->health);
    }
}
#endif
