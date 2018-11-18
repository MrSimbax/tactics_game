#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal; // for simplicity, not actually needed

uniform mat4 u_model;
uniform mat4 u_normal_model; // for simplicity, not actually needed in this shader
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);
}
