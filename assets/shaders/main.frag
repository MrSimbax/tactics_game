#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

struct point_light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant; 
    float linear;
    float quadratic;
};

struct material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform material mat;
uniform vec3 u_world_ambient;
uniform int u_point_lights_count;
uniform point_light point_lights[50];
uniform vec3 u_view_pos;

vec3 calc_point_light(point_light light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir = normalize(light.position - frag_pos);

    float diff = max(dot(normal, light_dir), 0.0);

    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), mat.shininess * 128);

    float distance = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * mat.ambient;
    vec3 diffuse = light.diffuse * diff * mat.diffuse;
    vec3 specular = light.specular * spec * mat.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
} 

void main()
{
    vec3 norm = normalize(Normal);
    vec3 view_dir = normalize(u_view_pos - FragPos);

    vec3 result = vec3(0);

    for(int i = 0; i < u_point_lights_count; i++)
        result += calc_point_light(point_lights[i], norm, FragPos, view_dir);

    result += u_world_ambient;

    FragColor = vec4(result, 1.0);
}
