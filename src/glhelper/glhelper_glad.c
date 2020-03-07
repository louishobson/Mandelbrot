/*
 * glhelper_glad.c
 * 
 * implementation of glhelper_glad.h
 *
 */



/* include glhelper_glad.h */
#include "glhelper_glad.h"



/* GLOBAL FLAGS */

/* volatile int GLH_GLAD_INIT_STATE
 *
 * will be zero or one to define if glad has been initialised
 * once glad has been initialised, it can't be uninitialised
 * therefore when this variable is one, it will never change back to zero
 */
volatile int GLH_GLAD_INIT_STATE = 0;



/* FUNCTION IMPLEMENATATIONS */

/* glh_load_glad
 *
 * load glad to current context, setting GLH_GLAD_INIT_STATE to window->window
 * 
 * return: 0 on success, and -1 on failure
 */
int glh_load_glad ()
{
    /* check glfw is initialised */
    /* __GLH_GLFW_INIT_CHECK__ ( 1, -1, "GLH ERROR: glfw must be initialised before loading glad to a context\n" ); */

    /* initialise glad */
    if ( !gladLoadGLLoader ( ( GLADloadproc ) glfwGetProcAddress ) )
    {
        /* initialisation failed, so produce error */
        fprintf ( stderr, "GLH ERROR: glad failed to initialise\n" );
        return -1;
    }

    /* ensure glad flag is set to 1 */
    GLH_GLAD_INIT_STATE = 1;

    /* return 0 for success */
    return 0;
}