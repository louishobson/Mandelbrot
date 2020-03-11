/*
 * glhelper_input.c
 * 
 * implementation of glhelper_input.h
 * 
 */



/* include glhelper_input.h */
#include "glhelper_input.h"



/* GLOBAL FLAGS AND MACROS */

/* volatile int GLH_DRAG_ACTIVE
 * 
 * will be zero or one to define if drag mode is active
 * is used by glh_set_mouse_drag_callback to determine when a drag event is occuring
 */
volatile int GLH_DRAG_ACTIVE = 0;

/* volatile size_t GLH_DRAG_STYLE
 *
 * stores a pointer to a cursor object to change the cursor to when dragging is occuring
 */
volatile size_t GLH_DRAG_STYLE = 0;

/* volatile size_t GLH_DRAG_CALLBACK
 *
 * stores a pointer to the current drag callback
 */
volatile size_t GLH_DRAG_CALLBACK = 0;



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

/* glh_set_mouse_move_callback
 *
 * set a callback for when the mouse is moved within the window
 * 
 * window: the window to apply the callback to
 * callback: the callback to run on mouse move (may be NULL to remove the callback)
 *           void ( glh_window_t window, double xpos, double ypos )
 *
 * return: 0 for success, -1 for failure
 */
int glh_set_mouse_move_callback ( glh_window_t window, glh_mouse_move_callback_t callback )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before setting a mouse move callback function\n" );

    /* set the callback function */
    glfwSetCursorPosCallback ( window, callback );

    /* return 0 for success */
    return 0;
}

/* glh_set_mouse_drag_callback
 *
 * set a callback for dragging on te screen
 * this is where the left mouse button is held while moving the mouse
 * NOTE: this will override any mouse move callbacks previously set
 *
 * window: the window to apply the callback to
 * cursor_style: the style of the cursor when dragging is in process
 * callback: the callback to run on mouse drag (may be NULL to remove the callback)
 *           void ( glh_window_t window, double xdrag, double ydrag )
 *
 * return: 0 for success, -1 for failure
 */
int glh_set_mouse_drag_calback ( glh_window_t window, glh_cursor_t cursor_style, glh_mouse_drag_callback_t callback )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before setting a mouse drag callback function\n" );

    /* if callback is not NULL */
    if ( callback )
    {
        /* set globals */
        GLH_DRAG_ACTIVE = 0;
        GLH_DRAG_STYLE = ( size_t ) cursor_style;
        GLH_DRAG_CALLBACK = ( size_t ) callback;
    
        /* set other callbacks */
        glh_set_mouse_move_callback ( window, __glh_mouse_drag_move_callback );
        glh_set_mouse_button_callback ( window, __glh_mouse_drag_button_callback );
    } else
    /* else */
    {
        /* unset globals */
        GLH_DRAG_ACTIVE = 0;
        GLH_DRAG_STYLE = 0;
        GLH_DRAG_CALLBACK = 0;

        /* unset other callbacks */
        glh_set_mouse_move_callback ( window, NULL );
        glh_set_mouse_button_callback ( window, NULL );

        /* set cursor to default */
        glh_set_cursor ( window, NULL );
    }

    /* return 0 for success */
    return 0;   
}

void __glh_mouse_drag_move_callback ( glh_window_t window, const double xpos, const double ypos )
{
    /* static variables for previous positions */
    static double prev_xpos = 0;
    static double prev_ypos = 0;

    /* if drag not active, set the previous positions and return */
    if ( GLH_DRAG_ACTIVE == 0 )
    {
        prev_xpos = xpos;
        prev_ypos = ypos;
        return;
    }

    /* else calculate the xdrag and ydrag */
    const double xdrag = xpos - prev_xpos;
    const double ydrag = ypos - prev_ypos;

    /* set the previous positions */
    prev_xpos = xpos;
    prev_ypos = ypos;

    /* call drag callback, if not NULL */
    if ( GLH_DRAG_CALLBACK > 0 ) ( * ( glh_mouse_drag_callback_t ) GLH_DRAG_CALLBACK ) ( window, xdrag, ydrag );
}

