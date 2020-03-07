/*
 * glhelper_input.c
 * 
 * implementation of glhelper_input.h
 * 
 */



/* include glhelper_input.h */
#include "glhelper_input.h"



/* FUNCTION IMPLEMENTATIONS */

/* glh_poll_events
 *
 * poll events
 * gets a snapshot of the events having occured since the last poll
 * 
 * return: 0 for success, -1 for failure
 */
int glh_poll_events ()
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before polling events\n" );

    /* poll events */
    glfwPollEvents ();

    /* return 0 for success */
    return 0;
}

/* glh_wait_events
 *
 * wait for events
 * sleep until an event has occured since the last poll or wait
 * 
 * timeout: timout for wait (or 0 for no timeout)
 * 
 * return: 0 for success, -1 for failure
 */
int glh_wait_events ( const double timeout )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before waiting on events\n" );

    /* wait for events */
    if ( timeout == 0.0f ) glfwWaitEvents ();
    else glfwWaitEventsTimeout ( timeout );

    /* return 0 for success */
    return 0;
}

/* glh_post_empty_event
 *
 * post an empty event
 * 
 * return: 0 for success, -1 for failure
 */
int glh_post_empty_event ()
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before posting an empty event\n" );

    /* post empty event */
    glfwPostEmptyEvent ();

    /* return 0 for success */
    return 0;
}

/* glh_set_key_callback 
 *
 * set a callback for when a key is pressd
 * 
 * window: the window to apply the callback to
 * callback: the callback to run on a key press (may be NULL to remove the callback)
 *           void ( glh_window_t window, int key, int scancode, int action, int mods )
 * 
 * return: 0 for success, -1 for failure
 */
int glh_set_key_callback ( glh_window_t window, glh_key_callback_t callback )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before setting a key callback function\n" );

    /* set the callback function */
    glfwSetKeyCallback ( window, callback );

    /* return 0 for success */
    return 0;
}

/* glh_get_key
 *
 * get the state of a given key for a window
 * 
 * return: the status of the key >= 0 for success, -1 for failure
 */
int glh_get_key ( glh_window_t window, const int key )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before getting a key's state\n" );

    /* return the state */
    return glfwGetKey ( window, key );
}