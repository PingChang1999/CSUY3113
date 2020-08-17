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

#include "Util.h"
#include "Effects.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"

#include "Start.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Level5.h"
#include "Level6.h"
#include "Level7.h"
#include "Level8.h"
#include "Level9.h"
#include "Level10.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene* currentScene;
Scene* sceneList[11];

Mix_Music* music;
Mix_Chunk* shoot;

GLuint font;

bool startScreen = true;

int lives = 3;

Effects* effects;

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    font = Util::LoadTexture("font2.png");
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("space.mp3");
    Mix_PlayMusic(music, -1);

    shoot = Mix_LoadWAV("shootsound.wav");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    sceneList[0] = new Start();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    sceneList[4] = new Level4();
    sceneList[5] = new Level5();
    sceneList[6] = new Level6();
    sceneList[7] = new Level7();
    sceneList[8] = new Level8();
    sceneList[9] = new Level9();
    sceneList[10] = new Level10();
    SwitchToScene(sceneList[0]);

    effects = new Effects(projectionMatrix, viewMatrix); //to know dimensions of the window to display

    effects->Start(FADEIN, 0.5f);

}

void ProcessInput() {

    currentScene->state.player->movement = glm::vec3(0);

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

            case SDLK_UP:
                //Move the player up
                break;

            case SDLK_DOWN:
                //Move the player down
                break;

            case SDLK_SPACE:
                break;

            case SDLK_RETURN:
                if (startScreen) {
                    SwitchToScene(sceneList[1]);
                    startScreen = false;
                }
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.player->movement.x = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->movement.x = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
    }
    if (keys[SDL_SCANCODE_UP]) {
        currentScene->state.player->movement.y = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animUp;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        currentScene->state.player->movement.y = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animDown;
    }

    if (keys[SDL_SCANCODE_X]) {
        if (!currentScene->state.player->shootFire) {
            currentScene->state.player->shootFire = true;
            Mix_PlayChannel(-1, shoot, 0);
            currentScene->state.fire->position = currentScene->state.player->position;
            currentScene->state.fire->position.x = currentScene->state.player->position.x + currentScene->state.player->width;
            currentScene->state.fire->isActive = true;
        }
    }

    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

bool lastCollidedBottom = false;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    if (currentScene->state.player->killed && lives > 0) {
        currentScene->Reset();
        lives--;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        currentScene->Update(FIXED_TIMESTEP);

        
        program.SetLightPosition(currentScene->state.player->position);

        if (lastCollidedBottom == false && currentScene->state.player->collidedBottom) {
            effects->Start(SHAKE, 2.0f);
        }

        lastCollidedBottom = currentScene->state.player->collidedBottom;

        effects->Update(FIXED_TIMESTEP);

        

        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;

    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player->position.x > 5) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }

    viewMatrix = glm::translate(viewMatrix, effects->viewOffset);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID); //telling opengl to use shader program 
    currentScene->Render(&program);

    if (startScreen) {
        Util::DrawText(&program, font, "Speed Up!", 0.5f, -0.25f, glm::vec3(2.0f, -1.0f, 0.0f));
        Util::DrawText(&program, font, "Press Enter to Continue", 0.5f, -0.25f, glm::vec3(1.0f, -2.0f, 0.0f));
        Util::DrawText(&program, font, "Use Arrows Keys to Move", 0.5f, -0.25f, glm::vec3(2.0f, -3.0f, 0.0f));
        Util::DrawText(&program, font, "Press X to Shoot", 0.5f, -0.25f, glm::vec3(2.0f, -4.0f, 0.0f));
        Util::DrawText(&program, font, "Shooting only work against the boss", 0.5f, -0.25f, glm::vec3(1.0f, -5.0f, 0.0f));
        Util::DrawText(&program, font, "Your speed is different every level", 0.5f, -0.25f, glm::vec3(1.0f, -6.0f, 0.0f));
    }
    else {
        if (lives == 0) {
            Util::DrawText(&program, font, "You Lose", 0.5f, -0.25f, glm::vec3(4.0f, -2.0f, 0.0f));
            currentScene->state.player->isActive = false;
        }
        else if (currentScene->state.player->win) {
            Util::DrawText(&program, font, "You Win", 0.5f, -0.25f, glm::vec3(160.0f, -2.0f, 0.0f));
            currentScene->state.player->isActive = false;
        }
        else {
            std::string textLives = "Lives: " + std::to_string(lives);
            Util::DrawText(&program, font, textLives, 0.5f, -0.25f, glm::vec3(3.0f, -1.0f, 0.0f));
        }
    }

    effects->Render();

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

        if (currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);
        Render();
    }

    Shutdown();
    return 0;
}