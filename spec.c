#include "SDW.h"

// COLOR
struct Color newColor(float r, float g, float b, float a)
{
    struct Color res;
    res.r = r;
    res.g = g;
    res.b = b;
    res.a = a;
    return res;
}
struct Color baseColor()
{
    return newColor(.1,.1,.1,1);
}
void setColor(struct Color color)
{
    glColor4f(color.r, color.g, color.b, color.a);
}

// SPEC
/**
 * newSpec
 *
 * convenience method for constructing new objects of struct
 */
struct Spec newSpec(
    double x, double y, double z,
    double dx, double dy, double dz,
    double sx, double sy, double sz,
    struct Color emission, struct Color specular, struct Color shiny)
{
    struct Spec res;
    res.x = x;
    res.y = y;
    res.z = z;
    res.dx = dx;
    res.dy = dy;
    res.dz = dz;
    res.sx = sx;
    res.sy = sy;
    res.sz = sz;
    res.emission = emission;
    res.shiny = shiny;
    res.specular = specular;
    return res;
}


/**
 * newSpecDefaultColor
 * TODO: consider refactor that this is what I really want typically
 *       while spec with color is rare to use... so update names
 *       for easier dev
 */
struct Spec newSpecDefaultColor(
    double x, double y, double z,
    double dx, double dy, double dz,
    double sx, double sy, double sz)
{
    return newSpec(x,y,z , dx,dy,dz , sx,sy,sz , baseColor(),baseColor(),baseColor());
}


/**
 * baseSpec
 *
 * a convenience method to construct a base specification
 */
struct Spec baseSpec()
{
    return newSpec(0,0,0, 0,0,0, 1,1,1, baseColor(), baseColor(), baseColor());
}


/**
 * applySpec
 *
 * a method to apply the specification transforms
 * in the correct order.
 */
void applySpec(struct Spec spec)
{
    // apply colors
    float shiny[] = {spec.shiny.r, spec.shiny.g, spec.shiny.b, spec.shiny.a};
    glMaterialfv(GL_FRONT,GL_SHININESS,shiny);
    float specular[] = {spec.specular.r, spec.specular.g, spec.specular.b, spec.specular.a};
    glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
    float emission[] = {spec.emission.r, spec.emission.g, spec.emission.b, spec.emission.a};
    glMaterialfv(GL_FRONT,GL_EMISSION,emission);

    // happens last: move its origin point
    glTranslated(spec.x, spec.y, spec.z);

    // currently applies in y, x, z order
    // so y should be the smallest angle
    // and z should be the largest
    // it can be subtle but this will make a difference
    glRotatef(spec.dz,0,0,1);  // z
    glRotatef(spec.dx,1,0,0);  // x
    glRotatef(spec.dy,0,1,0);  // y
                               //
    // happens first: change the size/stretch it
    glScaled(spec.sx, spec.sy, spec.sz);

    errorCheck("applySpec");
}
