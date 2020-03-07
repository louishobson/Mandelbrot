/*
 * mb_handler.h
 * 
 * handles the creation of a renderable, editable mandlebrot set
 */



/* pragma one */
#ifndef MB_HANDLER_H_INCLUDED
#define MB_HANDLER_H_INCLUDED



/* include general headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <math.h>

/* include glhelper.h */
#include <glhelper/glhelper.h>



/* STRUCTURES */

/* struct __mb_set_t
 *
 * structure to hold all the data needed to render a mandelbrot set
 */
typedef struct
{
    /* OPENGL OBJECTS AND DEPENDANCIES */

    /* vao, vbo and ebo */
    glh_object_t vao;
    glh_object_t vbo;
    glh_object_t ebo;

    /* shaders and shader program */
    glh_object_t vshader;
    glh_object_t fshader;
    glh_object_t sprogram;

    /* uniforms */
    glh_object_t uni_stretch;
    glh_object_t uni_translation;
    glh_object_t uni_breakout;
    glh_object_t uni_max_it;
    glh_object_t uni_power;

    /* MANDELBROT PARAMETERS */

    /* minimum ranges that should be visible on the real and imaginary axis */
    float re_min_range;
    float im_min_range;

    /* centre of the screen */
    float re_centre;
    float im_centre;

    /* current range of real and imaginary axis */
    float re_range;
    float im_range;

    /* breakout point and maximum iterations */
    float breakout;
    float max_it;

    /* power */
    int power;

    /* OTHER ATTRIBUTES */

    /* locking to ensure not rendering conflict */
    pthread_mutex_t draw_mutex;

} __mb_set_t;



/* TYPEDEFS */

/* typedef mb_set_t -> __mb_set_t * */
typedef __mb_set_t * mb_set_t;



/* FUNCTIONS */

/* mb_create_set
 *
 * constructs a mandelbrot set
 * 
 * re/im_min_range: initial minimum ranges for real and imaginary axis
 * re/im_centre: point at the ventre of the screen
 * breakout: initial breakout value
 * max_it: initial maximum iterations
 * 
 * return: mb_set_t set != NULL on success, == NULL on failure
 */
mb_set_t mb_create_set ( const float re_min_range, const float im_min_range, const float re_centre, const float im_centre, const float breakout, const float max_it, const int power );

/* __mb_create_empty_set
 *
 * constructs an empty mandelbrot set
 * 
 * return: an empty mandelbrot set, cannot fail
 */
mb_set_t __mb_create_empty_set ();

/* mb_destroy_set
 *
 * destroys a mandelbrot set
 * 
 * mb_set: the set to destroy
 *
 * return: 0 for success, -1 for failure
 */
int mb_destroy_set ( mb_set_t mb_set );

/* mb_draw
 *
 * draws the mandelbrot set onto a window
 * 
 * mb_set: the mandelbrot set to draw
 * window: the window to draw onto
 * 
 * return: 0 for success, -1 for failure
 */
int mb_draw ( mb_set_t mb_set, glh_window_t window );



/* #ifndef MB_HANDLER_H_INCLUDED */
#endif