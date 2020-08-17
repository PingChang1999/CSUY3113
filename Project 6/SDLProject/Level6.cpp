#include "Level6.h"
#define LEVEL6_WIDTH 50
#define LEVEL6_HEIGHT 8

#define LEVEL6_ENEMY_COUNT 27

unsigned int level6_data[] =
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

void Level6::Initialize() {

    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL6_WIDTH, LEVEL6_HEIGHT, level6_data, mapTextureID, 1.0f, 4, 1);

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, -2, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(2, 2, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 4.0f;
    state.player->textureID = Util::LoadTexture("spaceship.png");

    state.player->height = 0.8f;
    state.player->width = 0.8f;

    state.player->jumpPower = 7.0f;

    state.fire = new Entity();
    state.fire->isActive = false;
    state.fire->entityType = ATTACK;
    state.fire->position = state.player->position;
    state.fire->movement = glm::vec3(1.0, 0.0, 0.0);
    state.fire->velocity = glm::vec3(1.0, 0.0, 0.0);
    state.fire->speed = 7.0f;
    state.fire->textureID = Util::LoadTexture("fire.png");

    state.enemies = new Entity[LEVEL6_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("alien.png");

    for (int i = 0; i < LEVEL6_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].speed = 0.1;
        state.enemies[i].velocity = glm::vec3(0.1);
        state.enemies[i].acceleration = glm::vec3(0.0, 0.0, 0.0);
        state.enemies[i].aiType = WALKER;
        state.enemies[i].aiState = IDLE;

        state.enemies[i].width = 0.7;
        state.enemies[i].height = 0.7;
    }
    state.enemies[0].textureID = Util::LoadTexture("alien1.png");
    state.enemies[0].position = glm::vec3(40, -3, 0);
    state.enemies[0].speed = 4;

    state.enemies[1].position = glm::vec3(9, -1, 0);
    state.enemies[2].position = glm::vec3(9, -2, 0);
    state.enemies[3].position = glm::vec3(9, -5, 0);
    state.enemies[4].position = glm::vec3(9, -6, 0);

    state.enemies[5].position = glm::vec3(13, -1, 0);
    state.enemies[6].position = glm::vec3(13, -4, 0);
    state.enemies[7].position = glm::vec3(13, -5, 0);
    state.enemies[8].position = glm::vec3(13, -6, 0);

    state.enemies[9].position = glm::vec3(17, -1, 0);
    state.enemies[10].position = glm::vec3(17, -2, 0);
    state.enemies[11].position = glm::vec3(17, -3, 0);
    state.enemies[12].position = glm::vec3(17, -6, 0);

    state.enemies[13].position = glm::vec3(21, -1, 0);
    state.enemies[14].position = glm::vec3(21, -2, 0);
    state.enemies[15].position = glm::vec3(21, -3, 0);
    state.enemies[16].position = glm::vec3(21, -4, 0);

    state.enemies[17].position = glm::vec3(26, -1, 0);
    state.enemies[18].position = glm::vec3(26, -2, 0);
    state.enemies[19].position = glm::vec3(26, -4, 0);
    state.enemies[20].position = glm::vec3(26, -5, 0);
    state.enemies[21].position = glm::vec3(26, -6, 0);

    state.enemies[22].position = glm::vec3(31, -1, 0);
    state.enemies[23].position = glm::vec3(31, -2, 0);
    state.enemies[24].position = glm::vec3(31, -3, 0);
    state.enemies[25].position = glm::vec3(31, -4, 0);
    state.enemies[26].position = glm::vec3(31, -6, 0);
}

void Level6::Update(float deltaTime) {
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
    for (int i = 0; i < LEVEL6_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, state.map);
    }

    if (state.player->position.x >= 42) {
        state.nextScene = 7;
    }
}
void Level6::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);

    for (int i = 0; i < LEVEL6_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    if (state.fire->isActive) {
        state.fire->Render(program);
    }
}

void Level6::Reset() {
    Level6::Initialize();
}