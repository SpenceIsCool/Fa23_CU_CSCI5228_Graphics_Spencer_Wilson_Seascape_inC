/**
 * alpha.c
 *
 * @author: Spencer David Wilson
 * @course: CSCI 5229 Introduction to Computer Graphics
 * @prof: Willem A. (Vlakkies) Schreüder
 * @date: Dec 1, 2023
 */


#include "SDW.h"


// Defined in SDW.h
// TODO: update to C++ so this can be in a single file as a class
//
// typedef void (* alpha_func_type )(struct Spec);
// struct Alpha {
//     // handler function that tells us what to draw (expects a spec input)
//     alpha_func_type f;
//     // spec provided by user (to pass to f)
//     struct Spec spec;
//     // the projection matrix at time of definition
//     GLfloat projMat[16];
//     // the transition matrix at time of definition
//     GLfloat trxMat[16];
// };


/**
 * newAlpha
 *
 * construct a new alpha object given the f and spec provided
 * NOTE: at call time this will find the current projection and model view matrix
 *       as the definition time matrixes to reapply later
 *       so call this inside the relevant push/pop matrix set.
 */
struct Alpha newAlpha(alpha_func_type f, struct Spec spec)
{
    struct Alpha res;
    res.f = f;
    res.spec = spec;

    // set dynamically to the definition of the alpha object
    glGetFloatv(GL_PROJECTION_MATRIX, res.projMat);
    glGetFloatv(GL_MODELVIEW_MATRIX, res.trxMat);

    return res;
}


/**
 * applyAlpha
 *
 * reset the matrix to what it was when the alpha was defined
 * call the stored function f with the stored specification spec
 *
 * credit: https://stackoverflow.com/questions/29178121/store-a-matrix-created-by-a-function-opengl
 */
void applyAlpha(struct Alpha alpha)
{
    // reset the matrix to what it was when the alpha was defined
    // Switch to manipulating the projection matrix
    glMatrixMode(GL_PROJECTION);
    // apply the old projection matrix
    glLoadMatrixf(alpha.projMat);
    // Switch to manipulating the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    // apply the old modelview matrix
    glLoadMatrixf(alpha.trxMat);

    // call the stored function f with the stored specification spec
    alpha.f(alpha.spec);
}


