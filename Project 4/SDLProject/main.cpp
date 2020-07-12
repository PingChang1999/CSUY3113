#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

#define PLATFORM_COUNT 11
#define ENEMY_COUNT 3

struct GameState {
    Entity* player;
    Entity* flame; //player projectile
    Entity* fire; // enemy projectile
    Entity* platforms;
    Entity* enemies;
};

GameState state;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix, terrainMatrix, leftMatrix;

GLuint font;

SDL_Window* displayWindow;
bool gameIsRunning = true;

int* animIndices = new int[4]{ 3, 7, 11, 15 };
int animFrames = 4;
int animIndex = 0;

void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
    int cols = 4;
    int rows = 4;

    float u = (float)(index % cols) / (float)cols;
    float v = (float)(index / cols) / (float)rows;

    float width = 1.0f / (float)cols;
    float height = 1.0f / (float)rows;

    float texCoords[] = {
        u, v + height,
        u + width, v + height,
        u + width, v,

        u, v + height,
        u + width, v,
        u, v
    };

    float vertices[] = {
        -0,5, -0.5,
        0.5, -0.5,
        0.5, 0.5,

        -0.5, -0.5,
        0.5, 0.5,
        -0.5, 0.5
    };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position)
{
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;

    std::vector<float> vertices;
    std::vector<float> texCoords;

    for (int i = 0; i < text.size(); i++)
    {
        int index = (int)text[i];
        float offset = (size + spacing) * i;

        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;

        vertices.insert(vertices.end(),
            {
                offset + (-0.5f * size), 0.5f * size,
                offset + (-0.5f * size), -0.5f * size,
                offset + (0.5f * size), 0.5f * size,
                offset + (0.5f * size), -0.5f * size,
                offset + (0.5f * size), 0.5f * size,
                offset + (-0.5f * size), -0.5f * size
            });

        texCoords.insert(texCoords.end(),
            {
                u, v,
                u, v + height,
                u + width, v,
                u + width, v + height,
                u + width, v,
                u, v + height
            });
    }

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);

    glUseProgram(program->programID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    terrainMatrix = glm::mat4(1.0f);
    leftMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    font = LoadTexture("font1.png");

    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(-4, -1, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 1.5f;
    state.player->jumpPower = 0.5f;
    state.player->textureID = LoadTexture("HK_Knight.png");

    /*
    state.player->animRight = new int[4]{ 3, 7, 11, 15 };
    state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
    state.player->animUp = new int[4]{ 2, 6, 10, 14 };
    state.player->animDown = new int[4]{ 0, 4, 8, 12 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;

    state.player->height = 0.8f;
    state.player->width = 0.8f;
    */

    state.flame = new Entity();
    state.flame->isActive = false;
    state.flame->entityType = OBJECT;
    state.flame->position = state.player->position;
    state.flame->movement = glm::vec3(1, 0, 0);
    state.flame->velocity = glm::vec3(1, 0, 0);
    state.flame->speed = 2.0f;
    state.flame->textureID = LoadTexture("flame.png");



    state.platforms = new Entity[PLATFORM_COUNT];

    GLuint platformTextureID = LoadTexture("platformPack_tile001.png");

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(-5 + i, -3.25f, 0);
    }

    /*
    for (int i = 11; i < PLATFORM_COUNT - 1; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(i - 9, -1.25f, 0);
    }

    for (int i = 14; i < PLATFORM_COUNT; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(i - 14, -1.25f, 0);
    }
    */
    for (int i = 0; i < PLATFORM_COUNT; i++) { //update platform 1 time so modelMatrix would update
        state.platforms[i].Update(0, state.player, &state.enemies[state.player->enemiesKilled], state.fire, state.platforms, 0);
    }

    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = LoadTexture("ctg.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(4, -2.25, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].acceleration = glm::vec3(0.0, -9.81f, 0.0);
    state.enemies[0].aiType = WALKER;
    state.enemies[0].aiState = IDLE;

    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(3, -0.25, 0);
    state.enemies[1].speed = 1;
    state.enemies[1].acceleration = glm::vec3(0.0, -9.81f, 0.0);
    state.enemies[1].aiType = JUMPER; //WAITANDGO
    state.enemies[1].aiState = IDLE;
    state.enemies[1].isActive = false;

    state.enemies[2].entityType = ENEMY;
    state.enemies[2].textureID = enemyTextureID;
    state.enemies[2].position = glm::vec3(2, 2.25, 0);
    state.enemies[2].speed = 1;
    state.enemies[2].acceleration = glm::vec3(0.0, -9.81f, 0.0);
    state.enemies[2].aiType = THROWER;
    state.enemies[2].aiState = IDLE;
    state.enemies[2].isActive = false;

    state.fire = new Entity();
    state.fire->isActive = false;
    state.fire->entityType = OBJECT;
    state.fire->position = state.enemies[2].position;
    state.fire->movement = glm::vec3(1, 0, 0);
    state.fire->velocity = glm::vec3(1, 0, 0);
    state.fire->speed = -2.0f;
    state.fire->textureID = LoadTexture("fireball.png");
}

void ProcessInput() {

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->movement.x = -1.0f;
        state.player->animIndices = state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 1.0f;
        state.player->animIndices = state.player->animRight;
    }
    if (keys[SDL_SCANCODE_SPACE]) {
        if (!state.player->jump) state.player->jump = true;
    }
    if (keys[SDL_SCANCODE_X]) {
        if (!state.player->shootFlame) {
            state.player->shootFlame = true;
            state.flame->position = state.player->position;
            state.flame->position.x = state.player->position.x + state.player->width;
            state.flame->isActive = true;
        }
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP && state.player->isActive) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        state.player->Update(FIXED_TIMESTEP, state.player, &state.enemies[state.player->enemiesKilled], state.fire, state.platforms, PLATFORM_COUNT);

        if (state.player->collidedLeft || state.player->collidedRight) {
            state.player->isActive = false;
            state.player->isWin = false;
        }
        for (int i = 0; i < ENEMY_COUNT; i++) {
            if (state.enemies[i].isActive) {
                state.enemies[i].Update(FIXED_TIMESTEP, state.player, &state.enemies[i], state.fire, state.platforms, PLATFORM_COUNT);
            }
            if (state.enemies[i].shootFire) {
                state.fire->Update(FIXED_TIMESTEP, state.player, &state.enemies[i], state.fire, state.platforms, PLATFORM_COUNT);
                if (glm::distance(state.fire->position, state.player->position) > 4.0) {
                    state.fire->isActive = false;
                    state.enemies[i].shootFire = false;
                }
                if (!state.fire->isActive && !state.player->isActive) {
                    state.enemies[i].shootFire = false;
                }
            }
            if (state.flame->isActive) {
                state.flame->Update(FIXED_TIMESTEP, state.player, &state.enemies[i], state.fire, state.platforms, PLATFORM_COUNT);
                if (glm::distance(state.flame->position, state.player->position) > 4.0) {
                    state.flame->isActive = false;
                    state.player->shootFlame = false;
                }
                if (!state.flame->isActive && !state.enemies[i].isActive) {
                    state.player->shootFlame = false;
                }
            }
            state.enemies[state.player->enemiesKilled].isActive = true;
        }

        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (!state.player->isActive) {
        DrawText(&program, font, "Game Over", 0.5f, -0.25f, glm::vec3(-2.0f, 1.0f, 0.0f));
    }
    else if (state.player->enemiesKilled == ENEMY_COUNT) {
        DrawText(&program, font, "You Win", 0.5f, -0.25f, glm::vec3(-2.0f, 1.0f, 0.0f));
    }
    else {
        DrawText(&program, font, "Press Space to Jump", 0.5f, -0.25f, glm::vec3(-3.0f, 3.0f, 0.0f));
        DrawText(&program, font, "Press 'X' to fire", 0.5f, -0.25f, glm::vec3(-3.0f, 2.0f, 0.0f));
    }
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }

    state.player->Render(&program);

    if (state.flame->isActive) {
        state.flame->Render(&program);
    }

    if (state.fire->isActive) {
        state.fire->Render(&program);
    }

    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                gameIsRunning = false;
            }
        }

        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}