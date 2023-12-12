/**
 * project.c
 *
 * See README.md for instructions
 *
 * @author: Spencer David Wilson
 * @course: CSCI 5229 Introduction to Computer Graphics
 * @prof: Willem A. (Vlakkies) Schreüder
 * @date: Nov 29, 2023
 */


#include "SDW.h"



// GLOBALS
// PERSPECTIVE
const int __CUR_PERSPECTIVE_MODE_INIT=2;
int _CUR_PERSPECTIVE_MODE;
int _PREHELP_CUR_PERSPECTIVE_MODE;
const int __ORTHOGONAL_MODE=0;
const int __PROJECTION_MODE=1;
const int __FIRST_PERSON_MODE=2;
const int __PROJECTION_MODE_ROUNDER=3;
// LIGHTING
int _PREHELP_AMBIENT=0;
int _AMBIENT=1;
const int __LOCAL=100;
const int __DIFFUSE=100;
const int __SPECULAR=10;
const int __EMISSION=10;
int _LIGHT_AZIMUTH=0;  // LIGHT AZIMUTH
int _LIGHT_ELEVATION=90;  // LIGHT AZIMUTH
bool _PREHELP_NIGHT_STATE;
bool _NIGHT=false;
bool _PREHELP_FLASHLIGHT_STATE;
bool _FLASHLIGHT=true;
// OBJECTS
const int __OBJECT_INDEX_INIT=9;
// NEED TO USE DEFINE DUE TO DIFFERENT C VERSIONS!!!
// Credit: Willem A. (Vlakkies) Schreüder
// const int __OBJECT_INDEX_MAX=x;
# define __OBJECT_INDEX_MAX 9
int _OBJECT_INDEX;
const bool __OBJECT_MODE_INIT=true;
bool _OBJECT_MODE;
typedef void (*func_type)(void);
func_type _OBJECTS[__OBJECT_INDEX_MAX + 1];
func_type _SEASCAPE_LOCATOR[9];
// JELLYFISH SPECIAL
// TODO: less hacky
bool _JELLY_LESS_COOL=false;
// TRANSPARENT OBJECTS
int _ALPHAS_INDEX = 0;
#define _ALPHAS_LEN 1000
struct Alpha _ALPHAS[_ALPHAS_LEN];
// VIEW
const int __DISTANCE=2;
// filed of view
const int __FOV_FIRST_PERSON=100;
const int __FOV_OTHER=55;
int _FOV;
// aspect ratio
const double __ASP_INIT=1.0;
double _ASP;
// th, lattitude, azimuth
const int __AZIMUTH_INIT=160;
int _AZIMUTH;
int _PREHELP_AZIMUTH_STATE;
// ph, longitude, elevation
const int __ELEVATION_INIT=20;
int _ELEVATION;
int _PREHELP_ELEVATION_STATE;
//  size of world
const double __DIM_INIT=2.5;
double _DIM;
// eye position
const double __EX_INIT=-0.182050;
double _EX;
const double __EY_INIT=0.919020;
double _EY;
const double __EZ_INIT=-0.913911;
double _EZ;
// TEXTURES
unsigned int _TEX_FINS;
unsigned int _TEX_TURTLE_SHELL;
unsigned int _TEX_TURTLE_SCALES;
unsigned int _TEX_FINS_TILT;
unsigned int _TEX_BF;
unsigned int _TEX_SHRIMP;
unsigned int _TEX_REEF;
unsigned int _TEX_RED_FAN;
unsigned int _TEX_SCALES;
unsigned int _TEX_EYE;
unsigned int _TEX_BUMPS;
unsigned int _TEX_BRAIN_CORAL;
unsigned int _TEX_SAND;
unsigned int _TEX_CLIFF;
unsigned int _TEX_BLUE_TANG;
unsigned int _TEX_WATER;
// SHADERS
int _MY_SHADER;
// OTHER
const bool __PLAY_INIT=true;
bool _PLAY;
bool _PREHELP_PLAY_STATE;
const bool __COORD_LINES_INIT=false;
bool _COORD_LINES;
const bool __HELP_INIT=false;
bool _HELP;
const double __SIN_STEP_INIT=0;
double _SIN_STEP;
double _PREVIOUS_TIME=0;
const double __TIME_STEP=100;
double _RANDOM_DOUBLES[1000];


/**
 * updateFov
 *
 * use both for init and on mode changes
 * consistently set the _FOV
 * based on the _CUR_PERSPECTIVE_MODE
 */
void updateFov()
{
    _FOV = (_CUR_PERSPECTIVE_MODE == __FIRST_PERSON_MODE) ? __FOV_FIRST_PERSON : __FOV_OTHER;
}


/**
 * randomDouble
 *
 * construct a random number in range [-1,+1]
 * 
 * Credit: https://stackoverflow.com/questions/33058848/generate-a-random-double-between-1-and-1
 */
double randomDouble()
{
    double random_value;
    random_value = (double)rand()/RAND_MAX*2.0-1.0;//float in range -1 to 1
    return random_value;
}


/**
 * updateRandomDoubles
 *
 * change the known random doubles in _RANDOM_DOUBLES for better paced change of randomnes
 * between frame changes
 */
void updateRandomDoubles()
{
    for (int i=0 ; i < 1000 ; i++)
    {
        _RANDOM_DOUBLES[i] = randomDouble();
    }
}



/**
 * initGlobals
 *
 * Intitialize all _<X> globals to __<X>_INIT
 * can be done in space, but is done on
 * user input `0`, so having function makes it
 * for only one update needed, not multiples.
 *
 * BE SURE TO CALL IN MAIN
 * Can call in otherplaces to refresh to original state
*/
void initGlobals()
{
    _HELP = __HELP_INIT;
    _COORD_LINES = __COORD_LINES_INIT;
    _PLAY = __PLAY_INIT;
    _FOV = __FOV_OTHER;
    _ASP = __ASP_INIT;
    _DIM = __DIM_INIT;
    _CUR_PERSPECTIVE_MODE = __CUR_PERSPECTIVE_MODE_INIT;
    _SIN_STEP = __SIN_STEP_INIT;
    _OBJECT_MODE = __OBJECT_MODE_INIT;
    _OBJECT_INDEX = __OBJECT_INDEX_INIT;

    updateFov();
    updateRandomDoubles();

    // on '0' is overridden with call to EXPLORER seascape locator
    _AZIMUTH = __AZIMUTH_INIT;
    _ELEVATION = __ELEVATION_INIT;
    _EX = __EX_INIT;
    _EY = __EY_INIT;
    _EZ = __EZ_INIT;
}


/**
 * updateAlphas
 *
 * this should be the only way we interact with _ALPHAS array
 * append the new $alpha to _ALPHAS and increment the index
 * 
 * DON'T forget to call handleAllAlphas later if this ever gets called
 */
void updateAlphas(struct Alpha alpha) {
    _ALPHAS[_ALPHAS_INDEX] = alpha;
    _ALPHAS_INDEX++;
    if (_ALPHAS_INDEX > _ALPHAS_LEN)
    {
        fatal("overran static size of _ALPHA");
    }
}


/**
 * initTextures
 *
 * initialize all of the textures
 * Do this only once as it takes time to snarf the files
 * and doing it multiple times during execution wouldn't serve any value
 */
void initTextures()
{
    _TEX_BF = loadTexBmp("butterflyFish.bmp");
    _TEX_TURTLE_SHELL = loadTexBmp("turtleShell.bmp");
    _TEX_TURTLE_SCALES = loadTexBmp("turtleScales.bmp");
    _TEX_SHRIMP = loadTexBmp("shrimp.bmp");
    _TEX_REEF = loadTexBmp("reef.bmp");
    _TEX_RED_FAN = loadTexBmp("redFan.bmp");
    _TEX_SCALES = loadTexBmp("scalesSmall.bmp");
    _TEX_BRAIN_CORAL = loadTexBmp("brainCoralSmall.bmp");
    _TEX_BUMPS = loadTexBmp("sharkSkinSmall.bmp");
    _TEX_EYE = loadTexBmp("eyeSmall.bmp");
    _TEX_FINS = loadTexBmp("finsSmall.bmp");
    _TEX_FINS_TILT = loadTexBmp("finsSmallTilt.bmp");
    _TEX_BLUE_TANG = loadTexBmp("blueTang.bmp");
    _TEX_SAND = loadTexBmp("sand.bmp");
    _TEX_CLIFF = loadTexBmp("cliff.bmp");
    _TEX_WATER = loadTexBmp("water.bmp");
}


// DRAWINGS
/**
 * dispText
 *
 * Convenience routine to output raster text
 * credit: https://piazza.com/class/lls99rbz11v578/post/10
 */
#define LEN 8192  // Maximum length of text string
    __attribute__((format(printf, 1, 2)))
void dispText(const char* format , ...)
{
    char    buf[LEN];
    char*   ch=buf;
    va_list args;
    va_start(args, format);
    vsnprintf(buf,LEN,format,args);
    va_end(args);
    // Display the characters one at a time at the current raster position
    while (*ch) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
    errorCheck("dispText");
}


/**
 * drawCoordSystem
 *
 * draw xyz coordinate lines and label them appropriately 
 *
 * if _ELEVATION and _AZIMUTH are both 0:
 * - x is right
 * - y is up
 * - z is out toward the viewer
 */ 
void drawCoordSystem()
{
    glColor4f(1,1,1,1);

    // draw axis
    glBegin(GL_LINES);
    // X
    glVertex3d(0, 0, 0);
    glVertex3d(1, 0, 0);
    // Y
    glVertex3d(0, 0, 0);
    glVertex3d(0, 1, 0);
    // Z
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, 1);
    glEnd();

    // label axis
    glRasterPos3d(1, 0, 0);
    dispText("X");
    glRasterPos3d(0, 1, 0);
    dispText("Y");
    glRasterPos3d(0, 0, 1);
    dispText("Z");

    errorCheck("drawCoordSystem");
}


/**
 * updateRasterYStartSpecific
 *
 * move the raster position down a specific amount
 * return the new y value
 */
double updateRasterYStartSpecific(double y, double specific)
{
    // x always left aligned
    const double x = -0.75;
    // z doesn't really matter, but here anyway
    const double z = 0;

    double newY = y + specific;
    glRasterPos3d(x, newY, z);
    return newY;
}


/**
 * updateRasterYStart
 *
 * move the raster position down a standard amount
 * return the new y value
 */
double updateRasterYStart(double y)
{
    // Seems to work well for my machine
    // Hopefully works well for other displays
    const double standardYChange = -0.15;
    return updateRasterYStartSpecific(y, standardYChange);
}


/**
 * displayHelp
 *
 * display the help menu for the user (when they press h/H)
 */
