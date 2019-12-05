#include <SDL.h> 
#include "gfc_matrix.h"
#include "gf3d_vgraphics.h"
#include "gf3d_entity.h"
#include "gf3d_shape.h"

void gf3d_move_char(int dir, Entity* player, Box *a){
if(dir == 1){
            player->position.y -= .01;
           gf3d_box_update(a,player->position);
            gfc_matrix_translate(player->modelMat,vector3d(0,-.01,0));
            gf3d_vgraphics_camera_move(vector3d(0,0,.01));
            }
if(dir == 2){
            player->position.y += .01;
            gf3d_box_update(a,player->position);
            gfc_matrix_translate(player->modelMat,vector3d(0,.01,0));
            gf3d_vgraphics_camera_move(vector3d(0,0,-.01));
            }
if(dir == 3){
            player->position.x += .01;
            gf3d_box_update(a,player->position);
            gfc_matrix_translate(player->modelMat,vector3d(.01,0,0));
            gf3d_vgraphics_camera_move(vector3d(.01,0,0));
            }
if(dir == 4){
           player->position.x -= .01;
            gf3d_box_update(a,player->position);
            gfc_matrix_translate(player->modelMat,vector3d(-.01,0,0));
            gf3d_vgraphics_camera_move(vector3d(-.01,0,0));
            }
}
/*void gf3d_check_collisions(Entity* player, Box *a){
    if(gf3d_box_overlap(player->hitbox,b)){
                if(gf3d_player_touch(player, b->ent) == 0){
                b = gf3d_box (vector3d(100,100,100),0,0,0,vector3d(0,0,0),NULL);
                }
}*/
