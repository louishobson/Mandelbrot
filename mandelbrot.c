/*
 * mandelbrot.c
 * 
 * contains the main () entry point for the mandelbrot binary
 * 
 * creates window and mandelbrot objects, before setting up all the input callbacks, before entering an event-draw loop
 * cleans up after being closed or ESC is pressed
 * 
 */



/* include general headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

/* include mb_handler */
#include <mb_handler/mb_handler.h>



/* MACROS AND GLOBAL DEFINITIONS */

/* MANDELBROT_RANGE_COEFICIENT
 *
 * defines the change in minimum ranges of the set for each zoom
 */
#define MANDELBROT_RANGE_COEFICIENT 0.9

/* MANDELBROT_IT_COEFICIENT
 *
 * defines how the maximum number of iterations should increase with each zoom
 */
#define MANDELBROT_IT_COEFICIENT 1.0375

/* MANDELBROT_ROTATION_STEP
 *
 * defines the stop for each rotation of the set
 */
#define MANDELBROT_ROTATION_STEP acos ( -1 ) / 16.0

/* MANDELBROT_MOVE_STEP
 *
 * defines the fraction of the screen pressing the arrow keys should cause the set to move
 */
#define MANDELBROT_MOVE_STEP 0.075

/* volatile size_t mb_set_ptr
 *
 * int-casted pointer to the mandelbrot set currently being rendered
 */
volatile size_t mb_set_ptr = 0;

/* volatile double scroll_track
 *
 * keeps a track of how zoomed in the mandelbrot set is, through summing the yoffsets in the scroll callback
 */
volatile double scroll_track = 0.0;



/* CALLBACK FUNCTIONS */

/* mandelbrot_drag_callback
 *
 * callback when dragging occurs
 * 
 * window: the window the drag is being applied to
 * x,ydrag: the change in x and y
 */
void mandelbrot_drag_callback ( glh_window_t window, const double xdrag, const double ydrag )
{
    /* get the mandelbrot set */
    mb_set_t mb_set = ( mb_set_t ) mb_set_ptr;   

    /* get viewport size */
    int viewport_size [ 4 ];
    glh_get_viewport_size ( window, viewport_size );

    /* find change as fraction of viewport */
    const double xfrac = xdrag / viewport_size [ 2 ];
    const double yfrac = ydrag / viewport_size [ 3 ];

    /* add change to centre of mandelbrot by a fraction of what is currently visible on thr real and imaginary axis */
    mb_set->re_centre -= mb_set->re_range * xfrac;
    mb_set->im_centre += mb_set->im_range * yfrac;
}

/* mandelbrot_scroll_callback
 *
 * callback when scrolling occurs
 * 
 * window: window the scrolling is being applied to
 * x,yoffset: the change in x and y
 */
void mandelbrot_scroll_callback ( glh_window_t window, const double xoffset, const double yoffset )
{
    /* get the mandelbrot set */
    mb_set_t mb_set = ( mb_set_t ) mb_set_ptr; 

    /* add the offset to scroll track */
    scroll_track += yoffset;

    /* get mouse position */
    double xpos, ypos;
    glh_get_cursor_pos ( window, &xpos, &ypos );

    /* get viewport size */
    int viewport_size [ 4 ];
    glh_get_viewport_size ( window, viewport_size );

    /* find mouse position in terms of fractions of the viewport centre */
    const double xcfrac = ( xpos / viewport_size [ 2 ] ) - 0.5;
    const double ycfrac = ( ypos / viewport_size [ 3 ] ) - 0.5;

    /* find the scaling multiples */
    const double range_from_def_multiple = pow ( MANDELBROT_RANGE_COEFICIENT, scroll_track );
    const double range_from_curr_multiple = pow ( MANDELBROT_RANGE_COEFICIENT, yoffset );
    const double it_from_def_multiple = pow ( MANDELBROT_IT_COEFICIENT, scroll_track );

    /* find the change in real and imaginary ranges */
    const double re_range_change = ( mb_set->re_range * range_from_curr_multiple ) - mb_set->re_range;
    const double im_range_change = ( mb_set->im_range * range_from_curr_multiple ) - mb_set->im_range;

    /* find the new centre */
    mb_set->re_centre -= re_range_change * xcfrac;
    mb_set->im_centre += im_range_change * ycfrac;

    /* find the new real and imaginary min ranges */
    mb_set->re_min_range = MBDEF_RE_MIN_RANGE * range_from_def_multiple;
    mb_set->im_min_range = MBDEF_IM_MIN_RANGE * range_from_def_multiple;

    /* change the maximum iterations */
    mb_set->max_it = MBDEF_MAX_IT * it_from_def_multiple;
}

