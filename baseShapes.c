#include "SDW.h"

/**
 * ourTexturedSpherePolarVertex
 *
 * convenience method to plot polar coordinate as cartisian
 *
 * th: polar lattitude
 * ph: polar longitude
 *
 * Credit: CSCI 5229 example: ex8 Vertex
 */
void ourTexturedSpherePolarVertex(double th, double ph)
{
    // color added to make this not look flat without any lighting at play...
    double x = Sin(th)*Cos(ph);
    double y = Sin(ph);
    double z = Cos(th)*Cos(ph);
    glNormal3d(x,y,z);
    double xTex = x/2+0.5;
    double yTex = y/2+0.5;
    double zTex = z/2+0.5;
        // glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
    glTexCoord3f(xTex, yTex, zTex);
    glVertex3d(x,y,z);
    debugFlood("ourPolarVertex: th: %lf, ph: %lf", th, ph);
}

/**
 * drawTexturedSphere
 *
 * low stepInterval value means more triangles and smoother surface at a cost use wisely
 * ideally a value evenly divisble by 360
 *
 * Credit: CSCI 5229 examples: ex8 sphere1 (sphere2 has cull issues)
 */
void drawTexturedSphere(struct Spec spec, unsigned int texture, GLint mode, double stepInterval)
{

    glPushMatrix();
    applySpec(spec);

    // setup texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, mode);
    glBindTexture(GL_TEXTURE_2D,texture);

    int th;
    int ph;

    // lower cap
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_TRIANGLE_FAN);
    ourTexturedSpherePolarVertex(0,-90);
    for (th=360;th>0;th-=stepInterval) ourTexturedSpherePolarVertex(th, stepInterval-90);
    // handle edge case of step interval not evenly divisble by 360
    th = 0 ; ourTexturedSpherePolarVertex(th, stepInterval-90);
    glEnd();

    // center
    for (ph=stepInterval-90;ph<90-2*stepInterval;ph+=stepInterval)
    {   
        glBindTexture(GL_TEXTURE_2D,texture);
        glBegin(GL_QUAD_STRIP);
        for (int th=360;th>0;th-=stepInterval)
        {
            ourTexturedSpherePolarVertex(th,ph);
            ourTexturedSpherePolarVertex(th,ph+stepInterval);
        }
        // handle edge case of step interval not evenly divisble by 360
        th = 0;
        ourTexturedSpherePolarVertex(th,ph);
        ourTexturedSpherePolarVertex(th,ph+stepInterval);
        glEnd();
    }
    // handle edge case of step interval not evenly divisble by 360
    ph = 90-2*stepInterval;
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_QUAD_STRIP);
    for (int th=360;th>0;th-=stepInterval)
    {
        ourTexturedSpherePolarVertex(th,ph);
        ourTexturedSpherePolarVertex(th,ph+stepInterval);
    }
    // handle edge case of step interval not evenly divisble by 360
    th = 0;
    ourTexturedSpherePolarVertex(th,ph);
    ourTexturedSpherePolarVertex(th,ph+stepInterval);
    glEnd();


    // upper cap
    glBegin(GL_TRIANGLE_FAN);
    ourTexturedSpherePolarVertex(0,90);
    for (int th=0;th<360;th+=stepInterval) ourTexturedSpherePolarVertex(th,90-stepInterval);
    // handle edge case of step interval not evenly divisble by 360
    th = 360 ; ourTexturedSpherePolarVertex(th,90-stepInterval);
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    errorCheck("drawTexturedSphere");
}


/**
 * ourSpherePolarVertex
 *
 * convenience method to plot polar coordinate as cartisian
 *
 * th: polar lattitude
 * ph: polar longitude
 *
 * Credit: CSCI 5229 example: ex8 Vertex
 */
void ourSpherePolarVertex(double th, double ph)
{
   // color added to make this not look flat without any lighting at play...
   double x = Sin(th)*Cos(ph);
   double y = Sin(ph);
   double z = Cos(th)*Cos(ph);
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
   debugFlood("ourPolarVertex: th: %lf, ph: %lf", th, ph);
}


