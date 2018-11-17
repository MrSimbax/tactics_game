#include "lights.h"

#include <string>

using namespace tactics_game;

std::string point_light_name(size_t i, const std::string& prop);

void tactics_game::set_point_light(shader_program& program, point_light light, size_t i)
{
    program.set_vec3(point_light_name(i, "position"), light.position);
    program.set_vec3(point_light_name(i, "ambient"), light.ambient);
    program.set_vec3(point_light_name(i, "diffuse"), light.diffuse);
    program.set_vec3(point_light_name(i, "specular"), light.specular);
    program.set_float(point_light_name(i, "constant"), light.constant);
    program.set_float(point_light_name(i, "linear"), light.linear);
    program.set_float(point_light_name(i, "quadratic"), light.quadratic);
}

std::string point_light_name(size_t i, const std::string& prop)
{
    std::string name{"point_lights["};
    name += std::to_string(i);
    name += "].";
    name += prop;
    return name;
}
