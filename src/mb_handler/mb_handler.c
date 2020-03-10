/*
 * mb_handler.c
 * 
 * implementation of mb_handler.h
 */



/* include mb_handler.h */
#include "mb_handler.h"



/* FUNCTION IMPLEMENTATIONS */

/* mb_create_set
 *
 * constructs a mandelbrot set
 * 
 * re/im_min_range: initial minimum ranges for real and imaginary axis
 * re/im_centre: point at the ventre of the screen
 * 
 * return: pointer mb_set_t set != NULL on success, == NULL on failure
 */
mb_set_t mb_create_set ( const float re_min_range, const float im_min_range, const float re_centre, const float im_centre, const float breakout, const float max_it, const int power )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, NULL, "MB ERROR: glad must be initialised before creating a mandelbrot set" );

    /* create struct */
    mb_set_t mb_set = __mb_create_empty_set ();

    /* default vertices */
    const float vertices [ 12 ] = 
    {
        -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f
    };

    /* default indices */
    const unsigned indices [ 6 ] =
    {
        0, 1, 2,
        1, 2, 3
    };

    /* set up shader program (first, as is most likely to fail) */
    if ( ( mb_set->vshader = glh_create_shader_from_path ( "./src/shader/generic_vertex.glsl", GLH_GLSL_VERTEX_SHADER ) ) == -1 ||
         ( mb_set->fshader = glh_create_shader_from_path ( "./src/shader/mandelbrot_fragment.glsl", GLH_GLSL_FRAGMENT_SHADER ) ) == -1 ||
         ( mb_set->sprogram = glh_create_shader_program ( mb_set->vshader, -1, mb_set->fshader ) ) == -1 ||
         ( glh_use_shader_program ( mb_set->sprogram ) ) == -1 )
    {
        /* error creating shader program */
        fprintf ( stderr, "MB ERROR: failed to create shader program\n" );
        mb_destroy_set ( mb_set );
        return NULL;
    }

    /* set up vertex array object */
    if ( ( mb_set->vao = glh_create_vertex_array_object () ) == -1 ||
         ( mb_set->vbo = glh_create_vertex_buffer_object ( vertices, sizeof ( vertices ), GLH_BUFF_STATIC_DRAW ) ) == -1 ||
         ( mb_set->ebo = glh_create_element_buffer_object ( indices, sizeof ( indices ), GLH_BUFF_STATIC_DRAW ) ) == -1 ||
         ( glh_bind_vertex_buffer_object ( mb_set->vao, mb_set->vbo, 0, 3, GLH_TYPE_FLOAT, 3 * sizeof ( float ), ( void * ) 0 ) ) == -1 ||
         ( glh_bind_element_buffer_object ( mb_set->vao, mb_set->ebo ) ) == -1 ||
         ( glh_enable_vertex_attribute ( mb_set->vao, 0 ) ) == -1 )
    {
        /* error creating vertex array object */
        fprintf ( stderr, "MB ERROR: failed to set up vertex array object\n" );
        mb_destroy_set ( mb_set );
        return NULL;        
    }

    /* get uniform locations */
    if ( ( mb_set->uni_stretch = glh_get_uniform_location ( mb_set->sprogram, "mandelbrot_stretch" ) ) == -1 ||
         ( mb_set->uni_translation = glh_get_uniform_location ( mb_set->sprogram, "mandelbrot_translation" ) ) == -1 ||
         ( mb_set->uni_breakout = glh_get_uniform_location ( mb_set->sprogram, "mandelbrot_breakout" ) ) == -1 ||
         ( mb_set->uni_max_it = glh_get_uniform_location ( mb_set->sprogram, "mandelbrot_max_it" ) ) == -1 ||
         ( mb_set->uni_power = glh_get_uniform_location ( mb_set->sprogram, "mandelbrot_power" ) ) == -1 )
    {
        /* failed to get uniform location */
        fprintf ( stderr, "MB ERROR: failed to get uniform locations\n" );
        mb_destroy_set ( mb_set );
        return NULL;
    }

    /* set mandelbrot parameters */
    mb_set->re_min_range = re_min_range;
    mb_set->im_min_range = im_min_range;
    mb_set->re_centre = re_centre;
    mb_set->im_centre = im_centre;
    mb_set->breakout = breakout;
    mb_set->max_it = max_it;
    mb_set->power = power;

    /* set up the mutex */
    pthread_mutex_init ( &mb_set->draw_mutex, NULL );

    /* return mb_set */
    return mb_set;
}

/* __mb_create_empty_set
 *
 * constructs an empty mandelbrot set
 * 
 * return: pointer to an empty mandelbrot set, cannot fail
 */
