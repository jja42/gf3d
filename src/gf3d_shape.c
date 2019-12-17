#include "gf3d_shape.h"
#include "simple_logger.h"
#include "gf3d_model.h"
#include "gfc_matrix.h"

Box gf3d_box(Vector3D pos, float w, float h,float d, Vector3D offset)
{
    Box r;
    r.model = gf3d_model_load("cube");
    gfc_matrix_identity(r.mat);
    r.pos.x = pos.x + offset.x;
    r.pos.y = pos.y + offset.y;
    r.pos.z = pos.z + offset.z;
    r.offset.x = offset.x;
    r.offset.y = offset.y;
    r.offset.z = offset.z;
    r.width = w;
    r.height = h;
    r.depth = d;
    gfc_matrix_make_translation(r.mat, r.pos);
    r.mat[0][0] = w;
    r.mat[1][1] = h;
    r.mat[2][2] = d;
    return r;
}
void gf3d_box_update(Box *r,Vector3D pos){
    r->pos.x = pos.x + r->offset.x;
    r->pos.y = pos.y + r->offset.y;
    r->pos.z = pos.z + r->offset.z;
    gfc_matrix_make_translation(r->mat, r->pos);
    r->mat[0][0] = r->width;
    r->mat[1][1] = r->height;
    r->mat[2][2] = r->depth;
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

Uint8 gf3d_box_overlap_back(Box a,Box b)
{
    if (a.pos.x >= b.pos.x)
    {
        return 1;
    }
    return 0;
}
/*eol@eof*/
