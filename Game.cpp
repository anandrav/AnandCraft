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

    glEnable(GL_DEPTH_TEST);

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

    camera_rot_vel.y = 0;
    camera_rot_vel.x = 0;
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
            camera_rot_vel.y = (float)e.motion.xrel;
            camera_rot_vel.x = -(float)e.motion.yrel;
        }
    }
}

void Game::update() {
    //model.get_transform().rotate(0.03f, glm::vec3(1.f, 0.f, 0.f));
    camera.move_forward(-camera_vel.z);
    camera.move_left(-camera_vel.x);
    camera.move_up(camera_vel.y);
    //camera.transform.rotate(camera_rot_vel.x, glm::vec3(1.f,0.f,0.f));
    //camera.transform.rotate(camera_rot_vel.y,glm::vec3(0.f,1.f,0.f));
    // FIX ROLLING ISSUE! MAYBE MAKE TRANSFORM PRIVATE AND IMPLEMENT SET_YAW, SET_PITCH FUNCTIONS
    camera.pitch(camera_rot_vel.x/100.f);
    camera.yaw(camera_rot_vel.y/100.f);
}
void print_vertices(std::vector<Vertex> vertices) {
    for (int i = 0; i < vertices.size(); ++i) {
        std::cout << "(" << vertices[i].position.x << ", " << vertices[i].position.y << ", " << vertices[i].position.z << ") ";
    }
    std::cout << "\n";
}

