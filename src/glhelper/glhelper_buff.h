/*
 * glhelper_buff.h
 * 
 * defines structures and functions to handle various OpenGL buffers
 * 
 */



/* pragma one */
#ifndef GLHELPER_BUFF_H_INCLUDED
#define GLHELPER_BUFF_H_INCLUDED



/* INCLUDES */

/* include general headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

/* include glhelper_core.h */
#include "glhelper_core.h"

/* include glhelper_glad.h */
#include "glhelper_glad.h"



/* GLOBAL FLAGS AND MACROS */

/* GLH_BUFF_STREAM/STATIC/DYNAMIC_DRAW
 *
 * macros for the vertex buffer object management schemes
 * 
 * stream: sent once, used a few times
 * static: sent once, used many times
 * dynamic: changed a lot, used many times
 */
#define GLH_BUFF_STREAM_DRAW GL_STREAM_DRAW
#define GLH_BUFF_STATIC_DRAW GL_STATIC_DRAW
#define GLH_BUFF_DYNAMIC_DRAW GL_DYNAMIC_DRAW



/* FUNCTIONS */

/* glh_create_vertex_array_object
 *
 * creates a vertex array object
 * 
 * return: vao ID >= 0 for success, -1 for failure
 */
glh_object_t glh_create_vertex_array_object ();

/* glh_delete_vertex_array_object
 *
 * deletes a vertex array object
 * 
 * vao: the vertex array object to delete
 * 
 * return: 0 for success, -1 for failure
 */
int glh_delete_vertex_array_object( const glh_object_t vao );

/* glh_create_vertex_buffer_object
 *
 * creates a vertex buffer object containing the given data
 * 
 * vertices: array of data (usually floats for vertices)
 * v_size: size of array (in bytes)
 * buff_type: the buffer type from GLH_BUFF_STREAM/STATIC/DYNAMIC_DRAW
 *
 * return: buffer ID >= 0 for success, -1 for failure
 */
glh_object_t glh_create_vertex_buffer_object ( const void * vertices, const size_t v_size, const glh_type_t buff_type );

/* glh_delete_vertex_buffer_object
 *
 * deletes a vertex buffer object
 * 
 * vbo: vertex buffer object to delete
 * 
 * return: 0 for success, -1 for failure
 */

int glh_delete_vertex_buffer_object ( const glh_object_t vbo );

/* glh_create_element_buffer_object
 * 
 * creates an element buffer object containing the given data
 * 
 * elements: array of elements
 * e_size: size of array (in bytes)
 * buff_type: the buffer type from GLH_BUFF_STREAM/STATIC/DYNAMIC_DRAW
 *
 * return: buffer ID >= 0 for success, -1 for failure
 */
glh_object_t glh_create_element_buffer_object ( const void * elements, const size_t e_size, const glh_type_t buff_type );

/* glh_delete_element_buffer_object
 *
 * deletes a element buffer object
 * 
 * ebo: element buffer object to delete
 * 
 * return: 0 for success, -1 for failure
 */
int glh_delete_element_buffer_object ( const glh_object_t ebo );

/* glh_bind_vertex_buffer_object
 *
 * how to handle attributes of vertex data
 * 
 * vao: vertex array object to configure
 * vbo: the vertex buffer object to bind to the attribute
 * index: vertex attribute index
 * size: 1, 2, 3 or 4 for vec1,2,3,4
 * type: type of each component of the vector data, e.g.:
 *       GLH_BUFF_FLOAT
 * stride: distance (bytes) between elements in the array for the attribute
 * offset: offset of the attribute in the array
 *         ( void * ) 0 represents no offset
 * 
 * return: 0 for success, -1 for failure
 */
int glh_bind_vertex_buffer_object ( const glh_object_t vao, 
                                    const glh_object_t vbo, 
                                    const int index, 
                                    const int size, 
                                    const glh_type_t type, 
                                    const size_t stride,
                                    const void * offset );

/* glh_bind_element_buffer_object
 *
 * bind an ebo to a vao
 * 
 * vao: the vao to bind the ebo to
 * ebo: the ebo to bind
 * 
 * return: 0 for success, -1 for failure
 */
int glh_bind_element_buffer_object ( const glh_object_t vao, const glh_object_t ebo );

/* glh_enable_vertex_attribute
 *
 * enables a vertex attribute
 * vao: vertex array object to configure
 * 
 * index: the index to enable
 * 
 * return: 0 for success, -1 for failure
 */
int glh_enable_vertex_attribute ( const glh_object_t vao, const int index );

/* glh_disable_vertex_attribute
 *
 * enables a vertex attribute
 * 
 * vao: vertex array object to configure
 * index: the index to disable
 * 
 * return: 0 for success, -1 for failure
 */
int glh_disable_vertex_attribute ( const glh_object_t vao, const int index );

/* glh_get_uniform_location
 *
 * aquires an object reference for a uniform within a shader program
 * 
 * shader_program: the shader program containing the uniform
 * uniform_name: the name of the uniform (string)
 * 
 * return: unform location >= 0 for succes, -1 for failure (including uniform not found)
 */
glh_object_t glh_get_uniform_location ( const glh_object_t shader_program, const char * uniform_name );

/* glh_set_uniform_vec1,2,3,4
 *
 * assigns a vector uniform to values
 * NOTE CHANGES THE UNIFORM IN THE CURRENTLY ACTIVE SHADER PROGRAM
 * 
 * uniform: location of the uniform (from glh_get_uniform_location)
 * ...: 1-4 further floats depending on the appended 1-4
 * 
 * return: 0 for success, -1 for failure
 */
int glh_set_uniform_vec1 ( const glh_object_t uniform, const float x );
int glh_set_uniform_vec2 ( const glh_object_t uniform, const float x, const float y );
int glh_set_uniform_vec3 ( const glh_object_t uniform, const float x, const float y, const float z );
int glh_set_uniform_vec4 ( const glh_object_t uniform, const float x, const float y, const float z, const float w );

/* glh_set_uniform_mat2
 *
 * assigned a 2x2 matrix to values
 * 
 * NOTE CHANGES THE UNIFORM IN THE CURRENTLY ACTIVE SHADER PROGRAM
 * 
 * uniform: location of the uniform (from glh_get_uniform_location)
 * transpose: boolean as to whether to transpose the matrix
 * values: array of floats of expected size: 4
 * 
 * return: 0 for success, -1 for failure
 */
int glh_set_uniform_mat2 ( const glh_object_t uniform, const int transpose, const float * values ); 

/* glh_set_uniform_float
 *
 * assignes a float uniform a value
 * NOTE CHANGES THE UNIFORM IN THE CURRENTLY ACTIVE SHADER PROGRAM
 * 
 * uniform: location of the uniform (from glh_get_uniform_location)
 * value: the value to give the uniform
 * 
 * return: 0 for success, -1 for failure
 */
int glh_set_uniform_float ( const glh_object_t uniform, const float value );

/* glh_set_uniform_int
 *
 * assignes an int uniform a value
 * NOTE CHANGES THE UNIFORM IN THE CURRENTLY ACTIVE SHADER PROGRAM
 * 
 * uniform: location of the uniform (from glh_get_uniform_location)
 * value: the value to give the uniform
 * 
 * return: 0 for success, -1 for failure
 */
int glh_set_uniform_int ( const glh_object_t uniform, const int value );



/* #ifndef GLHELPER_BUFF_H_INCLUDED */
#endif 