/**
 * drawSphere
 *
 * low stepInterval value means more triangles and smoother surface at a cost use wisely
 * ideally a value evenly divisble by 360
 *
 * Credit: CSCI 5229 examples: ex8 sphere1 (sphere2 has cull issues)
 */
void drawSphere(struct Spec spec, double stepInterval)
{

    glPushMatrix();
    applySpec(spec);

    int th;
    int ph;

    // lower cap
    glBegin(GL_TRIANGLE_FAN);
    ourSpherePolarVertex(0,-90);
    for (th=360;th>0;th-=stepInterval) ourSpherePolarVertex(th, stepInterval-90);
    // handle edge case of step interval not evenly divisble by 360
    th = 0 ; ourSpherePolarVertex(th, stepInterval-90);
    glEnd();

    // center
    for (ph=stepInterval-90;ph<90-2*stepInterval;ph+=stepInterval)
    {   
        glBegin(GL_QUAD_STRIP);
        for (int th=360;th>0;th-=stepInterval)
        {
            ourSpherePolarVertex(th,ph);
            ourSpherePolarVertex(th,ph+stepInterval);
        }
        // handle edge case of step interval not evenly divisble by 360
        th = 0;
        ourSpherePolarVertex(th,ph);
        ourSpherePolarVertex(th,ph+stepInterval);
        glEnd();
    }
    // handle edge case of step interval not evenly divisble by 360
    ph = 90-2*stepInterval;
    glBegin(GL_QUAD_STRIP);
    for (int th=360;th>0;th-=stepInterval)
    {
        ourSpherePolarVertex(th,ph);
        ourSpherePolarVertex(th,ph+stepInterval);
    }
    // handle edge case of step interval not evenly divisble by 360
    th = 0;
    ourSpherePolarVertex(th,ph);
    ourSpherePolarVertex(th,ph+stepInterval);
    glEnd();


    // upper cap
    glBegin(GL_TRIANGLE_FAN);
    ourSpherePolarVertex(0,90);
    for (int th=0;th<360;th+=stepInterval) ourSpherePolarVertex(th,90-stepInterval);
    // handle edge case of step interval not evenly divisble by 360
    th = 360 ; ourSpherePolarVertex(th,90-stepInterval);
    glEnd();

    glPopMatrix();

    errorCheck("drawSphere");
}


/**
 * drawTexturedRibbon
 *
 * valPairs are in the shape f1, g1, f2, g2, ...
 *          where fi is the base line
 *          and gi is the secondary line
 * countValPairs is the length of valPairs to make things easier
 *
 * NOTE: creates second side by a bias in the z plane
 * ... SO, ribbon vertical must be in the xy plane!
 *
 * TODO: better comments
 * TODO: better testing of normals, it has bugs
 */
void drawTexturedRibbon(struct Spec spec,
        struct Coord valPairs[],
        int countValPairs,
        unsigned int texture,
        GLint mode)
{
    glPushMatrix();
    applySpec(spec);

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, mode);

    struct Coord myCoord0 = valPairs[0];
    struct Coord myCoord1 = valPairs[1];
    struct Coord myCoord2, myCoord3;
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_QUAD_STRIP);
    int i = 2;
    while (i < countValPairs)
    {
        double foo = (double) i / countValPairs;
        myCoord2 = valPairs[i];
        i++;
        myCoord3 = valPairs[i];
        i++;
        double bar = (double) i / countValPairs;

        double dx = myCoord0.x - myCoord1.x;
        double dy = myCoord0.y - myCoord1.y;
        double dz = myCoord0.z - myCoord1.z;

        // TODO: fix normal
        glNormal3f(-dx, -dy, -dz);
        glTexCoord2f(foo,0) ; plotCoord(myCoord0);
        glTexCoord2f(foo,1) ; plotCoord(myCoord1);
        glTexCoord2f(bar,0) ; plotCoord(myCoord2);
        glTexCoord2f(bar,1) ; plotCoord(myCoord3);

        myCoord0 = myCoord2;
        myCoord1 = myCoord3;
    }
    
    glTexCoord2f(1,0) ; plotCoord(myCoord2);
    glTexCoord2f(1,1) ; plotCoord(myCoord3);
    glEnd();

    // draw the reverse side
    myCoord0 = valPairs[0];
    myCoord1 = valPairs[1];
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_QUAD_STRIP);
    i = 2;
    while (i < countValPairs - 2)
    {
        double foo = (double) i / countValPairs;
        myCoord2 = valPairs[i];
        i++;
        myCoord3 = valPairs[i];
        i++;
        double bar = (double) i / countValPairs;

        double dx = myCoord0.x - myCoord1.x;
        double dy = myCoord0.y - myCoord1.y;
        double dz = myCoord0.z - myCoord1.z;

        // TODO: fix normal
        glNormal3f(-dx, -dy, dz);
        glTexCoord2f(foo,1) ; glVertex3f(myCoord1.x, myCoord1.y, myCoord1.z+0.001);
        glTexCoord2f(foo,0) ; glVertex3f(myCoord0.x, myCoord0.y, myCoord0.z+0.001);
        glTexCoord2f(bar,1) ; glVertex3f(myCoord3.x, myCoord3.y, myCoord3.z+0.001);
        glTexCoord2f(bar,0) ; glVertex3f(myCoord2.x, myCoord2.y, myCoord2.z+0.001);

        myCoord0 = myCoord2;
        myCoord1 = myCoord3;
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
    glPopMatrix();
    errorCheck("drawTexturedRibbon");
}