void displayHelp()
{
    // revert to fixed pipeline
    glUseProgram(0);

    glColor4f(1,1,1,1);

    // start at 1.0, updateRasterYStart will
    // move raster to 0.9 for us so it pads for display correctly
    double y = 1.0;
    y = updateRasterYStart(y);
    dispText("Now in help mode:");

    // add extra space between sections of the help menu
    y = updateRasterYStartSpecific(y, -0.25);
    dispText("       h: to enter and exit help mode");
    y = updateRasterYStart(y);
    dispText("       0: return to original setup");
    // add extra space between sections of the help menu
    y = updateRasterYStartSpecific(y, -0.25);
    dispText("       m: change view mode [orthogonal, perpective, first person]");
    y = updateRasterYStart(y);
    dispText("     o/O: change the object of interest (RECOMMEND: NOT first person mode 'm')");
    y = updateRasterYStart(y);
    dispText("   1 - 9: go to set points in the scene (RECOMMEND: first person mode 'm')");
    y = updateRasterYStart(y);
    dispText("       n: toggle Night and Day mode");
    y = updateRasterYStart(y);
    dispText("       f: toggle flashlight");
    y = updateRasterYStart(y);
    dispText("       l: toggle coordinate lines");
    y = updateRasterYStart(y);
    dispText("       p: pause/play auto changes");

    // add extra space between sections of the help menu
    y = updateRasterYStartSpecific(y, -0.25);
    dispText("  <asdw>: change player/diver position");
    y = updateRasterYStart(y);
    dispText("<arrows>: move the angles");

    // add extra space between sections of the help menu
    y = updateRasterYStartSpecific(y, -0.25);
    dispText("   ESC: end the program");

    errorCheck("displayHelp");
}


/**
 * handleEyeAlpha
 *
 * handler for the eye transparant orb
 * draws a transparent orb at provided spec location
 * typically used to updateAlphas
 */
void handleEyeAlpha(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glColor4f(.8,.8,.8,.5);
    drawSphere(newSpecDefaultColor(0,0,0 , 0,0,0 , 1.1,1.1,1.1), 45);
    glDisable(GL_BLEND);
    glPopMatrix();

    errorCheck("handleEyeAlpha");
}


/**
 * drawEye
 *
 * draw a low vertex sphere with a human eyeball texture
 * updateAlphas with a transparent higher vertex sphere
 *              that surounds the original eye
 */
void drawEye(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);
    drawTexturedSphere(baseSpec(), _TEX_EYE, GL_MODULATE, 90);
    glPopMatrix();

    updateAlphas(newAlpha(&handleEyeAlpha, spec));

    errorCheck("drawEye");
}


/**
 * drawStaghorn
 *
 * draw a staghorn coral shape
 */
void drawStaghorn(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);
    
    glColor3f(.8,.6,.3);

    double cylH = 0.4;
    double radius = 0.1;

    drawTexturedCylinder(newSpec(0,cylH/2,0, 90,90,0, 1,1,1 , baseColor(),baseColor(),baseColor()), radius, cylH, _TEX_BUMPS, GL_MODULATE, 45);
    double coneH = cylH * 1.1;
    drawTexturedCone(newSpec(0,cylH+coneH/2,0, 90,90,0, 1,1,1 , baseColor(),baseColor(),baseColor()), radius, coneH,  _TEX_BUMPS, GL_MODULATE,45);

    glColor3f(.8,.6,.4);
    drawTexturedCone(newSpec(-.45,cylH+coneH/2+.1,+.36, 120,120,0, 1,1,1 , baseColor(),baseColor(),baseColor()), radius, coneH*2,  _TEX_BUMPS, GL_MODULATE,45);
    glColor3f(.8,.6,.5);
    drawTexturedCone(newSpec(-.2,.6,+.15, 90,90,0, .4,.4,.4 , baseColor(),baseColor(),baseColor()), radius, coneH*2,  _TEX_BUMPS, GL_MODULATE,45);
    glColor3f(.8,.6,.6);
    drawTexturedCone(newSpec(-.3,.6,+.075, -130,-130,0, .2,.2,.2 , baseColor(),baseColor(),baseColor()), radius, coneH*2,  _TEX_BUMPS, GL_MODULATE,45);

    glPopMatrix();

    errorCheck("drawStaghorn");
}


/**
 * drawBrainCoral
 *
 * draw a brain coral shape
 */
void drawBrainCoral(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);

    glColor4f(1,1,1,1);
    drawTexturedSphere(newSpec(0,0,0 , 0,0,0 , 1,1,1 , baseColor(),baseColor(), baseColor()),
            _TEX_BRAIN_CORAL, GL_MODULATE, 30);

    glPopMatrix();
    errorCheck("drawBrainCoral");
}


/**
 * drawReef
 *
 * draw a collection of Staghorn on BrainCoral bases
 *
 * TODO: update to draw other kinds of coral as well, (or as a drawComplexReef)
 */
void drawReef(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);

    drawBrainCoral(newSpec(0,-.1,0 , 0,0,0 , 1,.2,1 , baseColor(),baseColor(),baseColor()));
    drawBrainCoral(newSpec(.4,.1,.13 , 0,0,0 , .3,.4,.5 , baseColor(),baseColor(),baseColor()));
    drawStaghorn(newSpec(.3,0,.7 , 0,0,0 , 1,1,1 , baseColor(),baseColor(),baseColor()));
    drawStaghorn(newSpec(-.7,0,-.3 , 0,0,0 , .5,.5,.5 , baseColor(),baseColor(),baseColor()));

    glPopMatrix();

}


/**
 * drawHammerhead
 *
 * draws a cute little shark creature
 */
void drawHammerhead(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);

    // eyes
    glColor3f(.7,.7,.7);
    drawEye(newSpec(-.55,.025,.85, 0,-25,0, .1,.05,.05 , baseColor(),newColor(0,1,1,1),newColor(0,1,1,1)));
    drawEye(newSpec(+.55,.025,.85, 0,+25,0, .1,.05,.05 , baseColor(),newColor(0,1,1,1),newColor(0,1,1,1)));

    // body
    glColor3f(.4,.4,.4);
    drawTexturedSphere(newSpec(0,0,0, 0,0,0, .4,.2,.9 , baseColor(),baseColor(),baseColor()),_TEX_BUMPS, GL_MODULATE, 45);
    // head
    drawTexturedSphere(newSpec(0,+.025,.75, 0,0,0, .7,.1,.2 , baseColor(),baseColor(),baseColor()), _TEX_BUMPS, GL_MODULATE, 45);
    // side fins forward
    glColor3f(.8,.8,.8);
    drawTexturedSphere(newSpec(+.2,-.2,.4, 90,45,0, .025,.1,.25 , baseColor(),baseColor(),baseColor()), _TEX_BUMPS, GL_MODULATE, 45);
    drawTexturedSphere(newSpec(-.2,-.2,.4, 90,-45,0, .025,.1,.25 , baseColor(),baseColor(),baseColor()), _TEX_BUMPS, GL_MODULATE, 45);
    // dorsal forward
    drawTexturedSphere(newSpec(0,+.2,.25, -45,0,0, .05,.2,.1 , baseColor(),baseColor(),baseColor()), _TEX_BUMPS, GL_MODULATE, 45);
    // side fins back
    drawTexturedSphere(newSpec(+.2,-.2,-.3, 90,45,0, .025,.05,.1 , baseColor(),baseColor(),baseColor()), _TEX_BUMPS, GL_MODULATE, 45);
    drawTexturedSphere(newSpec(-.2,-.2,-.3, 90,-45,0, .025,.05,.1 , baseColor(),baseColor(),baseColor()), _TEX_BUMPS, GL_MODULATE, 45);
    // dorsal back
    drawTexturedSphere(newSpec(0,+.2,-.4, -45,0,0, .025,.1,.05 , baseColor(),baseColor(),baseColor()), _TEX_BUMPS, GL_MODULATE, 45);
    // dorsal back
    drawTexturedSphere(newSpec(0,-.2,-.5, -45,0,0, .025,.05,.1 , baseColor(),baseColor(),baseColor()), _TEX_BUMPS, GL_MODULATE, 45);
    // tail top
    drawTexturedSphere(newSpec(0,+.2,-1, -45,0,0, .05,.4,.1 , baseColor(),baseColor(),baseColor()), _TEX_BUMPS, GL_MODULATE, 45);
    // tail bottom
    drawTexturedSphere(newSpec(0,-.1,-.8, 45,0,0, .05,.2,.1 , baseColor(),baseColor(),baseColor()), _TEX_BUMPS, GL_MODULATE, 45);
    
    glPopMatrix();
    errorCheck("drawHammerhead");
}


/**
 * genSlopePartial
 *
 * specialize method to draw slopes in the yz listed
 *
 * if sand enabled, use sand texture
 * else, use cliff texture
 *
 * TODO: Consider if we still want this now that we aren't in development and the points
 *       are fully known
 */
void genSlopePartial(double y0, double y1, double z0, double z1, bool sand)
{
    const double x1 = -.9;
    const double x2 = 1;
    glBindTexture(GL_TEXTURE_2D,sand?_TEX_SAND:_TEX_CLIFF);
    glBegin(GL_QUADS);
    setColor(newColor(1,1,1,1));
    if ( y1 < y0 )
        glNormal3f( 0, z1-z0, y0 - y1);
    else
        glNormal3f( 0, z1-z0, y1 - y0);
    glTexCoord2f(0,0) ; glVertex3f(x2,y0,z0);
    glTexCoord2f(1,0) ; glVertex3f(x1,y0,z0);
    glTexCoord2f(1,1) ; glVertex3f(x1,y1,z1);
    glTexCoord2f(0,1) ; glVertex3f(x2,y1,z1);
    glEnd();
}


/**
 * genSlopeSet
 *
 * specialize method to draw slopes in the yz listed
 *
 * @param points: array of y, then z points y0, z0, y1, z1, ...
 * @param size: len of points to make things easier
 *
 * TODO: Consider if we still want this now that we aren't in development and the points
 *       are fully known
 */
void genSlopeSet(double points[], int size)
{
    double y0 = points[0];
    double z0 = points[1];
    double y1 = y0;
    double z1 = z0;
    bool sand = true;
    for (int i = 2; i < size ; i++)
    {
        y0 = y1;
        z0 = z1;
        y1 = points[i];
        i++;
        z1 = points[i];
        genSlopePartial(y0, y1, z0, z1, sand);
        sand = !sand;
    }
}


/**
 * drawBlueTang
 *
 * draw a blue tang fish the lazy way
 * map a texture to a low vertex sphere
 */
void drawBlueTang(double x, double y, double z, double size)
{
    drawTexturedSphere(newSpecDefaultColor(x,y,z , 0,0,0 , size,size/2,size/10),
            _TEX_BLUE_TANG, GL_MODULATE, 45);
    errorCheck("drawBlueTang");
}


