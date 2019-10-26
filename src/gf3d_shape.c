#include "gf3d_shape.h"
#include "simple_logger.h"
#include "gf3d_model.h"
#include "gfc_matrix.h"

Box gf3d_box(Vector3D pos, float w, float h,float d)
{
    Box r;
    r.model = gf3d_model_load("cube");
    gfc_matrix_identity(r.mat);
    r.pos.x = pos.x;
    r.pos.y = pos.y;
    r.pos.z = pos.z;
    r.width = w;
    r.height = h;
    r.depth = d;
    gfc_matrix_make_translation(r.mat, pos);
    return r;
}
void gf3d_box_update(Box *r,Vector3D pos){
    r->pos.x = pos.x;
    r->pos.y = pos.y;
    r->pos.z = pos.z;
    gfc_matrix_make_translation(r->mat, pos);
    r->mat[0][0] = r->width;
    r->mat[1][1] = r->height;
    r->mat[2][2] = r->depth;
}
    
Sphere gf3d_sphere(Vector3D pos, float r)
{
    Sphere c;
    c.pos.x = pos.x;
    c.pos.y = pos.y;
    c.pos.z = pos.z;
    c.radius = r;
    return c;
}

Uint8 gf3d_point_in_box(Vector3D pos,Box r)
{
    if ((pos.x >= r.pos.x) && (pos.x <= r.pos.x + r.width)&&
        (pos.y >= r.pos.y) && (pos.y <= r.pos.y + r.height)){
        return 1;
    }
    return 0;
}

Uint8 gf3d_box_overlap(Box a,Box b)
{
    if ((a.pos.x +a.width >= b.pos.x-b.width)&&(a.pos.x -a.width <= b.pos.x+b.width)&&
        (a.pos.y +a.height >= b.pos.y-b.height)&&(a.pos.y -a.height <= b.pos.y+b.height)&&
        (a.pos.z +a.depth >= b.pos.z-b.depth)&&(a.pos.z -a.depth <= b.pos.z+b.depth))
    {
        return 1;
    }
    return 0;
}

Uint8 gf3d_point_in_sphere(Vector3D pos, Sphere c)
{
    if (vector3d_magnitude_compare(vector3d(c.pos.x-pos.x,c.pos.y-pos.y,c.pos.z-pos.z),c.radius) <= 0)return 1;
    return 0;
}

Uint8 gf3d_sphere_overlap(Sphere a, Sphere b)
{
    Vector3D v;
    vector3d_set(v,a.pos.x - b.pos.x,a.pos.y - b.pos.y,a.pos.z - b.pos.z);
    if (vector3d_magnitude_compare(v,a.radius+b.radius) <= 0)
    {
        return 1;
    }
    return 0;
}

Uint8 gf3d_sphere_box_overlap(Sphere a, Box b)
{
    Box newbox;
    newbox = gf3d_box(vector3d(b.pos.x - a.radius,b.pos.y,b.pos.z),b.width + a.radius+ a.radius,b.height,b.depth);
    if (gf3d_point_in_box(vector3d(a.pos.x,a.pos.y,a.pos.z),newbox))return 1;
    newbox = gf3d_box(vector3d(b.pos.x,b.pos.y - a.radius,b.pos.z),b.width,b.height + a.radius + a.radius,b.depth);
    if (gf3d_point_in_box(vector3d(a.pos.x,a.pos.y, a.pos.z),newbox))return 1;
    newbox = gf3d_box(vector3d(b.pos.x,b.pos.y,b.pos.z - a.radius),b.width,b.height, b.depth + a.radius + a.radius);
    if (gf3d_point_in_box(vector3d(a.pos.x,a.pos.y, a.pos.z),newbox))return 1;
    
    if (gf3d_point_in_sphere(vector3d(b.pos.x,b.pos.y,b.pos.z),a))return 1;
    if (gf3d_point_in_sphere(vector3d(b.pos.x+b.width,b.pos.y,b.pos.z),a))return 1;
    if (gf3d_point_in_sphere(vector3d(b.pos.x,b.pos.y+b.height,b.pos.z),a))return 1;
    if (gf3d_point_in_sphere(vector3d(b.pos.x,b.pos.y,b.pos.z+b.depth),a))return 1;
    if (gf3d_point_in_sphere(vector3d(b.pos.x+b.width,b.pos.y+b.height,b.pos.z),a))return 1;
    if (gf3d_point_in_sphere(vector3d(b.pos.x+b.width,b.pos.y+b.height,b.pos.z+b.depth),a))return 1;
    if (gf3d_point_in_sphere(vector3d(b.pos.x+b.width,b.pos.y,b.pos.z+b.depth),a))return 1;
    if (gf3d_point_in_sphere(vector3d(b.pos.x,b.pos.y+b.height,b.pos.z+b.depth),a))return 1;
    return 0;
}
/*eol@eof*/