/**
 * drawTexturedTube
 *
 * centers is an array of points in some line function 
 * countCenters should be the length of the centers array (for convenience)
 *
 * for normal reasons, this must be in the xz plane, y coord is ignored
 * you can use spec to shift it if needed afterward.
 */
void drawTexturedTube(struct Spec spec,
        struct Coord centers[],
        int countCenters,
        double radius,
        unsigned int texture,
        GLint mode,
        double stepInterval,
        bool hasCap,
        bool hasBase)
{
    glPushMatrix();
    applySpec(spec);

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, mode);

    int th;
    const double rep = 1;

    if (countCenters < 2)
    {
        fatal("foobar in drawTexturedTube required 2, but found %d", countCenters);
    }

    // CREDIT: https://stackoverflow.com/questions/1597830/iterate-through-a-c-array
    struct Coord myCoord0 = centers[0];
    struct Coord myCoord1;
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_QUAD_STRIP);
    for(int i = 1; i < countCenters; i++)
    {
        myCoord1 = centers[i];

        double basex = myCoord0.x;
        double basez = myCoord0.z;
        double capx = myCoord1.x;
        double capz = myCoord1.z;
        GLfloat foo = (i-1)/((double) countCenters);
        GLfloat bar = (i)/((double) countCenters);

        for (th=0;th<=360;th+=stepInterval)
        {
            glNormal3d((basex + capx)/2, radius*Cos(th), basez + radius*Sin(th));
            glTexCoord3f(foo, radius*Cos(th), radius*Sin(th));
            glVertex3d(basex,radius*Cos(th),basez+radius*Sin(th));
            glTexCoord3f(bar, radius*Cos(th), radius*Sin(th));
            glVertex3d(capx,radius*Cos(th),capz+radius*Sin(th));
            th += stepInterval;
            glNormal3d((basex+capx)/2, radius*Cos(th), radius*Sin(th));
            glTexCoord3f(foo, radius*Cos(th), radius*Sin(th));
            glVertex3d(basex,radius*Cos(th),basez+radius*Sin(th));
            glTexCoord3f(bar, radius*Cos(th), radius*Sin(th));
            glVertex3d(capx,radius*Cos(th),capz+radius*Sin(th));
        }
        glNormal3d((basex + capx)/2, radius*Cos(th), basez + radius*Sin(th));
        glTexCoord3f(foo, radius*Cos(th), radius*Sin(th));
        glVertex3d(basex,radius*Cos(th),basez+radius*Sin(th));
        glTexCoord3f(bar, radius*Cos(th), radius*Sin(th));
        glVertex3d(capx,radius*Cos(th),capz+radius*Sin(th));
        th = 360;
        glNormal3d((basex+capx)/2, radius*Cos(th), radius*Sin(th));
        glTexCoord3f(foo, radius*Cos(th), radius*Sin(th));
        glVertex3d(basex,radius*Cos(th),basez+radius*Sin(th));
        glTexCoord3f(bar, radius*Cos(th), radius*Sin(th));
        glVertex3d(capx,radius*Cos(th),capz+radius*Sin(th));

        // set base for next iteration
        myCoord0 = myCoord1;
    }
    glEnd();

    double basex = myCoord0.x;
    double basez = myCoord0.z;
    if (hasBase) {
        glBindTexture(GL_TEXTURE_2D,texture);
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(-1,0,0);
        glTexCoord2f(.5,.5);
        glVertex3d(basex, 0.0, basez);
        for (th=360;th>0;th-=stepInterval)
        {
            glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
            glVertex3d(basex,radius*Cos(th),basez+radius*Sin(th));
        }
        // must overlap 0 with 360, out of for loop ensures lap without stepInterval as even mod 360
        th = 0;
        glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
        glVertex3d(basex, radius*Cos(th), basez+radius*Sin(th));
        glEnd();
    }

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    errorCheck("drawTexturedTube");
}


