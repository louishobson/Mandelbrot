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



/* TYPEDEFS */

/* typedef glh_key_callback_t -> GLFWkeyfun */
typedef GLFWkeyfun glh_key_callback_t;



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

/* glh_get_key
 *
 * get the state of a given key for a window
 * 
 * return: the status of the key >= 0 for success, -1 for failure
 */
int glh_get_key ( glh_window_t window, const int key );  



/* #ifndef GLHELPER_INPUT_H_INCLUDED */
#endif