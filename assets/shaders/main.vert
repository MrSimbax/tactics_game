#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 u_model;
uniform mat3 u_normal_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    FragPos = vec3(u_model * vec4(a_pos, 1.0));
    Normal = u_normal_model * a_normal;
    gl_Position = u_projection * u_view * vec4(FragPos, 1.0);
}
