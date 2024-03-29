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



/* MACROS */

/* MANDELBROT_INSTALL_PATH
 *
 * defines the directory where the mandelbrot source is stored
 * can be set during compilation using -DMANDELBROT_INSTALL_PATH='"/path/"'
 */
#ifndef MANDELBROT_INSTALL_PATH
#define MANDELBROT_INSTALL_PATH "."
#endif

/* MANDELBROT_SHADER_PATH
 *
 * defines the directory in which to find shaders
 * can be set during compilation using -DMANDELBROT_SHADER_PATH='"/path/"'
 */
#ifndef MANDELBROT_SHADER_PATH
#define MANDELBROT_SHADER_PATH MANDELBROT_INSTALL_PATH "/src/shader"
#endif

/* MANDELBROT_VERTEX/FRAGMENT_SHADER_PATH
 *
 * defines the location of the vertex/fragment shaders
 * can be set during compilation using -DMANDELBROT_VERTEX/FRAGMENT_SHADER_PATH='"/path/file"'
 */
#ifndef MANDELBROT_VERTEX_SHADER_PATH
#define MANDELBROT_VERTEX_SHADER_PATH MANDELBROT_SHADER_PATH "/generic_vertex.glsl"
#endif
#ifndef MANDELBROT_FRAGMENT_SHADER_PATH
#define MANDELBROT_FRAGMENT_SHADER_PATH MANDELBROT_SHADER_PATH "/mandelbrot_fragment.glsl"
#endif

/* MBDEF_...
 * 
 * default mandelbrot parameters
 * can be passed to mb_set_t constructor to create a default mandelbrot set
 */
#define MBDEF_RE_MIN_RANGE 5.0
#define MBDEF_IM_MIN_RANGE 5.0
#define MBDEF_RE_CENTRE 0.0
#define MBDEF_IM_CENTRE 0.0
#define MBDEF_BREAKOUT 2.0
#define MBDEF_MAX_IT 40.0
#define MBDEF_POWER 2
#define MBDEF_ROTATION 0.0



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
    glh_object_t uni_rotation;

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

    /* rotation (in degrees) */
    float rotation;

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
mb_set_t mb_create_set ( const float re_min_range, const float im_min_range, const float re_centre, const float im_centre, const float breakout, const float max_it );

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