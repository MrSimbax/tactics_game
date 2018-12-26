#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 2) in vec2 a_tex_coords;

out vec2 TexCoords;

uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * vec4(a_pos, 1.0);
    TexCoords = a_tex_coords;
}
