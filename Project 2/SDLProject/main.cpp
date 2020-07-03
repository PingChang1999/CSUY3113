#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, player1Matrix, player2Matrix, ballMatrix, projectionMatrix;

// Start at 0, 0, 0
glm::vec3 player1_position = glm::vec3(0, 0, 0);
glm::vec3 player2_position = glm::vec3(0, 0, 0);

// Don't go anywhere (yet).
glm::vec3 player1_movement = glm::vec3(0, 0, 0);
glm::vec3 player2_movement = glm::vec3(0, 0, 0);

float player1_speed = 2.0f;
float player2_speed = 2.0f;

bool vertical = 0;
bool horizontal = 0;

float paddle1_h = 1.1f;
float paddle1_w = 0.5f;

float paddle2_h = 1.1f;
float paddle2_w = 0.5f;

float ball_h = 0.3f;
float ball_w = 0.3f;

float ball_x = 0.0f;
float ball_y = 0.0f;


GLuint playerTextureID;
GLuint ballTextureID;

Mix_Music* music;
Mix_Chunk* bounce;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Pong!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("dooblydoo.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

    bounce = Mix_LoadWAV("bounce.wav");

    viewMatrix = glm::mat4(1.0f);
    player1Matrix = glm::mat4(1.0f);
    player2Matrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    // Good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    playerTextureID = LoadTexture("paddle12x6.png");
    ballTextureID = LoadTexture("ball6x6.png");
}

void ProcessInput() {

    player1_movement = glm::vec3(0); //if nothing is pressed, we don't want to go anywhere
    player2_movement = glm::vec3(0);
    //ball_movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                // Move the player left
                break;

            case SDLK_RIGHT:
                // Move the player right
                break;

            case SDLK_SPACE:
                // Some sort of action
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_W]) {
        player1_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_S]) {
        player1_movement.y = -1.0f;
    }

    if (keys[SDL_SCANCODE_UP]) {
        player2_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        player2_movement.y = -1.0f;
    }

    if (glm::length(player1_movement) > 1.0f) {
        player1_movement = glm::normalize(player1_movement);
    }
    if (glm::length(player1_movement) > 1.0f) {
        player1_movement = glm::normalize(player1_movement);
    }
}

float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    //player1Matrix
    // Add (direction * units per second * elapsed time)
    player1_position += player1_movement * player1_speed * deltaTime;
    player1Matrix = glm::mat4(1.0f);
    player1_position.x = -4.75f;
    if (player1_position.y > 3.35) {
        player1_position.x = -4.75f;
        player1_position.y = 3.35f;
        player1Matrix = glm::translate(player1Matrix, glm::vec3(-4.75f, 3.35f, 0.0f));
    }
    else if (player1_position.y < -3.35) {
        player1_position.x = -4.75f;
        player1_position.y = -3.35f;
        player1Matrix = glm::translate(player1Matrix, glm::vec3(-4.75f, -3.35f, 0.0f));
    }
    else {
        player1Matrix = glm::translate(player1Matrix, player1_position);
    }

    //player2Matrix
    player2_position += player2_movement * player2_speed * deltaTime;
    player2Matrix = glm::mat4(1.0f);
    player2_position.x = 4.75f;
    if (player2_position.y > 3.35) {
        player2_position.x = 4.75f;
        player2_position.y = 3.35f;
        player2Matrix = glm::translate(player2Matrix, glm::vec3(4.75f, 3.35f, 0.0f));
    }
    else if (player2_position.y < -3.35) {
        player2_position.x = 4.75f;
        player2_position.y = -3.35f;
        player2Matrix = glm::translate(player2Matrix, glm::vec3(4.75f, -3.35f, 0.0f));
    }
    else {
        player2Matrix = glm::translate(player2Matrix, player2_position);
    }

    //ballMatrix
    float l_xdist = fabs(player1_position.x - ball_x) - ((paddle1_w + ball_w) / 2);
    float l_ydist = fabs(player1_position.y - ball_y) - ((paddle1_h + ball_h) / 2);

    float r_xdist = fabs(player2_position.x - ball_x) - ((paddle2_w + ball_w) / 2);
    float r_ydist = fabs(player2_position.y - ball_y) - ((paddle2_h + ball_h) / 2);

    if (horizontal == 0) {
        ball_x += 2.0f * deltaTime;
        if (r_xdist < 0 && r_ydist < 0) {
            Mix_PlayChannel(-1, bounce, 0);
            horizontal = 1;
        }
    }
    else {
        ball_x -= 2.0f * deltaTime;
        if (l_xdist < 0 && l_ydist < 0) {
            Mix_PlayChannel(-1, bounce, 0);
            horizontal = 0;
        }
    }

    if (vertical == 0) {
        ball_y += 2.0f * deltaTime;
        if (ball_y >= 3.6f) {
            Mix_PlayChannel(-1, bounce, 0);
            vertical = 1;
        }
    }
    else {
        ball_y -= 2.0f * deltaTime;
        if (ball_y <= -3.6f) {
            Mix_PlayChannel(-1, bounce, 0);
            vertical = 0;
        }
    }
    ballMatrix = glm::mat4(1.0f);
    ballMatrix = glm::translate(ballMatrix, glm::vec3(ball_x, ball_y, 0.0f));

    if (ball_x < -4.75f || ball_x > 4.75f) {
        gameIsRunning = false;
    }
      
}

void Render() {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glClear(GL_COLOR_BUFFER_BIT);

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);

    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    program.SetModelMatrix(player1Matrix);
    glBindTexture(GL_TEXTURE_2D, playerTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.SetModelMatrix(player2Matrix);
    glBindTexture(GL_TEXTURE_2D, playerTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.SetModelMatrix(ballMatrix);
    glBindTexture(GL_TEXTURE_2D, ballTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}