/**
 * drawTexturedUnitCylinder
 *
 * low stepInterval value means more triangles and smoother surface use wisely
 * ideally a value evenly divisble by 360
 *
 * TODO: note that a stepInterval = 90 produces a cube, reauthor cube to leverage this?
 *
 * Credit: CSCI 5229 examples: ex8 SolidPlane Fuselage, Tailpipe, ex14 circle or cone
 */
void drawTexturedUnitCylinder(struct Spec spec,
        unsigned int texture,
        GLint mode,
        double stepInterval,
        bool hasCap,
        bool hasBase)
{
    int th = 0;
    const double rep = 1;
    double length = 1;
    double radius = 1;
    double base = length / 2;
    double cap = -length / 2;
    glPushMatrix();
    applySpec(spec);
    
    // setup texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, mode);
    glBindTexture(GL_TEXTURE_2D,texture);

    // base e.g. tailpipe
    if (hasBase)
    {
        glBindTexture(GL_TEXTURE_2D,texture);
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(-1,0,0);
        glTexCoord2f(.5,.5);
        glVertex3d(base, 0.0, 0.0);
        for (th=360;th>0;th-=stepInterval)
        {
            glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
            glVertex3d(base, radius*Cos(th), radius*Sin(th));
            th -= stepInterval;
            glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
            glVertex3d(base, radius*Cos(th), radius*Sin(th));
        }
        // must overlap 0 with 360, out of for loop ensures lap without stepInterval as even mod 360
        th = 0;
        glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
        glVertex3d(base, radius*Cos(th), radius*Sin(th));
        glEnd();
    }

    // core e.g. fuselage
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<3600;th+=stepInterval)
    {
        glNormal3d((base+cap)/2, radius*Cos(th), radius*Sin(th));
        glTexCoord3f(0, radius*Cos(th), radius*Sin(th));
        glVertex3d(base,radius*Cos(th),radius*Sin(th));
        glTexCoord3f(1, radius*Cos(th), radius*Sin(th));
        glVertex3d(cap,radius*Cos(th),radius*Sin(th));
        th += stepInterval;
        glNormal3d((base+cap)/2, radius*Cos(th), radius*Sin(th));
        glTexCoord3f(0, radius*Cos(th), radius*Sin(th));
        glVertex3d(base,radius*Cos(th),radius*Sin(th));
        glTexCoord3f(1, radius*Cos(th), radius*Sin(th));
        glVertex3d(cap,radius*Cos(th),radius*Sin(th));
    }
    // must overlap 0 with 360, out of for loop ensures lap without stepInterval as even mod 360
    th = 360;
    glNormal3d((base+cap)/2, radius*Cos(th), radius*Sin(th));
    glTexCoord3f(0, radius*Cos(th), radius*Sin(th));
    glVertex3d(base,radius*Cos(th),radius*Sin(th));
    glTexCoord3f(1, radius*Cos(th), radius*Sin(th));
    glVertex3d(cap,radius*Cos(th),radius*Sin(th));
    glEnd();

    // cap e.g. tailpipe
    if (hasCap)
    {
        glBindTexture(GL_TEXTURE_2D,texture);
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(+1,0,0);
        glTexCoord2f(.5,.5);
        glVertex3d(cap, 0.0, 0.0);
        for (th=0;th<360;th+=stepInterval)
        {
            glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
            glVertex3d(cap,radius*Cos(th),radius*Sin(th));
        }
        // must overlap 0 with 360, out of for loop ensures lap without stepInterval as even mod 360
        th = 360;
        glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
        glVertex3d(cap, radius*Cos(th), radius*Sin(th));
        glEnd();
    }

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    errorCheck("drawTexturedUnitCylinder");
}