/**
 * drawBlueTangSpec
 *
 * draw a blue tang fish the lazy way
 * map a texture to a sphere
 * apply the provided spec
 */
void drawBlueTangSpec(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);

    drawBlueTang(0,0,0,1);

    glPopMatrix();
    errorCheck("drawBlueTangSpec");
}


/**
 * drawButterflyFish
 *
 * draw a butterfly fish the lazy way
 * map a texture to a low vertex sphere
 * NOTE: tooks some doing to get the texture made just the right way for this
 */
void drawButterflyFish(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);

    glColor4f(1,1,1,1);
    drawTexturedSphere(newSpecDefaultColor(0,0,0 , 0,0,0 , 1,.9,.1), _TEX_BF, GL_MODULATE, 45);

    glPopMatrix();
    errorCheck("drawButterflyFish");
}


/**
 * drawButterflyAnimation
 *
 * draw a butterfly fish that toggles between
 * some coral tips for us (like it would in the wild)
 */
void drawButterflyAnimation(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);
    
    // targets
    drawStaghorn(newSpec(1.1,-.5,.05 , 0,0,0 , .5,.5,.5 , baseColor(),baseColor(),baseColor()));
    drawStaghorn(newSpec(-.9,-.8,0 , 0,45,0 , .8,.8,.8 , baseColor(),baseColor(),baseColor()));

    // fish
    double x = Sin(_SIN_STEP*4);
    double y = Cos(_SIN_STEP*4);
    y = (y<0)?-y:y;
    drawButterflyFish(newSpecDefaultColor(x,y,0 , 0,0,300 , .2,.2,.2));

    // base
    drawBrainCoral(newSpecDefaultColor(0,-.8,0 , 0,0,0 , 1.2,.1,.3));
    drawBrainCoral(newSpecDefaultColor(1,-.8,0 , 0,0,0 , .5,.4,.35));

    // decor
    drawBrainCoral(newSpecDefaultColor(.4,-.8,.3 , 0,0,90 , .4,1,.5));
    drawStaghorn(newSpec(.3,-.5,.4 , 10,75,0 , 1.2,1.2,1.2 , baseColor(),baseColor(),baseColor()));

    glPopMatrix();
    errorCheck("drawButterflyAnimation");
}


/**
 * drawMouthShrimp
 *
 * specialized for drawGrouper
 * draw a shrimp that goes around the left gill
 * in the mouth
 * out the gill
 * forever
 */
void drawMouthShrimp(struct Spec spec)
{
    double x, y, z;

    glPushMatrix();
    applySpec(spec);

    drawTexturedSphere(baseSpec(), _TEX_SCALES, GL_MODULATE, 45);
    
    // here I get to have the shrimp sphere go perfectly around the gill prior to transorming
    // the gill shape
    // easier than customizing the xyz values...
    // xyz are changed from what you would expect
    // as the gill is in a special orientation for better fish scale texture rendering
    // NOTE: it now stretches the shrimp, but that is minor
    glColor4f(1,1,1,1);
    x = 0;
    y = Sin(_SIN_STEP*6);
    z = Cos(_SIN_STEP*6);
    drawTexturedSphere(newSpecDefaultColor(x,y,z , 90,0,0 , .4,.4,.4), _TEX_SHRIMP, GL_MODULATE, 45);

    glPopMatrix();
    errorCheck("drawMouthShrimp");
}


/**
 * drawGouper
 *
 * draw a grouper object.
 * designed so that the mouth and gils let you look inside of the grouper.
 *
 * param @hasMouthShrimp: determines if there is a shrimp going in the groupers mouth or not
 *
 * credit: https://www.fisheries.noaa.gov/s3/styles/original/s3/2021-01/640x427-goliath-grouper-illustration.jpg?itok=yCVXJBne
 */
void drawGrouper(struct Spec spec, bool hasMouthShrimp)
{
    glPushMatrix();
    applySpec(spec);

    // body
    glColor4f(.8,.6,.4,1);
    drawTexturedUnitCylinder(newSpecDefaultColor(0,0,0 , 0,0,0 , 1,.5,.5), _TEX_SCALES, GL_MODULATE, 45, false, false);
    drawTexturedCone(newSpecDefaultColor(.5,0,0 , 0,0,0 , 1,1,1), .5, 1, _TEX_SCALES, GL_MODULATE, 45);

    // ridge
    glColor4f(.4,.3,.25,1);
    int pointsCount = 10;
    struct Coord points[10] = { 
        // start
        newCoord(-.1, .5, 0),
        newCoord(-.1,.55,0),
        // high point
        newCoord(.1, .5, 0),
        newCoord(.1,.7,0),
        // pivot
        newCoord(.5, .5, 0),
        newCoord(.5,.55,0),
        // end
        newCoord(.6, .4, 0),
        newCoord(.7,.4,0),
        // repeate end due to limit of drawTexturedRibbon
        newCoord(.6, .4, 0),
        newCoord(.7,.4,0)
    };
    drawTexturedRibbon(baseSpec(), points, pointsCount, _TEX_FINS_TILT, GL_MODULATE);

    // end ridge
    glColor4f(.7,.55,.38,1);
    drawTexturedSphere(newSpecDefaultColor(.7,.5,0 , 0,0,300 , .2,.4,.01), _TEX_FINS, GL_MODULATE, 45);

    // tail
    glColor4f(.7,.55,.38,1);
    drawTexturedSphere(newSpecDefaultColor(1.5,.3,0 , 0,0,300 , .4,.5,.01), _TEX_FINS, GL_MODULATE, 45);
    glColor4f(.7,.55,.35,1);
    drawTexturedSphere(newSpecDefaultColor(1.5,-.3,0 , 180,0,90 , .4,.5,.01), _TEX_FINS, GL_MODULATE, 45);
    
    // under fin
    glColor4f(.4,.3,.25,1);
    drawTexturedSphere(newSpecDefaultColor(.7,-.5,0 , 180,0,90 , .2,.4,.01), _TEX_FINS, GL_MODULATE, 45);

    // side fins
    drawTexturedSphere(newSpecDefaultColor(-.1,-.1,.7 , 0,115,0 , .5,.3,.01), _TEX_FINS, GL_MODULATE, 45);
    drawTexturedSphere(newSpecDefaultColor(-.1,-.1,-.7 , 0,65,0 , .5,.3,.01), _TEX_FINS, GL_MODULATE, 45);

    // bottom fins
    drawTexturedSphere(newSpecDefaultColor(-.3,-.5,.2 , 75,115,10 , .3,.1,.01), _TEX_FINS, GL_MODULATE, 45);
    drawTexturedSphere(newSpecDefaultColor(-.3,-.5,-.2 , 105,115,10 , .3,.1,.01), _TEX_FINS, GL_MODULATE, 45);

    // face
    // so you can't see inner body, place a black sphere here
    glColor4f(0,0,0,1);
    drawTexturedSphere(newSpecDefaultColor(-.5,0,0 , 0,0,0 , .4,.45,.4), _TEX_SCALES, GL_MODULATE, 90);
    // top
    glColor4f(.8,.6,.4,1);
    drawTexturedSphere(newSpecDefaultColor(-.8,.1,0 , 0,0,45 , .6,.2,.5), _TEX_SCALES, GL_MODULATE, 45);
    // side
    drawTexturedSphere(newSpecDefaultColor(-.6,0,.35 , 90,0,0 , .4,.1,.35), _TEX_SCALES, GL_MODULATE, 45);
    drawTexturedSphere(newSpecDefaultColor(-.6,0,-.35 , 90,0,0 , .4,.1,.35), _TEX_SCALES, GL_MODULATE, 45);
    // bottom
    drawTexturedSphere(newSpecDefaultColor(-.7,-.4,0 , 0,0,185 , .4,.05,.4), _TEX_SCALES, GL_MODULATE, 45);
    // gills
    // right
    drawTexturedSphere(newSpecDefaultColor(-.9,-.3,.35 , 50,0,90 , .2,.05,.3), _TEX_SCALES, GL_MODULATE, 45);
    // left
    if (hasMouthShrimp)
    {
        drawMouthShrimp(newSpecDefaultColor(-.9,-.3,-.35 , 50,0,270 , .2,.05,.3));
    }
    else
    {
        drawTexturedSphere(newSpecDefaultColor(-.9,-.3,-.35 , 50,0,270 , .2,.05,.3), _TEX_SCALES, GL_MODULATE, 45);
    }
    // eye help cover side
    glColor4f(1,1,1,1);
    drawEye(newSpecDefaultColor(-.9,.3,.4 , 0,90,0 , .1,.1,.1));
    drawEye(newSpecDefaultColor(-.9,.3,-.4 , 0,90,0 , .1,.1,.1));

    glPopMatrix();
    errorCheck("drawGrouper");
}


/**
 * drawGrouperStation
 *
 * A grouper with a few shrimp on it
 * Most notable is the shrimp which goes in the
 * groupers mouth and out its gill. This happens in the wild
 */
void drawGrouperStation(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);

    // mouth shrimp done in drawGrouper
    drawGrouper(newSpecDefaultColor(0,0,0 , 0,180,0 , 1,1,1), true);

    // shrimps
    glColor4f(1,1,1,1);
    double x, y, z;

    // underbelly
    // just going in a line
    x = (1+-Sin(_SIN_STEP*3))/2*1.6-.7;
    y = -.5;
    z = 0;
    drawTexturedSphere(newSpecDefaultColor(x,y,z , 90,10,0 , .1,.1,.1), _TEX_SHRIMP, GL_MODULATE, 45);
    
    // backfin
    // going in a differnt shape of line
    x = Sin(_SIN_STEP*10)/4 - 1.5;
    y = (1+Sin(_SIN_STEP*10))*.2;
    z = 0;
    drawTexturedSphere(newSpecDefaultColor(x,y,z , 0,0,0 , .1,.1,.1), _TEX_SHRIMP, GL_MODULATE, 45);

    glPopMatrix();
    errorCheck("drawGrouperStation");
}


/**
 * handleRedFan
 *
 * draw a transparent fan object (likely use as a promise to updateAlphas)
 * this is just a simple sea fan texture on a low vertex flattened sphere
 */
void handleRedFan(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glColor4f(1,1,1,1);
    drawTexturedSphere(newSpecDefaultColor(0,.5,0 , 0,0,25 , .5,.6,.1), _TEX_RED_FAN, GL_MODULATE, 45);
    glDisable(GL_BLEND);

    glPopMatrix();
    errorCheck("handleRedFan");
}


/**
 * drawRedFan
 *
 * draw a red sea fan, it won't get displayed until the alphas are handled later
 */