/* mandelbrot_key_callback
 *
 * callback for when a key is pressed
 * 
 * window: window the key was pressed on
 * key: the key pressed
 * scancode: key-specific scancode
 * action: the action
 * mods: the modifier bits
 */
void mandelbrot_key_callback ( glh_window_t window, const int key, const int scancode, const int action, const int mods )
{
    /* get the mandelbrot set */
    mb_set_t mb_set = ( mb_set_t ) mb_set_ptr;

    /* if escape, set window should close */
    if ( glh_get_key ( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) glh_set_window_should_close ( window );
    
    /* if W/Q, increase/decrease the power */
    if ( glh_get_key ( window, GLFW_KEY_W ) == GLFW_PRESS ) mb_set->power += 1;
    if ( glh_get_key ( window, GLFW_KEY_Q ) == GLFW_PRESS ) mb_set->power -= 1;

    /* if A/S, rotate the set */
    if ( glh_get_key ( window, GLFW_KEY_A ) == GLFW_PRESS ) mb_set->rotation += MANDELBROT_ROTATION_STEP;
    if ( glh_get_key ( window, GLFW_KEY_S ) == GLFW_PRESS ) mb_set->rotation -= MANDELBROT_ROTATION_STEP;

    /* if +/-, zoom in and out */
    if ( glh_get_key ( window, GLFW_KEY_EQUAL ) == GLFW_PRESS ) mandelbrot_scroll_callback ( window, 0, 1 );
    if ( glh_get_key ( window, GLFW_KEY_MINUS ) == GLFW_PRESS ) mandelbrot_scroll_callback ( window, 0, -1 );

    /* arrow keys to move */
    if ( glh_get_key ( window, GLFW_KEY_LEFT ) == GLFW_PRESS ) mb_set->re_centre -= mb_set->re_range * MANDELBROT_MOVE_STEP;
    if ( glh_get_key ( window, GLFW_KEY_UP ) == GLFW_PRESS ) mb_set->im_centre += mb_set->im_range * MANDELBROT_MOVE_STEP;
    if ( glh_get_key ( window, GLFW_KEY_RIGHT ) == GLFW_PRESS ) mb_set->re_centre += mb_set->re_range * MANDELBROT_MOVE_STEP;
    if ( glh_get_key ( window, GLFW_KEY_DOWN ) == GLFW_PRESS ) mb_set->im_centre -= mb_set->im_range * MANDELBROT_MOVE_STEP;

    /* if R, reset to defaults */
    if ( glh_get_key ( window, GLFW_KEY_R ) == GLFW_PRESS )
    {
        mb_set->re_min_range = MBDEF_RE_MIN_RANGE;
        mb_set->im_min_range = MBDEF_IM_MIN_RANGE;
        mb_set->re_centre = MBDEF_RE_CENTRE;
        mb_set->im_centre = MBDEF_IM_CENTRE;
        mb_set->breakout = MBDEF_BREAKOUT;
        mb_set->max_it = MBDEF_MAX_IT;
        mb_set->power = MBDEF_POWER;
        mb_set->rotation = MBDEF_ROTATION;
        scroll_track = 0.0;
    }
}



/* MAIN */

int main ()
{
    /* init glfw and create window */
    glh_init_glfw ();
    glh_window_t window = glh_create_window ( "Test Window", 800, 600 );

    /* set key callback */
    glh_set_key_callback ( window, mandelbrot_key_callback );

    /* set drag and scroll callbacks */
    glh_cursor_t drag_cursor = glh_create_standard_cursor ( GLFW_HAND_CURSOR );
    glh_set_mouse_drag_calback ( window, drag_cursor, mandelbrot_drag_callback );
    glh_set_scroll_callback ( window, mandelbrot_scroll_callback );

    /* construct mandelbrot set from defaults */
    mb_set_t mb_set = mb_create_set ( MBDEF_RE_MIN_RANGE, MBDEF_IM_MIN_RANGE,
                                      MBDEF_RE_CENTRE,    MBDEF_IM_CENTRE,
                                      MBDEF_BREAKOUT,     MBDEF_MAX_IT );

    /* set the global variable to the pointer to the set */
    mb_set_ptr = ( size_t ) mb_set;

    /* if successfully created set */                                
    if ( mb_set )
    {
        /* while window should not close, draw set */
        while ( !glh_should_window_close ( window ) ) 
        {
            /* draw set */
            mb_draw ( mb_set, window );
        
            /* wait infinitely for events */
            glh_wait_events ( 0.0f );
        }

        /* destroy set */
        mb_destroy_set ( mb_set );
    }

    /* destroy cursor */
    glh_destroy_cursor ( drag_cursor );

    /* cleanup window and glfw */
    glh_destroy_window ( window );
    glh_terminate_glfw ();

    /* return 0 */
    return 0;
}