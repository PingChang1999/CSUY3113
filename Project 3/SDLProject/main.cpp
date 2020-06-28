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

#define PLATFORM_COUNT 28

struct GameState {
    Entity* player;
    Entity* platforms;
    Entity* goal;
};

GameState state;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix, goalMatrix;

GLuint font;

SDL_Window* displayWindow;
bool gameIsRunning = true;

float lastTicks = 0;

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

    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v };

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text,
    float size, float spacing, glm::vec3 position)
{
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;

    std::vector<float> vertices;
    std::vector<float> texCoords;

    for (int i = 0; i < text.size(); i++) {

        int index = (int)text[i];
        float offset = (size + spacing) * i;

        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;

        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            });

        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
            });

    } // end of for loop

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
    displayWindow = SDL_CreateWindow("Lunar Lander!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    goalMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    font = LoadTexture("font1.png");

    // Initialize Player
    state.player = new Entity();
    state.player->position = glm::vec3(0, 4.0f, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -0.15f, 0);

    state.player->velocity = glm::vec3(0);
    state.player->textureID = LoadTexture("ctg.png");

    state.platforms = new Entity[PLATFORM_COUNT];

    state.goal = new Entity();
    state.goal->position = glm::vec3(2, -3.75, 0);

    GLuint platformTextureID = LoadTexture("platformPack_tile041.png");
    GLuint goalTextureID = LoadTexture("platformPack_tile008.png");

    state.goal->textureID = goalTextureID;

    //bottom
    state.platforms[0].textureID = platformTextureID;
    state.platforms[0].position = glm::vec3(-1, -3.75, 0);

    state.platforms[1].textureID = platformTextureID;
    state.platforms[1].position = glm::vec3(0, -3.75, 0);

    state.platforms[2].textureID = platformTextureID;
    state.platforms[2].position = glm::vec3(1, -3.75, 0);

    state.platforms[3].textureID = platformTextureID;
    state.platforms[3].position = glm::vec3(-2, -3.75, 0);

    state.platforms[4].textureID = platformTextureID;
    state.platforms[4].position = glm::vec3(-3, -3.75, 0);

    state.platforms[5].textureID = platformTextureID;
    state.platforms[5].position = glm::vec3(3, -3.75, 0);

    state.platforms[6].textureID = platformTextureID;
    state.platforms[6].position = glm::vec3(-4, -3.75, 0);

    state.platforms[7].textureID = platformTextureID;
    state.platforms[7].position = glm::vec3(4, -3.75, 0);

    state.platforms[8].textureID = platformTextureID;
    state.platforms[8].position = glm::vec3(-5, -3.75, 0);

    state.platforms[9].textureID = platformTextureID;
    state.platforms[9].position = glm::vec3(5, -3.75, 0);

    //right
    state.platforms[10].textureID = platformTextureID;
    state.platforms[10].position = glm::vec3(5, -2.75, 0);

    state.platforms[11].textureID = platformTextureID;
    state.platforms[11].position = glm::vec3(5, -1.75, 0);

    state.platforms[12].textureID = platformTextureID;
    state.platforms[12].position = glm::vec3(5, -0.75, 0);

    state.platforms[13].textureID = platformTextureID;
    state.platforms[13].position = glm::vec3(5, 0.25, 0);

    state.platforms[14].textureID = platformTextureID;
    state.platforms[14].position = glm::vec3(5, 1.25, 0);

    state.platforms[15].textureID = platformTextureID;
    state.platforms[15].position = glm::vec3(5, 2.25, 0);

    state.platforms[16].textureID = platformTextureID;
    state.platforms[16].position = glm::vec3(5, 3.25, 0);

    //left
    state.platforms[17].textureID = platformTextureID;
    state.platforms[17].position = glm::vec3(-5, -2.75, 0);

    state.platforms[18].textureID = platformTextureID;
    state.platforms[18].position = glm::vec3(-5, -1.75, 0);

    state.platforms[19].textureID = platformTextureID;
    state.platforms[19].position = glm::vec3(-5, -0.75, 0);

    state.platforms[20].textureID = platformTextureID;
    state.platforms[20].position = glm::vec3(-5, 0.25, 0);

    state.platforms[21].textureID = platformTextureID;
    state.platforms[21].position = glm::vec3(-5, 1.25, 0);

    state.platforms[22].textureID = platformTextureID;
    state.platforms[22].position = glm::vec3(-5, 2.25, 0);

    state.platforms[23].textureID = platformTextureID;
    state.platforms[23].position = glm::vec3(-5, 3.25, 0);

    //obstacle
    state.platforms[24].textureID = platformTextureID;
    state.platforms[24].position = glm::vec3(3, 2.25, 0);

    state.platforms[25].textureID = platformTextureID;
    state.platforms[25].position = glm::vec3(2, 2.25, 0);

    state.platforms[26].textureID = platformTextureID;
    state.platforms[26].position = glm::vec3(-2, -0.75, 0);

    state.platforms[27].textureID = platformTextureID;
    state.platforms[27].position = glm::vec3(-1, -0.75, 0);


    for (int i = 0; i < PLATFORM_COUNT; i++) { //update platform 1 time so modelMatrix would update
        state.platforms[i].Update(0, state.platforms, state.goal, 0);
    }
    state.goal->Update(0, state.platforms, state.goal, 0);
}

void ProcessInput() {

    state.player->movement = glm::vec3(0);

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
                break;
            }
            break; // SDL_KEYDOWN
        }
    }


    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->acceleration.x -= 0.01f;
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->acceleration.x += 0.01f;
    }

    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
    }

}

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    state.player->Update(deltaTime, state.platforms, state.goal, PLATFORM_COUNT);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (!state.player->isActive) {
        if (!state.player->isLanded) {
            DrawText(&program, font, "Mission Failed", 0.5f, -0.25f, glm::vec3(-2.0f, 1.0f, 0.0f));
        }
        else {
            DrawText(&program, font, "Mission Successful", 0.5f, -0.25f, glm::vec3(-2.0f, 1.0f, 0.0f));
        }
    }

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }
    state.goal->Render(&program);
    state.player->Render(&program);

    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        SDL_Event event;
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}