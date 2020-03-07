/*
 * glhelper_draw.h
 * 
 * includes headers and declares structures and functions to abstract OpenGL screen-drawing functionality
 * 
 */



/* pragma one */
#ifndef GLHELPER_DRAW_H_INCLUDED
#define GLHELPER_DRAW_H_INCLUDED



/* INCLUDES */

/* include general headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* include glhelper_core.h */
#include "glhelper_core.h"

/* include glhelper_glad.h */
#include "glhelper_glad.h"

/* include glhelper_buff.h */
#include "glhelper_buff.h"



/* GLOBAL FLAGS AND MACROS */

/* GLH_DRAWMODE_POINTS/LINE_STRIP/LINE_LOOP/LINES/TRIANGLE_STRIP/TRIANGLE_FAN/TRIANGLES/QUAD_STRIP/QUADS/POLYGON
 *
 * drawmodes for glh rendering
 */
#define GLH_DRAWMODE_POINTS GL_POINTS
#define GLH_DRAWMODE_LINE_STRIP GL_LINE_STRIP
#define GLH_DRAWMODE_LINE_LOOP GL_LINE_LOOP
#define GLH_DRAWMODE_LINES GL_LINES
#define GLH_DRAWMODE_TRIANGLE_STRIP GL_TRIANGLE_STRIP
#define GLH_DRAWMODE_TRIANGLE_FAN GL_TRIANGLE_FAN
#define GLH_DRAWMODE_TRIANGLES GL_TRIANGLES
#define GLH_DRAWMODE_QUAD_STRIP GL_QUAD_STRIP
#define GLH_DRAWMODE_QUADS GL_QUADS
#define GLH_DRAWMODE_POLYGON GL_POLYGON



/* FUNCTIONS */

/* glh_set_clear_color
 *
 * sets the color to use to clear the screen
 * 
 * r/g/b/a: rgba values from 0.0-1.0
 * 
 * return: 0 for success, -1 for failure
 */
int glh_set_clear_color ( const float r, const float g, const float b, const float a );

/* glh_clear_screen
 *
 * clears the screen using the last set clear color
 * 
 * return: 0 for success, -1 for failure
 */
int glh_clear_screen ();

/* glh_draw_arrays
 *
 * draws vertices from a vao
 * ignores any data in the ebo
 * 
 * window: the window to draw on
 * vao: the vao to draw from
 * mode: the primative to render (see macros section)
 * start_index: index of the array to start on
 * count: number of vertices to draw
 *
 * return: 0 for success, -1 for failure
 */
int glh_draw_arrays ( const glh_object_t vao, const glh_type_t mode, const int start_index, const size_t count );

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
int glh_draw_elements ( const glh_object_t vao, const glh_type_t mode, const size_t count, const glh_type_t type );



/* #ifndef GLHELPER_DRAW_H_INCLUDED */
#endif