/**
 * drawTexturedCylinder
 *
 * low stepInterval value means more triangles and smoother surface use wisely
 * ideally a value evenly divisble by 360
 *
 * TODO: note that a stepInterval = 90 produces a cube, reauthor cube to leverage this?
 * TODO: rewrite over drawTexturedUnitCylinder
 *
 * Credit: CSCI 5229 examples: ex8 SolidPlane Fuselage, Tailpipe, ex14 circle or cone
 */
void drawTexturedCylinder(struct Spec spec,
        double radius,
        double length,
        unsigned int texture,
        GLint mode,
        double stepInterval)
{
    int th = 0;
    const double rep = 1;
    double base = spec.x - length / 2;
    double cap = spec.x + length / 2;
    glPushMatrix();
    applySpec(spec);
    
    // setup texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, mode);
    glBindTexture(GL_TEXTURE_2D,texture);

    // base e.g. tailpipe
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(-1,0,0);
    glTexCoord2f(.5,.5);
    glVertex3d(base, 0.0, 0.0);
    for (th=360;th>0;th-=stepInterval)
    {
        glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
        glVertex3d(base, radius*Cos(th), radius*Sin(th));
        th -= stepInterval;
        glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
        glVertex3d(base, radius*Cos(th), radius*Sin(th));
    }
    // must overlap 0 with 360, out of for loop ensures lap without stepInterval as even mod 360
    th = 0;
    glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
    glVertex3d(base, radius*Cos(th), radius*Sin(th));
    glEnd();

    // core e.g. fuselage
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_QUAD_STRIP);
    for (th=360;th>0;th-=stepInterval)
    {
        glNormal3d((base+cap)/2, radius*Cos(th), radius*Sin(th));
        glTexCoord3f(0, radius*Cos(th), radius*Sin(th));
        glVertex3d(base,radius*Cos(th),radius*Sin(th));
        glTexCoord3f(1, radius*Cos(th), radius*Sin(th));
        glVertex3d(cap,radius*Cos(th),radius*Sin(th));
        th -= stepInterval;
        glNormal3d((base+cap)/2, radius*Cos(th), radius*Sin(th));
        glTexCoord3f(0, radius*Cos(th), radius*Sin(th));
        glVertex3d(base,radius*Cos(th),radius*Sin(th));
        glTexCoord3f(1, radius*Cos(th), radius*Sin(th));
        glVertex3d(cap,radius*Cos(th),radius*Sin(th));
    }
    // must overlap 0 with 360, out of for loop ensures lap without stepInterval as even mod 360
    th = 0;
    glNormal3d((base+cap)/2, radius*Cos(th), radius*Sin(th));
    glTexCoord3f(0, radius*Cos(th), radius*Sin(th));
    glVertex3d(base,radius*Cos(th),radius*Sin(th));
    glTexCoord3f(1, radius*Cos(th), radius*Sin(th));
    glVertex3d(cap,radius*Cos(th),radius*Sin(th));
    glEnd();

    // cap e.g. tailpipe
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(+1,0,0);
    glTexCoord2f(.5,.5);
    glVertex3d(cap, 0.0, 0.0);
    for (th=0;th<360;th+=stepInterval)
    {
        glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
        glVertex3d(cap,radius*Cos(th),radius*Sin(th));
    }
    // must overlap 0 with 360, out of for loop ensures lap without stepInterval as even mod 360
    th = 360;
    glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
    glVertex3d(cap, radius*Cos(th), radius*Sin(th));
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    errorCheck("drawTexturedCylinder");
}


