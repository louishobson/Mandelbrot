/*
 * glhelper_input.h
 * 
 * includes headers and declares structures and functions to abstract glfw input functionality
 *
 */



/* pragma one */
#ifndef GLHELPER_INPUT_H_INCLUDED
#define GLHELPER_INPUT_H_INCLUDED



/* INCLUDES */

/* include general headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* include glhelper_core.h */
#include "glhelper_core.h"

/* include glhelper_glfw.h */
#include "glhelper_glfw.h"



/* GLOBAL FLAGS AND MACROS */

/* volatile int GLH_DRAG_ACTIVE
 * 
 * will be zero or one to define if drag mode is active
 * is used by glh_set_mouse_drag_callback to determine when a drag event is occuring
 */
extern volatile int GLH_DRAG_ACTIVE;

/* volatile size_t GLH_DRAG_STYLE
 *
 * stores a pointer to a cursor object to change the cursor to when dragging is occuring
 */
extern volatile size_t GLH_DRAG_STYLE;



/* TYPEDEFS */

/* typedef glh_cursor_t -> GLFWcursor * */
typedef GLFWcursor * glh_cursor_t;

/* typedef glh_key_callback_t -> GLFWkeyfun */
typedef GLFWkeyfun glh_key_callback_t;

/* typedef glh_mouse_move_callback_t -> GLFWcursorposfun */
typedef GLFWcursorposfun glh_mouse_move_callback_t;

/* typedef glh_mouse_drag_callback_t -> void ( * glh_mouse_drag_callback_t ) ( glh_window_t, double, double ) */
typedef void ( * glh_mouse_drag_callback_t ) ( glh_window_t, double, double );

/* typedef glh_mouse_button_callback_t -> GLFWmousebuttonfun */
typedef GLFWmousebuttonfun glh_mouse_button_callback_t;

/* typedef glh_scroll_callback_t -> GLFWscrollfun */
typedef GLFWscrollfun glh_scroll_callback_t;



/* FUNCTIONS */

/* glh_poll_events
 *
 * poll events
 * gets a snapshot of the events having occured since the last poll
 * 
 * return: 0 for success, -1 for failure
 */
int glh_poll_events ();

/* glh_wait_events
 *
 * wait for events
 * sleep until an event has occured since the last poll or wait
 * 
 * timeout: timout for wait (or 0 for no timeout)
 * 
 * return: 0 for success, -1 for failure
 */
int glh_wait_events ( const double timeout );

/* glh_post_empty_event
 *
 * post an empty event
 * 
 * return: 0 for success, -1 for failure
 */
int glh_post_empty_event ();

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
int glh_set_key_callback ( glh_window_t window, glh_key_callback_t callback );

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
int glh_set_mouse_move_callback ( glh_window_t window, glh_mouse_move_callback_t callback );

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
int glh_set_mouse_drag_calback ( glh_window_t window, glh_cursor_t cursor_style, glh_mouse_drag_callback_t callback );
void __glh_mouse_drag_move_callback ( glh_window_t window, const double xpos, const double ypos );
void __glh_mouse_drag_button_callback ( glh_window_t window, const int button, const int action, const int mods );

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
int glh_set_mouse_button_callback ( glh_window_t window, glh_mouse_button_callback_t callback );

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
int glh_set_scroll_callback ( glh_window_t window, glh_scroll_callback_t callback );

/* glh_get_key
 *
 * get the state of a given key for a window
 * 
 * window: the window to query the key from
 * key: keycode for the key to query
 * 
 * return: the status of the key >= 0 for success, -1 for failure
 */
int glh_get_key ( glh_window_t window, const int key );

/* glh_get_cursor_pos
 *
 * get the position of the cursor for a given window
 * 
 * window: the window query the mouse position from
 * x,ypos: will be set to the current positions of the cursor
 * 
 * return: 0 for success, -1 for failure
 */
int glh_get_cursor_pos ( glh_window_t window, double * xpos, double * ypos );

/* glh_get_mouse_button
 *
 * get the state of a mouse button
 * 
 * window: the window to query the button from
 * button: the button to query
 * 
 * return: the status of the key >= 0 for success, -1 for failure
 */
int glh_get_mouse_button ( glh_window_t window, const int button );

/* glh_create_standard_cursor
 *
 * create a standard cursor based on the current system cursor theme
 * 
 * type: type of the cursor style
 * 
 * return: glh_cursor_t != NULL on success, == NULL on failure
 */
glh_cursor_t glh_create_standard_cursor ( const int type );

/* glh_destroy_cursor
 *
 * destroy a cursor
 * NOTE: any windows using the cursor will be set to their default cursor
 * 
 * cursor: the cursor to destroy
 *
 * return: 0 for success, -1 for failure
 */
int glh_destroy_cursor ( glh_cursor_t cursor );

/* glh_set_cursor
 *
 * set the window's cursor
 * 
 * window: the window to apply the change to
 * cursor: the cursor the window should use (or can be NULL to use default)
 * 
 * return: 0 for success, -1 for failure
 */
int glh_set_cursor ( glh_window_t window, glh_cursor_t cursor );



/* #ifndef GLHELPER_INPUT_H_INCLUDED */
#endif