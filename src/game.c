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

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    Uint8 validate = 1;
    float frame = 0;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
    Model *model = NULL;
    Matrix4 modelMat;
    /*Model *model2 = NULL;
    Matrix4 modelMat2;*/
    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"-disable_validate") == 0)
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
    gf3d_entity_manager_init(2);
    // main game loop
    slog("gf3d main loop begin");
    gf3d_entity_spawn(gf3d_entity_load("dino"));
    //model = gf3d_model_load_animated("agumon_animated",5, 29);

    gfc_matrix_identity(modelMat);
    //gfc_matrix_translate(modelMat,vector3d(1.25,0,0));
   // model = gf3d_model_load("dino");
    /*gfc_matrix_rotate(
            modelMat,
            modelMat,
            3.10,
            vector3d(0,0,1)
        );*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here
        if (keys[SDL_SCANCODE_UP]){
            gfc_matrix_make_translation(modelMat,vector3d(0,-.01,0));
            //gf3d_vgraphics_camera_move(vector3d(0,0,.01));
        }
        if (keys[SDL_SCANCODE_DOWN]){
            gfc_matrix_translate(modelMat,vector3d(0,.01,0));
        }
        if (keys[SDL_SCANCODE_LEFT]){
            //gfc_matrix_translate(modelMat,vector3d(.01,0,0));
             gfc_matrix_rotate(
            modelMat,
            modelMat,
            .001,
            vector3d(0,0,.001));
        }
        if (keys[SDL_SCANCODE_RIGHT]){
            gfc_matrix_translate(modelMat,vector3d(-.01,0,0));
           /*gfc_matrix_rotate(
            modelMat,
            modelMat,
            .001,
            vector3d(0,0,-.001));*/
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
    if (keys[SDL_SCANCODE_RETURN]){
        //gf3d_entity_set();
    }

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);
        gf3d_model_draw(model,bufferFrame,commandBuffer,modelMat);
                //gf3d_model_draw(model,bufferFrame,commandBuffer,modelMat,(Uint32)frame);
               // frame = frame + 0.05;
                //if (frame >= 24)frame = 0;
        int j = gf3d_entity_max() - 1;
        for (int i = 0; i < j; i++)
    {
        if (gf3d_entity_get(i)->model != NULL){
            gf3d_model_draw(gf3d_entity_get(i)->model,bufferFrame,commandBuffer,gf3d_entity_get(i)->modelMat);
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
