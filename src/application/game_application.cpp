#include "game_application.h"

#include "../graphics/window/sdl_window.h"
#include "../graphics/shader/shader_program.h"

#include <plog/Log.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <boxer/boxer.h>

#include "../misc/custom_log.h"
#include "glad/glad.h"
#include "args.hxx"
#include <iostream>

using namespace tactics_game;

game_application::game_application()
    : is_running_{false}
{
}

game_application::~game_application() = default;

bool game_application::init_options(const int argc, char** argv)
{
    args::ArgumentParser parser("Tactics Game");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    const args::Flag vsync(parser, "vsync", "Vertical synchronization on", {"vsync"});
    const args::Flag fullscreen(parser, "fullscreen", "Fullscreen on", {'f', "fullscreen"});
    const args::Flag anti_aliasing(parser, "anti-aliasing", "Anti-aliasing on", {"aa"});
    args::ValueFlag<std::string> assets_dir(parser, "assets_dir", "Path to assets directory (default: ./assets)",
                                            {'d', "assets-dir"});
    args::ValueFlag<std::string> map(parser, "map_name",
                                     "Loads the map with given name (from assets/maps/map_name directory)", {"map"});

    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (const args::Help&)
    {
        LOG_INFO << parser;
        return false;
    }
    catch (const args::ParseError& e)
    {
        LOG_ERROR << e.what() << '\n' << parser;
        return false;
    }
    catch (const args::ValidationError& e)
    {
        LOG_ERROR << e.what() << '\n' << parser;
        return false;
    }

    if (vsync)
    {
        options_["vsync"] = "true";
    }
    else
    {
        options_["vsync"] = "false";
    }
    if (fullscreen)
    {
        options_["fullscreen"] = "true";
    }
    else
    {
        options_["fullscreen"] = "false";
    }
    if (anti_aliasing)
    {
        options_["anti-aliasing"] = "true";
    }
    else
    {
        options_["anti-aliasing"] = "false";
    }
    if (assets_dir)
    {
        options_["assets_dir"] = args::get(assets_dir);
    }
    else
    {
        options_["assets_dir"] = "./assets";
    }
    if (map)
    {
        options_["map"] = args::get(map);
    }
    else
    {
        options_["map"] = "demo";
    }

    return true;
}

int game_application::execute(const int argc, char* argv[])
{
    if (!init_options(argc, argv))
    {
        return 1;
    }

    if (!init())
    {
        LOG_FATAL << "game_application could not be started due to initialization error.";
        std::getchar();
        return 1;
    }

    LOG_INFO << "game_application is running.";

    boxer::show("Welcome to Tactics Game!\n"
                "\n"
                "It's a turn-based tactics game for 2 players (red and green). Red starts.\n"
                "Wins the player who destroys all the enemy units.\n"
                "\n"
                "You can press H at any time to see controls.\n"
                "Open the game from command line with -h argument to see options.",
                "Tactics Game");

    SDL_Event event{};
    while (is_running_)
    {
        const auto time = SDL_GetTicks() / 1000.0f;
        const auto delta_time = time - last_frame_time_;
        last_frame_time_ = time;

        while (SDL_PollEvent(&event))
        {
            handle_event(&event);
        }

        update(delta_time);
        render();
    }

    if (scene_renderer_->did_game_end())
    {
        const std::string player_color = scene_renderer_->get_current_player_id() == 0 ? "red" : "green";
        std::string message = "Game won by ";
        message += player_color;
        message += " player!\n";
        message += "Thanks for playing!";
        boxer::show(message.c_str(), "Tactics Game");
    }

    return 0;
}

void game_application::notify_on_camera_update()
{
    glm::ivec2 mouse_pos{};
    SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
    const glm::ivec2 size{window_->get_size().width, window_->get_size().height};
    scene_renderer_->on_mouse_motion(scene_renderer_->get_current_camera().mouse_to_ray(mouse_pos, size));
}