mb_set_t __mb_create_empty_set ()
{
    /* allocate set */
    mb_set_t mb_set = ( mb_set_t ) malloc ( sizeof ( __mb_set_t ) );

    /* set the set to its empty state */
    mb_set->vao = -1;
    mb_set->vbo = -1;
    mb_set->ebo = -1;

    mb_set->vshader = -1;
    mb_set->fshader = -1;
    mb_set->sprogram = -1;

    mb_set->uni_stretch = -1;
    mb_set->uni_translation = -1;
    mb_set->uni_breakout = -1;
    mb_set->uni_max_it = -1;

    mb_set->re_min_range = 0;
    mb_set->im_min_range = 0;

    mb_set->re_range = 0;
    mb_set->im_range = 0;

    mb_set->re_centre = 0;
    mb_set->im_centre = 0;

    mb_set->power = 0;

    memset ( &mb_set->draw_mutex, 0, sizeof ( pthread_mutex_t ) );

    /* return the set */
    return mb_set;
}

/* mb_destroy_set
 *
 * destroys a mandelbrot set
 * 
 * mb_set: the set to destroy
 *
 * return: 0 for success, -1 for failure
 */
int mb_destroy_set ( mb_set_t mb_set )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "MB ERROR: glad must be initialised before destroying a mandelbrot set" );

    /* if changed from empty, destroy attributes */
    if ( mb_set->vao != -1 ) glh_delete_vertex_array_object ( mb_set->vao );
    if ( mb_set->vbo != -1 ) glh_delete_vertex_buffer_object ( mb_set->vbo );
    if ( mb_set->ebo != -1 ) glh_delete_element_buffer_object ( mb_set->ebo );

    if ( mb_set->vshader != -1 ) glh_delete_shader ( mb_set->vshader );
    if ( mb_set->fshader != -1 ) glh_delete_shader ( mb_set->fshader );
    if ( mb_set->sprogram != -1 ) glh_delete_shader_program ( mb_set->sprogram );

    pthread_mutex_t zero_mutex;
    memset ( &zero_mutex, 0, sizeof ( pthread_mutex_t ) );
    if ( memcmp ( &mb_set->draw_mutex, &zero_mutex, sizeof ( pthread_mutex_t ) ) != 0 ) pthread_mutex_destroy ( &mb_set->draw_mutex ); 

    /* free the set */
    free ( mb_set );

    /* return 0 for success */
    return 0;    
}

/* mb_draw
 *
 * draws the mandelbrot set onto a window
 * 
 * mb_set: the mandelbrot set to draw
 * window: the window to draw onto
 * 
 * return: 0 for success, -1 for failure
 */
int mb_draw ( mb_set_t mb_set, glh_window_t window )
{
    /* check glfw and glad are initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "MB ERROR: glfw must be initialised before drawing a mandelbrot set" );
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "MB ERROR: glad must be initialised before drawing a mandelbrot set" );

    /* lock the mutex */
    pthread_mutex_lock ( &mb_set->draw_mutex );

    /* get viewport size */
    int viewport_size [ 4 ];
    glh_get_viewport_size ( window, viewport_size );

    /* create the stretch coeficients */
    const float re_stretch = mb_set->re_min_range / ( float ) viewport_size [ 2 ];
    const float im_stretch = mb_set->im_min_range / ( float ) viewport_size [ 3 ];

    /* set the overall stretch to the largest coeficient */
    const float stretch = fmaxf ( re_stretch, im_stretch );

    /* get the new ranges */
    const float re_range = ( float ) viewport_size [ 2 ] * stretch;
    const float im_range = ( float ) viewport_size [ 3 ] * stretch;

    /* set the new ranges */
    mb_set->re_range = re_range;
    mb_set->im_range = im_range;

    /* set translation */
    const float re_translation = 0 + mb_set->re_centre - ( re_range / 2 );
    const float im_translation = 0 + mb_set->im_centre - ( im_range / 2 );

    /* make window current and use shader program */
    glh_make_window_current ( window );
    glh_use_shader_program ( mb_set->sprogram );

    /* set uniforms */
    glh_set_uniform_vec4 ( mb_set->uni_stretch, stretch, stretch, 1.0f, 1.0f );
    glh_set_uniform_vec4 ( mb_set->uni_translation, re_translation, im_translation, 0.0f, 0.0f );
    glh_set_uniform_float ( mb_set->uni_breakout, mb_set->breakout );
    glh_set_uniform_int ( mb_set->uni_max_it, ( int ) mb_set->max_it );
    glh_set_uniform_int ( mb_set->uni_power, mb_set->power );

    /* clear, render and swap buffers */
    glh_set_clear_color ( 1.0f, 1.0f, 1.0f, 1.0f );
    glh_clear_screen ();
    glh_draw_elements ( mb_set->vao, GLH_DRAWMODE_TRIANGLES, 6, GLH_TYPE_UNSIGNED_INT );
    glh_swap_buffers ( window );

    /* unlock mutex */
    pthread_mutex_unlock ( &mb_set->draw_mutex );

    /* return 0 for success */
    return 0;
}