void drawRedFan(struct Spec spec)
{
    updateAlphas(newAlpha(&handleRedFan, spec));
    errorCheck("drawRedFan");
}


/**
 * drawBlockingReef
 *
 * draw a tall and skinyish cube that has a reef texture on it to dummy up some
 * semi-realistic reef system and help encourage a path of swimming for the
 * explorer. adds additional details to it like RedFans and the standard reefs
 */
void drawBlockingReef(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);

    drawTexturedCube(newSpecDefaultColor(0,.5,0 , 0,0,0 , .2,.5,1), _TEX_REEF, GL_MODULATE);

    drawRedFan(newSpecDefaultColor(0,0,1.2 , 0,45,0 , .5,.5,.5));
    drawRedFan(newSpecDefaultColor(.4,0,0 , 0,90,0 , .5,.5,.5));
    drawRedFan(newSpecDefaultColor(.4,+.3,.5 , 20,100,0 , .25,.4,.5));
    
    drawReef(newSpecDefaultColor(.4,0,-.5 , 0,90,0 , .4,.4,.4));

    glPopMatrix();
    errorCheck("drawBlockingReef");
}


/**
 * drawEel
 *
 * TODO: consider an eel cleaning station as well
 * TODO: consider the eel breathing mechanisms (open and shut mouth)
 */
void drawEel(struct Spec spec)
{
    // more pivots means more vertexes
    int pivots = 25;

    glPushMatrix();
    applySpec(spec);

    // TODO: make less hacky
    // array for the body shape
    struct Coord tubeArray[pivots];
    double radius = 0.5;
    // array for the underbody fin
    struct Coord ribbonArray[pivots*2];
    int j = 0;
    // use to make more of a flag wave in the body
    double width = 0;
    const double widthStep = 1.0/(double) pivots;
    double step = 1;
    const double stepInc = 1/((double)pivots)*2;
    for (int i = 0; i < pivots; i++)
    {
        // pre exec
        step -= stepInc;

        double th = i*360/pivots*2;

        // body
        tubeArray[i] = newCoord(step, -radius, Sin(th)*width);

        // fin
        ribbonArray[j] = tubeArray[i];
        j++;
        ribbonArray[j] = newCoord(step,-radius - .5,Sin(th)*1.4*width);
        j++;

        // post exec
        width += widthStep;
    } 

    // eyes
    glColor4f(1,1,1,1);
    drawEye(newSpecDefaultColor(1,.1,.2 , 0,90,30 , .1,.1,.1));
    drawEye(newSpecDefaultColor(.75,.1,-.2 , 0,120,30 , .1,.1,.1));
    // head
    glColor4f(.5,.7,.3,1);
    drawTexturedSphere(newSpecDefaultColor(.85,0,0 , 0,30,0 , .3,.2,.4), _TEX_FINS, GL_MODULATE, 90);
    // body
    glColor4f(.5,.6,.3,1);
    struct Spec bodySpec = newSpecDefaultColor(0,0,0 , 0,0,0 , .8,.25,.6);
    drawTexturedTube(bodySpec, tubeArray, pivots,  radius, _TEX_SCALES, GL_MODULATE, 90, false, true);
    glColor4f(.5,.8,.3,1);
    drawTexturedRibbon(bodySpec, ribbonArray, pivots * 2, _TEX_FINS, GL_MODULATE);

    glPopMatrix();
    errorCheck("drawEel");
}


/**
 * drawSkybox
 *
 * top is water
 * the sides and base are sea colored
 * NOTE: all clockwise to enable reverse face culling
 * TODO: the code looks clockwise to me, so what is going on here? I think I'm confused
 */
void drawSkybox(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);
    
    // TOP
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,_TEX_WATER);
    glBegin(GL_QUADS);
    glNormal3f(0,-1,0);
    glColor4f(1,1,1,1);
    glTexCoord2f(0,0) ; glVertex3f(-1, 1, 1);
    glTexCoord2f(0,1) ; glVertex3f(-1, 1, -1); 
    glTexCoord2f(1,1) ; glVertex3f(1, 1, -1);
    glTexCoord2f(1,0) ; glVertex3f(1, 1, 1);
    glEnd();  // GL_QUADS
    glDisable(GL_TEXTURE_2D);

    // sides
    float color[] = {1.0,0.5,0.5,1.0};
    glColor4f(1,.5,.5,1);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color);

    glBegin(GL_QUADS);
   
    // LEFT
    glNormal3f(1,0,0);
    glVertex3f(-1, -1, 1);
    glVertex3f(-1, -1, -1); 
    glVertex3f(-1, 1, -1);
    glVertex3f(-1, 1, 1);

    // BACK
    glNormal3f(0,0,1);
    glVertex3f(-1, 1, -1);
    glVertex3f(-1, -1, -1); 
    glVertex3f(1, -1, -1);
    glVertex3f(1, 1, -1);

    // RIGHT
    glNormal3f(-1,0,0);
    glVertex3f(1, -1, -1);
    glVertex3f(1, -1, 1); 
    glVertex3f(1, 1, 1);
    glVertex3f(1, 1, -1);
    
    // FRONT
    glNormal3f(0,0,-1);
    glVertex3f(-1, -1, 1);
    glVertex3f(-1, 1, 1); 
    glVertex3f(1, 1, 1);
    glVertex3f(1, -1, 1);
    
    // BOTTOM
    glNormal3f(0,1,0);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, -1, 1); 
    glVertex3f(1, -1, 1);
    glVertex3f(1, -1, -1);

    glEnd(); // GL_QUADS

    glPopMatrix();
    errorCheck("drawSkybox");
}


/**
 * handleJellyfishTop
 *
 * draws a transparent flattened sphere
 * with some colored spheres in side it
 * a helper to handleJellyfish
 */
void handleJellyfishTop(struct Spec spec)
{

    glPushMatrix();
    applySpec(spec);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glColor4f(.5,.8,1,.4); 
    if (!_JELLY_LESS_COOL)
    {
        // disable the depth test
        // so we can see the colored interior spheres
        glDisable(GL_DEPTH_TEST);
        drawSphere(newSpecDefaultColor(0,0,0 , 0,0,0 , 1,.2,1), 45);
        glColor4f(.8,.5,1,.1);
        drawSphere(newSpecDefaultColor(-.5,0,0 , 0,0,0 , .3,.1,.1), 45);
        drawSphere(newSpecDefaultColor(+.5,0,0 , 0,0,0 , .3,.1,.1), 45);
        drawSphere(newSpecDefaultColor(0,0,-.5 , 0,90,0 , .3,.1,.1), 45);
        drawSphere(newSpecDefaultColor(0,0,+.5 , 0,90,0 , .3,.1,.1), 45);
        glColor4f(.8,1,.5,.1);
        drawSphere(newSpecDefaultColor(-.35,0,-.35 , 0,135,0 , .3,.1,.1), 90);
        drawSphere(newSpecDefaultColor(-.35,0,+.35 , 0,45,0 , .3,.1,.1), 90);
        drawSphere(newSpecDefaultColor(+.35,0,+.35 , 0,135,0 , .3,.1,.1), 90);
        drawSphere(newSpecDefaultColor(+.35,0,-.35 , 0,45,0 , .3,.1,.1), 90);
        // re-enable the depth test!!!
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        drawSphere(newSpecDefaultColor(0,0,0 , 0,0,0 , 1,.2,1), 45);
    }
    glDisable(GL_BLEND);

    glPopMatrix();
    errorCheck("handleJellyfishTop");
}


/**
 * handleJellyfishSpiral
 *
 * construct a spiral structure comprised of transparent low vertex spheres
 * CAUTION: if _JELLY_LESS_COOL is false, this creates 50 spheres, else only 15 spheres
 * this shape of interest was found by accident in trying to create a randomized school of fish
 * a helper to handleJellyfish
 */
void handleJellyfishSpiral(struct Spec spec)
{

    glPushMatrix();
    applySpec(spec);

    double x, y, z, angle, size;
    x = 0;
    y = 0;
    z = 0;
    angle = 0;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    double points = 50;
    if (_JELLY_LESS_COOL)
    {
        points=15;
    }
    for (double i = 1; i < points ; i++)
    {
        y = i/points;

        z = Sin(y*360)/6;
        x = Cos(y*360)/6;
        angle = (y+1)*180;

        glColor4f(.5,.8,1,.1);
        size = .2*y;
        drawSphere(newSpecDefaultColor(x,y,z , 0,angle,0 ,  size,size,size), 90);
    }
    glDisable(GL_BLEND);

    glPopMatrix();
    errorCheck("handleJellyfishSpiral");
}


/**
 * handleJellyfish
 *
 * used to draw a jelly fish, typically through updateAlphas
 * can call directly, but it should be the last call of the scene drawings
 */
void handleJellyfish(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);
    handleJellyfishTop(newSpecDefaultColor(0,.9,.0 , 0,0,0 , 1,1,1));
    handleJellyfishSpiral(newSpecDefaultColor(0,-1.8,0 , 0,0,0 , 1,2.25,1));
    handleJellyfishSpiral(newSpecDefaultColor(.5,-.9,0 , 0,45,0 , 1,1.5,1));
    handleJellyfishSpiral(newSpecDefaultColor(0,-.9,.5 , 0,135,0 , 1,1.5,1));
    handleJellyfishSpiral(newSpecDefaultColor(0,-1.3,-.5 , 0,70,0 , 1,1.75,1));
    handleJellyfishSpiral(newSpecDefaultColor(-.5,-.9,0 , 0,150,0 , 1,1.5,1));
    glPopMatrix();
    errorCheck("handleJellyfish");
}


/**
 * drawJellyfish
 *
 * adds jellyfish drawing instructions to the ALPHAS list
 */
void drawJellyfish(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);
    updateAlphas(newAlpha(&handleJellyfish, baseSpec()));
    glPopMatrix();
    errorCheck("drawJellyfish");
}


/**
 * drawSquidTenticle
 *
 * draws a tubular structure for the squids tenticles
 */
void drawSquidTenticle(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);
    const int pivots = 4;
    struct Coord tubeArray[pivots] = {
        newCoord(0,0,0),
        newCoord(-.2,0,-.1),
        newCoord(-.5,0,-.2),
        newCoord(-1,0,-.1)
    };
    drawTexturedTube(newSpecDefaultColor(0,0,0 , 0,0,0 , 1,1,1), tubeArray, pivots, .1, _TEX_SCALES, GL_MODULATE, 45, false, true);
    glPopMatrix();
    errorCheck("drawSquidTenticle");
}


/**
 * handleSquidSides
 *
 * instructions to draw a transparent ribonlike object
 * used on the side of a squid body as fins
 */
