/*
 * mandelbrot.c
 * 
 * activate the window and drawing of the mandelbrot
 */



/* include general headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* include mb_handler */
#include <mb_handler/mb_handler.h>

/* main */
int main ()
{
    /* init glfw and create window */
    glh_init_glfw ();
    glh_window_t window = glh_create_window ( "Test Window", 800, 600 );

    /* construct mandelbrot set from defaults */
    mb_set_t mb_set = mb_create_set ( MBDEF_RE_MIN_RANGE, MBDEF_IM_MIN_RANGE,
                                      MBDEF_RE_CENTRE,    MBDEF_IM_CENTRE,
                                      MBDEF_BREAKOUT,     MBDEF_MAX_IT,
                                      MBDEF_POWER );

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

            /* if escape, set window should close */
            if ( glh_get_key ( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) glh_set_window_should_close ( window );
            
            /* if plus key, decrease visible range and increase max_it */
            if ( glh_get_key ( window, GLFW_KEY_EQUAL ) == GLFW_PRESS )
            {
                mb_set->re_min_range *= 0.925;
                mb_set->im_min_range *= 0.925;
                mb_set->max_it *= 1.0325;
            }

            /* if minus key, increase visible range and decrease max_it */
            if ( glh_get_key ( window, GLFW_KEY_MINUS ) == GLFW_PRESS )
            {
                mb_set->re_min_range /= 0.92;
                mb_set->im_min_range /= 0.925;
                mb_set->max_it /= 1.0325;
            }

            /* if arrow keys, increae/decrease the appropriate centre component by a multiple of the range */
            if ( glh_get_key ( window, GLFW_KEY_LEFT ) == GLFW_PRESS ) mb_set->re_centre -= ( mb_set->re_range * 0.075 );
            if ( glh_get_key ( window, GLFW_KEY_RIGHT ) == GLFW_PRESS ) mb_set->re_centre += ( mb_set->re_range * 0.075 );
            if ( glh_get_key ( window, GLFW_KEY_UP ) == GLFW_PRESS ) mb_set->im_centre += ( mb_set->im_range * 0.075 );
            if ( glh_get_key ( window, GLFW_KEY_DOWN ) == GLFW_PRESS ) mb_set->im_centre -= ( mb_set->im_range * 0.075 );
            
            /* if W/Q, increase/decrease the power */
            if ( glh_get_key ( window, GLFW_KEY_W ) == GLFW_PRESS ) mb_set->power += 1;
            if ( glh_get_key ( window, GLFW_KEY_Q ) == GLFW_PRESS ) mb_set->power -= 1;

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
            }
        }

        /* destroy set */
        mb_destroy_set ( mb_set );
    }

    /* cleanup window and glfw */
    glh_destroy_window ( window );
    glh_terminate_glfw ();

    /* return 0 */
    return 0;
}