bool game_application::init()
{
    try
    {
        init_window();
        init_graphics();
        init_input();
    }
    catch (const std::runtime_error& e)
    {
        LOG_ERROR << e.what();
        return false;
    }

    is_running_ = true;
    LOG_INFO << "game_application initialized successfully.";
    return true;
}

void game_application::init_window()
{
    const std::string title = "Tactics Game";

    window_size size;
    size.width = 800;
    size.height = 600;

    window_settings settings;
    settings.fullscreen = options_["fullscreen"] == "true";
    settings.resizable = true;
    settings.vsync = options_["vsync"] == "true";
    settings.anti_aliasing = options_["anti-aliasing"] == "true";

    window_.reset(new sdl_window(title, size, settings));

    window_->set_wireframe_mode(false);
    window_->set_mouse_trapped(false);
}

void game_application::update_aspect_ratio_in_cameras() const
{
    const auto aspect_ratio =
        static_cast<float>(window_->get_size().width)
        /
        static_cast<float>(window_->get_size().height);
    for (const auto& camera : scene_renderer_->get_all_cameras())
        camera->set_ratio(aspect_ratio);
}

void game_application::init_graphics()
{
    // Load assets from specified folder
    assets_manager_.set_root(options_["assets_dir"]);

    // Create shader programs
    shader_program_.reset(new shader_program{
        assets_manager_.get_shader_source("main.vert"),
        assets_manager_.get_shader_source("main.frag")
    });

    simple_color_shader_program_.reset(new shader_program{
        assets_manager_.get_shader_source("simple_color.vert"),
        assets_manager_.get_shader_source("simple_color.frag")
    });

    fow_shader_program_.reset(new shader_program{
        assets_manager_.get_shader_source("fow.vert"),
        assets_manager_.get_shader_source("fow.frag")
    });

    // Load map
    auto loaded_scene{assets_manager_.get_scene(options_["map"])};
    if (loaded_scene.scene.get_players().empty())
        throw std::runtime_error{"map could not be loaded"};

    for (auto& camera : loaded_scene.player_cameras)
    {
        camera.set_speed(16.0f);
        camera.set_offset(glm::vec3{5.0f, 7.5f, 5.0f});
        camera.get_bounds().xz_min = glm::vec3{0.0f};
        camera.get_bounds().xz_max = loaded_scene.scene.get_game_map()->get_size() - 1;
        camera.set_observer(this);
    }

    // Create map renderer
    game_map_renderer map_renderer{
            loaded_scene.scene.get_game_map(),
            graphics_object{assets_manager_.get_model("floor.obj")},
            graphics_object{assets_manager_.get_model("wall.obj")},
            graphics_object{assets_manager_.get_model("frame.obj")}
    };

    // Load movement grid model
    const auto grid_model = assets_manager_.get_model("frame.obj");

    // Create player renderers
    std::vector<player_renderer> player_renderers;
    player_renderers.emplace_back(
        loaded_scene.scene.get_players()[0], std::move(loaded_scene.player_cameras[0]), assets_manager_.get_model("player1.obj"), grid_model,
        glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});
    player_renderers.emplace_back(
        loaded_scene.scene.get_players()[1], std::move(loaded_scene.player_cameras[1]), assets_manager_.get_model("player2.obj"), grid_model,
        glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

    // Create light objects
    std::vector<std::vector<simple_color_object>> light_objects;
    light_objects.reserve(loaded_scene.scene.get_game_map()->get_size().y);
    const auto light_model = assets_manager_.get_model("point_light.obj");
    for (const auto& layer : loaded_scene.point_lights)
    {
        std::vector<simple_color_object> light_objects_layer;
        for (const auto& light : layer)
        {
            simple_color_object light_obj{light_model};
            light_obj.set_position(light.position);
            light_obj.set_color(glm::vec4{light.diffuse, 1.0f});
            light_objects_layer.push_back(std::move(light_obj));
        }
        light_objects.push_back(std::move(light_objects_layer));
    }
    light_objects_renderer light_objs_renderer{std::move(light_objects)};

    // Create UI renderer
    ui_renderer ui_renderer{
            simple_color_object{assets_manager_.get_model("ui_no_moves_left.obj").merged()},
            simple_color_object{assets_manager_.get_model("ui_moves_left.obj").merged()}
    };

    auto grid_obj = buffered_graphics_object{graphics_object{assets_manager_.get_model("grid.obj")}};

    scene_renderer_.reset(new game_scene_renderer(
        std::move(loaded_scene.scene),
        std::move(map_renderer),
        std::move(player_renderers),
        std::move(loaded_scene.point_lights),
        std::move(grid_obj),
        std::move(ui_renderer),
        std::move(light_objs_renderer),
        loaded_scene.world_ambient
    ));

    scene_renderer_->set_lights(*shader_program_);
    
    update_aspect_ratio_in_cameras();

    LOG_INFO << "Loaded map \"" << scene_renderer_->get_scene()->get_name() << "\"";

    for (int i = 0; i < 100; ++i)
    {
        //buffered_graphics_object{graphics_object{assets_manager_.get_model("floor.obj")}};
        buffered_mesh{grid_model.merged()};
    }
}

