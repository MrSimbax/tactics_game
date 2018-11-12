#include "sdl_window.h"

#include <glad/glad.h>
#include <plog/Log.h>

using namespace tactics_game;

bool sdl_window::is_sdl_initialized_ = false;

void APIENTRY gl_debug_output(GLenum source, GLenum type, GLuint id, GLenum severity,
                              GLsizei length, const GLchar* message, const void* user_param);

// ReSharper disable once CppParameterMayBeConst
sdl_window::sdl_window(const std::string& title, window_size size, const window_settings settings)
    : settings_{settings}
{
    init_sdl();

    window_ = create_sdl_window(title, size, settings);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);

    // Debug
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    context_ = SDL_GL_CreateContext(window_);
    if (!context_)
    {
        std::string message{"Failed to initialize OpenGL context: "};
        message += SDL_GetError();
        SDL_DestroyWindow(window_);
        SDL_Quit();
        throw opengl_initialization_error{message.c_str()};
    }

    if (SDL_GL_SetSwapInterval(settings.vsync ? 1 : 0) < 0)
    {
        LOG_WARNING << "VSync is not supported on this system";
    }

    if (!gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
    {
        std::string message{"Failed to initialize GLAD: "};
        message =+ SDL_GetError();
        SDL_DestroyWindow(window_);
        SDL_GL_DeleteContext(context_);
        SDL_Quit();
        throw opengl_initialization_error{message.c_str()};
    }

    set_up_opengl_debug_output();
    sdl_window::resize();
    sdl_window::set_wireframe_mode(false);
    glEnable(GL_DEPTH_TEST);
}

SDL_Window* sdl_window::create_sdl_window(const std::string& title, const window_size size,
                                          const window_settings settings)
{
    const auto window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        size.width, size.height,
        SDL_WINDOW_SHOWN |
        SDL_WINDOW_OPENGL |
        (settings.resizable ? SDL_WINDOW_RESIZABLE : 0) |
        (settings.fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
    );
    if (!window)
    {
        throw sdl_window_not_created_error{(std::string{"Failed to create window_: "} + SDL_GetError()).c_str()};
    }
    return window;
}

sdl_window::sdl_window(sdl_window&& other) noexcept
    : window_(other.window_),
      context_(other.window_),
      size_(other.size_),
      settings_(other.settings_),
      clear_color_(other.clear_color_),
      wireframe_mode_(other.wireframe_mode_)
{
    other.window_ = nullptr;
    other.context_ = nullptr;
}

sdl_window& sdl_window::operator=(sdl_window&& other) noexcept
{
    window_ = other.window_;
    context_ = other.window_;
    size_ = other.size_;
    settings_ = other.settings_;
    clear_color_ = other.clear_color_;
    wireframe_mode_ = other.wireframe_mode_;

    other.window_ = nullptr;
    other.context_ = nullptr;

    return *this;
}

sdl_window::~sdl_window()
{
    if (!context_ || !window_)
        return;

    SDL_GL_DeleteContext(context_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void sdl_window::init_sdl()
{
    if (!is_sdl_initialized_)
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            throw sdl_initialization_error{(std::string{"Failed to initialize SDL2: "} + SDL_GetError()).c_str()};
        }
        is_sdl_initialized_ = true;
    }
}

window_size sdl_window::get_size() const
{
    return size_;
}

void sdl_window::clear_screen()
{
    glClearColor(clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void sdl_window::resize()
{
    SDL_GetWindowSize(window_, &size_.width, &size_.height);
    glViewport(0, 0, size_.width, size_.height);
}

void sdl_window::swap_buffers()
{
    SDL_GL_SwapWindow(window_);
}

glm::vec4 sdl_window::get_clear_color()
{
    return clear_color_;
}

void sdl_window::set_clear_color(const glm::vec4 color)
{
    clear_color_ = color;
}

bool sdl_window::is_wireframe_mode_on()
{
    return wireframe_mode_;
}

void sdl_window::set_wireframe_mode(const bool mode)
{
    wireframe_mode_ = mode;
    if (mode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void sdl_window::set_up_opengl_debug_output() const
{
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_output, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
    else
    {
        LOG_WARNING << "Debug context could not be created";
    }
}

// ReSharper disable CppParameterMayBeConst
// ReSharper disable once CppParameterNeverUsed
void APIENTRY gl_debug_output(GLenum source, GLenum type, GLuint id, GLenum severity,
                              GLsizei length, const GLchar* message, const void* user_param)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::ostringstream oss{};
    oss << "OpenGL Debug message (id: " << id << "):\n" << message << "\n";

    oss << "Source: ";
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        oss << "API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        oss << "Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        oss << "Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        oss << "Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        oss << "Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        oss << "Other";
        break;
    default:
        oss << "Unknown";
    }
    oss << "\n";

    oss << "Type: ";
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        oss << "Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        oss << "Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        oss << "Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        oss << "Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        oss << "Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        oss << "Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        oss << "Push group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        oss << "Pop group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        oss << "Other";
        break;
    default:
        oss << "Unknown";
    }
    oss << "\n";

    oss << "Severity: ";
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        oss << "high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        oss << "medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        oss << "low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        oss << "notification";
        break;
    default:
        oss << "unknown";
    }
    oss << "\n";

    LOG_DEBUG << oss.str();
}

// ReSharper restore CppParameterMayBeConst
