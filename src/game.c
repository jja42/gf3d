#include <SDL.h>
#include <SDL_ttf.h>

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_model.h"
#include "gf3d_sprite.h"
#include "gf3d_entity.h"
#include "gf3d_shape.h"
#include "gf3d_input.h"
#include "gf3d_touch.h"
#include "gf3d_level.h"
#define M_PI 3.14159265358979323846
int main(int argc,char *argv[]) {
    //=========INIT VARS=========//
    Sprite *mouse = NULL;
    Sprite *start_button = NULL;
    Sprite *editor_button = NULL;
    Sprite *exit_button = NULL;
    Sprite *restart_button = NULL;
    Sprite *menu_button = NULL;
    Sprite *menu_background = NULL;
    Sprite *resume_button = NULL;
    Sprite *title = NULL;
    Sprite *off = NULL;
    Sprite *on = NULL;
    Sprite *large = NULL;
    Sprite *medium = NULL;
    Sprite *small = NULL;
    Sprite *rush = NULL;
    Sprite *destroy = NULL;
    Sprite *survive = NULL;
    Sprite *loot = NULL;
    Sprite *level1 = NULL;
    Sprite *level2 = NULL;
    Sprite *time_limit_text = NULL;
    Sprite *pickups = NULL;
    Sprite *enemies = NULL;
    Sprite *level_size = NULL;
    Sprite *objective = NULL;
    Sprite *save = NULL;
    Sprite *sword = NULL;
    Sprite *health_ui = NULL;
    Sprite *armor_ui = NULL;
    Sprite *health_bar = NULL;
    Sprite *armor_bar = NULL;
    Sprite *game_over = NULL;
    Level *level_1;
    Level *level_2;
    int mouseX, mouseY;
    Uint8 editing = 0;
    int level_num = 0;
    Uint8 edit = 0;
    Uint32 mouseFrame = 0;
    Uint8 paused = 0;
    int attack = 0;
    Uint8 menu = 1;
    Uint8 hit = 0;
    Uint8 attacking = 0;
    Uint8 level__1 = 1;
    Uint8 level__2 = 0;
    unsigned int lastTime = 0, currentTime, enemyLastTime = 0,respawnTime = 0;
    int done = 0;
    int c;
    Uint8 validate = 1;
    float frame = 0;
    const Uint8 *keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
    Model *model = NULL;
    Model *model2 = NULL;
    Matrix4 modelMat;
    Matrix4 modelMat2;
    int health_frame = 0;
    int armor_frame = 4;
    char *text = "";
    char *game_mode = NULL;
    Uint8 drops = 0;
    Uint8 timed = 0;
    int time_limit = 0;
    int enemy_max = 0;
    Vector3D levelStart;
    Vector3D levelEnd;
    Uint8 game__over = 0;
    Uint8 win = 0;
    Uint8 complete = 0;
    //=========VARS DONE=========//

    for (c = 1; c < argc; c++) {
        if (strcmp(argv[c], "-disable_validate") == 0) {
            validate = 0;
        }
    }
    init_logger("gf3d.log");
    slog("gf3d begin");
    gf3d_vgraphics_init(
            "gf3d",                 //program name
            1200,                   //screen width
            700,                    //screen height
            vector4d(0.51f, 0.75f, 1, 1),//background color
            0,                      //fullscreen
            validate                //validation
    );
    gf3d_entity_manager_init(10);
    // main game loop
    slog("gf3d main loop begin");


    //=========SETUP=========//
    mouse = gf3d_sprite_load("images/pointer.png", 32, 32, 16);
    start_button = gf3d_sprite_load("images/start_button.png", 471, 193, 1);
    editor_button = gf3d_sprite_load("images/edit_button.png", 471, 193, 1);
    exit_button = gf3d_sprite_load("images/exit_button.png", 471, 193, 1);
    restart_button = gf3d_sprite_load("images/restart_button.png", 471, 193, 1);
    menu_button = gf3d_sprite_load("images/menu_button.png", 471, 193, 1);
    menu_background = gf3d_sprite_load("images/menu_back.png", 2650, 1440, 1);
    title = gf3d_sprite_load("images/title.png", 1584, 225, 1);
    resume_button = gf3d_sprite_load("images/resume_button.png", 471, 193, 1);
    off = gf3d_sprite_load("images/off.png", 471, 193, 1);
    on = gf3d_sprite_load("images/on.png", 471, 193, 1);
    large = gf3d_sprite_load("images/large.png", 471, 193, 1);
    medium = gf3d_sprite_load("images/medium.png", 471, 193, 1);
    small = gf3d_sprite_load("images/small.png", 471, 193, 1);
    rush = gf3d_sprite_load("images/rush.png", 471, 193, 1);
    destroy = gf3d_sprite_load("images/destroy.png", 471, 193, 1);
    survive = gf3d_sprite_load("images/survive.png", 471, 193, 1);
    loot = gf3d_sprite_load("images/loot.png", 471, 193, 1);
    level1 = gf3d_sprite_load("images/level_1.png", 471, 193, 1);
    level2 = gf3d_sprite_load("images/level_2.png", 471, 193, 1);
    time_limit_text = gf3d_sprite_load("images/time_limit.png", 528, 75, 1);
    pickups = gf3d_sprite_load("images/pickups.png", 528, 75, 1);
    enemies = gf3d_sprite_load("images/enemies.png", 528, 75, 1);
    level_size = gf3d_sprite_load("images/level_size.png", 528, 75, 1);
    objective = gf3d_sprite_load("images/objective.png", 528, 75, 1);
    save = gf3d_sprite_load("images/save.png", 471, 193, 1);
    health_bar = gf3d_sprite_load("images/health_bar.png", 458, 65, 4);
    health_ui = gf3d_sprite_load("images/health_ui.png", 644, 194, 1);
    armor_bar = gf3d_sprite_load("images/armor_bar.png", 458, 65, 4);
    armor_ui = gf3d_sprite_load("images/armor_ui.png", 640, 190, 1);
    sword = gf3d_sprite_load("images/sword.png", 200, 200, 1);
    game_over = gf3d_sprite_load("images/game_over.png", 528, 75, 1);
    gf3d_entity_spawn_anim("samurai", 1, 101);
    Entity *player = gf3d_entity_get(0);
    gfc_matrix_translate(player->modelMat, vector3d(0, 0, -70));
    player->modelMat[3][3] += 8;
    player->health = 100;
    player->healthmax = 100;
    player->armor = 100;
    player->damage = 30;
    player->score = 0;
    model = gf3d_model_load("floor");
    model2 = gf3d_model_load("Wall");
    gfc_matrix_identity(modelMat);
    gfc_matrix_identity(modelMat2);
    modelMat2[3][3] -= .9f;
    modelMat[3][3] -= .8f;
    gf3d_entity_spawn_anim("robo", 1, 10);
    Entity *enemy = gf3d_entity_get(1);
    enemy->health = 90;
    enemy->modelMat[3][3] += 1;
    enemy->_set = 0;
    gf3d_entity_spawn("health");
    Entity *health = gf3d_entity_get(2);
    gf3d_entity_spawn("armor");
    Entity *armor = gf3d_entity_get(3);
    gf3d_entity_spawn("katana");
    Entity *katana = gf3d_entity_get(4);
    katana->_set = 1;
    gf3d_entity_spawn("boss");
    Entity *boss = gf3d_entity_get(5);
    boss->health = 180;
    gfc_matrix_rotate(modelMat2, modelMat2, 1.6f, vector3d(1, 0, 0));
    gfc_matrix_rotate(modelMat, modelMat, 1.6f, vector3d(0, 0, 1));
    gfc_matrix_rotate(modelMat, modelMat, 1.55f, vector3d(1, 0, 0));
    gfc_matrix_rotate(player->modelMat, player->modelMat, 1.6f, vector3d(1, 0, 0));
    gfc_matrix_rotate(player->modelMat, player->modelMat, 5.2f, vector3d(0, 1, 0));
    gfc_matrix_translate(modelMat2, vector3d(0, 0, -7.35f));
    gfc_matrix_rotate(enemy->modelMat, enemy->modelMat, 1.5f, vector3d(1, 0, 0));
    gfc_matrix_translate(enemy->modelMat, vector3d(0, 0, 12.25f));
    gfc_matrix_rotate(health->modelMat, health->modelMat, 1.5f, vector3d(1, 0, 0));
    gfc_matrix_rotate(health->modelMat, health->modelMat, 1.5f, vector3d(0, 1, 0));
    health->modelMat[3][3] += 1;
    gfc_matrix_rotate(armor->modelMat, armor->modelMat, 0, vector3d(0, 0, 1));
    gfc_matrix_rotate(armor->modelMat, armor->modelMat, 0, vector3d(0, 1, 0));
    gfc_matrix_rotate(boss->modelMat, boss->modelMat, 1.5f, vector3d(0, 1, 0));
    gfc_matrix_rotate(boss->modelMat, boss->modelMat, 1.5f, vector3d(0, 0, 1));
    boss->modelMat[3][3] -= .5f;
    armor->modelMat[3][3] -= .25f;
    katana->modelMat[3][3] += 10;
    gfc_matrix_rotate(katana->modelMat, katana->modelMat, 1.5f, vector3d(0, 1, 0));
    player->hitbox = gf3d_box(player->position, 2.5f, 2.0f, 8.0f, vector3d(-.5f, 0, 2));
    enemy->hitbox = gf3d_box(enemy->position, 3.0, 7.0, 5.0, vector3d(-8, 7, 0));
    health->hitbox = gf3d_box(health->position, .5, 2.5, 2.0, vector3d(-10, 0, 0));
    armor->hitbox = gf3d_box(armor->position, 1, 1, 1, vector3d(-3.5, 0, 1));
    boss->hitbox = gf3d_box(boss->position, 6.0, 6.0, 5.0, vector3d(-10.5, 0, 0));
    katana->hitbox = gf3d_box(katana->position, .5, .5, 5.0, vector3d(18, 0, 0));
    Box hitbox = gf3d_box(vector3d(100, 100, 100), 5.5f, 1, 1, vector3d(-6, 0, 3));
    //Box b = katana->hitbox;
    level_1 = gf3d_level_load("level_1");
    level_2 = gf3d_level_load("level_2");
    //=========SETUP COMPLETE=========//



    while (!done) {
        SDL_PumpEvents();   // update SDL's internal event structures
        currentTime = SDL_GetTicks();
        //=========MAIN MENU=========//
        SDL_GetMouseState(&mouseX, &mouseY);
        keys = SDL_GetKeyboardState(NULL);
        SDL_Event event;
        float prevhp = player->health;
        float prevarmor = player->armor;
        if (menu == 1 && edit == 0) {
            bufferFrame = gf3d_vgraphics_render_begin();
            gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_model_pipeline(), bufferFrame);
            gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_overlay_pipeline(), bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame, gf3d_vgraphics_get_graphics_model_pipeline());
            gf3d_command_rendering_end(commandBuffer);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame, gf3d_vgraphics_get_graphics_overlay_pipeline());
            gf3d_sprite_draw(menu_background, vector2d(0, 0), 0, bufferFrame, commandBuffer);
            gf3d_sprite_draw(title, vector2d(250, 50), 0, bufferFrame, commandBuffer);
            gf3d_sprite_draw(start_button, vector2d(500, 200), 0, bufferFrame, commandBuffer);
            gf3d_sprite_draw(editor_button, vector2d(500, 350), 0, bufferFrame, commandBuffer);
            gf3d_sprite_draw(exit_button, vector2d(500, 500), 0, bufferFrame, commandBuffer);
            gf3d_sprite_draw(mouse, vector2d(mouseX, mouseY), mouseFrame, bufferFrame, commandBuffer);
            gf3d_command_rendering_end(commandBuffer);

            gf3d_vgraphics_render_end(bufferFrame);
            while (SDL_PollEvent(&event)) // check to see if an event has happened
            {
                switch (event.type) {
                    case SDL_MOUSEBUTTONDOWN: // if the event is mouse click
                        //slog("%d",event.motion.x);
                        //slog("%d",event.motion.y);
                        if (event.motion.x >= 500 && event.motion.x <= 740 && event.motion.y >= 200 &&
                            event.motion.y <= 300)  // check if it is in the desired area
                        {
                            menu = 0;
                        }
                        if (event.motion.x >= 500 && event.motion.x <= 740 && event.motion.y >= 350 &&
                            event.motion.y <= 450) {
                            edit = 1;
                        }
                        if (event.motion.x >= 500 && event.motion.x <= 740 && event.motion.y >= 500 &&
                            event.motion.y <= 600) {
                            done = 1;
                        }

                }
            }
        }
        //=========EDIT MENU=========//
        if (edit == 1) {
            if (editing == 0) {
                enemy_max = 0;
                time_limit = 0;
                timed = 0;
                bufferFrame = gf3d_vgraphics_render_begin();
                gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_model_pipeline(), bufferFrame);
                gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_overlay_pipeline(), bufferFrame);
                commandBuffer = gf3d_command_rendering_begin(bufferFrame, gf3d_vgraphics_get_graphics_model_pipeline());
                gf3d_command_rendering_end(commandBuffer);
                commandBuffer = gf3d_command_rendering_begin(bufferFrame,
                                                             gf3d_vgraphics_get_graphics_overlay_pipeline());
                gf3d_sprite_draw(menu_background, vector2d(0, 0), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(level1, vector2d(500, 200), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(level2, vector2d(500, 350), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(menu_button, vector2d(500, 500), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(mouse, vector2d(mouseX, mouseY), mouseFrame, bufferFrame, commandBuffer);
                gf3d_command_rendering_end(commandBuffer);

                gf3d_vgraphics_render_end(bufferFrame);
                while (SDL_PollEvent(&event)) // check to see if an event has happened
                {
                    switch (event.type) {
                        case SDL_MOUSEBUTTONDOWN: // if the event is mouse click
                            if (event.motion.x >= 500 && event.motion.x <= 740 && event.motion.y >= 200 &&
                                event.motion.y <= 300) {
                                level_num = 1;
                                editing = 1;
                            }
                            if (event.motion.x >= 500 && event.motion.x <= 740 && event.motion.y >= 350 &&
                                event.motion.y <= 450) {
                                level_num = 2;
                                editing = 1;
                            }
                            if (event.motion.x >= 500 && event.motion.x <= 740 && event.motion.y >= 500 &&
                                event.motion.y <= 600) {
                                edit = 0;
                            }

                    }
                }
            }
            if (editing == 1) {
                bufferFrame = gf3d_vgraphics_render_begin();
                gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_model_pipeline(), bufferFrame);
                gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_overlay_pipeline(), bufferFrame);
                commandBuffer = gf3d_command_rendering_begin(bufferFrame, gf3d_vgraphics_get_graphics_model_pipeline());
                gf3d_command_rendering_end(commandBuffer);
                commandBuffer = gf3d_command_rendering_begin(bufferFrame,
                                                             gf3d_vgraphics_get_graphics_overlay_pipeline());
                gf3d_sprite_draw(menu_background, vector2d(0, 0), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(objective, vector2d(500, 25), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(rush, vector2d(100, 75), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(destroy, vector2d(350, 75), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(loot, vector2d(600, 75), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(survive, vector2d(850, 75), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(level_size, vector2d(500, 200), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(small, vector2d(150, 250), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(medium, vector2d(500, 250), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(large, vector2d(850, 250), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(enemies, vector2d(250, 375), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(time_limit_text, vector2d(750, 375), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(pickups, vector2d(500, 450), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(on, vector2d(250, 500), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(off, vector2d(750, 500), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(save, vector2d(500, 600), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(mouse, vector2d(mouseX, mouseY), mouseFrame, bufferFrame, commandBuffer);
                gf3d_command_rendering_end(commandBuffer);

                gf3d_vgraphics_render_end(bufferFrame);
                while (SDL_PollEvent(&event)) // check to see if an event has happened
                {
                    switch (event.type) {

                        case SDL_MOUSEBUTTONDOWN: // if the event is mouse click
                            if (event.motion.x >= 100 && event.motion.x <= 340 && event.motion.y >= 75 &&
                                event.motion.y <= 175) {
                                game_mode = "rush";
                                slog("rush");
                            }
                            if (event.motion.x >= 350 && event.motion.x <= 590 && event.motion.y >= 75 &&
                                event.motion.y <= 175) {
                                game_mode = "destroy";
                                slog("destroy");
                            }
                            if (event.motion.x >= 600 && event.motion.x <= 840 && event.motion.y >= 75 &&
                                event.motion.y <= 175) {
                                game_mode = "loot";
                                slog("loot");
                            }
                            if (event.motion.x >= 850 && event.motion.x <= 1090 && event.motion.y >= 75 &&
                                event.motion.y <= 175) {
                                game_mode = "survive";
                                slog("survive");
                            }
                            if (event.motion.x >= 150 && event.motion.x <= 390 && event.motion.y >= 250 &&
                                event.motion.y <= 350) {
                                levelStart = vector3d(20, 20, 10);
                                levelEnd = vector3d(-20, -20, 0);
                                slog("small");
                            }
                            if (event.motion.x >= 500 && event.motion.x <= 740 && event.motion.y >= 250 &&
                                event.motion.y <= 350) {
                                levelStart = vector3d(40, 40, 10);
                                levelEnd = vector3d(-40, -40, 0);
                                slog("medium");
                            }
                            if (event.motion.x >= 850 && event.motion.x <= 1090 && event.motion.y >= 250 &&
                                event.motion.y <= 350) {
                                levelStart = vector3d(100, 100, 10);
                                levelEnd = vector3d(-100, -100, 0);
                                slog("large");
                            }
                            if (event.motion.x >= 250 && event.motion.x <= 500 && event.motion.y >= 375 &&
                                event.motion.y <= 425) {
                                enemy_max++;
                            }
                            if (event.motion.x >= 750 && event.motion.x <= 1000 && event.motion.y >= 375 &&
                                event.motion.y <= 425) {
                                time_limit += 100;
                                timed = 1;
                            }
                            if (event.motion.x >= 250 && event.motion.x <= 490 && event.motion.y >= 500 &&
                                event.motion.y <= 600) {
                                drops = 1;
                            }
                            if (event.motion.x >= 750 && event.motion.x <= 990 && event.motion.y >= 500 &&
                                event.motion.y <= 600) {
                                drops = 0;
                            }
                            if (event.motion.x >= 500 && event.motion.x <= 740 && event.motion.y >= 600 &&
                                event.motion.y <= 700) {
                                gf3d_level_save(level_num, levelStart, levelEnd, game_mode, enemy_max, drops, timed,
                                                time_limit);
                                edit = 0;
                                editing = 0;
                                break;
                            }

                    }
                }
            }
        }
        //=========GAME LOOP=========//
        if (menu != 1) {
            if (paused != 1) {
                if (level__1 == 1) {
                    game_mode = level_1->game_mode;
                    levelStart = level_1->levelStart;
                    levelEnd = level_1->levelEnd;
                    enemy_max = level_1->enemy_max;
                    drops = level_1->drops;
                    timed = level_1->timed;
                    time_limit = level_1->time_limit;
                }
                if (level__2 == 1) {
                    game_mode = level_2->game_mode;
                    levelStart = level_2->levelStart;
                    levelEnd = level_2->levelEnd;
                    enemy_max = level_2->enemy_max;
                    drops = level_2->drops;
                    timed = level_2->timed;
                    time_limit = level_2->time_limit;
                }
                if (strcmp((const char *) &level_1->game_mode, "rush") == 0) {
                    Vector3D goal = vector3d(levelEnd.x, levelEnd.y, levelEnd.z);
                    if (player->position.x >= goal.x) {
                        //win = 1;
                        level__1 = 0;
                        level__2 = 1;
                    }
                    if (timed == 1) {
                        currentTime = SDL_GetTicks();
                        if (currentTime / 100 > time_limit) {
                            game__over = 1;
                        }
                    }
                }
                if (strcmp((const char *) &level_1->game_mode, "destroy") == 0) {
                    int goal = enemy_max;
                    if (enemy_max == 0) {
                        //win = 1;
                        level__1 = 0;
                        level__2 = 1;
                    }
                    if (enemy->_set != 1 && currentTime > respawnTime) {
                        enemy_max--;
                        enemy->_set = 1;
                    }
                    if (timed == 1) {
                        currentTime = SDL_GetTicks();
                        if (currentTime / 100 > time_limit) {
                            game__over = 1;
                        }
                    }

                }
                if (strcmp((const char *) &level_1->game_mode, "loot") == 0) {
                    int goal = 200;
                    if (timed == 1) {
                        currentTime = SDL_GetTicks();
                        if (currentTime / 100 > time_limit) {
                            game__over = 1;
                        }
                    }
                    if (player->score >= 200) {
                        //win = 1;
                        level__1 = 0;
                        level__2 = 1;
                    }
                    if (enemy->_set != 1 && currentTime > respawnTime) {
                        player->score += 50;
                        enemy->_set = 1;
                    }

                }
                if (strcmp((const char *) &level_1->game_mode, "survive") == 0) {
                    if (timed == 1) {
                        currentTime = SDL_GetTicks();
                        if (currentTime / 100 > time_limit) {
                            //win = 1;
                            level__1 = 0;
                            level__2 = 1;
                        }
                    }
                    if (enemy->_set != 1 && currentTime > respawnTime) {
                        enemy->_set = 1;
                    }
                }
                if (strcmp((const char *) &level_2->game_mode, "rush") == 0) {
                    Vector3D goal = vector3d(levelEnd.x, levelEnd.y, levelEnd.z);
                    if (player->position.x >= goal.x) {
                        //complete = 1;
                        level__2 = 0;
                        menu = 1;
                    }
                    if (timed == 1) {
                        currentTime = SDL_GetTicks();
                        if (currentTime / 100 > time_limit) {
                            game__over = 1;
                        }
                    }
                }
                if (strcmp((const char *) &level_2->game_mode, "destroy") == 0) {
                    int goal = enemy_max;
                    if (enemy_max == 0) {
                        //complete = 1;
                        level__2 = 0;
                        menu = 1;
                    }
                    if (enemy->_set != 1 && currentTime > respawnTime) {
                        enemy_max--;
                        enemy->_set = 1;
                    }
                    if (timed == 1) {
                        currentTime = SDL_GetTicks();
                        if (currentTime / 100 > time_limit) {
                            game__over = 1;
                        }
                    }
                }
                if (strcmp((const char *) &level_2->game_mode, "loot") == 0) {
                    int goal = 200;
                    if (timed == 1) {
                        currentTime = SDL_GetTicks();
                        if (currentTime / 100 > time_limit) {
                            game__over = 1;
                        }
                    }
                    if (player->score >= 200) {
                        //complete = 1;
                        level__2 = 0;
                        menu = 1;
                    }
                    if (enemy->_set != 1 && currentTime > respawnTime) {
                        player->score += 50;
                        enemy->_set = 1;
                    }
                }
                if (strcmp((const char *) &level_2->game_mode, "survive") == 0) {
                    if (timed == 1) {
                        currentTime = SDL_GetTicks();
                        if (currentTime / 100 > time_limit) {
                            //complete = 1;
                            level__2 = 0;
                            menu = 1;
                        }
                    }
                    if (enemy->_set != 1 && currentTime > respawnTime) {
                        enemy->_set = 1;
                    }
                }
                if (attacking != 1) {
                    //=========INPUT=========//
                    if (keys[SDL_SCANCODE_P]) {
                        paused = 1;
                    }
                    if (keys[SDL_SCANCODE_UP]) {
                        gf3d_move_char(1, player, &player->hitbox);
                    }
                    if (keys[SDL_SCANCODE_DOWN]) {
                        gf3d_move_char(2, player, &player->hitbox);
                    }
                    if (keys[SDL_SCANCODE_LEFT]) {
                        gf3d_move_char(3, player, &player->hitbox);
                    }
                    if (keys[SDL_SCANCODE_RIGHT]) {
                        gf3d_move_char(4, player, &player->hitbox);
                    }
                    int j = gf3d_entity_max() - 1;
                    for (int i = 1; i < j; i++) {
                        if (gf3d_entity_get(i) != NULL && gf3d_entity_get(i)->_set == 1) {
                            if (gf3d_box_overlap(player->hitbox, gf3d_entity_get(i)->hitbox)) {
                                gf3d_player_touch(player, gf3d_entity_get(i));
                            }
                        }
                    }
                    if (keys[SDL_SCANCODE_DOWN] && keys[SDL_SCANCODE_R]) {
                        gf3d_vgraphics_rotate_camera(-0.001f, 2);
                    }
                    if (keys[SDL_SCANCODE_RIGHT] && keys[SDL_SCANCODE_R]) {
                        gf3d_vgraphics_rotate_camera(0.001f, 0);
                    }
                    if (keys[SDL_SCANCODE_LEFT] && keys[SDL_SCANCODE_R]) {
                        gf3d_vgraphics_rotate_camera(-0.001f, 0);
                    }
                    if (keys[SDL_SCANCODE_UP] && keys[SDL_SCANCODE_R]) {
                        gf3d_vgraphics_rotate_camera(0.001f, 2);
                    }
                    if (keys[SDL_SCANCODE_SPACE] && currentTime > lastTime + 250) {
                        if (attacking == 0 && attack == 0) {
                            lastTime = currentTime;
                            attacking = 1;
                            gf3d_box_update(&hitbox, player->position);
                        }
                        if (attacking == 0 && attack == 1) {
                            if (currentTime <= lastTime + 1500) {
                                gf3d_box_update(&hitbox, player->position);
                                attacking = 1;
                                lastTime = currentTime;
                                frame = 24;
                            } else {
                                attack = 0;
                            }
                        }
                        if (attacking == 0 && attack == 2) {
                            if (currentTime <= lastTime + 1500) {
                                gf3d_box_update(&hitbox, player->position);
                                attacking = 1;
                                lastTime = currentTime;
                                frame = 38;
                            } else {
                                attack = 0;
                            }
                        }
                    }
                }
                gf3d_enemy_think(player, enemy, currentTime, enemyLastTime);
                float currenthp = player->health;
                float currentarmor = player->armor;
                if (currentarmor < prevarmor || currenthp < prevhp) {
                    enemyLastTime = currentTime;
                }
                if(enemy->health < 0){
                    respawnTime = currentTime + 2500;
                    enemy->_set = 0;
                    enemy->health = 90;
                }
            }
            //=========3D RENDERING=========//
            bufferFrame = gf3d_vgraphics_render_begin();
            gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_model_pipeline(), bufferFrame);
            gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_overlay_pipeline(), bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame, gf3d_vgraphics_get_graphics_model_pipeline());
            Box a = player->hitbox;
            gf3d_model_draw(model2, bufferFrame, commandBuffer, modelMat, 0);
            gf3d_model_draw(model, bufferFrame, commandBuffer, modelMat2, 0);
            //gf3d_model_draw(a.model, bufferFrame, commandBuffer, a.mat, 0);
            //gf3d_model_draw(hitbox.model, bufferFrame, commandBuffer, hitbox.mat, 0);
            //gf3d_model_draw(b.model, bufferFrame, commandBuffer, b.mat, 0);
            //=========RESOLVE GAME LOGIC=========//
            if (attacking == 1) {
                gf3d_model_draw(player->model, bufferFrame, commandBuffer, player->modelMat, (Uint32) frame);
                frame = frame + 0.05f;
                if (attack == 0) {
                    if (frame >= 15 && hit == 0) {
                        hit = 1;
                    }
                    if (frame >= 20) {
                        frame = 0;
                        hit = 0;
                        attack = 1;
                        attacking = 0;
                    }
                }
                if (attack == 1) {
                    if (frame >= 31 && hit == 0) {
                        hit = 1;
                    }

                    if (frame >= 38) {
                        frame = 0;
                        hit = 0;
                        attack = 2;
                        attacking = 0;
                    }
                }
                if (attack == 2) {
                    if (frame >= 54 && hit == 0) {
                        hit = 1;
                    }

                    if (frame >= 70) {
                        frame = 0;
                        hit = 0;
                        attacking = 0;
                        attack = 0;
                    }
                }
                if (hit == 1) {
                    int j = gf3d_entity_max() - 1;
                    for (int i = 1; i < j; i++) {
                        if (gf3d_entity_get(i) != NULL && gf3d_entity_get(i)->_set == 1) {
                            if (gf3d_box_overlap(hitbox, gf3d_entity_get(i)->hitbox)) {
                                Entity *ent = gf3d_entity_get(i);
                                ent->health -= player->damage;
                                ent->hitstun = currentTime + 1000;
                                slog("Enemy Health:%f", ent->health);
                            }
                        } else {
                            continue;
                        }
                    }
                }
            } else {
                gf3d_model_draw(player->model, bufferFrame, commandBuffer, player->modelMat, 0);
            }
            int j = gf3d_entity_max() - 1;
            for (int i = 1; i < j; i++) {
                if (gf3d_entity_get(i) != NULL && gf3d_entity_get(i)->_set == 1) {
                    gf3d_model_draw(gf3d_entity_get(i)->model, bufferFrame, commandBuffer,
                                    gf3d_entity_get(i)->modelMat,
                                    0);

                } else {
                    continue;
                }
            }
            gf3d_command_rendering_end(commandBuffer);
            //=========2D RENDERING=========//
            float currenthp = player->health;
            float currentarmor = player->armor;
            health_frame = (100 - currenthp) / 25;
            armor_frame = (100 - currentarmor) / 25;
            commandBuffer = gf3d_command_rendering_begin(bufferFrame, gf3d_vgraphics_get_graphics_overlay_pipeline());
            gf3d_sprite_draw(health_ui, vector2d(0, 0), 0, bufferFrame, commandBuffer);
            if (health_frame < 4) {
                gf3d_sprite_draw(health_bar, vector2d(90, 32), health_frame, bufferFrame, commandBuffer);
            }
            gf3d_sprite_draw(armor_ui, vector2d(0, 100), 0, bufferFrame, commandBuffer);
            if (armor_frame < 4) {
                gf3d_sprite_draw(armor_bar, vector2d(90, 132), armor_frame, bufferFrame, commandBuffer);
            }
            if (player->damage > 30) {
                gf3d_sprite_draw(sword, vector2d(100, 200), 0, bufferFrame, commandBuffer);
            }
            if(level__1 == 1){
                gf3d_sprite_draw(level1,vector2d(500,50),0,bufferFrame,commandBuffer);
            }
            if(level__2 == 1){
                gf3d_sprite_draw(level2,vector2d(500,50),0,bufferFrame,commandBuffer);
            }
            if (paused == 1) {
                gf3d_sprite_draw(menu_button, vector2d(500, 200), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(resume_button, vector2d(500, 350), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(exit_button, vector2d(500, 500), 0, bufferFrame, commandBuffer);
                gf3d_sprite_draw(mouse, vector2d(mouseX, mouseY), mouseFrame, bufferFrame, commandBuffer);
                while (SDL_PollEvent(&event)) // check to see if an event has happened
                {
                    switch (event.type) {
                        case SDL_MOUSEBUTTONDOWN: // if the event is mouse click
                            //slog("%d",event.motion.x);
                            //slog("%d",event.motion.y);
                            if (event.motion.x >= 500 && event.motion.x <= 740 && event.motion.y >= 200 &&
                                event.motion.y <= 300)  // check if it is in the desired area
                            {
                                menu = 1;
                            }
                            if (event.motion.x >= 500 && event.motion.x <= 740 && event.motion.y >= 350 &&
                                event.motion.y <= 450) {
                                paused = 0;
                            }
                            if (event.motion.x >= 500 && event.motion.x <= 740 && event.motion.y >= 500 &&
                                event.motion.y <= 600) {
                                done = 1;
                            }

                    }
                }
            }
            gf3d_command_rendering_end(commandBuffer);

            gf3d_vgraphics_render_end(bufferFrame);
        }
    if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
}
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
