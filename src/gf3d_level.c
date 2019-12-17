#include <stdlib.h>
#include <string.h>
#include <gfc_text.h>
#include "simple_json.h"
#include "gf3d_level.h"
#include "simple_logger.h"
Level *gf3d_level_load(char *filename)
{
    TextLine assetname;
    snprintf(assetname,GFCLINELEN,"levels/%s.json",filename);
    Level *level = malloc(4*sizeof(Level));
    SJson *json,*level_info;
    if (!filename)return NULL;
    json = sj_load(assetname);
    if (!json)
    {
        slog("failed to open level file %s",filename);
        return NULL;
    }
    level_info = sj_object_get_value(json,"level_info");
    if (!level_info)
    {
        slog("missing level info in level file %s",filename);
        sj_free(json);
        return NULL;
    }
    strncpy((char *) &level->game_mode, sj_get_string_value(sj_object_get_value(level_info, "name")), GFCLINELEN);
    int x1 = 0;
    sj_get_integer_value(sj_object_get_value(level_info,"start.x"),&x1);
    int y1 = 0;
    sj_get_integer_value(sj_object_get_value(level_info,"start.y"),&y1);
    int z1= 0;
    sj_get_integer_value(sj_object_get_value(level_info,"start.z"),&z1);
    int x2 = 0;
    sj_get_integer_value(sj_object_get_value(level_info,"end.x"),&x2);
    int y2 = 0;
    sj_get_integer_value(sj_object_get_value(level_info,"end.y"),&y2);
    int z2= 0;
    sj_get_integer_value(sj_object_get_value(level_info,"end.z"),&z2);
    sj_get_integer_value(sj_object_get_value(level_info,"enemies"),&level->enemy_max);
    sj_get_integer_value(sj_object_get_value(level_info,"drops"), (int *) &level->drops);
    sj_get_integer_value(sj_object_get_value(level_info,"timed"), (int *) &level->timed);
    sj_get_integer_value(sj_object_get_value(level_info,"time_limit"),&level->time_limit);
    level->levelStart = vector3d(x1,y1,z1);
    level->levelEnd = vector3d(x2,y2,z2);
    sj_free(json);
    slog("loaded level info for %s",filename);
    return level;
}
void *gf3d_level_save(int level_num,Vector3D levelStart,Vector3D levelEnd,char* game_mode,int enemy_max,Uint8 drops,Uint8 timed,int time_limit)
{
    char* filename;
    if(level_num == 1){
        filename = "level_1";
    }
    else{
        filename = "level_2";
    }
    SJson *json,*level_info;
    TextLine assetname;
    snprintf(assetname,GFCLINELEN,"levels/%s.json",filename);
    json = sj_object_new();
    level_info = sj_object_new();
    sj_object_insert(json,"level_info",level_info);
    //level_info = sj_object_get_value(json,"level_info");
    sj_object_insert(level_info,"name",sj_new_str(game_mode));
    sj_object_insert(level_info,"start.x",sj_new_int((int)levelStart.x));
    sj_object_insert(level_info,"start.y",sj_new_int((int)levelStart.y));
    sj_object_insert(level_info,"start.z",sj_new_int((int)levelStart.z));
    sj_object_insert(level_info,"end.x",sj_new_int((int)levelEnd.x));
    sj_object_insert(level_info,"end.y",sj_new_int((int)levelEnd.y));
    sj_object_insert(level_info,"end.z",sj_new_int((int)levelEnd.z));
    sj_object_insert(level_info,"enemies",sj_new_int(enemy_max));
    sj_object_insert(level_info,"drops",sj_new_int((int)drops));
    sj_object_insert(level_info,"timed",sj_new_int((int)timed));
    sj_object_insert(level_info,"time_limit",sj_new_int(time_limit));
    sj_save(json,assetname);
    slog("saved level info for %s",filename);
}