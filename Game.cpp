#include "Game.h"

Game::Game() : is_running(true), camera(Camera((float)WIDTH/(float)HEIGHT)) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Failed to init SDL\n";
        //return false;
    }
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    set_OpenGL_attributes();
    
    // Create our window centered at 800x600 resolution
    main_window = SDL_CreateWindow(
        "AnandCraft",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_OPENGL
    );
    if (!main_window) {
        std::cout << "Unable to create window\n";
        util::check_SDL_error(__LINE__);
        //return false;
    }
    
    main_context = SDL_GL_CreateContext(main_window);
    if (!main_context) {
        util::check_SDL_error(__LINE__);
    }
    
    // vertical sync
    SDL_GL_SetSwapInterval(1);
    
    // init GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    init();
}

void Game::set_OpenGL_attributes() {
    // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Use version 3.2 (modern OpenGL)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

Game::~Game() {
    SDL_GL_DeleteContext(main_context);

    SDL_DestroyWindow(main_window);

    SDL_Quit();
}

void Game::run_loop() {
    // fixed update time, variable rendering
    double previous = SDL_GetTicks();
    double lag = 0.0;
    while (is_running) {
        double current = SDL_GetTicks();
        double elapsed = current - previous;
        previous = current;
        lag += elapsed;

        process_input();

        while (lag >= MS_PER_UPDATE) {
            update();
            lag -= MS_PER_UPDATE;
        }

        render();
    }
}

void Game::process_input() {
    SDL_Event e;

    #define VEL 0.1f

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            is_running = false;
        }
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_w:
                if (camera_vel.z < 0.f) {
                    camera_vel.z = 0.f;
                }
                else {
                    camera_vel.z = VEL;
                }
                break;
            case SDLK_a:
                if (camera_vel.x < 0.f) {
                    camera_vel.x = 0.f;
                }
                else {
                    camera_vel.x = VEL;
                }
                break;
            case SDLK_s:
                if (camera_vel.z > 0.f) {
                    camera_vel.z = 0.f;
                }
                else {
                    camera_vel.z = -VEL;
                }
                break;
            case SDLK_d:
                if (camera_vel.x > 0.f) {
                    camera_vel.x = 0.f;
                }
                else {
                    camera_vel.x = -VEL;
                }
                break;
            case SDLK_SPACE:
                if (camera_vel.y < 0.f) {
                    camera_vel.y = 0.f;
                }
                else {
                    camera_vel.y = VEL;
                }
                break;
            case SDLK_LSHIFT:
                if (camera_vel.y > 0.f) {
                    camera_vel.y = 0.f;
                }
                else {
                    camera_vel.y = -VEL;
                }
                break;
            default:
                break;
            }
        }
        if (e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym) {
            case SDLK_w:
                if (camera_vel.z > 0.f) {
                    camera_vel.z = 0.f;
                }
                else {
                    camera_vel.z = -VEL;
                }
                break;
            case SDLK_a:
                if (camera_vel.x > 0.f) {
                    camera_vel.x = 0.f;
                }
                else {
                    camera_vel.x = -VEL;
                }
                break;
            case SDLK_s:
                if (camera_vel.z < 0.f) {
                    camera_vel.z = 0.f;
                }
                else {
                    camera_vel.z = VEL;
                }
                break;
            case SDLK_d:
                if (camera_vel.x < 0.f) {
                    camera_vel.x = 0.f;
                }
                else {
                    camera_vel.x = VEL;
                }
                break;
            case SDLK_SPACE:
                if (camera_vel.y > 0.f) {
                    camera_vel.y = 0.f;
                }
                else {
                    camera_vel.y = -VEL;
                }
                break;
            case SDLK_LSHIFT:
                if (camera_vel.y < 0.f) {
                    camera_vel.y = 0.f;
                }
                else {
                    camera_vel.y = VEL;
                }
                break;
            default:
                break;
            }
        }
        if (e.type == SDL_MOUSEMOTION) {
            camera_rot_vel.y = (float)-e.motion.xrel;
            camera_rot_vel.x = (float)e.motion.yrel;
        }
        else {
            camera_rot_vel.y = 0;
            camera_rot_vel.x = 0;
        }
    }
}

void Game::update() {
    model.get_transform().getRot().y += 0.03f;
    camera.translate(camera_vel);
    camera.rotate_x(camera_rot_vel.x);
    camera.rotate_y(camera_rot_vel.y);
}
void print_vertices(std::vector<Vertex> vertices) {
    for (int i = 0; i < vertices.size(); ++i) {
        std::cout << "(" << vertices[i].position.x << ", " << vertices[i].position.y << ", " << vertices[i].position.z << ") ";
    }
    std::cout << "\n";
}

void Game::init() {
    // cube
    float vertices[] = {
     0.5f,  0.5f, 0.5f,  // top right
     0.5f, -0.5f, 0.5f,  // bottom right
    -0.5f, -0.5f, 0.5f,  // bottom left
    -0.5f,  0.5f, 0.5f,   // top left
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3,    // second triangle
        7, 4, 5,
        7, 6, 5,
        7, 3, 0,
        7, 4, 0,
        2, 6, 5,
        2, 1, 5,
        1, 5, 4,
        1, 0, 4,
        2, 3, 7,
        2, 6, 7
    };

    std::vector<Vertex> vertices_vec;
    for (int i = 0; i < 24; i+=3) {
        float x = vertices[i];
        float y = vertices[i+1];
        float z = vertices[i+2];
        auto position = glm::vec3(x,y,z);
        Vertex vertex{ position, glm::vec3(), glm::vec2() };
        vertices_vec.push_back(vertex);
    }

    std::vector<unsigned int> indices_vec;
    for (int i = 0; i < 36; ++i) {
        indices_vec.push_back(indices[i]);
    }

    Shader shader = Shader("vert.glsl", "frag.glsl");
    Mesh mesh(vertices_vec, indices_vec);
    model = Model(mesh, shader);
}

void Game::render() {
    // clear
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearColor(0.25f, 0.0f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // render world
    model.render(camera);

    // render player


    // render UI/HUD

    // update display
    SDL_GL_SwapWindow(main_window);
}

