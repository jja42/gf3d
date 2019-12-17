#ifndef __GF3D_TOUCH_H__
#define __GF3D_TOUCH_H__
#include "gf3d_shape.h"
#include "gf3d_entity.h"
#include "simple_logger.h"
#include "gfc_matrix.h"
int gf3d_player_touch(Entity *player, Entity *ent){
if (strcmp((const char *) &ent->name, "health") == 0){
    slog("%f",player->health);
    slog("Health Up");
    player->health += 50;
    slog("%f",player->health);
    ent->_set = 0;
    return 0;
}
if (strcmp((const char *) &ent->name, "armor") == 0){
    slog("Player Armor:%f",player->armor);
    slog("Armor Up");
    player->armor += 50;
    slog("Player Armor:%f",player->armor);
    ent->_set = 0;
    return 0;
}
if (strcmp((const char *) &ent->name, "katana") == 0){
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
    player->armor -= 50;
    slog("Current Armor:%f",player->armor);
    }
    else{
    slog("Current Health:%f",player->health);
    slog("Taking Damage");
    player->health -= 50;
    slog("Current Health:%f",player->health);
    }
}
void gf3d_enemy_think(Entity *player, Entity*enemy, unsigned int currentTime,unsigned int enemyLastTime){
    /*Box a;
    a = gf3d_box(player->position, 2.5f, 2.0f, 8.0f,vector3d(-.5f,0,2));
    Box b;
    b = gf3d_box(enemy->position,3.0f, 7.0f, 5.0f,vector3d(-8,7,0));*/
    if(enemy->_set == 1 && currentTime > enemy->hitstun){
            if(enemy->health <= 0){
                enemy->_set = 0;
                //b = gf3d_box(vector3d(100,100,100),0,0,0,vector3d(0,0,0));
            }
        if(player->hitbox.pos.x >= enemy->hitbox.pos.x){
            if(gf3d_box_overlap(player->hitbox,enemy->hitbox)){
                if(currentTime > enemyLastTime + 2500){
                gf3d_enemy_touch(player);
                enemy->position.x -=.005f;
            gf3d_box_update(&enemy->hitbox,enemy->position);
            return;}
            }
            else{
            enemy->position.x += .005f;
            gf3d_box_update(&enemy->hitbox,enemy->position);
            gfc_matrix_translate(enemy->modelMat,vector3d(.005f,0,0));
            }
        }
        if(player->hitbox.pos.y >= enemy->hitbox.pos.y){
            if(gf3d_box_overlap(player->hitbox,enemy->hitbox)){
                if(currentTime > enemyLastTime + 2500){
                gf3d_enemy_touch(player);
                enemy->position.y -=.005f;
            gf3d_box_update(&enemy->hitbox,enemy->position);
                    return;}
            }
            else{
            enemy->position.y += .005f;
            gf3d_box_update(&enemy->hitbox,enemy->position);
            gfc_matrix_translate(enemy->modelMat,vector3d(0,.005f,0));
            }
    }
    if(player->hitbox.pos.x <= enemy->hitbox.pos.x){
            if(gf3d_box_overlap(player->hitbox,enemy->hitbox)){
                if(currentTime > enemyLastTime + 2500){
                gf3d_enemy_touch(player);
                enemy->position.x +=.005f;
            gf3d_box_update(&enemy->hitbox,enemy->position);
                    return;}
            }
            else{
            enemy->position.x -= .005f;
            gf3d_box_update(&enemy->hitbox,enemy->position);
            gfc_matrix_translate(enemy->modelMat,vector3d(-.005f,0,0));
            }
        }
        if(player->hitbox.pos.y <= enemy->hitbox.pos.y){
            if(gf3d_box_overlap(player->hitbox,enemy->hitbox)){
                if(currentTime > enemyLastTime + 2500){
                gf3d_enemy_touch(player);
                enemy->position.y +=.005f;
            gf3d_box_update(&enemy->hitbox,enemy->position);
                    return;}
            }
            else{
                enemy->position.y -= .005f;
            gf3d_box_update(&enemy->hitbox,enemy->position);
            gfc_matrix_translate(enemy->modelMat,vector3d(0,-.005f,0));
            }
    }
        }
}
#endif
