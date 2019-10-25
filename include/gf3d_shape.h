#ifndef __GF3D_SHAPE_H__
#define __GF3D_SHAPE_H__

#include "gfc_vector.h"
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
}Box;

/**
 * @brief make a GF3D Box pointer
 * @param pos the position of the box
 * @param w the width of the box
 * @param h the height of the box
 * @return a GF3D Box pointer
 */
Box gf3d_box(Vector3D pos, float w, float h, float d);

/**
 * @brief make a GF3D Sphere
 * @param x the center x
 * @param y the center y
 * @param r the radius
 * @return a GF3D Box
 */
Sphere gf3d_sphere(Vector3D, float r);

/**
 * @brief make an edge shape basesd on the points provided
 * @param x1 the X component of starting point
 * @param y1 the Y component of starting point
 * @param x2 the X component of ending point
 * @param y2 the Y component of ending point
 * @return the shape
 */
Edge gf3d_edge(float x1,float y1,float x2,float y2, float z1, float z2);

/**
 * @brief determine if the point lies within the box
 * @param pos the point to check
 * @param r the box to check
 * @return true if the point is inside the box, false otherwise
 */
Uint8 gf3d_point_in_box(Vector3D pos,Box r);

/**
 * @brief check if two boxes are overlapping
 * @param a pointer to box A
 * @param b pointer to box B
 * @return true if there is any overlap, false otherwise
 */
Uint8 gf3d_box_overlap(Box a,Box b);

/**
 * @brief check if the point lies within the sphere c
 * @param p the point to check
 * @param c the sphere to check
 * @return true if the point is in the sphere, false otherwise
 */
Uint8 gf3d_point_in_sphere(Vector3D pos,Sphere s);

/**
 * @brief check if two spheres are overlapping
 * @param a sphere A
 * @param b sphere B
 * @param returns true is there is overlap, false otherwise
 */
Uint8 gf3d_sphere_overlap(Sphere a, Sphere b);

/**
 * @brief check if a sphere and box overlap
 * @param a the Sphere
 * @param b the Box
 * @return true if there is any overlap, false otherwise
 */
Uint8 gf3d_sphere_box_overlap(Sphere a, Box b);

/**
 * @brief update a box
 * @param r the Box 
 * @param pos position to update to
 * @return updated Box
 */
void gf3d_box_update(Box *r,Vector3D pos);

#endif
