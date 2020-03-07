/*
 * glhelper_core.h
 * 
 * core structures for glhelper
 * 
 */



/* pragma one */
#ifndef GLHELPER_CORE_H_INCLUDED
#define GLHELPER_CORE_H_INCLUDED



/* include general headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* include opengl headers */
#include "glad/glad.h"
#include <GLFW/glfw3.h>



/* GLOBAL FLAGS AND MACROS */

/* GLH_TYPE_X
 *
 * macros for OpenGL enum types
 */
#define GLH_TYPE_BYTE GL_BYTE
#define GLH_TYPE_UNSIGNED_BYTE GL_UNSIGNED_BYTE
#define GLH_TYPE_SHORT GL_SHORT
#define GLH_TYPE_UNSIGNED_SHORT GL_UNSIGNED_SHORT
#define GLH_TYPE_INT GL_INT
#define GLH_TYPE_UNSIGNED_INT GL_UNSIGNED_INT
#define GLH_TYPE_FLOAT GL_FLOAT

/* TYPEDEFS */

/* typedef glh_object_t -> GLint */
typedef GLint glh_object_t;

/* typedef glh_type_t -> GLenum */
typedef GLenum glh_type_t;



/* #ifndef GLHELPER_CORE_H_INCLUDED */
#endif