void __glh_mouse_drag_button_callback ( glh_window_t window, const int button, const int action, const int mods )
{
    /* check for left mouse button */
    if ( button == GLFW_MOUSE_BUTTON_LEFT )
    {
        /* check for press */
        if ( action == GLFW_PRESS )
        {
            /* set drag to on */
            GLH_DRAG_ACTIVE = 1;

            /* change cursor to drag cursor */
            glh_set_cursor ( window, ( glh_cursor_t ) GLH_DRAG_STYLE );
        } else
        /* else */
        {
            /* set drag to off */
            GLH_DRAG_ACTIVE = 0;

            /* change cursor to default */
            glh_set_cursor ( window, NULL );
        }
    }
}

/* glh_set_mouse_button_callback
 *
 * set a callback for pressing buttons on the mouse
 * 
 * window: window to apply the callback to
 * callback: the callback to run on mouse button press (may be NULL to remove the callback)
 *           void ( glh_window_t window, int button, int action, int mods )
 *
 * return: 0 for success, -1 for failure
 */
int glh_set_mouse_button_callback ( glh_window_t window, glh_mouse_button_callback_t callback )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before setting a mouse button callback function\n" );

    /* set the callback function */
    glfwSetMouseButtonCallback ( window, callback );

    /* return 0 for success */
    return 0;
}

/* glh_set_scroll_callback
 *
 * set a callback for scrolling
 * 
 * window: window to apply the callback to
 * callback: the callback to run on scroll input (may be NULL to remove the callback)
 *           void ( glh_window_t window, double xoffset, double yoffset )
 *
 * return: 0 for success, -1 for failure
 */
int glh_set_scroll_callback ( glh_window_t window, glh_scroll_callback_t callback )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before setting a scroll callback function\n" );

    /* set the callback function */
    glfwSetScrollCallback ( window, callback );

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

/* glh_get_cursor_pos
 *
 * get the position of the cursor for a given window
 * 
 * window: the window query the mouse position from
 * x,ypos: will be set to the current positions of the cursor
 * 
 * return: 0 for success, -1 for failure
 */
int glh_get_cursor_pos ( glh_window_t window, double * xpos, double * ypos )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before getting the mouse position\n" );

    /* get the position */
    glfwGetCursorPos ( window, xpos, ypos );

    /* return 0 for success */
    return 0;
}

/* glh_get_mouse_button
 *
 * get the state of a mouse button
 * 
 * window: the window to query the button from
 * button: the button to query
 * 
 * return: the status of the key >= 0 for success, -1 for failure
 */
int glh_get_mouse_button ( glh_window_t window, const int button )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before getting the mouse button states\n" );

    /* get the state and return it */
    return glfwGetMouseButton ( window, button );
}

/* glh_create_standard_cursor
 *
 * create a standard cursor based on the current system cursor theme
 * 
 * type: type of the cursor style
 * 
 * return: glh_cursor_t != NULL on success, == NULL on failure
 */
glh_cursor_t glh_create_standard_cursor ( const int type )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, NULL, "GLH ERROR: glfw must be initialised before creating a standard cursor\n" );

    /* create and return a cursor */
    return glfwCreateStandardCursor ( type );
}

/* glh_destroy_cursor
 *
 * destroy a cursor
 * NOTE: any windows using the cursor will be set to their default cursor
 * 
 * cursor: the cursor to destroy
 *
 * return: 0 for success, -1 for failure
 */
int glh_destroy_cursor ( glh_cursor_t cursor )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before destroying a cursor\n" );

    /* destroy the cursor */
    glfwDestroyCursor ( cursor );

    /* return 0 for success */
    return 0; 
}

/* glh_set_cursor
 *
 * set the window's cursor
 * 
 * window: the window to apply the change to
 * cursor: the cursor the window should use (or can be NULL to use default)
 * 
 * return: 0 for success, -1 for failure
 */
int glh_set_cursor ( glh_window_t window, glh_cursor_t cursor )
{
    /* check glfw is initialised */
    __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before setting the cursor\n" );

    /* set the cursor for the current window */
    glfwSetCursor ( window, cursor );

    /* return 0 for success */
    return 0;
}