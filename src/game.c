#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_sprite.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_entity.h"
#include "gf3d_shape.h"
#include "gf3d_input.h"
#include "gf3d_touch.h"
#define M_PI 3.14159265358979323846
int main(int argc,char *argv[])
{
    //=========INIT VARS=========//
    Sprite *mouse = NULL;
    int mousex,mousey;
    Uint32 mouseFrame = 0;
    int attack = 0;
    Uint8 hit = 0;
    Uint8 attacking = 0;
    unsigned int lastTime = 0, currentTime, enemylastTime = 0, fliptime = 0;
    int done = 0;
    int c;
    Uint8 validate = 1;
    float frame = 0;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
    Model *model = NULL;
    Model *model2 = NULL;
    Matrix4 modelMat;
    Matrix4 modelMat2;
    //=========VARS DONE=========//
    
    for (c = 1; c < argc;c++)
    {
        if (strcmp(argv[c],"-disable_validate") == 0)
        {
            validate = 0;
        }
    }
    init_logger("gf3d.log");    
    slog("gf3d begin");
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        1200,                   //screen width
        700,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        0,                      //fullscreen
        validate                //validation
    );
    gf3d_entity_manager_init(10);
    // main game loop
    slog("gf3d main loop begin");
    
    
    //=========SETUP=========//
    mouse = gf3d_sprite_load("images/pointer.png",32,32, 16);
    gf3d_entity_spawn_anim("samurai",1,101);
    Entity *player = gf3d_entity_get(0);
    gfc_matrix_translate(player->modelMat,vector3d(0,0,-70));
    player->modelMat[3][3] += 8;
    player->health = 100;
    player->armor = 0;
    player->damage = 30;
    model = gf3d_model_load("floor");
    model2 = gf3d_model_load("Wall");
    gfc_matrix_identity(modelMat);
    gfc_matrix_identity(modelMat2);
    modelMat2[3][3] -= .9;
    modelMat[3][3] -= .8;
    gf3d_entity_spawn_anim("robo",1,10);
    Entity *enemy = gf3d_entity_get(1);
    enemy->health = 90;
    enemy->modelMat[3][3] += 1;
    gf3d_entity_spawn("health");
    Entity *health = gf3d_entity_get(2);
    gf3d_entity_spawn("armor");
    Entity *armor = gf3d_entity_get(3);
    gf3d_entity_spawn("katana");
    Entity *katana = gf3d_entity_get(4);
    gf3d_entity_spawn("boss");
    Entity *boss = gf3d_entity_get(5);
    boss->health = 180;
    gfc_matrix_rotate( modelMat2, modelMat2, 1.6, vector3d(1,0,0));
    gfc_matrix_rotate( modelMat, modelMat, 1.6, vector3d(0,0,1));
    gfc_matrix_rotate( modelMat, modelMat, 1.55, vector3d(1,0,0));
    gfc_matrix_rotate( player->modelMat, player->modelMat, 1.6, vector3d(1,0,0));
    gfc_matrix_rotate( player->modelMat, player->modelMat, 5.2, vector3d(0,1,0));
    gfc_matrix_translate(modelMat2, vector3d(0,0,-7.35));
    gfc_matrix_rotate( enemy->modelMat, enemy->modelMat, 1.5, vector3d(1,0,0));
    gfc_matrix_translate(enemy->modelMat, vector3d(0,0,12.25));
    gfc_matrix_rotate( health->modelMat, health->modelMat, 1.5, vector3d(1,0,0));
    gfc_matrix_rotate( health->modelMat, health->modelMat, 1.5, vector3d(0,1,0));
    health->modelMat[3][3] += 1;
     gfc_matrix_rotate( armor->modelMat, armor->modelMat, 0, vector3d(0,0,1));
     gfc_matrix_rotate( armor->modelMat, armor->modelMat, 0, vector3d(0,1,0));
     gfc_matrix_rotate( boss->modelMat, boss->modelMat, 1.5, vector3d(0,1,0));
     gfc_matrix_rotate( boss->modelMat, boss->modelMat, 1.5, vector3d(0,0,1));
     boss->modelMat[3][3] -=.5;
     armor->modelMat[3][3] -= .25;
     katana->modelMat[3][3] += 10;
     gfc_matrix_rotate( katana->modelMat, katana->modelMat, 1.5, vector3d(0,1,0));
    Box a = gf3d_box(player->position, 2.5, 2.0, 8.0,vector3d(-.5,0,2),player);
    Box b = gf3d_box (vector3d(100,100,100),0,0,0,vector3d(0,0,0),NULL);
    Box hitbox = gf3d_box(player->position,5.5,1,1,vector3d(-6,0,3),NULL);
    //=========SETUP COMPLETE=========//
    
    
    
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        currentTime = SDL_GetTicks();
        SDL_GetMouseState(&mousex,&mousey);
        slog("mouse (%i,%i)",mousex,mousey);
         //mouseFrame = (mouseFrame+1) %16;
        if(attacking != 1){
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here
       if (keys[SDL_SCANCODE_UP]){
            gf3d_move_char(1,player,&a);
        }
        if (keys[SDL_SCANCODE_DOWN]){
            gf3d_move_char(2,player,&a);
        }
        if (keys[SDL_SCANCODE_LEFT]){
            gf3d_move_char(3,player,&a);
        }
        if (keys[SDL_SCANCODE_RIGHT]){
           gf3d_move_char(4,player,&a);
        }
        
    if (keys[SDL_SCANCODE_DOWN]&&keys[SDL_SCANCODE_R]) {
    gf3d_vgraphics_rotate_camera(-0.001, 2);
    }
    if (keys[SDL_SCANCODE_RIGHT]&&keys[SDL_SCANCODE_R]) {
    gf3d_vgraphics_rotate_camera(0.001, 0);
    }
    if (keys[SDL_SCANCODE_LEFT]&&keys[SDL_SCANCODE_R]) {
    gf3d_vgraphics_rotate_camera(-0.001, 0);
    }
    if (keys[SDL_SCANCODE_UP]&&keys[SDL_SCANCODE_R]){
    gf3d_vgraphics_rotate_camera(0.001, 2);
    }
    if (keys[SDL_SCANCODE_SPACE] && currentTime > lastTime + 250){
        if(attacking == 0 && attack == 0){
        lastTime = currentTime;
        attacking = 1;
        gf3d_box_update(&hitbox,player->position);
        }
        if (attacking == 0 && attack == 1){
        if(currentTime <= lastTime + 1500){
            gf3d_box_update(&hitbox,player->position);
            attacking = 1;
            lastTime = currentTime;
            frame = 24;
        }
        else{
        attack = 0;
        }
        }
        if (attacking == 0 && attack == 2){
        if(currentTime <= lastTime + 1500){
            gf3d_box_update(&hitbox,player->position);
            attacking = 1;
            lastTime = currentTime;
            frame = 38;
        }
        else{
        attack = 0;
        }
        }
    }
        }
        gf3d_enemy_think(player,enemy,currentTime,enemylastTime);
        
        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_model_pipeline(),bufferFrame);
            gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_overlay_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame,gf3d_vgraphics_get_graphics_model_pipeline());

        // 2D overlay rendering
            gf3d_model_draw(model2,bufferFrame,commandBuffer,modelMat,0);
            gf3d_model_draw(model,bufferFrame,commandBuffer,modelMat2,0);
            gf3d_model_draw(a.model,bufferFrame,commandBuffer,a.mat,0);
            gf3d_model_draw(hitbox.model,bufferFrame,commandBuffer,hitbox.mat,0);
           gf3d_model_draw(b.model,bufferFrame,commandBuffer,b.mat,0);
            if (attacking == 1){
                gf3d_model_draw(player->model,bufferFrame,commandBuffer,player->modelMat,(Uint32)frame);
               frame = frame + 0.05;
               if(attack == 0){
               if(frame >= 15 && hit == 0){
                hit =  1;
                if(gf3d_box_overlap(b,hitbox)&& (strcmp(&b.ent->name,"boss")!= 0)){
                b.ent->health-= player->damage;
                b.ent->hitstun = currentTime + 1000;
                slog("Enemy Health:%f", b.ent->health);
                }
                }
                if (frame >= 20){
                    frame = 0;
                    hit = 0;
                    attack = 1;
                    attacking = 0;
                }
               }
               if (attack == 1){
               if(frame >= 31 && hit == 0){
                hit =  1;
                if(gf3d_box_overlap(b,hitbox)){
                if((strcmp(&b.ent->name,"boss"))!= 0){
                    b.ent->health-= player->damage;}
                b.ent->hitstun = currentTime+ 1000;
                slog("Enemy Health:%f", b.ent->health);
                }
               }
                   if (frame >= 38){
                   frame = 0;
                   hit = 0;
                   attacking = 0;
                   attack = 2;
                }
               }
                if (attack == 2){
                if(frame >= 54 && hit == 0){
                hit =  1;
                if(gf3d_box_overlap(b,hitbox)){
                if((strcmp(&b.ent->name,"boss"))!= 0){
                b.ent->health-= player->damage;
                b.ent->hitstun = currentTime + 1000;
                slog("Enemy Health:%f", b.ent->health);
                }
                else{
                if(b.ent->health > 45 || gf3d_box_overlap_back(b,hitbox)){
                b.ent->health-= player->damage;
                b.ent->hitstun = currentTime + 1000;
                slog("Enemy Health:%f", b.ent->health);}
                }
                }
                }
               if (frame >= 70){
                   frame = 0;
                   hit = 0;
                   attacking = 0;
                   attack = 0;
                }
               }
            }
            else{
            gf3d_model_draw(player->model, bufferFrame,commandBuffer,player->modelMat,0);        
            }
       int j = gf3d_entity_max() - 1;
        for (int i = 1; i < j; i++)
    {
        if (gf3d_entity_get(i)->_set == 1){
            gf3d_model_draw(gf3d_entity_get(i)->model,bufferFrame,commandBuffer,gf3d_entity_get(i)->modelMat,0);
        }
        else{
            continue;}
    }
                gf3d_command_rendering_end(commandBuffer);
             commandBuffer = gf3d_command_rendering_begin(bufferFrame,gf3d_vgraphics_get_graphics_overlay_pipeline());

                gf3d_sprite_draw(mouse,vector2d(mousex,mousey),mouseFrame, bufferFrame,commandBuffer);
                gf3d_command_rendering_end(commandBuffer);
            
        gf3d_vgraphics_render_end(bufferFrame);
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