/**
 * drawCylinder
 *
 * low stepInterval value means more triangles and smoother surface use wisely
 * ideally a value evenly divisble by 360
 *
 * TODO: note that a stepInterval = 90 produces a cube, reauthor cube to leverage this?
 *
 * Credit: CSCI 5229 examples: ex8 SolidPlane Fuselage, Tailpipe
 */
void drawCylinder(struct Spec spec, double radius, double length, double stepInterval)
{
    int th = 0;
    double base = spec.x - length / 2;
    double cap = spec.x + length / 2;
    glPushMatrix();
    applySpec(spec);

    // base e.g. tailpipe
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(-1,0,0);
    glVertex3d(base, 0.0, 0.0);
    for (th=360;th>0;th-=stepInterval)
    {
        glNormal3f(-1,0,0);
        glVertex3d(base, radius*Cos(th), radius*Sin(th));
        th -= stepInterval;
        glNormal3f(-1,0,0);
        glVertex3d(base, radius*Cos(th), radius*Sin(th));
    }
    // must overlap 0 with 360, out of for loop ensures lap without stepInterval as even mod 360
    th = 0;
    glNormal3f(-1,0,0);
    glVertex3d(base, radius*Cos(th), radius*Sin(th));
    glEnd();

    // core e.g. fuselage
    glBegin(GL_QUAD_STRIP);
    for (th=360;th>0;th-=stepInterval)
    {
        glNormal3d((base+cap)/2, radius*Cos(th), radius*Sin(th));
        glVertex3d(base,radius*Cos(th),radius*Sin(th));
        glNormal3d((base+cap)/2, radius*Cos(th), radius*Sin(th));
        glVertex3d(cap,radius*Cos(th),radius*Sin(th));
        th -= stepInterval;
        glNormal3d((base+cap)/2, radius*Cos(th), radius*Sin(th));
        glVertex3d(base,radius*Cos(th),radius*Sin(th));
        glNormal3d((base+cap)/2, radius*Cos(th), radius*Sin(th));
        glVertex3d(cap,radius*Cos(th),radius*Sin(th));
    }
    // must overlap 0 with 360, out of for loop ensures lap without stepInterval as even mod 360
    th = 0;
    glNormal3d((base+cap)/2, radius*Cos(th), radius*Sin(th));
    glVertex3d(base,radius*Cos(th),radius*Sin(th));
    glNormal3d((base+cap)/2, radius*Cos(th), radius*Sin(th));
    glVertex3d(cap,radius*Cos(th),radius*Sin(th));
    glEnd();

    // cap e.g. tailpipe
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(+1,0,0);
    glVertex3d(cap, 0.0, 0.0);
    for (th=0;th<360;th+=stepInterval)
    {
        glNormal3f(+1,0,0);
        glVertex3d(cap,radius*Cos(th),radius*Sin(th));
    }
    // must overlap 0 with 360, out of for loop ensures lap without stepInterval as even mod 360
    th = 360;
    glNormal3f(+1,0,0);
    glVertex3d(cap, radius*Cos(th), radius*Sin(th));
    glEnd();

    glPopMatrix();
    errorCheck("drawCylinder");
}

/**
 * drawTexturedCone
 *
 * low stepInterval value means more triangles and smoother surface use wisely
 * ideally a value evenly divisble by 360
 *
 * TODO: note that a stepInterval = 90 produces a cube, reauthor cube to leverage this?
 *
 * Credit: CSCI 5229 examples: ex8 SolidPlane Nose
 */
