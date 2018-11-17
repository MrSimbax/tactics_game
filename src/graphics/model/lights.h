#pragma once
#include "glm/glm.hpp"
#include "../shader/shader_program.h"

namespace tactics_game
{
class shader_program;

struct point_light
{
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant; 
    float linear;
    float quadratic;
};

struct dir_light
{
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

void set_point_light(shader_program& program, point_light light, size_t i);
}
