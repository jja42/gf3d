#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_entity.h"
#include "gf3d_shape.h"
#include "gf3d_touch.h"
#define M_PI 3.14159265358979323846

int main(int argc,char *argv[])
{
    int attack = 0;
    Uint8 hit = 0;
    Uint8 attacking = 0;
    Uint8 spawned = 0;
    Uint8 flipped = 0;
    Uint8 player_flipped = 0;
    Uint8  boss_flipped = 0;
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
    for (c = 1; c < argc;c++)
    {
        if (strcmp(argv[c],"-disable_validate") == 0)
        {
            validate = 0;
        }
    }
    c = 0;
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
    enemy->_set = 0;
    enemy->health = 90;
    enemy->modelMat[3][3] += 1;
    gf3d_entity_spawn("health");
    Entity *health = gf3d_entity_get(2);
    health->_set = 0;
    gf3d_entity_spawn("armor");
    Entity *armor = gf3d_entity_get(3);
    armor->_set = 0;
    gf3d_entity_spawn("katana");
    Entity *katana = gf3d_entity_get(4);
    katana->_set = 0;
    gf3d_entity_spawn("boss");
    Entity *boss = gf3d_entity_get(5);
    boss->health = 180;
    boss->_set = 0;
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
     gfc_matrix_translate(armor->modelMat, vector3d(10,0,0));
     gfc_matrix_rotate( boss->modelMat, boss->modelMat, 1.5, vector3d(0,1,0));
     gfc_matrix_rotate( boss->modelMat, boss->modelMat, 1.5, vector3d(0,0,1));
     boss->modelMat[3][3] -=.5;
     armor->modelMat[3][3] -= .25;
     katana->modelMat[3][3] += 10;
     gfc_matrix_rotate( katana->modelMat, katana->modelMat, 1.5, vector3d(0,1,0));
    Box a = gf3d_box(player->position, 2.5, 2.0, 8.0,vector3d(-.5,0,2),player);
    Box b = gf3d_box (vector3d(100,100,100),0,0,0,vector3d(0,0,0),NULL);
    Box hitbox = gf3d_box(player->position,5.5,1,1,vector3d(-6,0,0),NULL);
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        currentTime = SDL_GetTicks();
        if(attacking != 1){
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here
       if (keys[SDL_SCANCODE_UP]){
            player->position.y -= .01;
            gf3d_box_update(&a,player->position);
            if(gf3d_box_overlap(a,b)){
                if(gf3d_player_touch(player, b.ent) == 0){
                b = gf3d_box (vector3d(100,100,100),0,0,0,vector3d(0,0,0),NULL);
                spawned = 0;}
            player->position.y +=.01;
            gf3d_box_update(&a,player->position);
            }
            else{
            gfc_matrix_translate(player->modelMat,vector3d(0,-.01,0));
            gf3d_vgraphics_camera_move(vector3d(0,0,.01));
            }
        }
        if (keys[SDL_SCANCODE_DOWN]){
             player->position.y += .01;
            gf3d_box_update(&a,player->position);
            if(gf3d_box_overlap(a,b)){
                if(gf3d_player_touch(player, b.ent) == 0){
                b = gf3d_box (vector3d(100,100,100),0,0,0,vector3d(0,0,0),NULL);
                spawned = 0;}
            player->position.y -=.01;
            gf3d_box_update(&a,player->position);
            }
            else{
            gfc_matrix_translate(player->modelMat,vector3d(0,.01,0));
            gf3d_vgraphics_camera_move(vector3d(0,0,-.01));
            }
        }
        if (keys[SDL_SCANCODE_LEFT]){
            if(!player_flipped){
               gfc_matrix_rotate(player->modelMat,player->modelMat,1*M_PI,vector3d(0,1,0));
               hitbox = gf3d_box(player->position,5.5,1,1,vector3d(6,0,0),NULL);
               player_flipped = 1;}
            player->position.x+=.01;
            gf3d_box_update(&a,player->position);
            if(gf3d_box_overlap(a,b)){
                if(gf3d_player_touch(player, b.ent) == 0){
                b = gf3d_box (vector3d(100,100,100),0,0,0,vector3d(0,0,0),NULL);
                spawned = 0;}
            player->position.x-=.01;
            gf3d_box_update(&a,player->position);
            }
            else{
            gfc_matrix_translate(player->modelMat,vector3d(.01,0,0));
            gf3d_vgraphics_camera_move(vector3d(.01,0,0));
            }
        }
        if (keys[SDL_SCANCODE_RIGHT]){
           if(player_flipped){
               gfc_matrix_rotate(player->modelMat,player->modelMat,1*M_PI,vector3d(0,1,0));
               hitbox = gf3d_box(player->position,5.5,1,1,vector3d(-6,0,0),NULL);
               player_flipped = 0;}
           player->position.x-=.01;
           gf3d_box_update(&a,player->position);
            if(gf3d_box_overlap(a,b)){
            if(gf3d_player_touch(player, b.ent) == 0){
                b = gf3d_box (vector3d(100,100,100),0,0,0,vector3d(0,0,0),NULL);
                spawned = 0;}
            player->position.x+=.01;
            gf3d_box_update(&a,player->position);
           }
            else{
            gfc_matrix_translate(player->modelMat,vector3d(-.01,0,0));
            gf3d_vgraphics_camera_move(vector3d(-.01,0,0));
            }
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
    if (keys[SDL_SCANCODE_RETURN] && currentTime > lastTime + 250){
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
    if (keys[SDL_SCANCODE_H]){
        if (spawned == 0){
        spawned = 1;
        health->_set = 1;
        gfc_matrix_translate(health->modelMat, vector3d(player->position.x - 10,player->position.y,0));
        health->position = vector3d(player->position.x - 10,player->position.y,0);
        b = gf3d_box(health->position, .5, 2.5, 2.0,vector3d(-10,0,0),health);
        }
        }
        if (keys[SDL_SCANCODE_A]){
        if (spawned == 0){
        spawned = 1;
        armor->_set = 1;
        gfc_matrix_translate(armor->modelMat, vector3d(player->position.x - 10,0,0));
        katana->position = vector3d(player->position.x - 10,player->position.y,0);
        b = gf3d_box(armor->position, 1, 1, 1,vector3d(-3.5,0,1),armor);
        }
        }
        if (keys[SDL_SCANCODE_K]){
        if (spawned == 0){
        spawned = 1;
        katana->_set = 1;
        gfc_matrix_translate(katana->modelMat, vector3d(player->position.x - 10,player->position.y,0));
        katana->position = vector3d(player->position.x - 10,player->position.y,0);
        b = gf3d_box(katana->position, .5, .5, 5.0,vector3d(-2,0,0),katana);
        }
        }
        
    if (keys[SDL_SCANCODE_E]){
        if (spawned == 0){
        enemy->health = 90;
        lastTime = currentTime;
        spawned = 1;
        enemy->_set = 1;
        b = gf3d_box(enemy->position, 3.0, 7.0, 5.0,vector3d(-8,7,0),enemy);
        }
    }
    if (keys[SDL_SCANCODE_B]){
        if (spawned == 0){
        boss->health = 180;
        lastTime = currentTime;
        spawned = 1;
        boss->_set = 1;
        b = gf3d_box(boss->position, 6.0, 6.0, 5.0,vector3d(-10.5,0,0),boss);
        }
    }
        }
        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);
            gf3d_model_draw(model2,bufferFrame,commandBuffer,modelMat,0);
            gf3d_model_draw(model,bufferFrame,commandBuffer,modelMat2,0);
            //gf3d_model_draw(a.model,bufferFrame,commandBuffer,a.mat,0);
            //gf3d_model_draw(hitbox.model,bufferFrame,commandBuffer,hitbox.mat,0);
           //gf3d_model_draw(b.model,bufferFrame,commandBuffer,b.mat,0);
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
            
        if(enemy->_set == 1 && currentTime > enemy->hitstun){
            if(enemy->health <= 0){
                enemy->_set = 0;
                b = gf3d_box (vector3d(100,100,100),0,0,0,vector3d(0,0,0),NULL);
                spawned = 0;
            }
        if(a.pos.x >= b.pos.x){
            if(flipped){
                gfc_matrix_rotate(enemy->modelMat,enemy->modelMat,1*M_PI,vector3d(0,1,0));
                flipped = 0;
            }
            if(gf3d_box_overlap(a,b)){
                if(currentTime > enemylastTime + 1500){
                enemylastTime = currentTime;
                gf3d_enemy_touch(player);
                enemy->position.x -=.005;
            gf3d_box_update(&b,enemy->position);
            }
            }
            else{
            enemy->position.x += .005;
            gf3d_box_update(&b,enemy->position);
            gfc_matrix_translate(enemy->modelMat,vector3d(.005,0,0));
            }
        }
        if(a.pos.y >= b.pos.y){
            if(gf3d_box_overlap(a,b)){
                if(currentTime > enemylastTime + 1500){
                enemylastTime = currentTime;
                gf3d_enemy_touch(player);
                enemy->position.y -=.005;
            gf3d_box_update(&b,enemy->position);
            }
            }
            else{
            enemy->position.y += .005;
            gf3d_box_update(&b,enemy->position);
            gfc_matrix_translate(enemy->modelMat,vector3d(0,.005,0));
            }
    }
    if(a.pos.x <= b.pos.x){
        if(!flipped)
        {
            gfc_matrix_rotate(enemy->modelMat,enemy->modelMat,1*M_PI,vector3d(0,1,0));
            flipped = 1;
        }
            if(gf3d_box_overlap(a,b)){
                if(currentTime > enemylastTime + 1500){
                enemylastTime = currentTime;
                gf3d_enemy_touch(player);
                enemy->position.x +=.005;
            gf3d_box_update(&b,enemy->position);
            }
            }
            else{
            enemy->position.x -= .005;
            gf3d_box_update(&b,enemy->position);
            gfc_matrix_translate(enemy->modelMat,vector3d(-.005,0,0));
            }
        }
        if(a.pos.y <= b.pos.y){
            if(gf3d_box_overlap(a,b)){
                if(currentTime > enemylastTime + 1500){
                enemylastTime = currentTime;
                gf3d_enemy_touch(player);
                enemy->position.y +=.005;
            gf3d_box_update(&b,enemy->position);
            }
            }
            else{
                enemy->position.y -= .005;
            gf3d_box_update(&b,enemy->position);
            gfc_matrix_translate(enemy->modelMat,vector3d(0,-.005,0));
            }
    }
        }
    if(boss->_set == 1 && currentTime > boss->hitstun){
        if(boss->health <= 0){
                boss->_set = 0;
                b = gf3d_box (vector3d(100,100,100),0,0,0,vector3d(0,0,0),NULL);
                spawned = 0;
            }
        if(a.pos.x >= b.pos.x){
            if(flipped)
        {
            gfc_matrix_rotate(enemy->modelMat,enemy->modelMat,1*M_PI,vector3d(0,1,0));
            flipped = 0;
            boss_flipped = 0;
        }
            if(gf3d_box_overlap(a,b)){
                if(currentTime > enemylastTime + 1500){
                enemylastTime = currentTime;
                gf3d_boss_touch(player);
                boss->position.x -=.0025;
            gf3d_box_update(&b,boss->position);
            }
            }
            else{
                boss->position.x += .0025;
            gf3d_box_update(&b,boss->position);
            gfc_matrix_translate(boss->modelMat,vector3d(.0025,0,0));
            }
        }
        if(a.pos.y >= b.pos.y){
            if(gf3d_box_overlap(a,b)){
                if(currentTime > enemylastTime + 1500){
                enemylastTime = currentTime;
                gf3d_boss_touch(player);
                boss->position.y -=.0025;
            gf3d_box_update(&b,boss->position);
            }
            }
            else{
                 boss->position.y += .0025;
            gf3d_box_update(&b,boss->position);
            gfc_matrix_translate(boss->modelMat,vector3d(0,.0025,0));
            }
    }
    if(a.pos.x <= b.pos.x){
        if(!flipped)
        {
            fliptime = currentTime + 5000;
            flipped = 1;
        }
        if(currentTime > fliptime && boss_flipped == 0){
            gfc_matrix_rotate(boss->modelMat,boss->modelMat,1*M_PI,vector3d(0,1,0));
            boss_flipped = 1;
        }
            if(gf3d_box_overlap(a,b)){
                if(currentTime > enemylastTime + 1500){
                enemylastTime = currentTime;
                gf3d_boss_touch(player);
                boss->position.x +=.0025;
            gf3d_box_update(&b,boss->position);
            }
            }
            else{
                boss->position.x -= .0025;
            gf3d_box_update(&b,boss->position);
            gfc_matrix_translate(boss->modelMat,vector3d(-.0025,0,0));
            }
        }
        if(a.pos.y <= b.pos.y){
            if(gf3d_box_overlap(a,b)){
                if(currentTime > enemylastTime + 1500){
                enemylastTime = currentTime;
                gf3d_boss_touch(player);
                boss->position.y +=.0025;
            gf3d_box_update(&b,boss->position);
            }
            }
            else{
                boss->position.y -= .0025;
            gf3d_box_update(&b,boss->position);
            gfc_matrix_translate(boss->modelMat,vector3d(0,-.0025,0));
            }
    }
    }
       int j = gf3d_entity_max() - 1;
        for (int i = 1; i < j; i++)
    {
        if (gf3d_entity_get(i)->_set == 1 && spawned == 1){
            gf3d_model_draw(gf3d_entity_get(i)->model,bufferFrame,commandBuffer,gf3d_entity_get(i)->modelMat,0);
        }
        else{
            continue;}
    }
           // gf3d_model_draw(model2,bufferFrame,commandBuffer,modelMat2);
                
            gf3d_command_rendering_end(commandBuffer);
            
        gf3d_vgraphics_render_end(bufferFrame);
        if(player->health <= 0.0){
        done = 1;}
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    if(player->health <= 0.0){
        slog("Game Over");}
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