void drawTexturedCone(struct Spec spec,
        double radius,
        double length,
        unsigned int texture,
        GLint mode,
        double stepInterval)
{
    double tip = spec.x + length / 2;
    double base = spec.x - length / 2;
    const double rep = 1;
    int th;

    glPushMatrix();
    applySpec(spec);

    // setup texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, mode);
    glBindTexture(GL_TEXTURE_2D,texture);
    
    // cone
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_TRIANGLES);
    // https://math.stackexchange.com/questions/2257382/finding-the-unit-normal-to-a-cone
    const double offset = 1;
    for (th=0;th<360;)
    {
        glNormal3d(offset, Cos(th), Sin(th));
        glTexCoord2f(0.5,.5);
        glVertex3d(tip, 0.0, 0.0);
        glNormal3d(offset, Cos(th), Sin(th));
        glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
        glVertex3d(base,radius*Cos(th),radius*Sin(th));
        th += stepInterval;
        glNormal3d(offset, Cos(th), Sin(th));
        glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
        glVertex3d(base,radius*Cos(th),radius*Sin(th));
    }
    // must overlap 0 with 360, out of for loop ensures lap without stepInterval as even mod 360
    glNormal3d(offset, Cos(th), Sin(th));
    glTexCoord2f(0.5,.5);
    glVertex3d(tip, 0.0, 0.0);
    glNormal3d(offset, Cos(th), Sin(th));
    glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
    glVertex3d(base,radius*Cos(th),radius*Sin(th));
    th = 360;
    glNormal3d(offset, Cos(th), Sin(th));
    glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
    glVertex3d(base,radius*Cos(th),radius*Sin(th));
    glEnd();

    // base
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(-1,0,0);
    glTexCoord2f(.5,.5);
    glVertex3d(base, 0.0, 0.0);
    for (th=360;th>0;th-=stepInterval)
    {
        glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
        glVertex3d(base,radius*Cos(th),radius*Sin(th));
    }
    // must overlap 0 with 360, out of for loop ensures lap without stepInterval as even mod 360
    th = 0;
    glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
    glVertex3d(base, radius*Cos(th), radius*Sin(th));
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    errorCheck("drawTexturedCone");
}


/**
 * drawCone
 *
 * low stepInterval value means more triangles and smoother surface use wisely
 * ideally a value evenly divisble by 360
 *
 * TODO: note that a stepInterval = 90 produces a cube, reauthor cube to leverage this?
 *
 * Credit: CSCI 5229 examples: ex8 SolidPlane Nose
 */
void drawCone(struct Spec spec, double radius, double length, double stepInterval)
{
    double tip = spec.x + length / 2;
    double base = spec.x - length / 2;
    int th;

    glPushMatrix();
    applySpec(spec);

    
    // cone
    glBegin(GL_TRIANGLES);
    // https://math.stackexchange.com/questions/2257382/finding-the-unit-normal-to-a-cone
    const double offset = 1;
    for (th=0;th<360;)
    {
        glNormal3d(offset, Cos(th), Sin(th));
        glVertex3d(tip, 0.0, 0.0);
        glNormal3d(offset, Cos(th), Sin(th));
        glVertex3d(base,radius*Cos(th),radius*Sin(th));
        th += stepInterval;
        glNormal3d(offset, Cos(th), Sin(th));
        glVertex3d(base,radius*Cos(th),radius*Sin(th));
    }
    // must overlap 0 with 360, out of for loop ensures lap without stepInterval as even mod 360
    glNormal3d(offset, Cos(th), Sin(th));
    glVertex3d(tip, 0.0, 0.0);
    glNormal3d(offset, Cos(th), Sin(th));
    glVertex3d(base,radius*Cos(th),radius*Sin(th));
    th = 360;
    glNormal3d(offset, Cos(th), Sin(th));
    glVertex3d(base,radius*Cos(th),radius*Sin(th));
    glEnd();

    // base
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(-1,0,0);
    glVertex3d(base, 0.0, 0.0);
    for (th=360;th>0;th-=stepInterval)
    {
        glNormal3f(-1,0,0);
        glVertex3d(base,radius*Cos(th),radius*Sin(th));
    }
    // must overlap 0 with 360, out of for loop ensures lap without stepInterval as even mod 360
    th = 0;
    glNormal3f(-1,0,0);
    glVertex3d(base, radius*Cos(th), radius*Sin(th));
    glEnd();

    glPopMatrix();

    errorCheck("drawCone");
}


/**
 * drawTexturedCube
 *
 * draw a unit cube changed by the specification provided
 *
 * credit: CSCI 5229 examples: ex8 cube, ex15 cube
 */
