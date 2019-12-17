#ifndef __GF3D_ENTITY_H__
#define __GF3D_ENTITY_H__
#include "gf3d_model.h"
#include "simple_json.h"
#include "gf3d_shape.h"

typedef struct Entity_S
{
    Uint8           _inuse; /**<flag to keep track if this isntance is in use and should not be reassigned*/
    Uint8           _set;
    Model          *model;          /**<the 3d model for this entity*/
    Matrix4         modelMat;
    Vector3D        position;       /**<position of the entity in 3d space*/
    Vector3D        velocity;       /**<velocity of the entity in 3d space*/
    Vector3D        acceleration;   /**<acceleration of the entity in 3d space*/
    Vector3D        rotation;       /**<yaw, pitch, and roll of the entity*/
    Vector3D        scale;          /**<*please default to 1,1,1*/
    void (*think) (struct Entity_S* self);   /**<function called on entity think*/
    void (*update)(struct Entity_S* self);   /**<function called on entity update*/
    void (*touch) (struct Entity_S* self,struct Entity_S* other);   /**<function called on entity think*/
    float           health;
    float           healthmax;
    float           armor;
    Uint32          hitstun;
    float           damage;
    float           otherStuff;
    char*           name;
    char*           tag;
    Box             hitbox;
    void *data;                     /**<additional entity specific data*/

    int score;
}Entity;

/**
 * @brief initializes the entity subsystem
 * @param entity_max maximum number of simultaneous entities you wish to support
 */
void gf3d_entity_manager_init(Uint32 entity_max);

/**
 * @brief get an empty entity from the system
 * @return NULL on out of space or a pointer to an entity otherwise
 */
Entity *gf3d_entity_new();

/**
 * @brief free an active entity
 * @param self the entity to free
 */
void  gf3d_entity_free(Entity *self);

/**
 * @brief allows the max for the entity manager to be accessed
 * @return the max number of entities, 0 if entity manager is not initialized
 */
int gf3d_entity_max();

/**
 * @brief access an initialized entity
 * @param index the index of the entity to access
 * @return pointer to an entity if valid, null otherwise
 */
Entity *gf3d_entity_get(int index);

/**
 * @brief set up an entity to be drawn, using information from entity load
 * @param ent pointer to a loaded entity
 */
void gf3d_entity_spawn(char* filename);
/**
 * @brief set up an animated entity to be drawn, using information from entity load
 * @param ent pointer to a loaded entity
 */
void gf3d_entity_spawn_anim(char* filename, Uint32 startframe, Uint32 endframe);
/**
 * @brief takes information from json file and initializes the entity with that information
 * @param filename name of the file to be loaded
 */
Entity *gf3d_entity_load(char *filename);
#endif
