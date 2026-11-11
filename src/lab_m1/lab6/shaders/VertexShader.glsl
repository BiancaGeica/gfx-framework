#version 330

// Input
// TODO(student): Get vertex attributes from each location

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_tex_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

// Output
// TODO(student): Output values to fragment shader

vec4 Compute()
{
    vec3 position = v_position;

    position.y += sin(time);
    
    return Projection * View * Model * vec4(position, 1);
}
void main()
{
    // TODO(student): Send output to fragment shader
    gl_Position = Compute();

    // TODO(student): Compute gl_Position

}

// sa se modifice culorile in timp real - bonus 1
// sa nu se mai duca pe directia ciudata, ci sa se duca in sus si in jos cele doua cuburi

// bonus 2: un "blob" foarte colorat care se misca ca un nor ciudat