void handleSquidSides(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    const int pointsCount = 18;
    struct Coord points[pointsCount] = {
        newCoord(-1.00,+0.00,0),
        newCoord(-1.00,+0.20,0),

        newCoord(-0.75,+0.05,0),
        newCoord(-0.75,+0.25,-.1),

        newCoord(-0.50,+0.10,0),
        newCoord(-0.50,+0.30,-.2),

        newCoord(-0.25,+0.15,0),
        newCoord(-0.25,+0.35,-.1),

        newCoord(+0.00,+0.20,0),
        newCoord(+0.00,+0.40,0),

        newCoord(+0.25,+0.15,0),
        newCoord(+0.25,+0.35,+.1),

        newCoord(+0.50,+0.10,0),
        newCoord(+0.50,+0.30,+.2),

        newCoord(+0.75,+0.05,0),
        newCoord(+0.75,+0.25,+.1),

        newCoord(+1.00,+0.10,0),
        newCoord(+1.00,+0.30,0)
    };
    double c1 = _RANDOM_DOUBLES[0];
    if (c1 < -.1) {
        glColor4f(.5,.5,.5,1);
    } else {
        double c2 = _RANDOM_DOUBLES[1];
        double c3 = _RANDOM_DOUBLES[2];
        c1 = (c1 < 0) ? -c1 : c1;
        c2 = (c2 < 0) ? -c2 : c2;
        c3 = (c3 < 0) ? -c3 : c3;
        glColor4f(c1*.5, c2*.5, c3*.5, .5);
    }
    drawTexturedRibbon(baseSpec(), points, pointsCount, _TEX_FINS_TILT, GL_MODULATE);
    glDisable(GL_BLEND);
    glPopMatrix();
    errorCheck("handleSquidSides");
}


/**
 * drawSquid
 *
 * draws the full squid object (does add things to the _ALPHAS list)
 */
void drawSquid(struct Spec spec)
{

    glPushMatrix();
    applySpec(spec);
    double c1 = _RANDOM_DOUBLES[0];
    if (c1 < -.1) {
        glColor4f(.5,.5,.5,1);
    } else {
        double c2 = _RANDOM_DOUBLES[1];
        double c3 = _RANDOM_DOUBLES[2];
        c1 = (c1 < 0) ? -c1 : c1;
        c2 = (c2 < 0) ? -c2 : c2;
        c3 = (c3 < 0) ? -c3 : c3;
        glColor4f(c1*.5, c2*.5, c3*.5, 1);
    }
    // body
    drawSphere(newSpecDefaultColor(.2,0,0 , 0,0,0 , .8,.3,.2), 45);
    updateAlphas(newAlpha(&handleSquidSides, newSpecDefaultColor(.3,0,-.1 , 270,0,0 , .6,1,1)));
    updateAlphas(newAlpha(&handleSquidSides, newSpecDefaultColor(.3,0,+.1 , 90,180,0 , .6,1,1)));
    // face
    drawSphere(newSpecDefaultColor(-.5,0,0 , 0,0,0 , .3,.3,.3), 45);
    drawSquidTenticle(newSpecDefaultColor(-.6,0,0 , 30,0,45 , 1.1,.7,.5));
    drawSquidTenticle(newSpecDefaultColor(-.6,.1,0 , 30,0,55 , 1,.7,.5));
    drawSquidTenticle(newSpecDefaultColor(-.6,.1,+.1 , 45,0,55 , 1.05,.7,.5));
    drawSquidTenticle(newSpecDefaultColor(-.6,.1,-.1 , 15,0,55 , 1.025,.7,.5));
    drawSquidTenticle(newSpecDefaultColor(-.6,-.1,0 , 185,0,55 , 1.2,.7,.5));
    glColor4f(1,1,1,1);
    drawEye(newSpecDefaultColor(-.5,0,-.25 , 0,0,0 , .1,.1,.1));
    drawEye(newSpecDefaultColor(-.5,0,+.25 , 0,0,0 , .1,.1,.1));
    glPopMatrix();
    errorCheck("drawSquid");
}


/**
 * drawTurtle
 *
 * draws a sea turtle object
 */
void drawTurtle(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);

    glColor4f(1,1,1,1);
    drawTexturedSphere(newSpecDefaultColor(0,0,0 , 0,0,0 , .8,.2,.6), _TEX_TURTLE_SHELL, GL_MODULATE, 45);
    glColor4f(.8,.8,.8,1);
    // neck
    drawTexturedUnitCylinder(newSpecDefaultColor(.8,0,0 , 0,0,0 , .3,.1,.2), _TEX_TURTLE_SCALES, GL_MODULATE, 45, false, false);
    // head
    drawTexturedSphere(newSpecDefaultColor(.9,0,0 , 0,0,5 , .35,.1,.3), _TEX_TURTLE_SCALES, GL_MODULATE, 45);
    // eye left
    drawEye(newSpecDefaultColor(1,.1,-.2 , 75,0,0 , .075,.075,.075));
    // eye right
    drawEye(newSpecDefaultColor(1,.1,+.2 , 105,0,0 , .075,.075,.075));
    // front left
    drawTexturedSphere(newSpecDefaultColor(.3,.1,-.6 , 20,30,0 , .2,.05,.5), _TEX_TURTLE_SCALES, GL_MODULATE, 45);
    drawTexturedSphere(newSpecDefaultColor(-.2,.15,-.85 , 30,100,0 , .15,.05,.4), _TEX_TURTLE_SCALES, GL_MODULATE, 45);
    // front right
    drawTexturedSphere(newSpecDefaultColor(.5,-.1,+.6 , 20,30,0 , .2,.05,.5), _TEX_TURTLE_SCALES, GL_MODULATE, 45);
    drawTexturedSphere(newSpecDefaultColor(.35,-.25,.95 , 40,100,0 , .15,.05,.4), _TEX_TURTLE_SCALES, GL_MODULATE, 45);
    // back left
    drawTexturedSphere(newSpecDefaultColor(-.8,-.1,-.3 , 325,80,0 , .2,.05,.35), _TEX_TURTLE_SCALES, GL_MODULATE, 45);
    // back right
    drawTexturedSphere(newSpecDefaultColor(-.8,-.1,+.3 , 20,100,0 , .2,.05,.35), _TEX_TURTLE_SCALES, GL_MODULATE, 45);

    glPopMatrix();
    errorCheck("drawTurtle");
}


/**
 * drawTwinTurtles
 *
 * draw two turtles
 * that are connected by their back feet
 * spinning around the y axis together
 * both face upwards
 */
void drawTwinTurtles(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);

    double x, z, scale, dy;
    
    scale = .5;

    x = -Cos(_SIN_STEP*5)/2;
    z = -Sin(_SIN_STEP*5)/2;
    dy = 0+_SIN_STEP*5;
    drawTurtle(newSpecDefaultColor(-x,0,z, 0,(int)dy,0 , scale,scale,scale));

    x *= -1;
    z *= -1;
    dy += 180;
    drawTurtle(newSpecDefaultColor(-x,0,z, 0,(int)dy,0 , scale,scale,scale));

    glPopMatrix();
    errorCheck("drawTwinTurtles");
}


/**
 * drawBaitBall
 *
 * draw a TON of fish in a dense unit space
 * apply the provided spec
 */
void drawBaitBall(struct Spec spec)
{

    glPushMatrix();
    applySpec(spec);

    double x, y, z, angle, color, size;
    x = _RANDOM_DOUBLES[0];
    y = x;
    z = x;
    angle = x;

    for (int i = 1; i < 1000 ; i++)
    {
        z = y;
        y = x;
        x = _RANDOM_DOUBLES[i];
        color = (x + 3)/4;
        angle = (x+1)*180;

        glColor4f(color,color,color,1);
        size = .1*(x+1);
        drawBlueTangSpec(newSpecDefaultColor(x,y,z , 0,angle,0 ,  size,size,size));
    }

    glPopMatrix();
    errorCheck("drawBaitBall");
}


/**
 * drawBackdrop
 *
 * draw a skybox
 * and draw the sloping system that the reef lays on
 */
void drawBackdrop(struct Spec spec)
{
    glPushMatrix();
    applySpec(spec);

    glUseProgram(_MY_SHADER);

    // setup texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // top reef
    double set[12] = {
        .8, -1,
        .7, -.2,
        0, -.15,
        -.1, .7,
        -.8, .65,
        -1, 1
    };
    genSlopeSet(set, 12);
    glDisable(GL_TEXTURE_2D);

    drawSkybox(baseSpec());
    glUseProgram(0);

    glPopMatrix();
    errorCheck("drawBackdrop");
}
    

/**
 * drawSeaScape
 *
 * the main image of interest
 * NOTE: -z is high in y
 * NOTE: +x is on the right (?stage left?)
 */
