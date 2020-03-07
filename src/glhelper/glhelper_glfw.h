/*
 * glhelper_glfw.h
 * 
 * includes headers and declares structures and functions to abstract glfw functionality
 * 
 */



/* pragma one */
#ifndef GLHELPER_GLFW_H_INCLUDED
#define GLHELPER_GLFW_H_INCLUDED



/* INCLUDES */

/* include general headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* include glhelper_core.h */
#include "glhelper_core.h"

/* include glhelper_glad.h */
#include "glhelper_glad.h"



/* GLOBAL FLAGS AND MACROS */

/* volatile int GLH_GLFW_INIT_STATE
 *
 * will be zero or one to define if glfw is initialised
 */
extern volatile int GLH_GLFW_INIT_STATE;

/* volatile size_t GLH_GLFW_CURRENT
 *
 * stores a casted pointer of the window currently loaded into glad, or 0 for not loaded
 */
extern volatile size_t GLH_GLFW_CURRENT;

/* __GLH_GLFW_INIT_CHECK__
 * 
 * substitiution macro to check if glfw/glad is initiated
 * 
 * des_state: desired state (0 or 1)
 * rt (any): variable to return
 * ...: parameters to pass to fprintf
 * 
 */
#define __GLH_GLFW_INIT_CHECK__(des_state, rt, ...) \
        if ( GLH_GLFW_INIT_STATE != des_state ) { fprintf ( stderr, __VA_ARGS__ ); return rt; };



/* TYPEDEFS */

/* typedef glh_window_t -> GLFWwindow * */
typedef GLFWwindow * glh_window_t;

/* typedef glh_viewport_size_callback_t -> GLFWframebuffersizefun */
typedef GLFWframebuffersizefun glh_viewport_size_callback_t;


/* FUNCTIONS */

/* glh_init_glfw
 *
 * intiialise glfw, setting GLH_GLFW_INIT_STATE to 1
 * 
 * return: 0 on success, -1 on failure
 */
int glh_init_glfw ();

/* glh_terminate_glfw
 *
 * terminate glfw, setting GLH_GLFW_INIT_STATE to 0
 * 
 * return: 0 on success, -1 on failure
 */
int glh_terminate_glfw ();

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
glh_window_t glh_create_window ( const char * title, const int width, const int height );

/* glh_destroy_window
 *
 * destroys a glh_window_t and terminates glfw
 * 
 * window: the window to terminate
 * 
 * return: 0 for success, -1 for failure
 */
 int glh_destroy_window ( glh_window_t window );

 /* glh_make_window_current
  *
  * make a window the current window for OpenGL
  * quite expensive so don't over-use
  * 
  * window: the window to set as the curren t OpenGL context
  *
  * return: 0 on success, -1 on failure
  */
int glh_make_window_current ( glh_window_t window );

/* glh_set_window_size
 *
 * function to alter size of window
 * 
 * window: pointer to a window object
 * width/height: the new width and height
 * 
 * return: 0 for success, -1 for failure
 */
int glh_set_window_size ( glh_window_t window, const int width, const int height );

/* glh_set_viewport_size_callback
 *
 * function to set the callback to to resize the viewport when the window size is altered
 * 
 * window: the window to set the callback for
 * callback: function pointer to the callback (__glh_set_viewport_size is a good choice)
 *
 * return: 0 for success, -1 for failure
 */
int glh_set_viewport_size_callback ( glh_window_t window, glh_viewport_size_callback_t callback );

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
int glh_set_viewport_size ( glh_window_t window, const int xscale, const int yscale );
void __glh_set_viewport_size ( glh_window_t window, const int xscale, const int yscale );

/* glh_get_viewport_size
 *
 * function to get the size of the viewport
 * 
 * window: window to get the viewport size of
 * viewport_size: pointer to an array of 4 integers for xpos,ypos,width,height
 * 
 * return: 0 for success, -1 for failure
 */
int glh_get_viewport_size ( glh_window_t window, int * viewport_size ); 

/* glh_should_window_close
 *
 * function to determine if window should close
 * 
 * window: the window to test
 * 
 * return: boolean, true for close, false for no close flag set
 */
int glh_should_window_close ( glh_window_t window );

/* glh_set_window_should_close
 *
 * function to set the close flag on a window
 * 
 * window: window to set the close flag on
 * 
 * return: 0 for success, -1 for failure
 */
int glh_set_window_should_close ( glh_window_t window );

/* glh_swap_buffers
 *
 * swap the buffers for a window
 * 
 * window: the window to swap the buffers for
 * 
 * return: 0 for success, -1 for failure
 */
int glh_swap_buffers ( glh_window_t window );



/* ifndef GLHELPER_H_INCLUDED */
#endif