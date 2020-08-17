#include "Level1.h"
#define LEVEL1_WIDTH 50
#define LEVEL1_HEIGHT 8

#define LEVEL1_ENEMY_COUNT 13

unsigned int level1_data[] =
{
     3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

void Level1::Initialize() {

    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, -2, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("spaceship.png");

    state.player->height = 0.8f;
    state.player->width = 0.8f;

    state.fire = new Entity();
    state.fire->isActive = false;
    state.fire->entityType = ATTACK;
    state.fire->position = state.player->position;
    state.fire->movement = glm::vec3(1.0, 0.0, 0.0);
    state.fire->velocity = glm::vec3(1.0, 0.0, 0.0);
    state.fire->speed = 4.0f;
    state.fire->textureID = Util::LoadTexture("fire.png");


    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("alien.png");

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].speed = 1;
        state.enemies[i].velocity = glm::vec3(1);
        state.enemies[i].acceleration = glm::vec3(0, 0, 0);
        state.enemies[i].aiType = WALKER;
        state.enemies[i].aiState = IDLE;

        state.enemies[i].width = 0.7;
        state.enemies[i].height = 0.7;
    }
    state.enemies[0].textureID = Util::LoadTexture("alien1.png");
    state.enemies[0].position = glm::vec3(40, -5, 0);
    state.enemies[0].speed = 4;

    state.enemies[1].position = glm::vec3(9, -3, 0);

    state.enemies[2].position = glm::vec3(11, -3, 0);

    state.enemies[3].position = glm::vec3(13, -2, 0);

    state.enemies[4].position = glm::vec3(15, -2, 0);

    state.enemies[5].position = glm::vec3(17, -2, 0);

    state.enemies[6].position = glm::vec3(19, -4, 0);

    state.enemies[7].position = glm::vec3(21, -4, 0);

    state.enemies[8].position = glm::vec3(23, -4, 0);

    state.enemies[9].position = glm::vec3(25, -4, 0);

    state.enemies[10].position = glm::vec3(27, -3, 0);

    state.enemies[11].position = glm::vec3(29, -3, 0);

    state.enemies[12].position = glm::vec3(31, -3, 0);
}

void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, state.map);

    if (state.fire->isActive) {
        state.fire->Update(deltaTime, state.player, state.enemies, state.map);

        if (glm::distance(state.fire->position, state.player->position) > 3.0f) {
            state.fire->isActive = false;
            state.player->shootFire = false;
        }

        if (!state.fire->isActive && !state.enemies->isActive) {
            state.player->shootFire = false;
        }
    }
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, state.map);
    }

    if (state.player->position.x >= 42) {
        state.nextScene = 2;
    }
}
void Level1::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    if (state.fire->isActive) {
        state.fire->Render(program);
    }
}

void Level1::Reset() {
    Level1::Initialize();
}