void drawSeaScape()
{
    glColor4f(1,1,1,1);
    drawBackdrop(baseSpec());

    // BUTTERFLY
    // NOTE: Butterfly draws at -1, not 0
    drawButterflyAnimation(newSpecDefaultColor(0,.85,-0.8 , 10,0,0 , .1,.1,.1));
    drawButterflyAnimation(newSpecDefaultColor(-.02,.8,-0.6 , 10,120,0 , .05,.05,.05));
    drawReef(newSpecDefaultColor(.1,.75,-.5 , 10,75,0 , .12,.13,.14));

    drawBlockingReef(newSpecDefaultColor(-.3,.75,-.7 , 10,20,0 , .3,.3,.3));
    drawBlockingReef(newSpecDefaultColor(0,.7,-.26 , 10,90,0 , .3,.3,.3));

    drawBlockingReef(newSpecDefaultColor(.4,.73,-.5 , 10,180,0 , .3,.3,.3));
    
    // SQUID1
    drawSquid(newSpecDefaultColor(.3,.9,-.9, 0,0,0 , .05,.05,.05));

    // GROUPER
    drawReef(newSpecDefaultColor(.8,.76,-.6 , 10,0,0 , .1,.1,.1));
    drawGrouperStation(newSpecDefaultColor(.78,.81,-.6 , 0,65,0 , .05,.05,.05));
    drawBlockingReef(newSpecDefaultColor(1,.75,-.7 , 10,180,0 , .3,.3,.3));
    
    // SQUID POD
    drawSquid(newSpecDefaultColor(.55,.8,-.3, 0,270,0 , .05,.025,.025));
    drawSquid(newSpecDefaultColor(.50,.85,-.35, 0,275,0 , .025,.025,.025));
    drawSquid(newSpecDefaultColor(.50,.75,-.35, 0,275,0 , .015,.015,.015));
    drawSquid(newSpecDefaultColor(.60,.85,-.35, 0,275,0 , .015,.015,.015));
    
    // SHARK
    double x,y,z , dy, dz;
    x = (1+Sin(_SIN_STEP*5))/3;
    y = .2;
    z = (1+Cos(_SIN_STEP*5))/3;
    dy = 90+_SIN_STEP*5;
    drawHammerhead(newSpecDefaultColor(x,y,z, 0,(int)dy,0, .2,.2,.2));
    x = (1+Sin(180+_SIN_STEP*5))/3;
    y = .18;
    z = (1+Cos(180+_SIN_STEP*5))/3;
    dy = 270+_SIN_STEP*5;
    drawHammerhead(newSpecDefaultColor(x,y,z, 0,(int)dy,0, .15,.15,.15));

    // TURTLE
    // big turtle
    x *= .8;
    x += .3;
    y = (1 + Sin(_SIN_STEP*5))/2;
    z *= .8;
    z += .1;
    dy += 360 + 90;
    dy = 360 - dy;
    dz = (Sin(_SIN_STEP*5) < 1) ? 25 : 340;
    double scale = .15;
    drawTurtle(newSpecDefaultColor(-x,y,z, 0,(int)dy,(int)dz , scale,scale,scale));

    // EEL
    drawReef(newSpecDefaultColor(.55,.7,-.2 , 10,120,0 , .1,.1,.1));
    drawEel(newSpecDefaultColor(.51,.65,-.15 , 5,190,0 , .05,.05,.05));
    drawBrainCoral(newSpecDefaultColor(.55,.6,-.2 , 0,90,0 , .09,.03,.2));

    // the smaller turtles in helix
    y = .1;
    scale = .2;
    dy = 0;
    drawTwinTurtles(newSpecDefaultColor(-.5,y,.35 , 0,dy,0 , scale,scale,scale));
    y += .1;
    scale -= .02;
    dy += 30;
    drawTwinTurtles(newSpecDefaultColor(-.5,y,.35 , 0,dy,0 , scale,scale,scale));
    y += .1;
    scale -= .02;
    dy += 30;
    drawTwinTurtles(newSpecDefaultColor(-.5,y,.35 , 0,dy,0 , scale,scale,scale));
    y += .1;
    scale -= .02;
    dy += 30;
    drawTwinTurtles(newSpecDefaultColor(-.5,y,.35 , 0,dy,0 , scale,scale,scale));
    y += .1;
    scale -= .02;
    dy += 30;
    drawTwinTurtles(newSpecDefaultColor(-.5,y,.35 , 0,dy,0 , scale,scale,scale));
    y += .1;
    scale -= .02;
    dy += 30;
    drawTwinTurtles(newSpecDefaultColor(-.5,y,.35 , 0,dy,0 , scale,scale,scale));
    y += .1;
    scale -= .02;
    dy += 30;
    drawTwinTurtles(newSpecDefaultColor(-.5,y,.35 , 0,dy,0 , scale,scale,scale));
    y += .1;
    scale -= .02;
    dy += 30;
    drawTwinTurtles(newSpecDefaultColor(-.5,y,.35 , 0,dy,0 , scale,scale,scale));

    // BAITBALL
    drawGrouper(newSpecDefaultColor(-.4,.81,-.4 , 10,0,0 , .07,.07,.08), false);
    drawEel(newSpecDefaultColor(-.8,.85,-.7 , 10,270,0 , .07,.07,.08));
    drawBaitBall(newSpecDefaultColor(-.8,.89,-.4, 0,75,0 , .3,.3,.3));


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // READ!
    //  below here we will use handle<X> functions directly
    //  we are drawing alpha objects
    //  anything that isn't alpha'ed should be written above here in
    //  the function (can leverage updateAlphas as needed above here)
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    

    // JELLY FISH need special handling
    // so the heads at the top level jellys look cool
    _JELLY_LESS_COOL = false;
    handleJellyfish(newSpecDefaultColor(.2,.85,-.45, 0,0,45 , .025,.05,.025));
    handleJellyfish(newSpecDefaultColor(.1,.9,-.45, 0,0,50 , .015,.05,.015));
    handleJellyfish(newSpecDefaultColor(.1,.8,-.5, 0,0,40 , .01,.05,.01));
    // THE DEEP: Lots of Jellyfish!
    // but the ones in the deep do not
    // if _JELLY_LESS_COOL is left false
    //    1. the bug of the heads being visible is super obvious
    //    2. this will destroy the frame rate as it draws an extra 3,500 spheres!
    _JELLY_LESS_COOL=true;
    x = _RANDOM_DOUBLES[0];
    for (int i = 0 ; i <= 100 ; i += 1)
    {
        y = (x - 1)/3;
        z = ((x + 1)/8) + .7;
        dz = (x + 1)*22;
        x = _RANDOM_DOUBLES[i];
        handleJellyfish(newSpecDefaultColor(x,y,z , 0,0,dz , .025,.1,.025));
    }
   

    errorCheck("drawSeaScape");
}


/**
 * updateProjection
 *
 * Credit: CSCI 5229 examples: ex9 Project
 * Credit: https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml
 */
void updateProjection()
{
    // FOV on other things
    glMatrixMode(GL_PROJECTION);
    //  Undo previous transformations
    glLoadIdentity();
    // first person
    if (_CUR_PERSPECTIVE_MODE == 2)
    {
        gluPerspective(_FOV,  // fovy
                _ASP,  // aspect
                .01,  // znear, tiny for reasons...
                4*_DIM);  // zfar
    }
    //  Perspective transformation
    else if (_CUR_PERSPECTIVE_MODE == 1)
    {
        gluPerspective(_FOV,  // fovy
                _ASP,  // aspect
                _DIM/4,  // znear
                4*_DIM);  // zfar
    }
    //  Orthogonal projection
    else
    {
        //  Undo previous transformations
        glLoadIdentity();
        glOrtho(-_ASP*_DIM,
                 +_ASP*_DIM,
                 -_DIM,+_DIM,
                 -_DIM,+_DIM);
    }
    //  Switch to manipulating the model matrix
    glMatrixMode(GL_MODELVIEW);
    //  Undo previous transformations
    glLoadIdentity();
    errorCheck("updateProjection");
}


/**
 * handleSpotLight
 *
 * NOTE: Must be done before projections are set
 * handle LIGHT1, directional
 */
void handleSpotLight()
{
    glShadeModel(GL_SMOOTH);
    //  here is all the lighting code
    //  OpenGL should normalize normal vectors
    //  this means that scaling your objects, ogl will recalc the lenght of your normal vector
    glEnable(GL_NORMALIZE);
    //  Enable lighting (in general we can have it now, but still need types of lights)
    glEnable(GL_LIGHTING);
    //  Location of viewer for specular calculations
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,__LOCAL);
    //  glColor sets ambient and diffuse color materials
    //  this is a good shortcut to not have to set things for each and every plane using glcolor
    //  glColor sets the vertex
    //  we here enable it to light with ambient and diffuse components of lights
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    // https://ogldev.org/www/tutorial21/tutorial21.html
    if (_FLASHLIGHT && _CUR_PERSPECTIVE_MODE == __FIRST_PERSON_MODE)
    {
        // EXAMPLES: https://www.glprogramming.com/red/chapter05.html
        // READINGS: https://learn.microsoft.com/en-us/windows/win32/opengl/gllightfv
        GLfloat light1_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
        GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        // set before transforms, its at the eye position and it goes out in the Z direction
        // at the eye
        GLfloat light1_position[] = { 0,0,0, 1};
        // z direction from the eye
        GLfloat spot_direction[] = {0,0,-1};

        glEnable(GL_LIGHT1);
        glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
        // https://www.glprogramming.com/red/chapter05.html#name10
        glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 4.0);
    }
    else
    {
        glDisable(GL_LIGHT1);
    }

    errorCheck("handleSpotLight");
}


/**
 * handleLight
 *
 * handle LIGHT0, NON-directional
 */
void handleLight()
{
    glShadeModel(GL_SMOOTH);
    // 4th value required for homogenous coord system
    // glLightfv expect 4 component array W/O it specified we get garbage values! (and segfaults)
    // emision value defaults to 0 if never set... so we're good
    float Ambient[]   = {0.01*_AMBIENT ,0.01*_AMBIENT ,0.01*_AMBIENT ,1.0};
    float Diffuse[]   = {0.01*__DIFFUSE ,0.01*__DIFFUSE ,0.01*__DIFFUSE ,1.0};
    float Specular[]  = {0.01*__SPECULAR,0.01*__SPECULAR,0.01*__SPECULAR,1.0};

    //  Light position
    float Position[] = { __DISTANCE*Cos(_LIGHT_AZIMUTH), __DISTANCE*Cos(_LIGHT_ELEVATION), __DISTANCE*Sin(_LIGHT_AZIMUTH), 1.0};

    // TODO: make thes conditionals less hacky, this works but its really weird logic to use
    // if first person, move the light to be inside of the unit cube
    // and don't draw a ball
    if (_CUR_PERSPECTIVE_MODE == __FIRST_PERSON_MODE)
    {
        Position[0] = Position[0]/2;
        Position[1] = .85;
        Position[2] = Position[2]/2;
    }
    // if not in first person then draw the light ball
    // Draw light position as ball (still no lighting here)
    // draw the ball to REPRESENT the light, not really the light itself
    // but, removing the ball causes problems on help menu, so just make it black instead
    else if (_HELP)
    {
        glColor3f(0,0,0);
        drawSphere(newSpec( 0,0,_DIM+.2 , 0,0,0 , .1,.1,.1, newColor(1,1,1,1), baseColor(), baseColor()), 90);
    }
    // in other cases, the ball is visible and moving in an x-like loop around the unit space
    else
    {
        glColor3f(1,1,1);
        drawSphere(newSpec( Position[0],Position[1],Position[2] , 0,0,0 , .1,.1,.1, newColor(1,1,1,1), baseColor(), baseColor()), 90);
    }

    debugFlood(">> light position\n\tAZ: %lf, EZ: %lf\n\tLAZ: %lf, LEZ: %lf\n\tx: %lf, y: %lf, z: %lf",
               _AZIMUTH, _ELEVATION,
               _LIGHT_AZIMUTH, _LIGHT_ELEVATION,
               Position[0], Position[1], Position[2]);

    //  OpenGL should normalize normal vectors
    //  this means that scaling your objects, ogl will recalc the lenght of your normal vector
    glEnable(GL_NORMALIZE);
    //  Enable lighting (in general we can have it now, but still need types of lights)
    glEnable(GL_LIGHTING);
    //  Location of viewer for specular calculations
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,__LOCAL);
    //  glColor sets ambient and diffuse color materials
    //  this is a good shortcut to not have to set things for each and every plane using glcolor
    //  glColor sets the vertex
    //  we here enable it to light with ambient and diffuse components of lights
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    // only have the light on if it is not night time
    if (!_NIGHT)
    {
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        //  this sets the position of the light
        glLightfv(GL_LIGHT0,GL_AMBIENT , Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE , Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR, Specular);
        glLightfv(GL_LIGHT0,GL_POSITION, Position);
    }
    else
    {
        glDisable(GL_LIGHT0);
    }

    errorCheck("handleLight");
}


