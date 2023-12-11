#ifndef SDW
#define SDW

/**
 * SDW: Spencer David Wilson
 *
 * A collection of useful functions accumulated in the semester
 * TODO: move things to C++ for better OOP and architecture of the code
 * CREDIT: CSCI 5229: CSCIx229.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

// GLEW _MUST_ be included first
#ifdef USEGLEW
#include <GL/glew.h>
#endif
//  Get all GL prototypes
#define GL_GLEXT_PROTOTYPES
//  Select SDL, SDL2, GLFW or GLUT
#if defined(SDL2)
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#elif defined(SDL)
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#elif defined(GLFW)
#include <GLFW/glfw3.h>
#elif defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//  Make sure GLU and GL are included
#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
// Tell Xcode IDE to not gripe about OpenGL deprecation
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif
//  Default resolution
//  For Retina displays compile with -DRES=2
#ifndef RES
#define RES 1
#endif


//  Cosine and Sine in degrees
//  Uses a rounded pi to avoid true 0s in output - PROTIP
#define CosReal(x) (cos((x)*M_PI/180))
#define SinReal(x) (sin((x)*M_PI/180))
// Credit: CSCI 5229 examples: ex8
// rounding error intentional for overlap resolution
#define Cos(x) (cos((x)*3.14159265/180))
#define Sin(x) (sin((x)*3.14159265/180))

// TODO: can the struct be written in the c file? how?


struct Coord {
    double x;
    double y;
    double z;
};


struct Color {
    float r;
    float g;
    float b;
    float a;
};


struct Spec {
    // origin
    double x;
    double y;
    double z;

    // rotation
    double dx;
    double dy;
    double dz;

    // scale
    double sx;
    double sy;
    double sz;

    // light properties
    struct Color emission;
    struct Color specular;
    struct Color shiny;
};


typedef void (* alpha_func_type )(struct Spec);
struct Alpha {
    // handler function that tells us what to draw (expects a spec input)
    alpha_func_type f;
    // spec provided by user (to pass to f)
    struct Spec spec;
    // the projection matrix at time of definition
    GLfloat projMat[16];
    // the transition matrix at time of definition
    GLfloat trxMat[16];
};


#ifdef __cplusplus
extern "C" {
#endif


// COORD: coord.c
struct Coord newCoord(double x, double y, double z);
void printCoord(struct Coord coord);
void plotCoord(struct Coord coord);

// PRINTERS: printers.c
void printStuff(char const* f, ...);
void debugFlood(char const* f, ...);
void debugStream(char const* f, ...);
void debugTrickle(char const* f, ...);
void fatal(char const* f, ...);
void errorCheck(const char* where);

// SPECIFICATION spec.c
struct Color newColor(float r, float g, float b, float a);
struct Color baseColor();
void setColor(struct Color color);
struct Spec newSpec(
    double x, double y, double z,
    double dx, double dy, double dz,
    double sx, double sy, double sz,
    struct Color emission, struct Color specular, struct Color shiny);
struct Spec newSpecDefaultColor(
    double x, double y, double z,
    double dx, double dy, double dz,
    double sx, double sy, double sz);
struct Spec baseSpec();
void applySpec(struct Spec spec);

// ALPHA alpha.c
struct Alpha newAlpha(alpha_func_type f, struct Spec spec);
void applyAlpha(struct Alpha alpha);

// SHAPES baseShapes.c
void ourSpherePolarVertex(double th, double ph);
void drawSphere(struct Spec spec, double stepInterval);
void drawCylinder(struct Spec spec, double radius, double length, double stepInterval);
void drawCone(struct Spec spec, double radius, double length, double stepInterval);
void drawCube(struct Spec spec);
void drawTexturedCube(struct Spec spec, unsigned int texture, GLint mode);
void drawTexturedSphere(struct Spec spec, unsigned int texture, GLint mode, double stepInterval);
void drawTexturedUnitCylinder(struct Spec spec,
        unsigned int texture,
        GLint mode,
        double stepInterval,
        bool hasCap,
        bool hasBase);
void drawTexturedRibbon(struct Spec spec,
        struct Coord valPairs[],
        int countValPairs,
        unsigned int texture,
        GLint mode);
void drawTexturedTube(struct Spec spec,
        struct Coord centers[],
        int countCenters,
        double radius,
        unsigned int texture,
        GLint mode,
        double stepInterval,
        bool hasCap,
        bool hasBase);
void drawTexturedCylinder(struct Spec spec,
        double radius,
        double length,
        unsigned int texture,
        GLint mode,
        double stepInterval);
void drawTexturedCone(struct Spec spec,
        double radius,
        double length,
        unsigned int texture,
        GLint mode,
        double stepInterval);


// IMGS loadtexbmp.c
unsigned int loadTexBmp(const char* file);


// shader.c
void printShaderLog(int obj,char* file);
char* readText(char *file);
int createShader(GLenum type, char* file);
void printProgramLog(int obj);
int initShader();


#ifdef __cplusplus
}
#endif

#endif  // ifndef SDW
