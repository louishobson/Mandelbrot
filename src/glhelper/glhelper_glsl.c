/*
 * glhelper_glsl.c
 * 
 * implementation of glhelper_glsl.h
 *
 */



/* include glhelper_glsl.h */
#include "glhelper_glsl.h"



/* FUNCTION IMPLEMENTATIONS */

/* glh_import_shader
 *
 * reads the source of an external shader file
 * 
 * path: path to the shader
 * 
 * return: non-NULL on success, NULL on failure
 *         REMEMBER TO FREE THE RETURN STRING TO AVOID MEMORY LEAK
 */
char * glh_import_shader ( const char * path )
{
    /* stat the shader */
    struct stat stat_buff;
    if ( stat ( path, &stat_buff ) < 0 )
    {
        /* stat failed: file cannot be accessed, so produce error */
        fprintf ( stderr, "GLH ERROR: shader at path '%s' could not be stat'ed\n", path );
        return NULL;
    }

    /* open the file */
    FILE * shader_fp = fopen ( path, "r" );
    if ( !shader_fp )
    {
        /* opening file failed, so produce error */
        fprintf ( stderr, "GLH ERROR: shader at path '%s' could be stat'ed, but failed to be opened\n" );
        return NULL;
    }
    
    /* allocate a buffer to fit the contents of the file (+! for trailing NULL byte) */
    char * shader_buff = ( char * ) malloc ( stat_buff.st_size + 1 );

    /* read the file into the buffer */
    size_t bytes_read = 0;
    if ( ( bytes_read = fread ( shader_buff, 1, stat_buff.st_size, shader_fp ) ) < stat_buff.st_size )
    {
        /* failed to completely read file, so produce error */
        fprintf ( stderr, "GLH ERROR: shader at path '%s' has size %luB, but only %luB could be read\n", path, stat_buff.st_size, bytes_read );
        free ( shader_buff );
        return NULL;
    }

    /* close file */
    fclose ( shader_fp );

    /* set the last byte of shader_buff to NULL */
    shader_buff [ stat_buff.st_size ] = '\0';

    /* return shader_buff */
    return shader_buff;
}

/* glh_create_shader
 *
 * function to compile a shader object
 * 
 * shader_source: source code for the shader
 * shader_type: type of shader (GLH_GLSL_VERTEX/GEOMETRY/FRAGMENT_SHADER)
 * 
 * return: shader ID >= 0 for success, -1 for failure
 */
glh_object_t glh_create_shader ( const char * shader_source, const glh_type_t shader_type )
{
    /* check glad has been initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised in order to create a shader\n" );

    /* create shader object, add the source and compile */
    const glh_object_t shader = glCreateShader ( shader_type );
    glShaderSource ( shader, 1, &shader_source, NULL );
    glCompileShader ( shader );
    /* check compilation success */
    int comp_success;
    glGetShaderiv ( shader, GL_COMPILE_STATUS, &comp_success );
    if ( !comp_success )
    {
        /* compilation failed, so produce error */
        char comp_log [ GLH_GLSL_SHADER_LOG_SIZE ];
        glGetShaderInfoLog ( shader, GLH_GLSL_SHADER_LOG_SIZE, NULL, comp_log );
        glDeleteShader ( shader );
        fprintf ( stderr, "GLH ERROR: shader compilation failed\n\nBEGIN INFO LOG\n\n%s\nEND INFO LOG\n\n", comp_log );          
        return -1;
    }

    /* return shader object */
    return shader;
}

/* glh_create_shader_form_path
 * 
 * function to create a shader object from a path, rather than its source
 * 
 * shader_path: path to the shader
 * shader_type: type of shader (GLH_GLSL_VERTEX/GEOMETRY/FRAGMENT_SHADER)
 * 
 * return: shader ID >= 0 for success, -1 for failure
 */
glh_object_t glh_create_shader_from_path ( const char * shader_path, const int shader_type )
{
    /* check glad has been initialised */
   __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised in order to create a shader\n" )

    /* load shader into buffer */
    char * shader_source = glh_import_shader ( shader_path );
    if ( !shader_source )
    {
        /* failed to import shader, so return error */
        return -1;
    }


    /* create shader */
    glh_object_t shader = glh_create_shader ( shader_source, shader_type );
    
    /* free source */
    free ( shader_source );

    /* return the shader, regardless of if its creation succeeded or not */
    return shader;
}

/* glh_delete_shader
 *
 * deletes a shader
 * note that any program the shader is attached to will still function
 * 
 * shader: shader to delete
 * 
 * return: 0 for success, -1 for failure
 */
int glh_delete_shader ( const glh_object_t shader )
{
    /* check glad has been initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised in order to delete a shader\n" );

    /* delete shader */
    glDeleteShader ( shader );

    /* return 0 for success */
    return 0;
}

/* glh_create_shader_program
 *
 * function to create a shader program
 * 
 * vertex/geometry/fragment_shader: shader objects
 * 
 * return: shader program ID >= 0 for success, -1 for failure
 */
glh_object_t glh_create_shader_program ( const glh_object_t vertex_shader, const glh_object_t geometry_shader, const glh_object_t fragment_shader )
{
    /* check glad has been initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised in order to create a shader program\n" );

    /* create shader program object */
    const glh_object_t shader_program = glCreateProgram ();

    /* attach respective shaders, if supplied */
    if ( vertex_shader >= 0 ) glAttachShader ( shader_program, vertex_shader );
    if ( geometry_shader >= 0 ) glAttachShader ( shader_program, geometry_shader );
    if ( fragment_shader >= 0 ) glAttachShader ( shader_program, fragment_shader );

    /* link the program */
    glLinkProgram ( shader_program );
    /* check linking success */
    int link_success;
    glGetProgramiv ( shader_program, GL_LINK_STATUS, &link_success );
    if ( !link_success )
    {
        /* linking failed, so produce error */
        char link_log [ GLH_GLSL_SHADER_LOG_SIZE ];
        glGetProgramInfoLog ( shader_program, GLH_GLSL_SHADER_LOG_SIZE, NULL, link_log );
        glDeleteProgram ( shader_program );
        fprintf ( stderr, "GLH ERROR: shader program linking failed\n\nBEGIN INFO LOG\n\n%s\nEND INFO LOG\n\n", link_log );
        return -1;
    }

    /* return shader program */
    return shader_program;
}

/* glh_delete_shader_program
 *
 * function to delete a shader program
 * 
 * shader_program: shader program to delete
 * 
 * return: 0 for success, -1 for failure
 */
int glh_delete_shader_program ( const glh_object_t shader_program )
{
    /* check glad has been initialised */
    __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised in order to delete a shader program\n" );

    /* delete shader program */
    glDeleteProgram ( shader_program );

    /* return 0 for success */
    return 0;
}

/* glh_use_shader_program
 *
 * set a shader program to be used for the graphics pipeline
 * 
 * shader_program: shader program to set to be used
 * 
 * return: 0 for success, -1 for failure
 */
int glh_use_shader_program ( const glh_object_t shader_program )
{
    /* check glad has been initialised */
   __GLH_GLAD_INIT_CHECK__ ( 1, -1, "GLH ERROR: glad must be initialised in order to delete a shader program\n" );

    /* set shader program as the one to be used */
    glUseProgram ( shader_program );

    /* return 0 for success */
    return 0;
}