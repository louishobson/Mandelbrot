/*
 * glhelper_draw.c
 * 
 * implementation of glhelper_draw.h
 * 
 */



/* include glhelper_draw.h */
#include "glhelper_draw.h"



/* FUNCTION IMPLEMENTATIONS */

/* glh_set_clear_color
 *
 * sets the color to use to clear the screen
 * 
 * r/g/b/a: rgba values from 0.0-1.0
 * 
 * return: 0 for success, -1 for failure
 */
int glh_set_clear_color ( const float r, const float g, const float b, const float a )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before setting the clear color\n" );

    /* set clear color */
    glClearColor ( r, g, b, a );

    /* return 0 for success */
    return 0;
}

/* glh_clear_screen
 *
 * clears the screen using the last set clear color
 * 
 * return: 0 for success, -1 for failure
 */
int glh_clear_screen ()
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before clearing the screen\n" );

    /* clear screen */
    glClear ( GL_COLOR_BUFFER_BIT );

    /* return 0 for success */
    return 0;
}

/* glh_draw_arrays
 *
 * draws vertices from a vao
 * ignores any data in the ebo
 * 
 * vao: the vao to draw from
 * mode: the primative to render (see macros section)
 * start_index: index of the array to start on
 * count: number of vertices to draw
 *
 * return: 0 for success, -1 for failure
 */
int glh_draw_arrays ( const glh_object_t vao, const glh_type_t mode, const int start_index, const size_t count )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before drawing arrays\n" );

    /* bind vao */
    glBindVertexArray ( vao );

    /* draw arrays */
    glDrawArrays ( mode, start_index, count );

    /* unbind vao */
    glBindVertexArray ( 0 );

    /* return 0 for success */
    return 0;
}

/* glh_draw_elements
 *
 * draws vertices from a vao
 * uses ebo data
 * 
 * vao: the vao to draw from
 * mode: the primative to render (see macros section)
 * count: number indices to draw
 * type: the type of the data in the ebo
 *
 * return: 0 for success, -1 for failure
 */
int glh_draw_elements ( const glh_object_t vao, const glh_type_t mode, const size_t count, const glh_type_t type )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before drawing arrays\n" );

    /* bind vao */
    glBindVertexArray ( vao );

    /* draw elements */
    glDrawElements ( mode, count, type, 0 );

    /* unbind vao */
    glBindVertexArray ( 0 );

    /* return 0 for success */
    return 0;
}