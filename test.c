/*
 * test.c
 * 
 * test parts of the system
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <mb_handler/mb_handler.h>

int main ()
{
    glh_init_glfw ();

    glh_window_t window = glh_create_window ( "Test Window", 800, 600 );

    mb_set_t mb_set = mb_create_set ( 5, 5, 0, 0, 2, 40, 2 );

    if ( mb_set )
    {
        while ( !glh_should_window_close ( window ) ) 
        {
            mb_draw ( mb_set, window );
        
            glh_wait_events ( 0.0f );

            if ( glh_get_key ( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) glh_set_window_should_close ( window );
            if ( glh_get_key ( window, GLFW_KEY_EQUAL ) == GLFW_PRESS )
            {
                mb_set->re_min_range *= 0.925;
                mb_set->im_min_range *= 0.925;
                mb_set->max_it *= 1.0325;
            }
            if ( glh_get_key ( window, GLFW_KEY_MINUS ) == GLFW_PRESS )
            {
                mb_set->re_min_range /= 0.92;
                mb_set->im_min_range /= 0.925;
                mb_set->max_it /= 1.0325;
            }
            if ( glh_get_key ( window, GLFW_KEY_LEFT ) == GLFW_PRESS ) mb_set->re_centre -= ( mb_set->re_range * 0.075 );
            if ( glh_get_key ( window, GLFW_KEY_RIGHT ) == GLFW_PRESS ) mb_set->re_centre += ( mb_set->re_range * 0.075 );
            if ( glh_get_key ( window, GLFW_KEY_UP ) == GLFW_PRESS ) mb_set->im_centre += ( mb_set->im_range * 0.075 );
            if ( glh_get_key ( window, GLFW_KEY_DOWN ) == GLFW_PRESS ) mb_set->im_centre -= ( mb_set->im_range * 0.075 );
            
            if ( glh_get_key ( window, GLFW_KEY_W ) == GLFW_PRESS ) mb_set->power += 1;
            if ( glh_get_key ( window, GLFW_KEY_Q ) == GLFW_PRESS ) mb_set->power -= 1;

            if ( glh_get_key ( window, GLFW_KEY_R ) == GLFW_PRESS )
            {
                mb_set->re_min_range = 5;
                mb_set->im_min_range = 5;
                mb_set->re_centre = 0;
                mb_set->im_centre = 0;
                mb_set->breakout = 2;
                mb_set->max_it = 40;
                mb_set->power = 2;
            }
            
        }

        mb_destroy_set ( mb_set );
    }

    glh_destroy_window ( window );

    glh_terminate_glfw ();

    return 0;
}