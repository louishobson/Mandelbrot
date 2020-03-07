/*
 * mandelbrot_fragment.glsl
 * 
 * mandelbrot fragment shader
 */

#version 330 core



/* INPUT AND OUTPUT */

/* output colour */
out vec4 FragColor;



/* UNIFORMS */

/* mandelbrot_stretch/translation
 * 
 * uniforms to be applied to a fragment to transform it to a value of c for the mandelbrot function
 */
uniform vec4 mandelbrot_stretch;
uniform vec4 mandelbrot_translation;

/* mandelbrot_breakout
 *
 * uniform for the breakout point of the mandelbrot iteration
 */
uniform float mandelbrot_breakout;

/* mandelbrot_max_it
 *
 * uniform for the maximum number of iterations of the mandelbrot function
 */
uniform int mandelbrot_max_it;

/* mandelbrot_power
 * 
 * uniform for the power of z
 */
 uniform int mandelbrot_power;



 /* MACROS */

 /* complex_multiply
 *
 * multiplies two complex numbers
 *
 * __z0,1: complex numbers to multiply
 */
#define complex_multiply(__z0, __z1) vec2 ( ( __z0.x * __z1.x ) - ( __z0.y * __z1.y ), ( __z0.x * __z1.y ) + ( __z0.y * __z1.x ) )

/* complex_square
 *
 * square a complex number
 *
 * __z: the complex number to square
 */
#define complex_square(__z) vec2 ( ( __z.x * __z.x ) - ( __z.y * __z.y ), ( 2 * __z.x * __z.y ) )

/* complex_cube
 *
 * cube a complex number
 *
 * __z: the complex number to cube
 */
#define complex_cube(__z) vec2 ( ( __z.x * __z.x * __z.x ) - ( 3 * __z.x * __z.y * __z.y ), ( 3 * __z.x * __z.x * __z.y ) - ( __z.y * __z.y * __z.y ) )

/* complex_abs
 *
 * find the absolute of a complex number
 *
 * __z: the complex number to find the absolute of
 */
#define complex_abs(__z) sqrt ( ( __z.x * __z.x ) + ( __z.y * __z.y ) )

/* complex_reciprocal
 *
 * find the reciprocal of a complex number
 *
 * __z: the complex number to find the reciprocal of
 */
#define complex_reciprocal(__z) ( __z == vec2 ( 0.0f, 0.0f ) ? vec2 ( 0.0f, 0.0f ) : vec2 ( __z.x , - __z.y ) / ( ( __z.x * __z.x ) + ( __z.y * __z.y ) ) )



/* FUNCTIONS */

/* complex_pow
 *
 * raise a complex number to a power
 *
 * z: complex number
 * p: power to raise it by
 *
 * return: the complex number raised to the power
 */
vec2 complex_pow ( const vec2 z, const int p )
{
    /* if z == 0 */
    if ( z == vec2 ( 0.0f, 0.0f ) )
    {
        /* if power is 0, return 1 */
        
    }

    /* find abs of p */
    int absp = abs ( p );

    /* vector to return */
    vec2 zp = vec2 ( 1.0f, 0.0f );

    /* iterate multiplying by z until desired power is achieved */
    int it;
    for ( it = 0; it < absp; ++it ) zp = complex_multiply ( zp, z );

    /* if negative p, find recipricol */
    if ( p < 0 ) zp = complex_reciprocal ( zp );

    /* return zp */
    return zp;
}

/* transform_vector
 *
 * transforms a vector through a stretch followd by a translation
 *
 * initial_vector: initial vector
 * stretch: stretch to apply to the initial vector
 * translation: translation to apply to the vector
 *
 * return: the transformed vector
 */
vec4 transform_vector ( const vec4 initial_vector, const vec4 stretch, const vec4 translation )
{
    /* create new vector */
    vec4 transformed_vector = initial_vector;
    /* apply stretch */
    transformed_vector *= stretch;
    /* apply translation */
    transformed_vector += translation;
    /* return the new vector */
    return transformed_vector;
}

/* iterate_on_mandelbrot
 *
 * c: the complex number to test in the form x + yi
 * breakout: the absolute value at which to break out on
 * max_it: the maximum number of iterations
 *
 * return: the number of iterations before reaching breakout or max_it
 */
int iterate_on_mandelbrot ( const vec2 c, const float breakout, const int max_it )
{
    /* initial input to the mandelbrot function */
    vec2 z = vec2 ( 0.0f, 0.0f );
    float absab = 0;
    /* initiate iteration loop */
    int it;
    for ( it = 0; absab < breakout && it < max_it; ++it )
    {
        /* raise z to power and add c */
        z = complex_square ( z ) + c;
        /* find the absolute */
        absab = complex_abs ( z );
    }
    /* return iterations completed */
    return it;
}

/* iterate_on_multibrot
 *
 * c: the complex number to test in the form x + yi
 * breakout: the absolute value at which to break out on
 * max_it: the maximum number of iterations
 * power: the power of z
 *
 * return: the number of iterations before reaching breakout or max_it
 */
int iterate_on_multibrot ( const vec2 c, const float breakout, const int max_it, const int power )
{
    /* initial input to the mandelbrot function */
    vec2 z = vec2 ( 0.0f, 0.0f );
    float absab = 0;
    vec2 zp;
    /* initiate iteration loop */
    int it;
    for ( it = 0; absab < breakout && it < max_it; ++it )
    {
        /* raise z to power */
        z = complex_pow ( z, power );
        /* if negative power, and returned 0, a zero error occured */
        if ( power < 0 && z == vec2 ( 0.0f, 0.0f ) ) return 1;
        /* add c */
        z += c;
        /* find the absolute */
        absab = complex_abs ( z );
    }
    /* return iterations completed */
    return it;
}

void main ()
{
    /* transform frag coords */
    vec4 new_frag_coord = transform_vector ( gl_FragCoord, mandelbrot_stretch, mandelbrot_translation );
    /* if mandelbrot_power == 2, use normal function */
    float mandelbrot_constant;
    if ( mandelbrot_power == 2 ) mandelbrot_constant = iterate_on_mandelbrot ( new_frag_coord.xy, mandelbrot_breakout, mandelbrot_max_it );
    else mandelbrot_constant = iterate_on_multibrot ( new_frag_coord.xy, mandelbrot_breakout, mandelbrot_max_it, mandelbrot_power );
    /* fully in set colour */
    if ( mandelbrot_constant == mandelbrot_max_it ) FragColor = vec4 ( 0.0, 0.0, 0.0, 1.0 );
    /* if not in set */
    else FragColor = vec4 ( 1 - ( mandelbrot_constant / mandelbrot_max_it ), 1 - ( mandelbrot_constant / mandelbrot_max_it ), 1 - ( mandelbrot_constant / mandelbrot_max_it ), 1.0f );
}