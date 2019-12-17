//
// Created by gomachi on 12/15/19.
//

#ifndef GF3D_GF3D_LEVEL_H
#define GF3D_GF3D_LEVEL_H
#include "gfc_vector.h"
typedef struct
{
    Vector3D        levelStart; //lower bounds of level
    Vector3D        levelEnd;   //upper bounds of level
    char*           game_mode;  //objective of level
    int             enemy_max;  //how many enemies to spawn
    Uint8           drops;      //do enemies drop pickups
    Uint8           timed;      //is there a time limit
    int             time_limit; //what is the time limit

    void *data;                     /**<additional data*/

}Level;
/**
 * @brief takes information from json file and initializes the level with that information
 * @param filename name of the file to be loaded
 */
Level *gf3d_level_load(char *filename);
/**
 * @brief takes information from selected options and initializes a json file with that level info
 * @param level_num which level to alter
 * @param levelStart start coordinates for the level map
 * @param levelEnd end coordinates for the level map
 * @param game_mode what the objective of the level is
 * @param enemy_max how many enemies to spawn throughout the level
 * @param drops whether or not enemies drop pickups
 * @param timed whether or not the level is time dependant
 * @param time_limit the max time allowed for timed levels
 */

void *gf3d_level_save(int level_num,Vector3D levelStart,Vector3D levelEnd,char* game_mode,int enemy_max,Uint8 drops,Uint8 timed,int time_limit);
#endif //GF3D_GF3D_LEVEL_H
