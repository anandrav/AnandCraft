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
    // TODO: incorporate interpolation for animation/shaders
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

    #define VEL 1.1f/5.f
    // 50 updates per second times 1.1/5 is 11 units/second

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            is_running = false;
        }
        if (e.type == SDL_KEYDOWN) {
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
            camera_rot.y = (float)e.motion.xrel;
            camera_rot.x = -(float)e.motion.yrel;
        }
    }
}

//void make_a_mesh() {
//    // cube w/ 24 vertices to make textures and normals (lighting) work
//    float vertices[] = {
//        // x+ face
//        0.5f,  0.5f, 0.5f,      // top right front
//        0.5f, -0.5f, 0.5f,      // bottom right front
//        0.5f,  0.5f, -0.5f,     // top right back
//        0.5f, -0.5f, -0.5f,     // bottom right back
//        // x- face
//        -0.5f, -0.5f, 0.5f,     // bottom left front
//        -0.5f,  0.5f, 0.5f,     // top left front
//        -0.5f, -0.5f, -0.5f,    // bottom left back
//        -0.5f,  0.5f, -0.5f,    // top left back
//        // y+ face
//        0.5f,  0.5f, 0.5f,      // top right front
//        -0.5f,  0.5f, 0.5f,     // top left front
//        0.5f,  0.5f, -0.5f,     // top right back
//        -0.5f,  0.5f, -0.5f,    // top left back
//        // y- face
//        0.5f, -0.5f, 0.5f,      // bottom right front
//        -0.5f, -0.5f, 0.5f,     // bottom left front
//        0.5f, -0.5f, -0.5f,     // bottom right back
//        -0.5f, -0.5f, -0.5f,    // bottom left back
//        // z+ face
//        0.5f,  0.5f, 0.5f,      // top right front
//        0.5f, -0.5f, 0.5f,      // bottom right front
//        -0.5f, -0.5f, 0.5f,     // bottom left front
//        -0.5f,  0.5f, 0.5f,     // top left front
//        // z- face
//         0.5f,  0.5f, -0.5f,    // top right back
//         0.5f, -0.5f, -0.5f,    // bottom right back
//        -0.5f, -0.5f, -0.5f,    // bottom left back
//        -0.5f,  0.5f, -0.5f     // top left back
//    };
//
//    unsigned int indices[] = {  // note that we start from 0!
//        // x+ face
//        0, 1, 2,
//        1, 2, 3,
//        // x- face
//        4, 5, 6,
//        5, 6, 7,
//        // y+ face
//        8, 9, 10,
//        9, 10, 11,
//        // y- face
//        12, 13, 14,
//        13, 14, 15,
//        // z+ face
//        16, 17, 18,
//        18, 19, 16,
//        // z- face
//        20, 21, 22,
//        22, 23, 20
//    };
//
//    std::vector<Vertex> vertices_vec;
//    for (int i = 0; i < 24; i += 1) {
//        float x = vertices[3*i];
//        float y = vertices[3*i + 1];
//        float z = vertices[3*i + 2];
//        auto position = glm::vec3(x, y, z);
//        float u = 0.f;
//        float v = 0.f;
//        auto tex_coords = glm::vec2(u, v);
//        Vertex vertex{ position, glm::vec3(), tex_coords };
//        vertices_vec.push_back(vertex);
//    }
//
//    std::vector<unsigned int> indices_vec;
//    for (int i = 0; i < 36; ++i) {
//        indices_vec.push_back(indices[i]);
//    }
//
//    Mesh mesh(vertices_vec, indices_vec);
//}

void Game::update() {
    camera.move_forward(-camera_vel.z);
    camera.move_left(-camera_vel.x);
    camera.move_up(camera_vel.y);
    camera.pitch(glm::radians(camera_rot.x / 2.f));
    camera.yaw(glm::radians(camera_rot.y) / 2.f);
    camera_rot.y = 0;
    camera_rot.x = 0;


    if (SDL_GetTicks() % 11000 <= 1000) {
        single_block->set_block(Block::State(Block::ID::DIRT));
    }
    else if (SDL_GetTicks() % 11000 <= 2000) {
        single_block->set_block(Block::State(Block::ID::GRASS));
    }
    else if (SDL_GetTicks() % 11000 <= 3000) {
        single_block->set_block(Block::State(Block::ID::STONE));
    }
    else if (SDL_GetTicks() % 11000 <= 4000) {
        single_block->set_block(Block::State(Block::ID::COBBLESTONE));
    }
    else if (SDL_GetTicks() % 11000 <= 5000) {
        single_block->set_block(Block::State(Block::ID::SAND));
    }
    else if (SDL_GetTicks() % 11000 <= 6000) {
        single_block->set_block(Block::State(Block::ID::PLANK));
    }
    else if (SDL_GetTicks() % 11000 <= 7000) {
        single_block->set_block(Block::State(Block::ID::LOG));
    }
    else if (SDL_GetTicks() % 11000 <= 8000) {
        single_block->set_block(Block::State(Block::ID::LEAF));
    }
    else if (SDL_GetTicks() % 11000 <= 9000) {
        single_block->set_block(Block::State(Block::ID::BRICK));
    }
    else if (SDL_GetTicks() % 11000 <= 10000) {
        single_block->set_block(Block::State(Block::ID::GLASS));
    }
    else if (SDL_GetTicks() % 11000 <= 11000) {
        single_block->set_block(Block::State(Block::ID::BOOKSHELF));
    }
}

void Game::init() {
    camera.move_forward(-2.f);

    // Create texture from image
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("texture-atlas.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    single_block = new SingleBlockDemo(Block::State(Block::ID::GRASS));
    single_block->set_location(glm::vec3(-2.f, 0, -2.f));

    grid = new Grid();

    for (int x = 0; x < 16 * 2; ++x) {
        for (int y = 0; y < 16 * 2; ++y) {
            for (int z = 0; z < 16 * 2; ++z) {
                if (!grid->has_block_at(x, y, z)) {
                    std::cout << "NO BLOCK AT " << x << " " << y << " " << z << std::endl;
                }
            }
        }
    }

    if (!grid->has_block_at(0, 0, 16)) {
        std::cout << "NO BLOCK HERE" << std::endl;
    }

    std::cout << Block::get_is_opaque(grid->get_block_at(0,0,16).id) << std::endl;

    std::cout << "-1 / 16 = " << -1 / 16 << std::endl;
}

void Game::render() {
    // clear
    glClearColor(.60f, .70f, .95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture);
    // render world
    single_block->render(camera);
    grid->render_opaque(camera);
    grid->render_transparent(camera);

    // render UI/HUD


    // update display
    SDL_GL_SwapWindow(main_window);
}