void drawTexturedCube(struct Spec spec, unsigned int texture, GLint mode)
{
    //  Save transformation
    glPushMatrix();
    //  Transform cube
    applySpec(spec);

    // setup texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, mode);
    glBindTexture(GL_TEXTURE_2D,texture);

    //  Cube
    //  Front
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0, 1);
    glTexCoord2f(0,0) ; glVertex3f(-1,-1, 1);
    glTexCoord2f(1,0) ; glVertex3f(+1,-1, 1);
    glTexCoord2f(1,1) ; glVertex3f(+1,+1, 1);
    glTexCoord2f(0,1) ; glVertex3f(-1,+1, 1);
    glEnd();
    //  Back
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0, -1);
    glTexCoord2f(0,0) ; glVertex3f(+1,-1,-1);
    glTexCoord2f(1,0) ; glVertex3f(-1,-1,-1);
    glTexCoord2f(1,1) ; glVertex3f(-1,+1,-1);
    glTexCoord2f(0,1) ; glVertex3f(+1,+1,-1);
    glEnd();
    //  Right
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_QUADS);
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0) ; glVertex3f(+1,-1,+1);
    glTexCoord2f(1,0) ; glVertex3f(+1,-1,-1);
    glTexCoord2f(1,1) ; glVertex3f(+1,+1,-1);
    glTexCoord2f(0,1) ; glVertex3f(+1,+1,+1);
    glEnd();
    //  Left
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0) ; glVertex3f(-1,-1,-1);
    glTexCoord2f(1,0) ; glVertex3f(-1,-1,+1);
    glTexCoord2f(1,1) ; glVertex3f(-1,+1,+1);
    glTexCoord2f(0,1) ; glVertex3f(-1,+1,-1);
    glEnd();
    //  Top
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_QUADS);
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0) ; glVertex3f(-1,+1,+1);
    glTexCoord2f(1,0) ; glVertex3f(+1,+1,+1);
    glTexCoord2f(1,1) ; glVertex3f(+1,+1,-1);
    glTexCoord2f(0,1) ; glVertex3f(-1,+1,-1);
    glEnd();

    //  Bottom
    //  Clockwise
    glBindTexture(GL_TEXTURE_2D,texture);
    glBegin(GL_QUADS);
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0,0) ; glVertex3f(-1,-1,-1);
    glTexCoord2f(1,0) ; glVertex3f(+1,-1,-1);
    glTexCoord2f(1,1) ; glVertex3f(+1,-1,+1);
    glTexCoord2f(0,1) ; glVertex3f(-1,-1,+1);
    glEnd();

    //  Undo transformations
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    errorCheck("drawTexturedCube");
}
/**
 * drawCube
 *
 * draw a unit cube changed by the specification provided
 *
 * credit: CSCI 5229 examples: ex8 cube
 */
void drawCube(struct Spec spec)
{
    //  Save transformation
    glPushMatrix();
    //  Transform cube
    applySpec(spec);

    //  Cube
    glBegin(GL_QUADS);
    //  Front
    glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1);
    glVertex3f(+1,-1, 1);
    glVertex3f(+1,+1, 1);
    glVertex3f(-1,+1, 1);
    //  Back
    glNormal3f( 0, 0, -1);
    glVertex3f(+1,-1,-1);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,+1,-1);
    glVertex3f(+1,+1,-1);
    //  Right
    glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1);
    glVertex3f(+1,-1,-1);
    glVertex3f(+1,+1,-1);
    glVertex3f(+1,+1,+1);
    //  Left
    glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,-1,+1);
    glVertex3f(-1,+1,+1);
    glVertex3f(-1,+1,-1);
    //  Top
    glNormal3f( 0,+1, 0);
    glVertex3f(-1,+1,+1);
    glVertex3f(+1,+1,+1);
    glVertex3f(+1,+1,-1);
    glVertex3f(-1,+1,-1);
    //  Bottom
    //  Clockwise
    glNormal3f( 0,-1, 0);
    glVertex3f(-1,-1,-1);
    glVertex3f(+1,-1,-1);
    glVertex3f(+1,-1,+1);
    glVertex3f(-1,-1,+1);

    //  End
    glEnd();

    //  Undo transformations
    glPopMatrix();

    errorCheck("drawCube");
}