void Game::init() {
    camera.move_forward(-2.f);
    //// cube
    //float vertices[] = {
    // 0.5f,  0.5f, 0.5f,  // top right
    // 0.5f, -0.5f, 0.5f,  // bottom right
    //-0.5f, -0.5f, 0.5f,  // bottom left
    //-0.5f,  0.5f, 0.5f,   // top left
    // 0.5f,  0.5f, -0.5f,
    // 0.5f, -0.5f, -0.5f,
    //-0.5f, -0.5f, -0.5f,
    //-0.5f,  0.5f, -0.5f
    //};
    //float tex_coords[] = {
    // 0.5f,  0.5f, 0.5f,  // top right
    // 0.5f, -0.5f, 0.5f,  // bottom right
    //-0.5f, -0.5f, 0.5f,  // bottom left
    //-0.5f,  0.5f, 0.5f,   // top left
    // 0.5f,  0.5f, -0.5f,
    // 0.5f, -0.5f, -0.5f,
    //-0.5f, -0.5f, -0.5f,
    //-0.5f,  0.5f, -0.5f
    //};
    //unsigned int indices[] = {  // note that we start from 0!
    //    0, 1, 3,   // first triangle
    //    1, 2, 3,    // second triangle
    //    7, 4, 5,
    //    7, 6, 5,
    //    7, 3, 0,
    //    7, 4, 0,
    //    2, 6, 5,
    //    2, 1, 5,
    //    1, 5, 4,
    //    1, 0, 4,
    //    2, 3, 7,
    //    2, 6, 7
    //};

    //std::vector<Vertex> vertices_vec;
    //for (int i = 0; i < 24; i+=3) {
    //    float x = vertices[i];
    //    float y = vertices[i+1];
    //    float z = vertices[i+2];
    //    auto position = glm::vec3(x,y,z);
    //    Vertex vertex{ position, glm::vec3(), glm::vec2() };
    //    vertices_vec.push_back(vertex);
    //}

    //std::vector<unsigned int> indices_vec;
    //for (int i = 0; i < 36; ++i) {
    //    indices_vec.push_back(indices[i]);
    //}

    // Create texture from image
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("texture-atlas.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    // cube w/ 24 vertices to make textures and normals (lighting) work
    float vertices[] = {
    // x+ face
    0.5f,  0.5f, 0.5f,      // top right front
    0.5f, -0.5f, 0.5f,      // bottom right front
    0.5f,  0.5f, -0.5f,     // top right back
    0.5f, -0.5f, -0.5f,     // bottom right back
    // x- face
    -0.5f, -0.5f, 0.5f,     // bottom left front
    -0.5f,  0.5f, 0.5f,     // top left front
    -0.5f, -0.5f, -0.5f,    // bottom left back
    -0.5f,  0.5f, -0.5f,    // top left back
    // y+ face
    0.5f,  0.5f, 0.5f,      // top right front
    -0.5f,  0.5f, 0.5f,     // top left front
    0.5f,  0.5f, -0.5f,     // top right back
    -0.5f,  0.5f, -0.5f,    // top left back
    // y- face
    0.5f, -0.5f, 0.5f,      // bottom right front
    -0.5f, -0.5f, 0.5f,     // bottom left front
    0.5f, -0.5f, -0.5f,     // bottom right back
    -0.5f, -0.5f, -0.5f,    // bottom left back
    // z+ face
    0.5f,  0.5f, 0.5f,      // top right front
    0.5f, -0.5f, 0.5f,      // bottom right front
    -0.5f, -0.5f, 0.5f,     // bottom left front
    -0.5f,  0.5f, 0.5f,     // top left front
    // z- face
     0.5f,  0.5f, -0.5f,    // top right back
     0.5f, -0.5f, -0.5f,    // bottom right back
    -0.5f, -0.5f, -0.5f,    // bottom left back
    -0.5f,  0.5f, -0.5f     // top left back
    };

    // side faces
#define GRASS_SIDE_TOP 1.f
#define GRASS_SIDE_LEFT 0.f + (1.f/16.f) * 3
#define GRASS_SIDE_BOTTOM 1.f - 1.f/16.f
#define GRASS_SIDE_RIGHT 0.f + (1.f/16.f) * 4

    // top face
#define GRASS_YPOS_TOP 1.f
#define GRASS_YPOS_LEFT 0.f
#define GRASS_YPOS_BOTTOM 15.f/16.f
#define GRASS_YPOS_RIGHT 1.f/16.f

    // bottom face
#define GRASS_YNEG_TOP 1.f
#define GRASS_YNEG_LEFT 2.f/16.f
#define GRASS_YNEG_BOTTOM 1.f - 1.f/16.f
#define GRASS_YNEG_RIGHT 3.f/16.f

        float tex_coords[] = {
        // x+ face
        GRASS_SIDE_LEFT, GRASS_SIDE_TOP,     // top right front
        GRASS_SIDE_LEFT, GRASS_SIDE_BOTTOM,     // bottom right front
        GRASS_SIDE_RIGHT, GRASS_SIDE_TOP,     // top right back
        GRASS_SIDE_RIGHT, GRASS_SIDE_BOTTOM,     // bottom right back
        // x- face
        GRASS_SIDE_RIGHT, GRASS_SIDE_BOTTOM,   // bottom left front
        GRASS_SIDE_RIGHT, GRASS_SIDE_TOP,   // top left front
        GRASS_SIDE_LEFT, GRASS_SIDE_BOTTOM,  // bottom left back
        GRASS_SIDE_LEFT, GRASS_SIDE_TOP,  // top left back
        // y+ face
        GRASS_YPOS_RIGHT, GRASS_YPOS_BOTTOM,   // top right front
        GRASS_YPOS_LEFT, GRASS_YPOS_BOTTOM,   // top left front
        GRASS_YPOS_RIGHT,  GRASS_YPOS_TOP,   // top right back
        GRASS_YPOS_LEFT, GRASS_YPOS_TOP,   // top left back
        // y- face
        GRASS_YNEG_RIGHT, GRASS_YNEG_BOTTOM,   // bottom right front
        GRASS_YNEG_LEFT, GRASS_YNEG_BOTTOM,   // bottom left front
        GRASS_YNEG_RIGHT,  GRASS_YPOS_TOP,   // bottom right back
        GRASS_YNEG_LEFT, GRASS_YNEG_TOP,   // bottom left back
        // z+ face
        GRASS_SIDE_RIGHT,  GRASS_SIDE_TOP,   // top right front
        GRASS_SIDE_RIGHT, GRASS_SIDE_BOTTOM,   // bottom right front
        GRASS_SIDE_LEFT, GRASS_SIDE_BOTTOM,   // bottom left front
        GRASS_SIDE_LEFT,  GRASS_SIDE_TOP,  // top left front
        // z- face
        GRASS_SIDE_LEFT,  GRASS_SIDE_TOP,   // top right back
        GRASS_SIDE_LEFT, GRASS_SIDE_BOTTOM, // bottom right back
        GRASS_SIDE_RIGHT, GRASS_SIDE_BOTTOM,   // bottom left back
        GRASS_SIDE_RIGHT,  GRASS_SIDE_TOP,    // top left back
    };

    //float tex_coords[] = {
    //    // x+ face
    //    0.f, 1.f,     // top right front
    //    0.f, 0.f,      // bottom right front
    //    1.f, 1.f,     // top right back
    //    1.f, 0.f,     // bottom right back
    //    // x- face
    //    1.f, 0.f,   // bottom left front
    //    1.f, 1.f,   // top left front
    //    0.f, 0.f,   // bottom left back
    //    0.f, 1.f,  // top left back
    //    // y+ face
    //    1.f, 0.f,   // top right front
    //    0.f, 0.f,   // top left front
    //    1.f,  1.f,   // top right back
    //    0.f, 1.f,   // top left back
    //    // y- face
    //    1.f, 0.f,   // bottom right front
    //    0.f, 0.f,   // bottom left front
    //    1.f,  1.f,   // bottom right back
    //    0.f, 1.f,   // bottom left back
    //    // z+ face
    //    1.f,  1.f,   // top right front
    //    1.f,  0.f,  // bottom right front
    //    0.f, 0.f,   // bottom left front
    //    0.f,  1.f,    // top left front
    //    // z- face
    //    1.f,  1.f,   // top right back
    //    1.f, 0.f,   // bottom right back
    //    0.f, 0.f,   // bottom left back
    //    0.f,  1.f   // top left back
    //};
    unsigned int indices[] = {  // note that we start from 0!
        // x+ face
        0, 1, 2,
        1, 2, 3,
        // x- face
        4, 5, 6,
        5, 6, 7,
        // y+ face
        8, 9, 10,
        9, 10, 11,
        // y- face
        12, 13, 14,
        13, 14, 15,
        // z+ face
        16, 17, 18,
        18, 19, 16,
        // z- face
        20, 21, 22,
        22, 23, 20
    };

    std::vector<Vertex> vertices_vec;
    for (int i = 0; i < 24; i += 1) {
        float x = vertices[3*i];
        float y = vertices[3*i + 1];
        float z = vertices[3*i + 2];
        auto position = glm::vec3(x, y, z);
        float u = tex_coords[2*i];
        float v = 1.f-tex_coords[2*i + 1];
        auto tex_coords = glm::vec2(u, v);
        Vertex vertex{ position, glm::vec3(), tex_coords };
        vertices_vec.push_back(vertex);
    }

    std::vector<unsigned int> indices_vec;
    for (int i = 0; i < 36; ++i) {
        indices_vec.push_back(indices[i]);
    }

    Shader shader = Shader("basic_vert.glsl", "basic_frag.glsl");
    Mesh mesh(vertices_vec, indices_vec);
    model = Model(mesh, shader);
    model.get_transform().rotate(glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
}

void Game::render() {
    // clear
    glClearColor(.60f, .70f, .95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture);
    // render world
    model.render(camera);

    // render player

    // render UI/HUD

    // update display
    SDL_GL_SwapWindow(main_window);
}

