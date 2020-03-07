/*
 * generic_vertex.glsl
 * 
 * generic vertex shader
 */

#version 330 core

/* vertex attribute */
layout ( location = 0 ) in vec3 pos;

/* main */
void main ()
{
    /* set the position to be the same as the attribute, with an alpha of 1.0 */
    gl_Position = vec4 ( pos.x, pos.y, pos.z, 1.0f );
}