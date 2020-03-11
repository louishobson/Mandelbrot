/*
 * glhelper_buff.c
 * 
 * implementation of glhelper_buff.h
 * 
 */



/* include glhelper_buff.h */
#include "glhelper_buff.h"



/* FUNCTION IMPLEMENTATIONS */

/* glh_create_vertex_array_object
 *
 * creates a vertex array object
 * the new vertex array object will be bound on return
 * 
 * return: vao ID >= 0 for success, -1 for failure
 */
glh_object_t glh_create_vertex_array_object ()
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before creating a vertex array object\n" );

    /* create object */
    glh_object_t vao;
    glGenVertexArrays ( 1, &vao );

    /* return vertex array object */
    return vao;
}

/* glh_delete_vertex_array_object
 *
 * deletes a vertex array object
 * 
 * vao: the vertex array object to delete
 * 
 * return: 0 for success, -1 for failure
 */
int glh_delete_vertex_array_object( const glh_object_t vao )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before deleting a vertex array object\n" );

    /* delete the array object */
    glDeleteVertexArrays ( 1, &vao );

    /* return 0 for success */
    return 0;
}

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
glh_object_t glh_create_vertex_buffer_object ( const void * vertices, const size_t v_size, const glh_type_t buff_type )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before creating a vertex buffer object\n" );

    /* create object */
    glh_object_t vbo;
    glGenBuffers ( 1, &vbo );
    
    /* bind the object to GL_ARRAY_BUFFER */
    glBindBuffer ( GL_ARRAY_BUFFER, vbo );
    
    /* send the data to the buffer */
    glBufferData ( GL_ARRAY_BUFFER, v_size, vertices, buff_type );

    /* unbind vbo */
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );
    
    /* return the buffer */
    return vbo;
}

/* glh_delete_vertex_buffer_object
 *
 * deletes a vertex buffer object
 * 
 * vbo: vertex buffer object to delete
 * 
 * return: 0 for success, -1 for failure
 */
int glh_delete_vertex_buffer_object ( const glh_object_t vbo )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before deleting a vertex buffer object\n" );
    
    /* delete the buffer */
    glDeleteBuffers ( 1, &vbo );

    /* return 0 for success */
    return 0;
}

/* glh_create_element_buffer_object
 * 
 * creates an element buffer object containing the given data
 * the new buffer will be bound to GL_ELEMENT_ARRAY_BUFFER on return
 * 
 * elements: array of elements
 * e_size: size of array (in bytes)
 * buff_type: the buffer type from GLH_BUFF_STREAM/STATIC/DYNAMIC_DRAW
 *
 * return: buffer ID >= 0 for success, -1 for failure
 */
glh_object_t glh_create_element_buffer_object ( const void * elements, const size_t e_size, const glh_type_t buff_type )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before creating an element buffer object\n" );

    /* create object */
    glh_object_t ebo;
    glGenBuffers ( 1, &ebo );

    /* bind the object to GL_ELEMENT_ARRAY_BUFFER */
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, ebo );

    /* send the data to the buffer */
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, e_size, elements, buff_type );

    /* unbind ebo */
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );

    /* return the buffer */
    return ebo;
}

/* glh_delete_element_buffer_object
 *
 * deletes a element buffer object
 * 
 * ebo: element buffer object to delete
 * 
 * return: 0 for success, -1 for failure
 */
int glh_delete_element_buffer_object ( const glh_object_t ebo )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before deleting an element buffer object\n" );
    
    /* delete the buffer */
    glDeleteBuffers ( 1, &ebo );

    /* return 0 for success */
    return 0;
}


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
                                    const void * offset )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before configuring a vertex attribute\n" );

    /* bind the vao and vbo */
    glBindVertexArray ( vao );
    glBindBuffer ( GL_ARRAY_BUFFER, vbo );

    /* configure attribute pointer */
    glVertexAttribPointer ( index, size, type, GL_FALSE, stride, offset );

    /* unbind vao and vbo */
    glBindVertexArray ( 0 );
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );

    /* return 0 for success */
    return 0;
}