/**
 * handleAllAlphas
 *
 * handle each drawing instruction in _ALPHAS
 */
void handleAllAlphas()
{
    // use _ALPHAS_INDEX not _ALPHAS_LEN for efficiency
    for (int i = 0 ; i < _ALPHAS_INDEX ; i++)
    {
        applyAlpha(_ALPHAS[i]);
    }
}


/**
 * displayProgram
 *
 * draw the current state of the program (sea scape or any other object or help menu)
 */
void displayProgram()
{
    // reset color index for consistent colors if showing lorenz system
    glColor3f(1,1,1);

    // glUseProgram(0);
    if (_COORD_LINES) drawCoordSystem();

    // draw the current object grouping of interest
    // special work to add fog if drawing the full scene
    if (_OBJECT_INDEX == __OBJECT_INDEX_MAX)
    {
        // enable fog
        glEnable(GL_FOG);
        glFogf(GL_FOG_MODE,  GL_EXP);
        glFogf(GL_FOG_DENSITY, 4);
        // want a blueish fog color for this scene.
        float fogColor[3]={0.1f,0.1f,0.2f};
        glFogfv(GL_FOG_COLOR, fogColor);
        // draw the objects of interest (will add to _ALPHAS)
        _OBJECTS[_OBJECT_INDEX]();

        // setup fog color differently
        // alpha fog colors being non 0 causes coloration of
        // what used to be black in the object in undesired ways
        // so set the fog color to black now
        float fogColorAlpha[3]={0.0f,0.0f,0.0f};
        glFogfv(GL_FOG_COLOR, fogColorAlpha);
        // draw the alpha objects
        handleAllAlphas();

        // disable fog before other states occur
        glDisable(GL_FOG);
    }
    else
    {
        // for any other object
        // fog isn't needed
        _OBJECTS[_OBJECT_INDEX]();
        handleAllAlphas();
    }

    errorCheck("displayProgram");
}


/**
 * handleView
 *
 * handle different view types
 * Credit: CSCI 5229: Ex9
 */
void handleView()
{
    // Ex9
    // Perspective - set eye position
    if (_CUR_PERSPECTIVE_MODE == __ORTHOGONAL_MODE)
    {   
        // Set view angle: order matters!
        // apply y before x
        // recall it applies in reverse order
        glRotatef(_ELEVATION,1,0,0);  // x
        glRotatef(_AZIMUTH,0,1,0);  // y
        debugFlood("ORTHOGANAL MODE:\n\tAZIMUTH: %d, ELEVATION: %d\n",
                   _AZIMUTH, _ELEVATION);
    }
    else if (_CUR_PERSPECTIVE_MODE == __PROJECTION_MODE)
    {   
        const double vx = +2*_DIM*Sin(_AZIMUTH)*Cos(_ELEVATION);
        const double vy = -2*_DIM              *Sin(_ELEVATION);
        const double vz = -2*_DIM*Cos(_AZIMUTH)*Cos(_ELEVATION);
        const double ex = -vx;
        const double ey = -vy;
        const double ez = -vz;
        const double Ux = 0;
        // needs offset rounding, don't use real cosine
        const double Uy = Cos(_ELEVATION);
        const double Uz = 0;
        
        debugFlood("PROJECTION MODE:\n\tAZIMUTH: %d, ELEVATION: %d,\n\tEx: %lf, Ey: %lf, Ez:%lf,\n\tx: %lf, y: %lf, z:%lf,\n\tUx: %lf, Uy: %lf, Uz: %lf\n",
                   _AZIMUTH, _ELEVATION,
                   ex, ey, ez,
                   Ux, Uy, Uz);

        gluLookAt(ex,ey,ez , vx,vy,vz , Ux,Uy,Uz);
    }
    else  // (_CUR_PERSPECTIVE_MODE == __FIRST_PERSON_MODE)
    {
        const double ex = _EX;
        const double ey = _EY;
        const double ez = _EZ;
        // need to be slightly off angle (can't be //) so use the rounded trig functions
        // w/o DIM it just wobbles
        const double vx = +2*_DIM*Sin(_AZIMUTH)*Cos(_ELEVATION);
        const double vy = -2*_DIM              *Sin(_ELEVATION);
        const double vz = -2*_DIM*Cos(_AZIMUTH)*Cos(_ELEVATION);
        const double Ux = 0;
        const double Uy = Cos(_ELEVATION);
        // helps put z in the correct direction
        const double Uz = 0;

        // SEA_DEBUG: change from "debogFlood" to "printStuff" for easy location tracking
        debugFlood("FIRST_PERSON_MODE\n\tAZ: %d, EL: %d\n\tEx: %lf, Ey: %lf, Ez:%lf,\n\tx: %lf, y: %lf, z:%lf,\n\tUx: %lf, Uy: %lf, Uz: %lf\n",
                   _AZIMUTH, _ELEVATION,
                   ex, ey, ez,
                   vx, vy, vz,
                   Ux, Uy, Uz);

        gluLookAt(ex,ey,ez , vx,vy,vz , Ux,Uy,Uz);
    }
}


/**
 * display
 *
 * the function used by glut glutDisplayFunc
 * on state machine updates to re-draw the screen
 *
 * credit: CSCI 5229 examples: ex4, ex9
 */
void display()
{
    // reset the index each time this is called and redraws all the things...
    _ALPHAS_INDEX=0;

    // clear the image (to avoid old images)
    // AND reset z-buffer (to ensure things layer correctly in 3rd dimension on turns)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // only show the front pixel
    glEnable(GL_DEPTH_TEST);
    // must now ensure all things rotate clockwise (unless you want a reverse cull like a backdrop)
    glEnable(GL_CULL_FACE);

    // reset previous transforms
    glLoadIdentity();
    // MUST BE BEFORE handleView so its in the same plane
    handleSpotLight();
    updateProjection();
    handleView();
    // MUST BE AFTER handle view (or else the light is on its own plane
    // handleLight here so the help menu works correctly on state changes
    handleLight();

    // EITHER: display the help menu
    // OR: display the lorenz system state
    if (_HELP) displayHelp();
    else displayProgram();

    // set text at bottom left of screen
    glWindowPos2i(5, 5);  // reset raster position
    glColor4f(1,1,1,1);
    dispText("h - help | mode: %d, AZ: %d, EL: %d", _CUR_PERSPECTIVE_MODE, _AZIMUTH, _ELEVATION);

    errorCheck("display");
    glFlush();
    glutSwapBuffers();
}


/**
 * reshape
 *
 * the function used by glut glutReshapeFunc
 * GLUT calls this routine when the display window is resized
 *
 * credit: CSCI 5229 examples: ex9
 */
void reshape(int width, int height)
{
    // Ratio of the width to the height of the window
    _ASP = (height>0) ? (double)width/height : 1;
    // Set the viewport to the entire window
    glViewport(0,0, RES*width,RES*height);
    // set projection
    updateProjection();

    errorCheck("reshape");
}


/**
 * special
 *
 * the function used by glut glutSpecialFunc
 * GLUT calls this routine when an arrow key is pressed
 *
 * credit: CSCI 5229 examples: ex4
 */
void special(int key,int x,int y)
{
    const double movement = 5;
    if (key == GLUT_KEY_RIGHT)
    {
        _AZIMUTH += movement;
    }
    else if (key == GLUT_KEY_LEFT)
    {
        _AZIMUTH -= movement;
    }
    else if (key == GLUT_KEY_UP)
    {
        // ELEVATION inverts for first person
        if (_CUR_PERSPECTIVE_MODE == __FIRST_PERSON_MODE)
        {
            _ELEVATION -= movement;
        }
        else
        {
            _ELEVATION += movement;
        }
    }
    else if (key == GLUT_KEY_DOWN)
    {
        if (_CUR_PERSPECTIVE_MODE == __FIRST_PERSON_MODE)
        {
            _ELEVATION += movement;
        }
        else
        {
            _ELEVATION -= movement;
        }
    }
    
    // Keep angles to [0,360] degrees
    if (_AZIMUTH < 0) _AZIMUTH = 360 + _AZIMUTH;
    if (_ELEVATION < 0) _ELEVATION = 360 + _ELEVATION;
    _AZIMUTH %= 360;
    _ELEVATION %= 360;

    // Update Projection
    updateProjection();

    // Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();

    errorCheck("special");
}


/**
 * idle
 *
 * the function used by glutIdleFunc
 * GLUT calls this routine on idle time
 *
 * credit: CSCI 5229 examples ex5
 */
void idle()
{
    bool changes = false;
    double t = glutGet(GLUT_ELAPSED_TIME);
    double tDelta = t - _PREVIOUS_TIME;
    if (!_HELP && _PLAY)
    {
        if (tDelta > __TIME_STEP)
        {
            changes = true;

            _SIN_STEP += 1;
            const double movement = 5;
            _LIGHT_AZIMUTH += movement;
            _LIGHT_AZIMUTH %= 360;
            _LIGHT_ELEVATION += movement*2;
            _LIGHT_ELEVATION %= 360;
            updateRandomDoubles();
        }
    }

    if (changes)
    {
        // Don't forget to reset _PREVIOUS_TIME
        _PREVIOUS_TIME = t;
        glutPostRedisplay(); 
    }
}


/**
 * updateObjectIndex
 *
 * change _OBJECT_INDEX by delta
 * within its bounds
 */
void updateObjectIndex(int delta)
{
    _OBJECT_INDEX += delta;
    if (_OBJECT_INDEX < 0)
    {
        const int newDelta = _OBJECT_INDEX;
        _OBJECT_INDEX = __OBJECT_INDEX_MAX + 1;
        updateObjectIndex(newDelta);
    }

    _OBJECT_INDEX %= __OBJECT_INDEX_MAX + 1;
    debugFlood("updateObjectIndex: %d\n", _OBJECT_INDEX);
}


/**
 * updateEye
 *
 * update the eye point _E<> based on current user input asdw
 * a: -leftRight
 * s: +backForward
 * d: +leftRight
 * w: -backForward
 */
void updateEye(double leftRight, double backForward)
{
    double xDelta = Cos(_AZIMUTH)*Cos(_ELEVATION)*leftRight + Sin(_AZIMUTH)*Cos(_ELEVATION)*backForward;
    double yDelta = -backForward*Sin(_ELEVATION); 
    double zDelta = Sin(_AZIMUTH)*Cos(_ELEVATION)*leftRight + -Cos(_AZIMUTH)*Cos(_ELEVATION)*backForward;
    _EX += xDelta;
    _EY += yDelta;
    _EZ += zDelta;
}


