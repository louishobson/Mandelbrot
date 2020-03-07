/*
 * glhelper_glfw.c
 * 
 * implementation of glhelper_glfw.h
 * 
 */



/* include glhelper.h */
#include "glhelper_glfw.h"



/* GLOBAL FLAGS */

/* int GLH_GLFW_INIT_STATE
 *
 * will be zero or one to define if glfw is initialised
 */
volatile int GLH_GLFW_INIT_STATE = 0;

/* volatile size_t GLH_GLFW_CURRENT
 *
 * stores a casted pointer of the window currently loaded into glad, or 0 for not loaded
 */
volatile size_t GLH_GLFW_CURRENT = 0;



/* FUNCTION IMPLEMENTATIONS */

/* glh_init_glfw
 *
 * initialise glfw, setting GLH_GLFW_INIT_STATE to 1
 * 
 * return: 0 on success, -1 on failure
 */
int glh_init_glfw ()
{
    /* check init flag */
    __GLH_GLFW_INIT_CHECK__ ( 0, 0, "GLH WARN: glfw cannot be reinitialised\n" );

    /* init glfw */
    glfwInit ();

    /* set version to 3, and only use core OpenGL functionality */
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    /* set init flag to 1 */
    GLH_GLFW_INIT_STATE = 1;

    /* return 0 for success */
    return 0;
}

/* glh_terminate_glfw
 *
 * terminate glfw, setting GLH_GLFW_INIT_STATE to 0
 * 
 * return: 0 on success, -1 on failure
 */
int glh_terminate_glfw ()
{
    /* check init flag */
    __GLH_GLFW_INIT_CHECK__ ( 1, 0, "GLH WARN: glfw must be initialised before it can be terminated\n" );

    /* terminate glfw */
    glfwTerminate ();

    /* set flag to 0 */
    GLH_GLFW_INIT_STATE = 0;

    /* return 0 for success */
    return 0;
}

/* glh_create_window
 * 
 * creates a glh_window_t using glfw
 * 
 * title: title to give the window
 * width/height: the initial width/height of the window in pixels
 * 
 * returns: glh_window_t object
 *          glh_window_t is non-NULL on success, or will be NULL on failure
 */
glh_window_t glh_create_window ( const char * title, const int width, const int height )
{
    /* window object with window set to NULL */
    glh_window_t window = NULL;

    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, window, "GLH ERROR: glfw must be initialised before creating window\n" );

    /* attempt to create the window */
    window = glfwCreateWindow ( width, height, title, NULL, NULL );

    /* check success */
    if ( !window )
    {
        /* failed to create window, so produce error */
        fprintf ( stderr, "GLH ERROR: failed to create window\n" );
        return window;
    }

    /* make window current */
    if ( glh_make_window_current ( window ) < 0 )
    {
        /* failed, so close window and return error window */
        glh_destroy_window ( window );
        window = NULL;
        return window;
    }

    /* set current viewport size */
    glh_set_viewport_size ( window, width, height );

    /* set the viewport callback */ 
    glh_set_viewport_size_callback ( window, __glh_set_viewport_size );

    /* clear the viewport */
    glClear ( GL_COLOR_BUFFER_BIT );

    /* finally swap the buffer */
    glfwSwapBuffers ( window );

    /* return the window */
    return window;
}

/* glh_destroy_window
 *
 * destroys a glh_window_t and terminates glfw
 * 
 * window: the window to terminate
 * 
 * return: 0 for success, -1 for failure
 */
 int glh_destroy_window ( glh_window_t window )
 {
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before destroying a window\n" );

     /* destroy window */
     glfwDestroyWindow ( window );
 }

  /* glh_make_window_current
  *
  * make a window the current window for OpenGL
  * quite expensive so don't over-use
  * 
  * window: the window to set as the curren t OpenGL context
  *
  * return: 0 on success, -1 on failure
  */