/* glh_bind_element_buffer_object
 *
 * bind an ebo to a vao
 * 
 * vao: the vao to bind the ebo to
 * ebo: the ebo to bind
 * 
 * return: 0 for success, -1 for failure
 */
int glh_bind_element_buffer_object ( const glh_object_t vao, const glh_object_t ebo )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before bind an ebo to a vao\n" );

    /* bind the vao then the ebo */
    glBindVertexArray ( vao );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, ebo );

    /* unbind the vao then the ebo */
    glBindVertexArray ( 0 );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );

    /* return 0 for success */
    return 0;
}

/* glh_enable_vertex_attribute
 *
 * enables a vertex attribute
 * 
 * vao: vertex array object to configure
 * index: the index to enable
 * 
 * return: 0 for success, -1 for failure
 */
int glh_enable_vertex_attribute ( const glh_object_t vao, const int index )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before enabling a vertex attribute\n" );

    /* bind the vao */
    glBindVertexArray ( vao );

    /* enable the attribute */
    glEnableVertexAttribArray ( index );

    /* unbind vao */
    glBindVertexArray ( 0 );

    /* return 0 for success */
    return 0;
}

/* glh_disable_vertex_attribute
 *
 * enables a vertex attribute
 * 
 * vao: vertex array object to configure
 * index: the index to disable
 * 
 * return: 0 for success, -1 for failure
 */
int glh_disable_vertex_attribute ( const glh_object_t vao, const int index )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before disabling a vertex attribute\n" );

    /* bind the vao */
    glBindVertexArray ( vao );

    /* disable the attribute */
    glDisableVertexAttribArray ( index );

    /* unbind vao */
    glBindVertexArray ( 0 );

    /* return 0 for success */
    return 0;
}

/* glh_get_uniform_location
 *
 * aquires an object reference for a uniform within a shader program
 * 
 * shader_program: the shader program containing the uniform
 * uniform_name: the name of the uniform (string)
 * 
 * return: unform location >= 0 for succes, -1 for failure (including uniform not found)
 */
glh_object_t glh_get_uniform_location ( const glh_object_t shader_program, const char * uniform_name )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before getting a uniform's location\n" );

    /* return the location of the uniform */
    return glGetUniformLocation ( shader_program, uniform_name );
}

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
int glh_set_uniform_vec1 ( const glh_object_t uniform, const float x )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before setting a uniform's value\n" );

    /* set uniform */
    glUniform1f ( uniform, x );

    /* return 0 for success */
    return 0;
}

int glh_set_uniform_vec2 ( const glh_object_t uniform, const float x, const float y )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before setting a uniform's value\n" );

    /* set uniform */
    glUniform2f ( uniform, x, y );

    /* return 0 for success */
    return 0;
}

int glh_set_uniform_vec3 ( const glh_object_t uniform, const float x, const float y, const float z )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before setting a uniform's value\n" );

    /* set uniform */
    glUniform3f ( uniform, x, y, z );

    /* return 0 for success */
    return 0;
}

int glh_set_uniform_vec4 ( const glh_object_t uniform, const float x, const float y, const float z, const float w )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before setting a uniform's value\n" );

    /* set uniform */
    glUniform4f ( uniform, x, y, z, w );

    /* return 0 for success */
    return 0;
}

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
int glh_set_uniform_mat2 ( const glh_object_t uniform, const int transpose, const float * values )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before setting a matrix's value\n" );

    /* set uniform */
    glUniformMatrix2fv (  uniform, 1, transpose, values );

    /* return 0 for success */
    return 0;
}

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
int glh_set_uniform_float ( const glh_object_t uniform, const float value )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before setting a uniform's value\n" );

    /* set the value */
    glUniform1f ( uniform, value );

    /* return 0 for success */
    return 0;
}

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
int glh_set_uniform_int ( const glh_object_t uniform, const int value )
{
    /* check glad is initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised before setting a uniform's value\n" );

    /* set the value */
    glUniform1i ( uniform, value );

    /* return 0 for success */
    return 0;
}