/**
 * key
 *
 * the function used by glut glutKeyboardFunc
 * GLUT calls this routine when a non-arrow key is pressed
 *
 * credit: CSCI 5229 examples: ex6
 */
void key(unsigned char ch, int x, int y)
{

    unsigned char ESC = 27;
    if (ch == ESC)
    {
        exit(0);
    }

    else if (ch == 'h' || ch == 'H')
    { 
        _HELP = !_HELP;
        // entering help mode
        if (_HELP)
        {
            // pause play and be able to reset when leaving help state
            _PREHELP_PLAY_STATE = _PLAY;
            _PLAY = false;
            _PREHELP_FLASHLIGHT_STATE = _FLASHLIGHT;
            _FLASHLIGHT = false;
            _PREHELP_NIGHT_STATE = _NIGHT;
            _NIGHT = false;
            // reset view angle and be able to reset when leaving help state
            _PREHELP_ELEVATION_STATE = _ELEVATION;
            _PREHELP_AZIMUTH_STATE = _AZIMUTH; 
            _ELEVATION = 0;
            _AZIMUTH = 0;

            _PREHELP_AMBIENT = _AMBIENT;
            _AMBIENT = 100;

            _PREHELP_CUR_PERSPECTIVE_MODE = _CUR_PERSPECTIVE_MODE;
            _CUR_PERSPECTIVE_MODE = 0;
        }
        // leaving help mode
        else 
        {
            // reset play state when leaving help state
            _PLAY = _PREHELP_PLAY_STATE;
            _FLASHLIGHT = _PREHELP_FLASHLIGHT_STATE;
            _NIGHT = _PREHELP_NIGHT_STATE;
            _ELEVATION = _PREHELP_ELEVATION_STATE;
            _AZIMUTH = _PREHELP_AZIMUTH_STATE;
            _CUR_PERSPECTIVE_MODE = _PREHELP_CUR_PERSPECTIVE_MODE;
            _AMBIENT = _PREHELP_AMBIENT; 
            updateFov();
        }
    }

    // only allowed when not in help mode
    else if (!_HELP)
    {
        const double movement = 0.01;
        if (ch == 'l' || ch == 'L') _COORD_LINES = !_COORD_LINES;
        else if (ch == 'p' || ch == 'P') _PLAY = !_PLAY;
        else if (ch == 'm' || ch == 'M')
        {
            _CUR_PERSPECTIVE_MODE += 1;
            _CUR_PERSPECTIVE_MODE %= __PROJECTION_MODE_ROUNDER;
            updateFov();
        }
        else if (ch == '0')
        {
            initGlobals();
            _SEASCAPE_LOCATOR[0]();
        }
        else if (ch == '1') _SEASCAPE_LOCATOR[0]();
        else if (ch == '2') _SEASCAPE_LOCATOR[1]();
        else if (ch == '3') _SEASCAPE_LOCATOR[2]();
        else if (ch == '4') _SEASCAPE_LOCATOR[3]();
        else if (ch == '5') _SEASCAPE_LOCATOR[4]();
        else if (ch == '6') _SEASCAPE_LOCATOR[5]();
        else if (ch == '7') _SEASCAPE_LOCATOR[6]();
        else if (ch == '8') _SEASCAPE_LOCATOR[7]();
        else if (ch == '9') _SEASCAPE_LOCATOR[8]();
        else if (ch == 'o') updateObjectIndex(-1);
        else if (ch == 'O') updateObjectIndex(+1);
        else if (ch == 'a') updateEye(-movement,0);
        else if (ch == 's') updateEye(0,-movement);
        else if (ch == 'w') updateEye(0,+movement);
        else if (ch == 'd') updateEye(+movement,0);
        else if (ch == 'f' || ch == 'F') _FLASHLIGHT = !_FLASHLIGHT;
        else if (ch == 'n' || ch == 'N') _NIGHT = !_NIGHT;
    }

    updateProjection();

    // Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();

    errorCheck("key");
}


/**
 * SERIES of void functions that are bound later
 *
 * seems easier than managing switch statements
 *
 * developer note: to determine the Az,El,Exyz for this, just
 * explore the space with printing on for the eye position in first person
 * from 'handleView' (see: SEA_DEBUG)
 *
 * Credit for void pointers: https://stackoverflow.com/questions/5309859/how-to-define-an-array-of-functions-in-c
 */
void setExplorerSeaScapeLocation() {
    // EXPLORER
    _AZIMUTH=115;
    _ELEVATION=0;
    _EX=-0.259444;
    _EY=0.827450;
    _EZ=-0.875480;
}
void setGrouperSeaScapeLocation() {
    // GROUPER
    _AZIMUTH=210;
    _ELEVATION=5;
	_EX=0.855958;
    _EY=0.817475;
    _EZ=-0.702144;
}
void setBaitBallSeaScapeLocation() {
    // BAITBALL
    _AZIMUTH=220;
    _ELEVATION=5;
    _EX=-0.530599;
    _EY=0.925335;
    _EZ=-0.837623;
}
void setButterflySeaScapeLocation() {
    // BUTTERFLY
    _AZIMUTH=60;
    _ELEVATION=350;
    _EX=-0.168720;
    _EY=0.837282;
    _EZ=-0.623340;
}
void setEelSeaScapeLocation() {
    // EEL
    _AZIMUTH=30;
    _ELEVATION=355;
	_EX=0.432915;
    _EY=0.648527;
    _EZ=-0.041536;
}
void setJellyfishSeaScapeLocation()
{
    // JELLY
    _AZIMUTH=145;
    _ELEVATION=5;
	_EX=0.046685;
    _EY=0.908647;
    _EZ=-0.587243;
}
void setSharkSeaScapeLocation() {
    // SHARK
	_AZIMUTH=115;
    _ELEVATION=10;
    _EX=-0.074073;
    _EY=0.253498;
    _EZ=0.019485;
}
void setTurtleSeaScapeLocation() {
    // TURTLE
	_AZIMUTH=45;
    _ELEVATION=90;
    _EX=-0.500842;
    _EY=0.877987;
    _EZ=0.348815;
}
void setSquidSeaScapeLocation() {
    // SQUID POD
	_AZIMUTH=210;
    _ELEVATION=15;
    _EX=0.625634;
    _EY=0.847912;
    _EZ=-0.426524;
}
/**
 * initSeaScapeLocator
 *
 * developer note: to determine the Az,El,Exyz for this, just
 * explore the space with printing on for the eye position in first person
 * from 'handleView' (see: SEA_DEBUG)
 *
 * if you change the length, be sure to change it at global level
 */
void initSeaScapeLocator()
{
    int i = 0;
    _SEASCAPE_LOCATOR[i] = &setExplorerSeaScapeLocation ; i++;
    _SEASCAPE_LOCATOR[i] = &setButterflySeaScapeLocation ; i++;
    _SEASCAPE_LOCATOR[i] = &setJellyfishSeaScapeLocation ; i++;
    _SEASCAPE_LOCATOR[i] = &setGrouperSeaScapeLocation ; i++;
    _SEASCAPE_LOCATOR[i] = &setSquidSeaScapeLocation ; i++;
    _SEASCAPE_LOCATOR[i] = &setSharkSeaScapeLocation ; i++;
    _SEASCAPE_LOCATOR[i] = &setEelSeaScapeLocation ; i++;
    _SEASCAPE_LOCATOR[i] = &setTurtleSeaScapeLocation ; i++;
    _SEASCAPE_LOCATOR[i] = &setBaitBallSeaScapeLocation ; i++;
    _SEASCAPE_LOCATOR[0]();
}


/**
 * SERIES of void functions that are bound later
 *
 * seems easier than managing switch statements
 *
 * Credit for void pointers: https://stackoverflow.com/questions/5309859/how-to-define-an-array-of-functions-in-c
 */
void makeTwinTurtles() { drawTwinTurtles(newSpecDefaultColor(0,0,0 , 0,0,0 , 2,2,2)); }
void makeEel() { drawEel(baseSpec()); }
void makeBaitBall() { drawBaitBall(baseSpec()); }
void makeSquid() { drawSquid(baseSpec()); }
void makeJellyfish() {
    _JELLY_LESS_COOL = false;
    drawJellyfish(baseSpec());
}
void makeButterflyAnimation() { drawButterflyAnimation(baseSpec()); }
void makeGrouperStation() { drawGrouperStation(baseSpec()); }
void makeBlockingReef() { drawBlockingReef(baseSpec()); }
void makeBackdrop() { drawBackdrop(baseSpec()); }
void makeSeaScape() { drawSeaScape(); }
/**
 * initObjects
 *
 * if you change the element count here,
 * you have to change it in the globals as well
 */
void initObjects()
{
    int i = 0;
    _OBJECTS[i] = &makeTwinTurtles ; i++;
    _OBJECTS[i] = &makeEel ; i++;
    _OBJECTS[i] = &makeBaitBall ; i++;
    _OBJECTS[i] = &makeSquid ; i++;
    _OBJECTS[i] = &makeJellyfish ; i++;
    _OBJECTS[i] = &makeButterflyAnimation ; i++;
    _OBJECTS[i] = &makeGrouperStation ; i++;
    _OBJECTS[i] = &makeBlockingReef ; i++;
    _OBJECTS[i] = &makeBackdrop ; i++;
    _OBJECTS[i] = &makeSeaScape ; i++;
}


/**
 * main
 *
 * standard C required function. first called function. workhorse
 *
 * credit: CSCI 5229 examples: ex6, ex4
 */
int main(int argc, char* argv[])
{
    // initialize global variables to initial values
    initGlobals();
    initObjects();
    initSeaScapeLocator();
    // alphas Doesn't need initialization
    // as handleAllAlphas looks only at things drawn in this loop

    // extra lines for ease of reading
    printStuff("\n\n\n\n\n");
    
    glutInit(&argc,argv);

    // Request true color window for best viewing
    // AND double buffered for smoother loading
    // AND depth buffer to ensure depth displays correctly on turns
    // AND alpha for transparent object support
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);

    glutInitWindowSize(750,750);
    glutCreateWindow("Spencer Wilson Final Project");
#ifdef USEGLEW
    if (glewInit()!=GLEW_OK) fatal("Error initializing GLEW\n");
#endif

    // load my special shader
    // need to do after glut is initialized
    // should only do once
    _MY_SHADER=initShader();

    // bind all the glut things
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutIdleFunc(idle);
    glutKeyboardFunc(key);

    // need to do after glut is initialized
    // should only do once
    initTextures();

    // Enable Z-buffer depth test so depth displays correctly on turns
    glEnable(GL_DEPTH_TEST);

    // for RandomDouble seeding, only do once
    srand ( time ( NULL));

    // actually start the glut thing
    glutMainLoop();
    return 0;
}