int glh_make_window_current ( glh_window_t window )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before making a window current\n" );

    /* make context current */
    glfwMakeContextCurrent ( window );

    /* reload glad, if necessary */
    if ( GLH_GLFW_CURRENT != ( size_t ) window )
    {
        if ( glh_load_glad () < 0 )
        {
            /* failed, so return failure */
            return -1;
        }
    } else
    {
        /* print info that didn't reload glad */
        //fprintf ( stderr, "GLH INFO: did not reload glad for already current window\n" );
    }

    /* set GLH_GLFW_CURRENT flag */
    GLH_GLFW_CURRENT = ( size_t ) window;

    /* return 0 for success */
    return 0;
}

/* glh_set_window_size
 *
 * function to alter size of window
 * 
 * window: pointer to a window object
 * width/height: the new width and height
 * 
 * return: 0 for success, -1 for failure
 */
int glh_set_window_size ( glh_window_t window, const int width, const int height )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before setting window size\n" );
    
    /* alter the window size */
    glfwSetWindowSize ( window, width, height );

    /* return 0 */
    return 0;
}

/* glh_set_viewport_size_callback
 *
 * function to set the callback to to resize the viewport when the window size is altered
 * 
 * window: the window to set the callback for
 * callback: function pointer to the callback (__glh_set_viewport_size_callback is a good choice)
 *
 * return: 0 for success, -1 for failure
 */
int glh_set_viewport_size_callback ( glh_window_t window, glh_viewport_size_callback_t callback )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before setting viewport callback\n" );

    /* call the viewport resize callback on window size change */
    glfwSetFramebufferSizeCallback ( window, callback );

    /* return 0 for success */
    return 0;
}

/* glh_set_viewport_size and automatic callback
 *
 * function to alter size of window
 * the window supplied will be set as the current context
 * 
 * window: pointer to a window object
 * width/height: the new width and height
 * 
 * return: 0 for success, -1 for failure
 */
int glh_set_viewport_size ( glh_window_t window, const int xscale, const int yscale )
{
    /* check glfw and glad are initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised to change viewport size\n" );
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised to change viewport size\n" );

    /* set window to current context */
    glh_make_window_current ( window );

    /* alter the viewport size */
    glViewport ( 0, 0, xscale, yscale );

    /* return 0 for success */
    return 0;
}

void __glh_set_viewport_size ( glh_window_t window, const int xscale, const int yscale )
{
    /* call __glh_set_viewport_size */
    glh_set_viewport_size ( window, xscale, yscale );
}

/* glh_get_viewport_size
 *
 * function to get the size of the viewport
 * 
 * window: window to get the viewport size of
 * viewport_size: pointer to an array of 4 integers for xpos,ypos,width,height
 * 
 * return: 0 for success, -1 for failure
 */
int glh_get_viewport_size ( glh_window_t window, int * viewport_size )
{
    /* check glfw and glad are initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised to get viewport size\n" );
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised to get viewport size\n" );

    /* set window to current context */
    glh_make_window_current ( window );

    /* get viewport size */
    glGetIntegerv ( GL_VIEWPORT, viewport_size );

    /* return 0 for success */
    return 0;
}

/* glh_should_window_close
 *
 * function to determine if window should close
 * 
 * window: the window to test
 * 
 * return: 1 for close, 0 for no close flag set, -1 for failure
 */
int glh_should_window_close ( glh_window_t window )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised to check window close flag\n" );

    /* return window should close state */
    return glfwWindowShouldClose ( window );
}

/* glh_set_window_should_close
 *
 * function to set the close flag on a window
 * 
 * window: window to set the close flag on
 * 
 * return: 0 for success, -1 for failure
 */
int glh_set_window_should_close ( glh_window_t window )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised to set window close flag\n" );

    /* set close flag for window */
    glfwSetWindowShouldClose ( window, GLFW_TRUE );

    /* return 0 for success */
    return 0;
}

/* glh_swap_buffers
 *
 * swap the buffers for a window
 * 
 * window: the window to swap the buffers for
 * 
 * return: 0 for success, -1 for failure
 */
int glh_swap_buffers ( glh_window_t window )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before swapping buffers on window\n" );

    /* swap buffers */
    glfwSwapBuffers ( window );

    /* return 0 for success */
    return 0;
}