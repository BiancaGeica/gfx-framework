#version 330

// Input
layout(location = 0) in vec3 v_position; // in spatiul obiect
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;


void main()
{
    // TODO(student): Compute world space vectors
    vec3 world_position = vec3 (Model * vec4 (v_position, 1));
    vec3 world_normal = normalize (vec3 (Model * vec4 (v_normal, 0))); // 0 ca sa nu translateze

    vec3 L = normalize (light_position - world_position); // L trebuie sa fie de lungime 1, deci il normalizam
    vec3 V = normalize( eye_position - world_position);


    // TODO(student): Define ambient light component
    float ambient_light = material_kd * 0.25;

    // TODO(student): Compute diffuse light component
    float diffuse_light = material_kd * 1 * max (dot(world_normal,L), 0);

    // TODO(student): Compute specular light component

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Gouraud (1971) shading method. There is also the Phong (1975) shading
    // method, which we'll use in the future. Don't mix them up!
    vec3 R = reflect (-L, world_normal);
    float specular_light = 0;
    if (diffuse_light > 0)
    {
        specular_light = material_ks * 1 * pow(max(dot(V, R), 0), material_shininess);
    }

    // TODO(student): Compute light
    float d = distance(light_position, world_position);
    float atenuare = 1 / (pow(d, 2) + 1);

    float object_light = ambient_light + atenuare * (diffuse_light + specular_light); 

    // TODO(student): Send color light output to fragment shader
    color = vec3(1);
    color = object_color * object_light;

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
