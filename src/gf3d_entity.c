#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"
#include "simple_json.h"
#include "gf3d_entity.h"

typedef struct
{
    Entity *entity_list;
    int  entity_max;
}EntityManager;

static EntityManager gf3d_entity_manager = {0};

void gf3d_entity_manager_close()
{
    if(gf3d_entity_manager.entity_list != NULL)
    {
        free(gf3d_entity_manager.entity_list);
    }
    memset(&gf3d_entity_manager,0,sizeof(EntityManager));
}

void gf3d_entity_manager_init(Uint32 entity_max)
{
    gf3d_entity_manager.entity_max = entity_max;
    gf3d_entity_manager.entity_list = (Entity*)gfc_allocate_array(sizeof(Entity),entity_max);
    if (!gf3d_entity_manager.entity_list)
    {
        slog("failed to allocate entity list");
        return;
    }
    atexit(gf3d_entity_manager_close);
}

Entity *gf3d_entity_new()
{
    int i;
    for (i = 0; i < gf3d_entity_manager.entity_max; i++)
    {
        if (gf3d_entity_manager.entity_list[i]._inuse)continue;
        //. found a free entity
        memset(&gf3d_entity_manager.entity_list[i],0,sizeof(Entity));
        gf3d_entity_manager.entity_list[i]._inuse = 1;
        return &gf3d_entity_manager.entity_list[i];
    }
    slog("request for entity failed: all full up");
    return NULL;
}
void gf3d_entity_spawn(Entity * ent)
{   
    ent->model = gf3d_model_load(ent->name);
    gfc_matrix_identity(ent->modelMat);
     gfc_matrix_make_translation(
            ent->modelMat,
            ent->position
        );
}

Entity *gf3d_entity_load(char *filename)
{
    TextLine assetname;
    snprintf(assetname,GFCLINELEN,"entities/%s.json",filename);
    Entity *ent = NULL;
    ent = gf3d_entity_new();
    if (!ent)
    {
        return NULL;
    }
    SJson *json,*ent_info;
    if (!filename)return NULL;
    json = sj_load(assetname);
    if (!json)
    {
        slog("failed to entity file %s",filename);
        return NULL;
    }
    ent_info = sj_object_get_value(json,"ent_info");
    if (!ent_info)
    {
        slog("missing entity info in entity file %s",filename);
        sj_free(json);
        gf3d_entity_free(ent);
        return NULL;
    }
    strncpy(ent->name,sj_get_string_value(sj_object_get_value(ent_info,"name")),GFCLINELEN);
    
    sj_get_integer_value((ent_info,"position.x"),&ent->position.x);
    sj_get_integer_value((ent_info,"position.y"),&ent->position.y);
    sj_get_integer_value((ent_info,"position.z"),&ent->position.z);
    sj_free(json);
    slog("loaded ent info for %s",filename);
    return ent;
}

int gf3d_entity_max(){
    if (gf3d_entity_manager.entity_max != NULL){
        return gf3d_entity_manager.entity_max;
    }
    else{
        return 0;}
}

Entity *gf3d_entity_get(int i){
    if (gf3d_entity_manager.entity_list[i]._set = 1){
        return &gf3d_entity_manager.entity_list[i];
    }
    return NULL;
}
void gf3d_entity_free(Entity *self)
{
    if (!self)
    {
        slog("self pointer is not valid");
        return;
    }
    self->_inuse = 0;
    gf3d_model_free(self->model);
    if (self->data != NULL)
    {
        slog("warning: data not freed at entity free!");
    }
}


/*eol@eof*/