void game_application::init_input()
{
    input_manager_.bind_key_to_action(SDLK_a, input_action::camera_left);
    input_manager_.bind_key_to_action(SDLK_d, input_action::camera_right);
    input_manager_.bind_key_to_action(SDLK_w, input_action::camera_forward);
    input_manager_.bind_key_to_action(SDLK_s, input_action::camera_backward);
    input_manager_.bind_key_to_action(SDLK_q, input_action::camera_rotate_left);
    input_manager_.bind_key_to_action(SDLK_e, input_action::camera_rotate_right);
    input_manager_.bind_key_to_action(SDLK_SPACE, input_action::camera_up);
    input_manager_.bind_key_to_action(SDLK_LCTRL, input_action::camera_down);
    input_manager_.bind_key_to_action(SDLK_RETURN, input_action::end_turn);
    input_manager_.bind_key_to_action(SDLK_TAB, input_action::next_unit);
    input_manager_.bind_key_to_action(SDLK_SEMICOLON, input_action::debug);
    input_manager_.bind_key_to_action(SDLK_h, input_action::show_help);

    input_manager_.bind_action_down(input_action::camera_left, [this]
    {
        camera_direction_.x -= 1.0f;
    });
    input_manager_.bind_action_up(input_action::camera_left, [this]
    {
        camera_direction_.x += 1.0f;
    });

    input_manager_.bind_action_down(input_action::camera_right, [this]
    {
        camera_direction_.x += 1.0f;
    });
    input_manager_.bind_action_up(input_action::camera_right, [this]
    {
        camera_direction_.x -= 1.0f;
    });

    input_manager_.bind_action_down(input_action::camera_forward, [this]
    {
        camera_direction_.z += 1.0f;
    });
    input_manager_.bind_action_up(input_action::camera_forward, [this]
    {
        camera_direction_.z -= 1.0f;
    });

    input_manager_.bind_action_down(input_action::camera_backward, [this]
    {
        camera_direction_.z -= 1.0f;
    });
    input_manager_.bind_action_up(input_action::camera_backward, [this]
    {
        camera_direction_.z += 1.0f;
    });

    input_manager_.bind_action_down(input_action::camera_up, [this]
    {
        scene_renderer_->set_current_layer(scene_renderer_->get_current_layer() + 1);
    });

    input_manager_.bind_action_down(input_action::camera_down, [this]
    {
        scene_renderer_->set_current_layer(scene_renderer_->get_current_layer() - 1);
    });

    input_manager_.bind_action_down(input_action::camera_rotate_left, [this]
    {
        scene_renderer_->get_current_camera().rotate_left();
    });
    input_manager_.bind_action_up(input_action::camera_rotate_right, [this]
    {
        scene_renderer_->get_current_camera().rotate_right();
    });

    /*input_manager_.bind_mouse_motion([this](const glm::ivec2 offset)
    {
        camera_.process_mouse(offset);
    });*/
    input_manager_.bind_mouse_scroll([this](const int offset)
    {
        scene_renderer_->get_current_camera().process_scroll(offset);
    });

    if (length(camera_direction_) != 0.0f)
        camera_direction_ = normalize(camera_direction_);

    input_manager_.bind_mouse_button_down([this](const glm::ivec2 mouse_pos, const int button)
    {
        const glm::ivec2 size{window_->get_size().width, window_->get_size().height};
        scene_renderer_->on_mouse_click(scene_renderer_->get_current_camera().mouse_to_ray(mouse_pos, size), button);
    });

    input_manager_.bind_action_down(input_action::debug, [this]
    {
        LOG_DEBUG << "Camera position: " << scene_renderer_->get_current_camera().get_position();
    });

    input_manager_.bind_mouse_motion([this](const glm::ivec2 mouse_pos, const glm::ivec2 offset)
    {
        const glm::ivec2 size{window_->get_size().width, window_->get_size().height};
        scene_renderer_->on_mouse_motion(scene_renderer_->get_current_camera().mouse_to_ray(mouse_pos, size));
    });

    input_manager_.bind_action_down(input_action::end_turn, [this]()
    {
        scene_renderer_->start_new_turn();
    });

    input_manager_.bind_action_down(input_action::next_unit, [this]()
    {
        scene_renderer_->select_next_unit();
    });

    input_manager_.bind_action_down(input_action::show_help, []()
    {
        boxer::show(
            "Camera controls:\n"
            "* WSAD - moves camera\n"
            "* QE - rotates camera\n"
            "* Wheel Up/Down - zooming\n"
            "* Ctrl/Space - change map level down/up\n"
            "\n"
            "Units controls depend on mouse position:\n"
            "* LMB - select/deselect unit\n"
            "* RMB - if unit selected:\n"
            "* * move unit to movable tile\n"
            "* * * movable tiles are marked with blue grid\n"
            "* * * moving up/down is allowed only on tiles marked with frames\n"
            "* * * up to 2 moves per turn for the unit\n"
            "* * shoot enemy unit which is visible by the unit\n"
            "* * * always ends turn for the unit\n"
            "* * * if hit, the enemy unit disappears from map, otherwise nothing happens\n"
            "* * * probability of hitting depends on distance, cover and height (shooter wants to be higher than target)\n"
            "* * * * light green outline = close to 100%\n"
            "* * * * dark green outline = close to 1%\n"
            "* * * * no outline = 0% (enemy is not visible for the unit)\n"
            "* Tab - select next unit who can still move\n"
            "\n"
            "Game controls:\n"
            "* H - shows this information\n"
            "* Enter - ends current turn\n"
            "* Alt + F4 - closes the game\n"
            "\n"
            "Indicator in the top left corner:\n"
            "* Colour of the indicator shows whose turn is it\n"
            "* ! means there are still units who can move, v/ means it's safe to end turn",
            "Tactics Game Help");
    });
}

void game_application::handle_event(SDL_Event* event)
{
    switch (event->type)
    {
    case SDL_QUIT:
        is_running_ = false;
        break;
    case SDL_WINDOWEVENT:
        switch (event->window.event)
        {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            window_->resize();
            update_aspect_ratio_in_cameras();
            break;
        default: ;
        }
        break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEMOTION:
    case SDL_MOUSEWHEEL:
        input_manager_.handle_event(event);
        break;
    default: ;
    }
}

void game_application::update(const float delta_time)
{
    if (scene_renderer_->did_game_end())
    {
        is_running_ = false;
    }
    scene_renderer_->get_current_camera().process_keyboard(camera_direction_, delta_time);
}

void game_application::render() const
{
    window_->clear_screen();
    scene_renderer_->render(*shader_program_, *simple_color_shader_program_, *fow_shader_program_);
    window_->swap_buffers();
}
