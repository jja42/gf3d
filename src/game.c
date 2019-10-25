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

int main(int argc,char *argv[])
{
    int attack = 0;
    Uint8 hit = 0;
    Uint8 attacking = 0;
    unsigned int lastTime = 0, currentTime;
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
    //gf3d_entity_spawn("burger");
    gf3d_entity_spawn_anim("samurai",1,101);
    Entity *player = gf3d_entity_get(0);
    Entity *enemy = gf3d_entity_get(1);
    gfc_matrix_translate(player->modelMat,vector3d(0,0,-70));
    player->modelMat[3][3] += 8;
    //model = gf3d_model_load_animated("agumon_animated",5, 29);
    model = gf3d_model_load("floor");
    model2 = gf3d_model_load("Wall");
    gfc_matrix_identity(modelMat);
    //modelMat[3][3] -= .95;
    gf3d_entity_spawn("dino");
    gfc_matrix_identity(modelMat2);
    modelMat2[3][3] -= .9;
    modelMat[3][3] -= .8;
    gfc_matrix_rotate( modelMat2, modelMat2, 1.6, vector3d(1,0,0));
    gfc_matrix_rotate( modelMat, modelMat, 1.6, vector3d(0,0,1));
    gfc_matrix_rotate( modelMat, modelMat, 1.55, vector3d(1,0,0));
    gfc_matrix_rotate( player->modelMat, player->modelMat, 1.6, vector3d(1,0,0));
    gfc_matrix_rotate( player->modelMat, player->modelMat, 5.2, vector3d(0,1,0));
    gfc_matrix_translate(modelMat2, vector3d(0,0,-7.35));
    gfc_matrix_rotate( enemy->modelMat, enemy->modelMat, 1.5, vector3d(0,0,1));
    Box a = gf3d_box(player->position, 2.0, 10.0, 10.0);
    Box b = gf3d_box(enemy->position, 3.3, 5.0, 5.0);
    Box hitbox = gf3d_box(player->position,11,1,1);
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        currentTime = SDL_GetTicks();
        if(attacking != 1){
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here
        if (keys[SDL_SCANCODE_UP]){
            player->position.z += .01;
            gfc_matrix_translate(player->modelMat,vector3d(0,-.01,0));
            gf3d_vgraphics_camera_move(vector3d(0,0,.01));
        }
        if (keys[SDL_SCANCODE_DOWN]){
            player->position.z += .01;
            gfc_matrix_translate(player->modelMat,vector3d(0,.01,0));
            gf3d_vgraphics_camera_move(vector3d(0,0,-.01));
        }
        if (keys[SDL_SCANCODE_LEFT]){
            player->position.x+=.01;
            gf3d_box_update(&a,player->position);
            if(gf3d_box_overlap(a,b)){
                slog("Can't move, colliding!");
            player->position.x-=.01;
            gf3d_box_update(&a,player->position);
            }
            else{
            gfc_matrix_translate(player->modelMat,vector3d(.01,0,0));
            gf3d_vgraphics_camera_move(vector3d(.01,0,0));
            }
        }
        if (keys[SDL_SCANCODE_RIGHT]){
           player->position.x-=.01;
           gf3d_box_update(&a,player->position);
            if(gf3d_box_overlap(b,a)){
                slog("Can't move, colliding!");
            player->position.x+=.01;
            gf3d_box_update(&a,player->position);
           }
            else{
            gfc_matrix_translate(player->modelMat,vector3d(-.01,0,0));
            gf3d_vgraphics_camera_move(vector3d(-.01,0,0));
            }
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
        if(currentTime <= lastTime + 750){
            attacking = 1;
            lastTime = currentTime;
            frame = 24;
        }
        else{
        attack = 0;
        }
        }
        if (attacking == 0 && attack == 2){
        if(currentTime <= lastTime + 750){
            attacking = 1;
            lastTime = currentTime;
            frame = 38;
        }
        else{
        attack = 0;
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
            if (attacking == 1){
                gf3d_model_draw(player->model,bufferFrame,commandBuffer,player->modelMat,(Uint32)frame);
               frame = frame + 0.05;
               if(attack == 0){
               if(frame >= 15 && hit == 0){
                hit =  1;
                if(gf3d_box_overlap(b,hitbox)){
                slog("hit entity");
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
               if (frame >= 38){
                   frame = 0;
                   hit = 0;
                   attacking = 0;
                   attack = 2;
                }
               }
                if (attack == 2){
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
        if (gf3d_entity_get(i)->model != NULL){
            gf3d_model_draw(gf3d_entity_get(i)->model,bufferFrame,commandBuffer,gf3d_entity_get(i)->modelMat,0);
        }
        else{
            continue;}
    }
           // gf3d_model_draw(model2,bufferFrame,commandBuffer,modelMat2);
                
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
