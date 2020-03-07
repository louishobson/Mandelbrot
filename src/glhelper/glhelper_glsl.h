/*
 * glhelper_glsl.h
 * 
 * includes headers and declares structures and functions to abstract creating and implementing glsl shaders
 * 
 */



/* pragma one */
#ifndef GLHELPER_GLSL_H_INCLUDED
#define GLHELPER_GLSL_H_INCLUDED



/* INCLUDES */

/* include general headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* to stat shader files */
#include <sys/types.h>
#include <sys/stat.h>

/* include glhelper_core.h */
#include "glhelper_core.h"

/* include glhelper_glad.h */
#include "glhelper_glad.h"



/* GLOBAL FLAGS AND MACROS */

/* GLH_GLSL_SHADER_LOG_SIZE
 *
 * defines the size of the log message when an error occurs generating a shader program
 */
#define GLH_GLSL_SHADER_LOG_SIZE 512

/* GLH_GLSL_VERTEX/GEOMETRY/FRAGMENT_SHADER
 * 
 * macros for the GL shader types
 */
#define GLH_GLSL_VERTEX_SHADER GL_VERTEX_SHADER
#define GLH_GLSL_GEOMETRY_SHADER GL_GEOMETRY_SHADER
#define GLH_GLSL_FRAGMENT_SHADER GL_FRAGMENT_SHADER


/* FUNCTIONS */

/* glh_import_shader
 *
 * reads the source of an external shader file
 * 
 * path: path to the shader
 * 
 * return: non-NULL on success, NULL on failure
 *         REMEMBER TO FREE THE RETURN STRING TO AVOID MEMORY LEAK
 */
char * glh_import_shader ( const char * path );

/* glh_create_shader
 *
 * function to compile a shader object
 * 
 * shader_source: source code for the shader
 * shader_type: type of shader (GLH_GLSL_VERTEX/GEOMETRY/FRAGMENT_SHADER)
 * 
 * return: shader ID >= 0 for success, -1 for failure
 */
glh_object_t glh_create_shader ( const char * shader_source, const glh_type_t shader_type );

/* glh_create_shader_form_path
 * 
 * function to create a shader object from a path, rather than its source
 * 
 * shader_path: path to the shader
 * shader_type: type of shader (GLH_GLSL_VERTEX/GEOMETRY/FRAGMENT_SHADER)
 * 
 * return: shader ID >= 0 for success, -1 for failure
 */
glh_object_t glh_create_shader_from_path ( const char * shader_path, const int shader_type );

/* glh_delete_shader
 *
 * deletes a shader
 * note that any program the shader is attached to will still function
 * 
 * shader: shader to delete
 * 
 * return: 0 for success, -1 for failure
 */
int glh_delete_shader ( const glh_object_t shader );

/* glh_create_shader_program
 *
 * function to create a shader program
 * 
 * vertex/geometry/fragment_shader: shader objects, or -1 for no user-defined shader
 * 
 * return: shader program ID >= 0 for success, -1 for failure
 */
glh_object_t glh_create_shader_program ( const glh_object_t vertex_shader, const glh_object_t geometry_shader, const glh_object_t fragment_shader  );

/* glh_delete_shader_program
 *
 * function to delete a shader program
 * 
 * shader_program: shader program to delete
 * 
 * return: 0 for success, -1 for failure
 */
int glh_delete_shader_program ( const glh_object_t shader_program );

/* glh_use_shader_program
 *
 * set a shader program to be used for the graphics pipeline
 * 
 * shader_program: shader program to set to be used
 * 
 * return: 0 for success, -1 for failure
 */
int glh_use_shader_program ( const glh_object_t shader_program );



/* #ifndef GLHELPER_GLSL_H_INCLUDED */
#endif