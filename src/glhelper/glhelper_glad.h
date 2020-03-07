/*
 * glhelper_glad.h
 * 
 * functions to initialise glad
 * 
 */



/* pragma one */
#ifndef GLHELPER_GLAD_H_INCLUDED
#define GLHELPER_GLAD_H_INCLUDED



/* INCLUDES */

/* include general headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* include glhelper_core.h */
#include "glhelper_core.h"



/* GLOBAL FLAGS AND MACROS */

/* volatile int GLH_GLAD_INIT_STATE
 *
 * will be zero or one to define if glad has been initialised
 * once glad has been initialised, it can't be uninitialised
 * therefore when this variable is one, it will never change back to zero
 */
extern volatile int GLH_GLAD_INIT_STATE;

/* __GLH_GLAD_INIT_CHECK__
 * 
 * substitiution macro to check if glfw/glad is initiated
 * 
 * des_state: desired state (0 or 1)
 * rt (any): variable to return
 * ...: parameters to pass to fprintf
 * 
 */
#define __GLH_GLAD_INIT_CHECK__(des_state, rt, ...) \
        if ( GLH_GLAD_INIT_STATE != des_state ) { fprintf ( stderr, __VA_ARGS__ ); return rt; };



/* FUNCTIONS */

/* glh_load_glad
 *
 * load glad to current context
 * 
 * return: 0 on success, and -1 on failure
 */
int glh_load_glad ();



/* #ifndef GLHELPER_GLAD_H_INCLUDED */
#endif