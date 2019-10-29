#ifndef __GF3D_SHAPE_H__
#define __GF3D_SHAPE_H__

#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "gf3d_model.h"
#include "gf3d_entity.h"
#include <SDL.h>

typedef struct
{
    double x1,y1,x2,y2,z1,z2;
}Edge;

typedef struct
{
    Vector3D pos;
    float radius;
}Sphere;

typedef struct
{
    Vector3D pos;
    float width,height,depth;
    Model* model;
    Matrix4 mat;
    Vector3D offset;
    Entity* ent;
}Box;

/**
 * @brief make a GF3D Box pointer
 * @param pos the position of the box
 * @param w the width of the box
 * @param h the height of the box
 * @param offset vector offset to align with model
 * @param Ent the entity to associate with this box
 * @return a GF3D Box pointer
 */
Box gf3d_box(Vector3D pos, float w, float h,float d, Vector3D offset, Entity *ent);

/**
 * @brief check if two boxes are overlapping
 * @param a pointer to box A
 * @param b pointer to box B
 * @return true if there is any overlap, false otherwise
 */
Uint8 gf3d_box_overlap(Box a,Box b);

/**
 * @brief check if two boxes are overlapping with a touching the rightmost side of b
 * @param a pointer to box A
 * @param b pointer to box B
 * @return true if there is any overlap on that side, false otherwise
 */
Uint8 gf3d_box_overlap_back(Box a, Box b);
/**
 * @brief update a box
 * @param r the Box 
 * @param pos position to update to
 * @return updated Box
 */
void gf3d_box_update(Box *r,Vector3D pos);

#endif
